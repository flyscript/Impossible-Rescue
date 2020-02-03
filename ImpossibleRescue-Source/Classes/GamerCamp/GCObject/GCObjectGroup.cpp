////////////////////////////////////////////////////////////////////////////////////////////////////////////
// (C) Gamer Camp / Alex Darby 2018
// Distributed under the MIT license - see readme.md
////////////////////////////////////////////////////////////////////////////////////////////////////////////
#include <memory.h>
#include <string.h>
#include "base/ccMacros.h"

#include "GamerCamp/GCObject/GCObjectGroup.h"
#include "GamerCamp/GCObject/GCObjectManager.h"
#include "GamerCamp/GCObject/GCObject.h"


//////////////////////////////////////////////////////////////////////////
// constructor
//////////////////////////////////////////////////////////////////////////
CGCObjectGroup::CGCObjectGroup()
: m_uNumGCObjects		( 0 )
, m_eResetBehaviour		( EResetAlive )
, m_uKillStackCount		( 0 )
{
	// initialise
	memset( m_apGCObjects, 0, sizeof( m_apGCObjects ) );
}


//////////////////////////////////////////////////////////////////////////
// destructor
//////////////////////////////////////////////////////////////////////////
// virtual
CGCObjectGroup::~CGCObjectGroup()
{
}


//////////////////////////////////////////////////////////////////////////
// adds the registering instance to the end of the registration array
//////////////////////////////////////////////////////////////////////////
void CGCObjectGroup::OnObjectRegister( CGCObject* pArgObject )
{
	GCASSERT( ( m_uNumGCObjects < EMaxGCObjects), "you have run out of space in a GCObjectGroup! maximum oijects handled is EMaxGCObjects" );

	// insert the GCObject at the end of m_apGCObjects
	// N.B. post increment of object count
	m_apGCObjects[ m_uNumGCObjects++ ] = pArgObject;

	// call polymorphic post-register function for derived classes
	VPostObjectRegister( pArgObject );
}


//////////////////////////////////////////////////////////////////////////
// removes the unregistering instance from the array and shuffles the 
// remaining instances down to maintain registration order
//////////////////////////////////////////////////////////////////////////
void CGCObjectGroup::OnObjectUnRegister( CGCObject* pArgObject )
{	
	// iterate the array from the end til we find it
	u32 uFoundAt = EMaxGCObjects;

	// we do this backwards because it'll be faster with the assumed reverse unregistration order
	// N.B. tip - don't try this backwards iteration using an unsigned int...
	for( i32 iReverse = ( m_uNumGCObjects - 1) ; iReverse >= 0; --iReverse )
	{
		if( m_apGCObjects[ iReverse ] == pArgObject )
		{
			uFoundAt = iReverse;
			break;
		}
	}

	// report an error if we didn't find it
	if( EMaxGCObjects == uFoundAt )
	{
		CCLOGERROR(		"CGCObjectGroup::UnRegisterObj - failed to find object [%p] in object group. This is a BUG.", 
						pArgObject );
	}
	else
	{
		// pre-unregister for derived types
		VPreObjectUnRegister( m_apGCObjects[ uFoundAt ] );

		// shuffle the array to leave no gaps and maintain registration order
		// N.B. this will not do anything if the CGCObject was found at the end of the array
		for( u32 uCopyTo = uFoundAt; uCopyTo < ( m_uNumGCObjects - 1 ); ++uCopyTo )
		{
			m_apGCObjects[ uCopyTo ] = m_apGCObjects[ uCopyTo + 1 ];
		}

		// set the old last value to 0 and decrement
		// n.b. pre decrement of object count
		m_apGCObjects[ --m_uNumGCObjects ] = 0;
	}
}


//////////////////////////////////////////////////////////////////////////
// find a named object in this group - return NULL on not found.
//
// Allows CGCObjectGroups to be used as an object registries.
// 
// N.B. if pstrObjectName is NULL will return the 1st object of 
// idObjectClass found called from static CGCObjectManager::FindObject -> 
// CGCObjectManager::OnFindObject
//////////////////////////////////////////////////////////////////////////
CGCObject* CGCObjectGroup::OnFindObject( const char* pstrObjectName, GCTypeID idObjectClass )
{
	CGCObject* pcReturn = NULL;

	// no point even looking if we don't handle this type
	if( VHandlesThisTypeId( idObjectClass ) )
	{
		// if client code passed pstrObjectName as 0 it just wants the first match...
		if( pstrObjectName == 0 )
		{
			for( u32 uIndex = 0; uIndex < m_uNumGCObjects; ++uIndex )
			{		
				if( idObjectClass == m_apGCObjects[ uIndex ]->GetGCTypeID() )
				{
					pcReturn = m_apGCObjects[ uIndex ];
					break;
				}
			}
		}
		else
		{
			for( u32 uIndex = 0; uIndex < m_uNumGCObjects; ++uIndex )
			{
				CGCObject* pObject = m_apGCObjects[ uIndex ];

				// if there is no name getName will return 0
				const char* pstrName = pObject->GetName().c_str();

				if( pstrName && ( 0 == strcmp( pstrName, pstrObjectName ) ) )
				{
					pcReturn = pObject;
					break;
				}
			}
		}
	}

	return pcReturn;
}


//////////////////////////////////////////////////////////////////////////
// empty default implementation
// called b4 VOnObjectResourceAcquire from CGCObjectManager::Initialise 
//////////////////////////////////////////////////////////////////////////
void CGCObjectGroup::VOnGroupResourceAcquire( void )
{}

//////////////////////////////////////////////////////////////////////////
// empty default implementation
// called after VOnObjectResourceAcquire from CGCObjectManager::Initialise
//
// this is specifically provided to allow groups of objects to init their 
// anims efficiently, which has to be done after they've acquired their 
// resources hence the name of this function...
//
//virtual 
//////////////////////////////////////////////////////////////////////////
void CGCObjectGroup::VOnGroupResourceAcquire_PostObject( void )
{}

//////////////////////////////////////////////////////////////////////////
// 
//////////////////////////////////////////////////////////////////////////
// virtual
void CGCObjectGroup::VOnGroupReset( void )
{
	// clean up the current state
	// N.B. we can just empty everything other than the live list
	m_lstObjectsDead.Clear();
	m_lstObjectsToResurrect.Clear();
	KillStack_Flush();

	// objects *must* be removed from the live list explicitly to ensure 
	// correct lifetime management.
	// 
	// N.B. This is _philosophically_ correct - we DO NOT make objects dead 
	// when added to the deadlist, we make them dead when they LEAVE the 
	// live list.
	// 
	// for an example of why this is important see: CGCObjProjectile 
	// & CGCObjGroupProjectile this is a class that uses box2d 
	// physics and resets dead in its group manager
	while( !m_lstObjectsLive.IsEmpty() )
	{
		CGCObject* pToRemove = m_lstObjectsLive.GetLast();
		LiveList_Remove( pToRemove );
	}
}


//////////////////////////////////////////////////////////////////////////
// 
// N.B. IMPORTANT!! VOnKilled() is triggered by LiveList_Remove - i.e. when 
// objects are removed from the live list - NOT when added to the deadlist
// 
//////////////////////////////////////////////////////////////////////////
// virtual
void CGCObjectGroup::VOnGroupUpdate( f32 fTimeStep )
{
	// first empty the kill stack into the dead list
	while( KillStack_Count() )
	{
		CGCObject* pObjToKill = KillStack_Pop();
		DeadList_AddBack( LiveList_Remove( pObjToKill ) );
	}

	// next empty objects to resurrect into live list
	while( !m_lstObjectsToResurrect.IsEmpty() )
	{
		CGCObject* pToResurrect = m_lstObjectsToResurrect.GetLast();
		LiveList_AddBack( ResurrectList_Remove( pToResurrect ) );
	}
}


//////////////////////////////////////////////////////////////////////////
// 
//////////////////////////////////////////////////////////////////////////
// virtual
void CGCObjectGroup::VOnGroupResourceRelease( void )
{
	// clean up all the internal state
	m_lstObjectsLive.Clear();
	m_lstObjectsDead.Clear();
	m_lstObjectsToResurrect.Clear();
	KillStack_Flush();
}


//////////////////////////////////////////////////////////////////////////
// 
//////////////////////////////////////////////////////////////////////////
void CGCObjectGroup::VOnObjectResourceAcquire( void )
{
	for( u32 uIndex = 0; uIndex < m_uNumGCObjects; ++uIndex )
	{
		m_apGCObjects[ uIndex ]->VOnResourceAcquire();
	}
}


//////////////////////////////////////////////////////////////////////////
// 
//////////////////////////////////////////////////////////////////////////
// virtual
void CGCObjectGroup::VOnObjectReset( void )
{
	if( GetResetBehaviour() == EResetAlive )
	{
		for( u32 uIndex = 0; uIndex < m_uNumGCObjects; ++uIndex )
		{
			m_apGCObjects[ uIndex ]->VOnReset();
			LiveList_AddBack( m_apGCObjects[ uIndex ] );
		}
	}
	else
	{
		for( u32 uIndex = 0; uIndex < m_uNumGCObjects; ++uIndex )
		{
			m_apGCObjects[ uIndex ]->VOnReset();
			DeadList_AddBack( m_apGCObjects[ uIndex ] );			
		}
	}
}


//////////////////////////////////////////////////////////////////////////
// 
//////////////////////////////////////////////////////////////////////////
// virtual
void CGCObjectGroup::VOnObjectUpdate( f32 fTimeStep )
{
	for(	CGCObject* pCurrent = m_lstObjectsLive.GetFirst(); 
			pCurrent != 0; 
			pCurrent = m_lstObjectsLive.GetNext( pCurrent ) )
	{
		pCurrent->VOnUpdate( fTimeStep );
	}
}



//////////////////////////////////////////////////////////////////////////
// 
//////////////////////////////////////////////////////////////////////////
// virtual
void CGCObjectGroup::VOnObjectResourceRelease( void )
{
	// shut down all the managed objects
	for( i32 iReverse = ( m_uNumGCObjects - 1) ; iReverse >= 0; --iReverse )
	{
		m_apGCObjects[ iReverse ]->VOnResourceRelease();
	}
}


//////////////////////////////////////////////////////////////////////////
// call this function to kill an object:
// * will be put into the dead list at the start of the next frame
// * must be in the live list when this function is called
//////////////////////////////////////////////////////////////////////////
// virtual
void CGCObjectGroup::VOnObjectKill( CGCObject* pArgObject )
{
    CCAssert(   m_lstObjectsLive.Contains( pArgObject ), 
                "CGCObjectGroup::VOnObjectKill - only objects currently contained in the live list should be killed" );
	KillStack_Push( pArgObject );
}
                           

//////////////////////////////////////////////////////////////////////////
// call this function to resurrect an object:
// * will be put into the live list at the start of the next frame
// * must be in the dead list when this function is called
//////////////////////////////////////////////////////////////////////////
// virtual
void CGCObjectGroup::VOnObjectResurrect( CGCObject* pArgObject )
{
    CCAssert(   m_lstObjectsDead.Contains( pArgObject ), 
                "CGCObjectGroup::VOnObjectResurrect - only objects currently in the dead list should be resurrected" );
	ResurrectList_AddBack( DeadList_Remove( pArgObject ) );
}
                                          

