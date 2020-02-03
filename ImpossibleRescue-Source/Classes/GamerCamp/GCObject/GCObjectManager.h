////////////////////////////////////////////////////////////////////////////////////////////////////////////
// (C) Gamer Camp / Alex Darby 2018
// Distributed under the MIT license - see readme.md
////////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef	_GCOBJECTMANAGER_H_
#define	_GCOBJECTMANAGER_H_

#ifndef _NOALLOCLIST_H_
	#include "GamerCamp/Collections/NoAllocList.h"
#endif

#ifndef _GCTYPES_H_
	#include "GamerCamp/Core/GCTypes.h"
#endif

#ifndef _GCOBJECTGROUP_H_
	#include "GamerCamp/GCObject/GCObjectGroup.h"
#endif


//////////////////////////////////////////////////////////////////////////
// forward declare CGCObject
class CGCObject;

//////////////////////////////////////////////////////////////////////////
// typedef for a no alloclist of CGCObjectGroup
typedef TNoAllocList< CGCObjectGroup > CGCObjectGroupList;


//////////////////////////////////////////////////////////////////////////
// CGCObjectManager is the root of all the gamercamp game logic code
//
// All CGCObjectGroup derived types register themselves with this class
// on construction, and this class is responsible for managing them via 
// the CGCObject interface.
//
// CGCObjectManager maintains one (or more) object groups which are used 
// to contain and manage the lifetimes of all CGCObject derived types.
//
// Unlike CGCObjects CGCObjectGroups must be manually registered and 
// unregistered with CGCObjectManager (this is to do with the necessity to 
// call virtual functions during the registration process which precludes
// doing it during construction).
//
// CGCObjects are assigned to a CGCObjectGroup at registration based on 
// their class id (see GCTypes.h for the class id code). CGCObjectGroups 
// can be queried to determine which sub-types of CGCObject they can contain.
//
// A 'default object group' must be supplied which will handle any type of
// CGCObject. It signifies that it is 'default' by handling the type id
// GCTYPEID_INVALID.
//
// 
//
//////////////////////////////////////////////////////////////////////////
class CGCObjectManager
{
private:
	CGCObjectGroupList	m_lstObjectGroups;		// list of CGCObjectGroups, used to manage objects
	CGCObjectGroup*		m_pDefaultObjectGroup;	// special case object group for default object handling	

	// called by static fn ObjectGroupRegister
	void OnObjectGroupRegister( CGCObjectGroup* pArgObjGroup );					
	
	// called by static fn ObjectGroupUnRegister
	void OnObjectGroupUnRegister( CGCObjectGroup* pArgObjGroup );					
	
	// called by static fn FindObject
	// allows CGCObjectManager to be used as an object registry
	// returns NULL if not found
	CGCObject* OnFindObject( const char* pstrObjectName, GCTypeID idObjectClass );	

	// helper function for registering objects
	CGCObjectGroup* GetObjectGroupThatHandles( GCTypeID idManagedType );

	// helper function for finding a group by its GCTypeID
	CGCObjectGroup* GetObjectGroupByID( GCTypeID idOfObjectGroup );
	
//////////////////////////////////////////////////////////////////////////
// "active object manager" - allows singleton-esque interface
// allows us to keep singleton style access syntax, but > 1 CGCObjectManager to exist 
private:
	// the currently active object manager
	static CGCObjectManager* sm_pActiveObjectManager;

	// accessor for active instance
	inline static CGCObjectManager* ActiveInstance( void );

public:
	// can be created and deleted freely
	CGCObjectManager	();
	~CGCObjectManager	();

	// sets the instance as the 'active' object manager
	// all static functions operate on the active object manager
	inline void SetAsActiveObjectManager( void );

// "active object manager" - allows singleton-esque interface
//////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////
// static singleton-esque interface
// operates on the *active* CGCObjectManager

	// this function is the root of the update for all CGCObjectGroups, and CGCObjects
	static void	Update( f32 fArgTimeStep );


	//////////////////////////////////////////////////////////////////////////
	// public interface - connection point to IGCGameLayer
	// N.B. the interface of CGCObject is mirrored in CGCObjectManager

		// This function should be called after all CGCObjects have been created.
		// All CGCObjects must be created after all CGCObjectGroups.
		// No resource acquisition (loading data, dynamic memory etc.) should happen after this function.
		static void OnResourceAcquire	( void );			

		// This should be called directly after OnResourceAcquire to set the initial state
		// of all game objects. This function can be called at any time to set the 
		// game back to its initial post initialisation state. 
		static void OnReset			    ( void );			

		// This should be called once per game frame
 		static void OnUpdate		    ( f32 fTimeStep );	

		// This function is the logical opposite of OnResourceAcquire.
		// All resources acquired in OnResourceAcquire should be release here.
		static void OnResourceRelease	( void );			

	// public interface
	//////////////////////////////////////////////////////////////////////////


	//////////////////////////////////////////////////////////////////////////
	// CGCObjectGroup handling

		// N.B. default Object group is set by registering one that handles type GCTYPEID_INVALID
		static void ObjectGroupRegister		( CGCObjectGroup* pArgGroup );
		static void ObjectGroupUnRegister	( CGCObjectGroup* pArgGroup );

		// Allows object groups to be retrieved by the types they handle
		// i.e. allows CGCObjectManager to be used as an object registry for CGCObjectGroup 
		// derived types as well as CGCObject derived ones.
		static CGCObjectGroup* FindObjectGroupThatHandles( GCTypeID idManagedByGroup );

		// Allows an object group to be retrieved by its type (as opposed to the type it handles) 
		// i.e. allows CGCObjectManager to be used as an object registry for CGCObjectGroup 
		// derived types as well as CGCObject derived ones.
		static CGCObjectGroup* FindObjectGroupByID( GCTypeID idOfObjectGroup );

	// CGCObjectGroup handling
	//////////////////////////////////////////////////////////////////////////


	//////////////////////////////////////////////////////////////////////////
	// Game Object Handling
		
		// called by CGCObject::CGCObject() to register with CGCObjectManager
		static void			ObjectRegister	( CGCObject* pArgObject );

		// called by CGCObject::~CGCObject() to register with CGCObjectManager
		static void			ObjectUnRegister( CGCObject* pArgObject );

		// allows CGCObjectManager to be used as an object registry
		// if pstrObjectName is NULL will return the 1st object of idObjectClass found
		// returns NULL if not found
		static CGCObject*	FindObject		( const char* pstrObjectName, GCTypeID idObjectClass );

		// called during gameplay to remove from the update list
		static void			ObjectKill		( CGCObject* pArgObject );

	// Game Object Handling
	//////////////////////////////////////////////////////////////////////////

// static singleton interface
//////////////////////////////////////////////////////////////////////////
};



//////////////////////////////////////////////////////////////////////////
// internal accessor for the active instance pointer
//////////////////////////////////////////////////////////////////////////
// private, static 
inline CGCObjectManager* CGCObjectManager::ActiveInstance( void )
{
	return sm_pActiveObjectManager;
}


//////////////////////////////////////////////////////////////////////////
// sets the instance as the 'active' object manager
// all static functions operate on the active object manager
//////////////////////////////////////////////////////////////////////////
inline void CGCObjectManager::SetAsActiveObjectManager( void )
{
	sm_pActiveObjectManager = this;
}


#endif
