////////////////////////////////////////////////////////////////////////////////////////////////////////////
// (C) Gamer Camp / Alex Darby 2018
// Distributed under the MIT license - see readme.md
////////////////////////////////////////////////////////////////////////////////////////////////////////////
#include "base/ccMacros.h"

#include "GamerCamp/GCObject/GCObjectManager.h"
#include "GamerCamp/GCObject/GCObject.h"


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// initialisation of static class variables
// private
CGCObjectManager* CGCObjectManager::sm_pActiveObjectManager = NULL;



//////////////////////////////////////////////////////////////////////////
//
// N.B. default Object group is set by registering one that handles type GCTYPEID_INVALID
//////////////////////////////////////////////////////////////////////////
// private
void CGCObjectManager::OnObjectGroupRegister( CGCObjectGroup* pArgObjGroup )
{
	// should really check that no other group is registered which 
	// handles the same type and return an error code...
	if( pArgObjGroup->VHandlesThisTypeId( GCTYPEID_INVALID ) )
	{
		CCAssert( !m_pDefaultObjectGroup, "only 1 default object group (i.e. handles GCTYPEID_INVALID) may be registered" );
		m_pDefaultObjectGroup = pArgObjGroup;
	}
	else
	{
		m_lstObjectGroups.InsertBack( pArgObjGroup );
	}
}



//////////////////////////////////////////////////////////////////////////
// called by static fn ObjectGroupUnRegister
//////////////////////////////////////////////////////////////////////////
// private
void CGCObjectManager::OnObjectGroupUnRegister( CGCObjectGroup* pArgObjGroup )
{
	if( pArgObjGroup == m_pDefaultObjectGroup )
	{
		m_pDefaultObjectGroup = NULL;
	}
	else
	{
		m_lstObjectGroups.Remove( pArgObjGroup );
	}
}



//////////////////////////////////////////////////////////////////////////
// called by static fn FindObject
// allows CGCObjectManager to be used as an object registry
// if pstrObjectName is NULL will return the 1st object of idObjectClass found
// returns NULL if not found
//////////////////////////////////////////////////////////////////////////
// private
CGCObject* CGCObjectManager::OnFindObject( const char* pstrObjectName, GCTypeID idObjectClass )
{
	CGCObject* pcReturn = NULL;
	
	if( m_pDefaultObjectGroup )
	{
		// CGCObjectGroup::OnFindObject returns NULL if not found
		pcReturn = m_pDefaultObjectGroup->OnFindObject( pstrObjectName, idObjectClass );
	}

	if( !pcReturn )
	{
		for(	CGCObjectGroup* pCurrent = m_lstObjectGroups.GetFirst();
				pCurrent != 0;
				pCurrent = m_lstObjectGroups.GetNext( pCurrent ) )
		{
			pcReturn = pCurrent->OnFindObject( pstrObjectName, idObjectClass );

			// stop looking as soon as we find it
			if( pcReturn )
			{
				break;
			}
		}
	}

	return pcReturn;
}



//////////////////////////////////////////////////////////////////////////
// helper function for registering objects
//////////////////////////////////////////////////////////////////////////
// private
CGCObjectGroup* CGCObjectManager::GetObjectGroupThatHandles( GCTypeID idManagedType ) 
{
	CGCObjectGroup* pTypeHandler = m_pDefaultObjectGroup;

	for(	CGCObjectGroup* pCurrent = m_lstObjectGroups.GetFirst();
			pCurrent != 0;
			pCurrent = m_lstObjectGroups.GetNext( pCurrent ) )
	{
		if( pCurrent->VHandlesThisTypeId( idManagedType ) )
		{
			pTypeHandler = pCurrent;
			break;
		}
	}
	return pTypeHandler;
}
	


//////////////////////////////////////////////////////////////////////////
// helper function for finding a group by its GCTypeID
//////////////////////////////////////////////////////////////////////////
// private
CGCObjectGroup* CGCObjectManager::GetObjectGroupByID( GCTypeID idOfObjectGroup )
{
	CGCObjectGroup* pObjectGroup = NULL;

	// n.b. lazy && checks pointer and only uses it if it's valid
	if( m_pDefaultObjectGroup && ( m_pDefaultObjectGroup->VGetTypeId() == idOfObjectGroup ) )
	{
		pObjectGroup = m_pDefaultObjectGroup;
	}
	else
	{
		for(	CGCObjectGroup* pCurrent = m_lstObjectGroups.GetFirst();
				pCurrent != 0;
				pCurrent = m_lstObjectGroups.GetNext( pCurrent ) )
		{
			if( pCurrent->VGetTypeId() == idOfObjectGroup )
			{
				pObjectGroup = pCurrent;
				break;
			}
		}
	}

	return pObjectGroup;
}



//////////////////////////////////////////////////////////////////////////
// constructor - sets singleton pointer
//////////////////////////////////////////////////////////////////////////
CGCObjectManager::CGCObjectManager()
: m_pDefaultObjectGroup ( 0 )
{
}



//////////////////////////////////////////////////////////////////////////
// destructor - unsets singleton pointer
//////////////////////////////////////////////////////////////////////////
CGCObjectManager::~CGCObjectManager()
{
	// make sure we don't leave ActiveInstance as a dangling pointer...
	if( sm_pActiveObjectManager == this )
	{
		sm_pActiveObjectManager = NULL;
	}
}



//////////////////////////////////////////////////////////////////////////
// iterate registered instances and initialise them
//////////////////////////////////////////////////////////////////////////
// static 
void CGCObjectManager::OnResourceAcquire( void )
{
	CGCObjectManager* pThis = ActiveInstance();

	////////////////////////////////////////////////////////////////////////
	// acquire group resources first
	// N.B. special case before general
	for(	CGCObjectGroup* pCurrent = pThis->m_lstObjectGroups.GetFirst(); 
			pCurrent != 0; 
			pCurrent = pThis->m_lstObjectGroups.GetNext( pCurrent ) )
	{
		pCurrent->VOnGroupResourceAcquire();
	}

	if( pThis->m_pDefaultObjectGroup )
	{
		pThis->m_pDefaultObjectGroup->VOnGroupResourceAcquire();
	}

	////////////////////////////////////////////////////////////////////////
	// N.B. special case before general
	for(	CGCObjectGroup* pCurrent = pThis->m_lstObjectGroups.GetFirst(); 
			pCurrent != 0; 
			pCurrent = pThis->m_lstObjectGroups.GetNext( pCurrent ) )
	{
		pCurrent->VOnObjectResourceAcquire();
	}

	if( pThis->m_pDefaultObjectGroup )
	{
		pThis->m_pDefaultObjectGroup->VOnObjectResourceAcquire();
	}

	////////////////////////////////////////////////////////////////////////
	// post object acquire phase
	// N.B. special case before general
	for(	CGCObjectGroup* pCurrent = pThis->m_lstObjectGroups.GetFirst(); 
			pCurrent != 0; 
			pCurrent = pThis->m_lstObjectGroups.GetNext( pCurrent ) )
	{
		pCurrent->VOnGroupResourceAcquire_PostObject();
	}

	if( pThis->m_pDefaultObjectGroup )
	{
		pThis->m_pDefaultObjectGroup->VOnGroupResourceAcquire_PostObject();
	}
}



//////////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////////
// static 
void CGCObjectManager::OnReset( void )
{
	CGCObjectManager* pThis = ActiveInstance();

	////////////////////////////////////////////////////////////////////////
	// reset groups first
	for(	CGCObjectGroup* pCurrent = pThis->m_lstObjectGroups.GetFirst(); 
			pCurrent != 0; 
			pCurrent = pThis->m_lstObjectGroups.GetNext( pCurrent ) )
	{
		pCurrent->VOnGroupReset();
	}

	if( pThis->m_pDefaultObjectGroup )
	{
		pThis->m_pDefaultObjectGroup->VOnGroupReset();
	}

	////////////////////////////////////////////////////////////////////////
	// and then their contained objects
	for(	CGCObjectGroup* pCurrent = pThis->m_lstObjectGroups.GetFirst(); 
			pCurrent != 0; 
			pCurrent = pThis->m_lstObjectGroups.GetNext( pCurrent ) )
	{
		pCurrent->VOnObjectReset();
	}

	if( pThis->m_pDefaultObjectGroup )
	{
		pThis->m_pDefaultObjectGroup->VOnObjectReset();
	}
}



//////////////////////////////////////////////////////////////////////////
// note 2 phase update - preupdate followed by update
//////////////////////////////////////////////////////////////////////////
//static 
void CGCObjectManager::OnUpdate( f32 fTimeStep )
{
	CGCObjectManager* pThis = ActiveInstance();

	//////////////////////////////////////////////////////////////////////////
	// In order to correctly manage the lifetimes of objects we need a 
	// separate pre-update pass for object groups 
	for(	CGCObjectGroup* pCurrent = pThis->m_lstObjectGroups.GetFirst(); 
			pCurrent != 0; 
			pCurrent = pThis->m_lstObjectGroups.GetNext( pCurrent ) )
	{
		pCurrent->VOnGroupUpdate( fTimeStep );
	}

	if( pThis->m_pDefaultObjectGroup )
	{
		pThis->m_pDefaultObjectGroup->VOnGroupUpdate( fTimeStep );
	}

	//////////////////////////////////////////////////////////////////////////
	// update all the objects
	for(	CGCObjectGroup* pCurrent = pThis->m_lstObjectGroups.GetFirst(); 
			pCurrent != 0; 
			pCurrent = pThis->m_lstObjectGroups.GetNext( pCurrent ) )
	{
		pCurrent->VOnObjectUpdate( fTimeStep );
	}

	if( pThis->m_pDefaultObjectGroup )
	{
		pThis->m_pDefaultObjectGroup->VOnObjectUpdate( fTimeStep );
	}
}



//////////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////////
//static 
void CGCObjectManager::OnResourceRelease( void )
{
	CGCObjectManager* pThis = ActiveInstance();

	////////////////////////////////////////////////////////////////////////
	// N.B. reverse of initialise - objects before groups
	if( pThis->m_pDefaultObjectGroup )
	{
		pThis->m_pDefaultObjectGroup->VOnObjectResourceRelease();
	}

	for(	CGCObjectGroup* pCurrent = pThis->m_lstObjectGroups.GetLast(); 
			pCurrent != 0; 
			pCurrent = pThis->m_lstObjectGroups.GetPrevious( pCurrent ) )
	{
		pCurrent->VOnObjectResourceRelease();
	}

	////////////////////////////////////////////////////////////////////////
	// groups
	if( pThis->m_pDefaultObjectGroup )
	{
		pThis->m_pDefaultObjectGroup->VOnGroupResourceRelease();
	}

	for(	CGCObjectGroup* pCurrent = pThis->m_lstObjectGroups.GetLast(); 
			pCurrent != 0; 
			pCurrent = pThis->m_lstObjectGroups.GetPrevious( pCurrent ) )
	{
		pCurrent->VOnGroupResourceRelease();
	}
}



//////////////////////////////////////////////////////////////////////////
// N.B. default Object group is set by registering one that handles type GCTYPEID_INVALID
//////////////////////////////////////////////////////////////////////////
//static 
void CGCObjectManager::ObjectGroupRegister( CGCObjectGroup* pArgGroup )
{
	if( NULL != ActiveInstance() )
	{
		ActiveInstance()->OnObjectGroupRegister( pArgGroup );
	}
	else
	{
		CCLOGERROR( "CGCObjectManager::ObjectGroupRegister - no active CGCObjectManager!" );
	}
}



//////////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////////
//static 
void CGCObjectManager::ObjectGroupUnRegister( CGCObjectGroup* pArgGroup )
{
	if( NULL != ActiveInstance() )
	{
		ActiveInstance()->OnObjectGroupUnRegister( pArgGroup );
	}
	else
	{
		CCLOGERROR( "CGCObjectManager::ObjectGroupUnRegister - no active CGCObjectManager!" );
	}
}



//////////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////////
//static 
CGCObjectGroup* CGCObjectManager::FindObjectGroupThatHandles( GCTypeID idManagedByGroup )
{
	CGCObjectGroup* pcReturn = NULL;
	if( NULL != ActiveInstance() )
	{
		pcReturn = ActiveInstance()->GetObjectGroupThatHandles( idManagedByGroup );
	}
	else
	{
		CCLOGERROR( "CGCObjectManager::FindObjectGroupThatHandles - no active CGCObjectManager!" );
	}

	return pcReturn;
}



//////////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////////
//static 
CGCObjectGroup* CGCObjectManager::FindObjectGroupByID( GCTypeID idOfObjectGroup )
{
	CGCObjectGroup* pcReturn = NULL;
	if( NULL != ActiveInstance() )
	{
		pcReturn = ActiveInstance()->GetObjectGroupByID( idOfObjectGroup );
	}
	else
	{
		CCLOGERROR( "CGCObjectManager::FindObjectGroupByID - no active CGCObjectManager!" );
	}

	return pcReturn;
}



//////////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////////
//static 
void CGCObjectManager::ObjectRegister( CGCObject* pArgObject )
{
	if( NULL != ActiveInstance() )
	{
		CCAssert( pArgObject, "CGCObjectManager::ObjectRegister - pArgObject is a bad pointer!" );
		CCAssert(	ActiveInstance()->m_pDefaultObjectGroup, 
						"CGCObjectManager::ObjectRegister - m_pDefaultObjectGroup is not set!" );
		ActiveInstance()->GetObjectGroupThatHandles( pArgObject->GetGCTypeID() )->OnObjectRegister( pArgObject );
	}
	else
	{
		CCLOGERROR( "CGCObjectManager::ObjectUnRegister - no active CGCObjectManager!" );
	}
}



//////////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////////
//static 
void CGCObjectManager::ObjectUnRegister( CGCObject* pArgObject )
{
	if( NULL != ActiveInstance() )
	{
		CCAssert( pArgObject, "CGCObjectManager::ObjectUnRegister - pArgObject is a bad pointer!" );
		CCAssert(	ActiveInstance()->m_pDefaultObjectGroup, 
						"CGCObjectManager::ObjectUnRegister - m_pDefaultObjectGroup is not set!" );
		ActiveInstance()->GetObjectGroupThatHandles( pArgObject->GetGCTypeID() )->OnObjectUnRegister( pArgObject );
	}
	else
	{
		CCLOGERROR( "CGCObjectManager::OnObjectUnRegister - no active CGCObjectManager!" );
	}
}



//////////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////////
//static 
CGCObject* CGCObjectManager::FindObject( const char* pstrObjectName, GCTypeID idObjectClass )
{
	CGCObject* pcReturn = NULL;

	if( NULL != ActiveInstance() )
	{
		pcReturn = ActiveInstance()->OnFindObject( pstrObjectName, idObjectClass );
	}
	else
	{
		CCLOGERROR( "CGCObjectManager::OnObjectUnRegister - no active CGCObjectManager!" );
	}

	return pcReturn;
}



//////////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////////
//static 
void CGCObjectManager::ObjectKill( CGCObject* pArgObject )
{
	if( NULL != ActiveInstance() )
	{
		CCAssert( pArgObject, "CGCObjectManager::ObjectKill - pArgObject is a bad pointer!" );
		CCAssert(	ActiveInstance()->m_pDefaultObjectGroup, 
						"CGCObjectManager::ObjectKill - m_pDefaultObjectGroup is not set!" );
		ActiveInstance()->GetObjectGroupThatHandles( pArgObject->GetGCTypeID() )->VOnObjectKill( pArgObject );
	}
	else
	{
		CCLOGERROR( "CGCObjectManager::ObjectKill- no active CGCObjectManager!" );
	}
}


//////////////////////////////////////////////////////////////////////////
// calls OnUpdate on singleton
//////////////////////////////////////////////////////////////////////////
//static 
void CGCObjectManager::Update( f32 fArgTimeStep )
{
	if( NULL != ActiveInstance() )
	{
		// ITickable::smTickSec is the system variable for the time between ticks
		ActiveInstance()->OnUpdate( fArgTimeStep );
	}
	else
	{
		CCLOGERROR( "CGCObjectManager::Update - no active CGCObjectManager! this is a bug!" );
	}
}



