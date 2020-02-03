////////////////////////////////////////////////////////////////////////////////////////////////////////////
// (C) Gamer Camp / Alex Darby 2018
// Distributed under the MIT license - see readme.md
////////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef _GCOBJECT_H_
#define _GCOBJECT_H_

#include <string>

#ifndef _NOALLOCLIST_H_
	#include "GamerCamp/Collections/NoAllocList.h"
#endif

#ifndef _GCTYPES_H_
	#include "GamerCamp/Core/GCTypes.h"
#endif


//////////////////////////////////////////////////////////////////////////
// This class forms the basis of the GC template game code operation.
//
// It is only ever created by derived types.
//
// Instances of CGCObject register themselves with CGCObjectManager on
// creation
//
// CGCObjectManager manages CGCObjects via CGCObjectGroup and their 
// logically equivalent polymorphic interfaces - see the function comments 
// below for more detail.
//
// To see exactly when and how these functions will be called, and for a 
// 'big picture' view of how the game logic update works see
// GCObjectManager.h, and GCObjectGroup.h.
//
//////////////////////////////////////////////////////////////////////////
class CGCObject 
: public CNoAllocListable	// N.B. this gives the ability for this type to be stored in an allocation free list 
{
private:
	GCTypeID			m_idConcreteClass;	// this stores the type id of the derived class
	std::string 		m_ccstrName;

	// private default constructor
	CGCObject( void );

	// prevent copy
	CGCObject( const CGCObject& );
	CGCObject& operator=( const CGCObject& );

protected:
	// only deriving classes may construct this class, and must pass their GCTypeID.
	// Deriving classes must use GetGCTypeIDOf( <typename> ) to set this value,
	// see the constructor of CGCObjPlayer for a demo of how you do this.
	CGCObject( GCTypeID idDerivedClass );


public:
	//////////////////////////////////////////////////////////////////////////
	// we need a virtual destructor since delete will be called on pointers of 
	// this class to delete derived types.
	virtual ~CGCObject();


	//////////////////////////////////////////////////////////////////////////
	// accessors for member data

	// m_idConcreteClass is set in the constructor CGCObject( GCTypeID idDerivedClass ).
	// Deriving classes must use GetGCTypeIDOf( <typename> ) to set this value.
	// Assuming this is done, it is simple to check which derived type a CGCObject* in
	// fact points to by checking GetGCTypeID() against GetGCTypeIDOf( <typename> ) and
	// performing the appropriate static_cast<>
	GCTypeID GetGCTypeID( void ) const
	{
		return m_idConcreteClass;
	}

	// gets this instance's name
	const std::string& GetName( void )
	{
		return m_ccstrName;
	}

	// sets this instance's	name
	// N.B. this works because there are lots of copy constructors defined for std::string
	// i.e. at least from another std::string and const char* which are the main use cases...
	template< typename TStringType >
	void SetName( TStringType tName )
	{
		m_ccstrName = tName;
	}

	//////////////////////////////////////////////////////////////////////////
	// game object interface

		// This will be called exactly once for each CGCObject derived class instance 
		// that is registered with CGCObjectManager when CGCObjectManager::OnResourceAcquire()
		// is called.
		//
		// No resources should be acquired (i.e. dynamic memory / loading etc.) after this function is called
		virtual void VOnResourceAcquire( void )
        {}

		// OnReset() is called once after OnResourceAcquire(), and then again 
		// whenever CGCObjectManager is asked to reset. 	    
		// This is NOT construction!!
		// No dynamic allocation should happen as a result of a call to this function.
		// This function should put the object back into its post creation state.
		//
		// N.B. pure virtual - deriving types must implement this.
		//
		virtual void VOnReset( void ) = 0;

		// OnUpdate is called for each CGCObject derived class when CGCObjectManager is updated
		//
		// An empty default version is supplied, as doing nothing is a valid choice.
		//
		// An empty default version is supplied, as doing nothing is a valid choice.
		//
		virtual void VOnUpdate( float fTimeStep )
		{}

		// OnResourceRelease is the opposite of OnResourceAcquire, this will get called exactly once
		// for each instance of a CGCObject derived type that is registered
		// with CGCObjectManager when CGCObjectManager::Shutdown() is called.
		//
		// Any resources acquired in OnResourceAcquire should be released here.
		//
		virtual void VOnResourceRelease( void )
        {}

		// OnKilled is called when this object is added to its corresponding CGCObjectGroup's 
		// dead list after a call to CGCObjectManager::ObjectKill().
		// Once this has been called, the object will not receive any updates until after
		// it has been placed back onto the live list following a call to VOnResurrected.
		// THIS IS NOT A DESTRUCTOR, this simply means this instance is no longer "live".
		//
		// An empty default version is supplied, as doing nothing is a valid choice.
		//
		virtual void VOnKilled( void )
		{}

		// OnResurrected is called when this object is added back into the live list following 
		// a call to its containing CGCObjectGroup's VOnObjectResurrect().
		//
		// An empty default version is supplied, as doing nothing is a valid choice.
		//
		virtual void VOnResurrected( void )
		{}

	// game object interface
	//////////////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////////////
	// safely cast a pointer to a GCObject derived type to its EXACT type
	// returns nullptr if instance type doesn't match
	//////////////////////////////////////////////////////////////////////////
	template< class TGCObjectOrDerivedPtr >
	static TGCObjectOrDerivedPtr SafeCastToDerived( CGCObject* pGCObjectDerived )
	{
		if( pGCObjectDerived->GetGCTypeID() == TGCTypeIDGenerator< std::remove_pointer< TGCObjectOrDerivedPtr >::type >::GetTypeID() )
		{
			return static_cast< TGCObjectOrDerivedPtr >( pGCObjectDerived );
		}
		return nullptr;
	}
};
#endif