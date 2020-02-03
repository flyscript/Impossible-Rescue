////////////////////////////////////////////////////////////////////////////////////////////////////////////
// (C) Gamer Camp / Alex Darby 2018
// Distributed under the MIT license - see readme.md
////////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef	_GCOBJECTGROUP_H_
#define	_GCOBJECTGROUP_H_

#ifndef _NOALLOCLIST_H_
	#include "GamerCamp/Collections/NoAllocList.h"
#endif

#ifndef _GCTYPES_H_
	#include "GamerCamp/Core/GCTypes.h"
#endif

#ifndef _GCOBJECT_H_
	#include "GamerCamp/GCObject/GCObject.h"
#endif


//////////////////////////////////////////////////////////////////////////
// typedef for a no alloc list of CGCObject
typedef TNoAllocList< CGCObject > CGCObjectList;


//////////////////////////////////////////////////////////////////////////
// Base class for all CGCObjectGroup derived types
//
// CGCObjectGroup encapsulates all of the logic for managing lifetime
// of CGCObjects, and - like CGCGameManager - mirrors the interface of 
// CGCObject.
//
// CGCObjectManager requires at least one class derived from this to be 
// registered in order to work - if you don't need special behaviour use
// CGCObjGroupDefault.
//
// When a CGCObject is created it will register with CGCObjectManager.
// CGCObjectManager iterates the list of registered CGCObjectGroups until
// it finds one that handles the type of the registering CGCObject and 
// calls OnObjectRegister.
// 
// CGCGameManager is the root of the game logic and the core game logic 
// interface is made up of the following functions:
//
//	CGCObjectManager::OnResourceAcquire()
//	CGCObjectManager::OnReset()
//	CGCObjectManager::OnUpdate()
//	CGCObjectManager::OnResourceRelease()			
//
// Logical analogues to this functional interface exist in both the 
// CGCObjectGroup and CGCObject interfaces, and when these functions are 
// called on CGCObjectManager it calls the corresponding functions on 
// all registered CGCObjectGroups which then call the appropriate functions
// on their registered CGCObjects.
//
// As well as handling specific instances of CGCObject derived types for 
// GCGObjectManager, CGCObjectGroups are a handy way to manage pools of 
// objects of a specific type (avoiding dynamic allocation), and also to 
// manage the update of large groups of objects.
//
// CGCObjectGroups maintain two lists which between them contain all the 
// objects they manage a 'live list' (updated once per frame) and 
// 'dead list' (not updated). Managing these lists in a philosophically 
// correct manner is the most complicated part of CGCObjectGroup's 
// behaviour.
//
// When live objects are killed they are removed from the live list 
// and inserted into the dead list at the start of the NEXT frame; this 
// means that rather than dying instantly objects exist for all 
// interactions in the frame in which they die.
//
// Similarly, objects in the dead list (i.e. 'free' objects not currently 
// in use) can be 'resurrected' into a live state, but (like deaths) all 
// resurrections are cached during a frame and done at the start of the next 
// frame.
//
// Both of these steps happen in a pre-update pass that is done for all
// CGCObjectGroups before any CGCObjectGroups are updated. 
//
// This means that: 
// * Only the objects that are live at the beginning of a frame will be 
// updated, and... 
// * All objects that are live at the beginning of a frame will 
// be updated that frame, even if they are 'killed' at a point 
// before their VOnUpdate() function is called.
//
// Both of these are important for ensuring consistent behaviour in a 
// game system because they allow issues whereby the behaviour of game 
// logic is dependent on the order which CGCObjects update in.
//
// for example:
//
// Imagine a game where creatures (CGCGameObjects) fighting each other 
// by calculating damage and inflicting it on their opponent during their 
// update function; and an opponent that has 0 health remaining after an 
// attack tells itself to 'die'. 
//
// If we have 2 creatures (A & B) that do 50 damage per hit, and who each 
// have < 50 health left at the start of a frame; then without this guarantee 
// that all objects live at the start of the frame will update, not only 
// would whichever of A & B that updated first kill the other, but they 
// would also take no damage themselves.
//
//////////////////////////////////////////////////////////////////////////
class CGCObjectGroup
: public CNoAllocListable
{
public:
	enum
	{
		EMaxGCObjects = 1024 // constant maximum number of managed CGCObjects
	};

protected:
	// controls behaviour in OnReset
	enum EResetBehaviour
	{
		EResetAlive,	// OnReset puts objects into the live list (after calling OnResurrect)
		EResetDead		// OnReset puts objects into the dead list (after calling OnKill)
	};


private:	
	// array of registered CGCObjects. 
	// An array is used so we can preserve the original construction order to rebuild
	// m_lstLiveObjects on reset. This can't be done with two lists, as a CGCObject can
	// only be stored in one CGCObjectList at a time.
	CGCObject*		m_apGCObjects[ EMaxGCObjects ];
	u32				m_uNumGCObjects;

	EResetBehaviour m_eResetBehaviour;				// see EResetBehaviour and OnReset()

	//////////////////////////////////////////////////////////////////////////
	// managing lifetimes of objects
		CGCObjectList	m_lstObjectsLive;				// list of live objects - updated by VOnUpdate
		CGCObjectList	m_lstObjectsDead;				// list of dead objects - not updated
	
		CGCObjectList	m_lstObjectsToResurrect;		// list of objects waiting to resurrect @ start of next frame - not updated

		CGCObject*		m_apKillStack[ EMaxGCObjects ];	// stack of objects waiting to die @ end of frame
		u32				m_uKillStackCount;				// count in stack

		// put an object into the live list or the dead list
		// N.B. assume pcObject not in any other lists
		inline void			LiveList_AddBack		( CGCObject* pcObject );
		inline CGCObject*	LiveList_Remove			( CGCObject* pcObject );

		inline void			DeadList_AddBack		( CGCObject* pcObject );
		inline CGCObject*	DeadList_Remove			( CGCObject* pcObject );

		inline void			ResurrectList_AddBack	( CGCObject* pcObject );
		inline CGCObject*	ResurrectList_Remove	( CGCObject* pcObject );


		// push an object onto the killstack
		inline void			KillStack_Push		( CGCObject* pcAddToStack );
		// pop an object off the killstack
		inline CGCObject*	KillStack_Pop		( void );
		// check count in killstack
		inline u32			KillStack_Count		( void );
		// reset state of killstack
		inline void			KillStack_Flush		( void );

	// managing lifetimes of objects
	//////////////////////////////////////////////////////////////////////////

protected:
	// polymorphic post register / pre unregister functions for derived type behaviour. 
	// Both supplied with empty defaults as this is completely valid behaviour.
	virtual void VPostObjectRegister( CGCObject* pArgObject )
	{}

	virtual void VPreObjectUnRegister( CGCObject* pArgObject )
	{}

	// allows derived types to change default reset behaviour
	// defaults to putting all contained objects on the live list in OnReset()
	inline void				SetResetBehaviour( EResetBehaviour eBehaviour );
	inline EResetBehaviour	GetResetBehaviour( void );

	// check counts of objects
	inline u32				GetCountLive( void );
	inline u32				GetCountDead( void );
	inline u32				GetCountRegistered( void );

	inline const CGCObject*	GetRegisteredObjectAtIndex( unsigned int iIndex );

	// get a dead object to resurrect
	inline CGCObject*		GetDeadObject( void );

public:
	CGCObjectGroup();		
	virtual ~CGCObjectGroup();

	// returns true if this CGCObjectGroup handles the specified type id,
	// else false.
	// N.B. pure virtual in this class
	virtual bool VHandlesThisTypeId( GCTypeID idQueryType ) = 0;

	// must return the typeid of the CGCObjectGroup derived class
	// N.B. pure virtual in this class
	virtual GCTypeID VGetTypeId( void ) = 0;

	// register / unregister objects - call virtual functions internally
	void OnObjectRegister		( CGCObject* pArgObject );
	void OnObjectUnRegister		( CGCObject* pArgObject );
	
	// find a named object in this group - return NULL if not found
	//
	// N.B. if pstrObjectName is NULL will return the 1st object of idObjectClass found
	// called from static CGCObjectManager::FindObject -> CGCObjectManager::OnFindObject
	CGCObject* OnFindObject( const char* pstrObjectName, GCTypeID idObjectClass );
	
	// CGCObjectGroup interface mirrors CGCGameObject interface
	//
	// N.B. if you override OnUpdate or OnReset you must still call these base versions from
	// the derived class, since they implement key functionality 
	virtual void VOnGroupResourceAcquire			( void );			// called b4 VOnObjectResourceAcquire from CGCObjectManager::Initialise 
	virtual void VOnGroupResourceAcquire_PostObject	( void );			// called after VOnObjectResourceAcquire from CGCObjectManager::Initialise 
	virtual void VOnGroupReset						( void );			// called b4 VOnObjectResourceAcquire from CGCObjectManager::Reset
	virtual void VOnGroupUpdate						( f32 fTimeStep );	// called b4 VOnObjectResourceAcquire from CGCObjectManager::OnUpdate
	virtual void VOnGroupResourceRelease			( void );			// called AFTER VOnObjectResourceAcquire from CGCObjectManager::ShutDown

	virtual void VOnObjectResourceAcquire	( void );			// called after VOnGroupResourceAcquire from CGCObjectManager::Initialise
	virtual void VOnObjectReset				( void );			// called after VOnGroupResourceAcquire from CGCObjectManager::Reset
	virtual void VOnObjectUpdate			( f32 fTimeStep );	// called after VOnGroupResourceAcquire from CGCObjectManager::OnUpdate
	virtual void VOnObjectResourceRelease	( void );			// called after VOnGroupResourceAcquire from CGCObjectManager::ShutDown 

	// called from CGCObjectManager when an object handled by this group is killed 
	virtual void VOnObjectKill		( CGCObject* pArgObject );

	// called to resurrect an object
	virtual void VOnObjectResurrect	( CGCObject* pArgObject );


	//////////////////////////////////////////////////////////////////////////
	// 'visitor pattern' interface for accessing contained objects without breaking encapsulation

		// ForEachObjectIn_LiveList(), ForEachObjectIn_DeadList(), & ForEachObject() allow deriving classes 
		// to iterate the private live / dead lists & the entire set of registered objects
		//
		// C++11 templated visitor interface - this is intended for use with functors
		// (N.B. see struct SGCObjectGatherer below for example use)
		// tFunctor must return bool and take a CGCObject* as its argument
		//
		// The specified function will be called back once for each GCObject in the 
		// list with the void* specified, and a pointer to the CGCObject.
		//
		// N.B. You MUST not do anything destructive to the list in the callback!
		// If you need to kill objects or something, cache them and remove them 
		// after VisitListXXXX has returned.
	
		// call tFunctor once for each cGCObject managed by the list
		// N.B. tFunctor must return bool and take a CGCObject* as its argument
		template< typename TFunctor >
		void ForEachObject( TFunctor& tFunctor )
		{ 
			for( u32 uLoop = 0; uLoop < m_uNumGCObjects; ++uLoop )
			{
				// keep going until tFunctor returns false
				if( !tFunctor( m_apGCObjects[ uLoop ] ) )
				{
					break;		    
				}
			}
		}

		// call pfnVisitor with pUserData once for each cGCObject in the live list
		template< typename TFunctor >
		void ForEachObjectIn_LiveList( TFunctor& tFunctor )
		{
			for(	CGCObject* pCurrent = m_lstObjectsLive.GetFirst();
					pCurrent != 0;
					pCurrent = m_lstObjectsLive.GetNext( pCurrent ) )
			{
				// keep going until pfnVisitor returns false
				if( !tFunctor( pCurrent ) )
				{
					break;
				}
			}		
		}

		// call tFunctor once for each cGCObject in the live list by the list
		// N.B. tFunctor must return bool and take a CGCObject* as its argument
		template< typename TFunctor >
		void ForEachObject_InDeadList( TFunctor& tFunctor )
		{
			for(	CGCObject* pCurrent = m_lstObjectsDead.GetFirst();
					pCurrent != 0;
					pCurrent = m_lstObjectsDead.GetNext( pCurrent ) )
			{
				// keep going until tFunctor returns false
				if( !tFunctor( pCurrent ) )
				{
					break;
				}
			}
		}

		// iterates the managed objects BACKWARDS calling delete on them
		// 
		// N.B. the reverse order is significant as it ensures that the array
		// remains valid when the CGCObjects remove themselves on destruction
		void DestroyObjectsReverseOrder()
		{ 
			for( i32 iIndex = ( m_uNumGCObjects - 1); iIndex >=0; --iIndex )
			{
				delete m_apGCObjects[ iIndex ];
				m_apGCObjects[ iIndex ] = nullptr;				
			}
		}

		// iterates the managed objects BACKWARDS calling 
		// tFunctorCallBeforeDestructor before deleting them
		// 
		// N.B. the reverse order is significant as it ensures that the array
		// remains valid when the CGCObjects remove themselves on destruction
		//
		// N.N.B. tFunctor must take a CGCObject* as its argument
		template< typename TFunctor >
		void DestroyObjectsReverseOrder( TFunctor& tFunctorCallBeforeDestructor )
		{ 
			for( i32 iIndex = ( m_uNumGCObjects - 1); iIndex >=0; --iIndex )
			{
				tFunctorCallBeforeDestructor( m_apGCObjects[ iIndex ] );
				delete m_apGCObjects[ iIndex ];
				m_apGCObjects[ iIndex ] = nullptr;				
			}
		}
		
	// 'visitor pattern' interface for accessing contained objects without breaking encapsulation
	//////////////////////////////////////////////////////////////////////////

};


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// inline functions inline functions inline functions inline functions inline functions inline functions inline functions inline functions
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
// put an object into the live list
// N.B. assume pcObject not in any other lists
//////////////////////////////////////////////////////////////////////////
// private
inline void CGCObjectGroup::LiveList_AddBack( CGCObject* pcObject )
{
	m_lstObjectsLive.InsertBack( pcObject );
	pcObject->VOnResurrected();
}


//////////////////////////////////////////////////////////////////////////
// removes the passed object from the live list, and returns it	to allow
// chaining of this function e.g.
// AddToDeadList( RemoveFromLiveList( pObject ) );
// 
// N.B. IMPORTANT!! VOnKilled() is triggered by this function - i.e. when 
// objects are removed from the live list
// 
//////////////////////////////////////////////////////////////////////////
inline CGCObject* CGCObjectGroup::LiveList_Remove( CGCObject* pcObject )
{
	m_lstObjectsLive.Remove( pcObject );
	pcObject->VOnKilled();
	return pcObject;
}


//////////////////////////////////////////////////////////////////////////
// put an object into the dead list
// N.B. assume pcObject not in any other lists
// 
// N.B. IMPORTANT!! VOnKilled() is triggered by LiveList_Remove - i.e. when 
// objects are removed from the live list
// 
//////////////////////////////////////////////////////////////////////////
// private
inline void CGCObjectGroup::DeadList_AddBack( CGCObject* pcObject )
{
	// N.B. we DO NOT Kill objects when added to the deadlist, 
	// we make them dead when they LEAVE the live list
	m_lstObjectsDead.InsertBack( pcObject );
}


//////////////////////////////////////////////////////////////////////////
// removes the passed object from the dead list, and returns it	to allow
// chaining of this function e.g.
// AddToLiveList( RemoveFromDeadList( pObject ) );
//////////////////////////////////////////////////////////////////////////
inline CGCObject* CGCObjectGroup::DeadList_Remove( CGCObject* pcObject )
{
	m_lstObjectsDead.Remove( pcObject );
	return pcObject;
}


//////////////////////////////////////////////////////////////////////////
// put an object into the dead list
// N.B. assume pcObject not in any other lists
//////////////////////////////////////////////////////////////////////////
// private
inline void CGCObjectGroup::ResurrectList_AddBack( CGCObject* pcObject )
{
	// N.B. we DO NOT Kill objects when added to the deadlist, 
	// we make them dead when they LEAVE the live list
	m_lstObjectsToResurrect.InsertBack( pcObject );
}


//////////////////////////////////////////////////////////////////////////
// removes the passed object from the resurrect list, and returns it to 
// allow chaining of this function e.g.
// AddToLiveList( RemoveFromResurrectList( pObject ) );
//////////////////////////////////////////////////////////////////////////
inline CGCObject* CGCObjectGroup::ResurrectList_Remove( CGCObject* pcObject )
{
	m_lstObjectsToResurrect.Remove( pcObject );
	return pcObject;
}


//////////////////////////////////////////////////////////////////////////
// Called from VOnObjectKill.
// Puts the object into a stack ready to be killed at the start of the 
// next frame.
// N.B. will only add the object to the stack if it's not in it.
//////////////////////////////////////////////////////////////////////////
// private
inline void CGCObjectGroup::KillStack_Push( CGCObject* pcAddToStack )
{
	// check if it's already in the kill stack - this should be pretty fast
	for( u32 uLoop = 0; uLoop < m_uKillStackCount; ++uLoop )
	{
		if( m_apKillStack[ uLoop ] == pcAddToStack )
		{
			return;
		}
	}

	// n.b. post increment
	m_apKillStack[ m_uKillStackCount++ ] = pcAddToStack;
}


//////////////////////////////////////////////////////////////////////////
// pops the current top off the killstack, shrinking it 
//////////////////////////////////////////////////////////////////////////
// private
inline CGCObject* CGCObjectGroup::KillStack_Pop( void )
{
	// n.b. !PRE! decrement
	return m_apKillStack[ --m_uKillStackCount ];
}


//////////////////////////////////////////////////////////////////////////
// gets current count of objects in kill stack
//////////////////////////////////////////////////////////////////////////
// private
inline u32 CGCObjectGroup::KillStack_Count( void )
{
	return m_uKillStackCount;
}


//////////////////////////////////////////////////////////////////////////
// flushes the kill stack
//////////////////////////////////////////////////////////////////////////
// private
inline void CGCObjectGroup::KillStack_Flush( void )
{
	m_uKillStackCount = 0;
}


//////////////////////////////////////////////////////////////////////////
// allows derived types to change default reset behaviour
// defaults to putting all contained objects on the live list in OnReset()
//////////////////////////////////////////////////////////////////////////
// protected
inline void CGCObjectGroup::SetResetBehaviour( EResetBehaviour eBehaviour )
{
	m_eResetBehaviour = eBehaviour;
}


//////////////////////////////////////////////////////////////////////////
// returns the current reset behaviour
//////////////////////////////////////////////////////////////////////////
// protected
inline CGCObjectGroup::EResetBehaviour CGCObjectGroup::GetResetBehaviour( void )
{
	return m_eResetBehaviour;
}


//////////////////////////////////////////////////////////////////////////
// returns the tail of the deadlist - will be NULL if empty
//////////////////////////////////////////////////////////////////////////
// protected
inline CGCObject* CGCObjectGroup::GetDeadObject( void )
{
	return m_lstObjectsDead.GetLast();
}


//////////////////////////////////////////////////////////////////////////
// count of current live list
//////////////////////////////////////////////////////////////////////////
// protected
inline u32 CGCObjectGroup::GetCountLive( void )
{
	return m_lstObjectsLive.GetCount();
}


//////////////////////////////////////////////////////////////////////////
// count of current dead list
//////////////////////////////////////////////////////////////////////////
// protected
inline u32 CGCObjectGroup::GetCountDead( void )
{
	return m_lstObjectsDead.GetCount();
}


//////////////////////////////////////////////////////////////////////////
// total count of registered objects
//////////////////////////////////////////////////////////////////////////
// protected
inline u32 CGCObjectGroup::GetCountRegistered( void )
{
	return m_uNumGCObjects;
}


//////////////////////////////////////////////////////////////////////////
// get iIndex-th registered object
//////////////////////////////////////////////////////////////////////////
// protected
inline const CGCObject*	CGCObjectGroup::GetRegisteredObjectAtIndex( unsigned int uIndex )
{
	if( uIndex < m_uNumGCObjects )
	{
		return m_apGCObjects[ uIndex ];
	}
	return nullptr;
}

#endif
