////////////////////////////////////////////////////////////////////////////////////////////////////////////
// (C) Gamer Camp / Alex Darby 2019
// Distributed under the MIT license - see readme.md
////////////////////////////////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
#ifndef _GCINPUTCONTROLLER_H_
#define _GCINPUTCONTROLLER_H_

#include <type_traits>

#ifndef _GCINPUTCONTROLLERMANAGER_H_
	#include "GamerCamp/GameController/GCControllerManager.h"
#endif

#include "GamerCamp/Core/GCTypes.h"

#include "platform/CCGLView.h"
#include "cocos2d/external/glfw3/include/win32/glfw3.h"
#include "cocos/base/CCController.h"



#if defined (WIN32)

using namespace cocos2d;


//////////////////////////////////////////////////////////////////////////
// thin wrapper around a controllerId to make the code checking
// CGCControllerManager less ugly / verbose
// 
// n.b. the intent is that you should treat this as a value type
// 
//////////////////////////////////////////////////////////////////////////
class CGCController
{
public:
					CGCController					( CGCControllerManager::EControllerId eControllerId );
					~CGCController					();
	
	bool			IsActive						();	 // actually only inlined in the .cpp as definition isn't visible

	void			Reset							();

	bool			ButtonIsPressed					( Controller::Key eKeyCode );

	bool			ButtonHasJustBeenPressed		( Controller::Key eKeyCode );
	bool			ButtonHasJustBeenReleased		( Controller::Key eKeyCode );

	f32				GetCurrentAxisValueRaw			( Controller::Key eAxisKeyCode );
	f32				GetCurrentAxisDeadzoned			( Controller::Key eAxisKeyCode, float fDeadzone );

	cocos2d::Vec2	GetCurrentStickValueRaw			( Controller::Key eAxisKeyCodeX, Controller::Key eAxisKeyCodeY );
	cocos2d::Vec2	GetCurrentStickValueDeadzoned	( Controller::Key eAxisKeyCodeX, Controller::Key eAxisKeyCodeY, float fDeadzone );

private:

	CGCControllerManager::EControllerId m_eControllerId;

	CGCController() = delete;
};



//////////////////////////////////////////////////////////////////////////
// templated action to key map which allows an arbitrary (but unique!) set
// of client code defined enum values to map to the underlying cocos2d
// controller enum cocos2d::Controller::Key
// 
// this is largely for readability / convenience - e.g. instead of 
// cocos2d::Controller::BUTTON_A we might have EBUTTON_INTERACT
// 
// instances of this type are created using a friend function - again 
// a nicety reducing ugliness of code & likelihood of getting stuck 
// trying to correctly initialise an instance of the type
// 
// NOTE: this type contains a std::unordered_map so should very much be 
// treated as a "create once at init" sort of object for performance
// and/or memory useage reasons
// 
// need to forward declare both the template AND the template function 
// used to create instances of it so the fn can be declared friend
template< typename TActionType > class TGCActionToKeyMap;

template< typename TActionType, u32 uArraySize >
TGCActionToKeyMap< TActionType >* TCreateActionToKeyMap( const TActionType( &aeActions )[ uArraySize ], const Controller::Key( &aeCocosKeys )[ uArraySize ] );

//////////////////////////////////////////////////////////////////////////
template< typename TActionType >
class TGCActionToKeyMap
{
private:
	std::unordered_map< TActionType, cocos2d::Controller::Key > m_mapActionToKey;

	// default constructor deleted and only other constructor private
	TGCActionToKeyMap()	= delete;
	TGCActionToKeyMap( const TActionType aeActions[], const Controller::Key aeCocosKeys[], u32 uArraySize );

public:

	Controller::Key GetKeyForAction( TActionType eAction );


	// friend function used to create instances of this type
	template< typename TActionType, u32 uArraySize >
	friend TGCActionToKeyMap< TActionType >* TCreateActionToKeyMap( const TActionType( &aeActions )[ uArraySize ], const Controller::Key( &aeCocosKeys )[ uArraySize ] );
};

// template function to auto-deduce the action type and array sizes and generally make creating the action map less ugly  / error prone
// NOTE: this dynamically allocates an instance of TGCActionToKeyMap which the calling code is responsible for deleting
template< typename TActionType, u32 uArraySize >
TGCActionToKeyMap< TActionType >* TCreateActionToKeyMap( const TActionType		(&aeActions)	[ uArraySize ], 
														const Controller::Key	(&aeCocosKeys)	[ uArraySize ] )
{
	return new TGCActionToKeyMap< TActionType >( aeActions, aeCocosKeys, uArraySize );
}


//////////////////////////////////////////////////////////////////////////
// template class which wraps CGCController: 
// 
//	a) makes it more elegant to declare & use CGCController
//	
// 	b) allows you to make an action map from a user defined enum to the 
// 	underlying cocos2d axis key codes so that they can have names which 
// 	make semantic in the game code
//		
//	uses TGCActionToKeyMap to map the actions
//	
//	this type is intended to be treated essentially as a value type.
//	
//	It only takes a reference to the instance of TGCActionToKeyMap it uses
//	so it has very small memory footprint.
//	
//	NOTE: if the referenced TGCActionToKeyMap is freed before this class
//	this is likely to cause issues!
//		
// again it's created via a templated function to reduce typing / ugliness
// so we need to forward declare the friend function and the type
template< typename TActionType > class TGCController;

template< typename TActionType >
TGCController< TActionType > TGetActionMappedController( CGCControllerManager::EControllerId eControllerId, TGCActionToKeyMap< TActionType >& rActionToKeyMap );


//////////////////////////////////////////////////////////////////////////
template< typename TActionType >
class TGCController : public CGCController
{
private:
	TGCActionToKeyMap< TActionType >& m_rActionToKeyMap;

	// default constructor deleted and only other constructor private
	TGCController() = delete;
	TGCController( CGCControllerManager::EControllerId eControllerId, TGCActionToKeyMap< TActionType >& rActionToKeyMap );

public:
							~TGCController					();

	inline bool				ButtonIsPressed					( TActionType eAction );

	inline bool				ButtonHasJustBeenPressed		( TActionType eAction );
	inline bool				ButtonHasJustBeenReleased		( TActionType eAction );

	inline f32				GetCurrentAxisValueRaw			( TActionType eAxisAction );
	inline f32				GetCurrentAxisDeadzoned			( TActionType eAxisAction, float fDeadzone );

	inline cocos2d::Vec2	GetCurrentStickValueRaw			( TActionType eAxisActionX, TActionType eAxisActionY );
	inline cocos2d::Vec2	GetCurrentStickValueDeadzoned	( TActionType eAxisActionX, TActionType eAxisActionY, float fDeadzone );


	// friend function used to create instances of this type
	template< typename TActionType >
	friend TGCController< TActionType > TGetActionMappedController( CGCControllerManager::EControllerId eControllerId, TGCActionToKeyMap< TActionType >& rActionToKeyMap );
};

// template function to auto-deduce the action type and generally make creating the action map less ugly
template< typename TActionType >
TGCController< TActionType > TGetActionMappedController( CGCControllerManager::EControllerId eControllerId, TGCActionToKeyMap< TActionType >& rActionToKeyMap )
{
	return TGCController< TActionType >( eControllerId, rActionToKeyMap );
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// * TGCActionToKeyMap * TGCActionToKeyMap * TGCActionToKeyMap * TGCActionToKeyMap * TGCActionToKeyMap * TGCActionToKeyMap * TGCActionToKeyMap *
// * TGCActionToKeyMap * TGCActionToKeyMap * TGCActionToKeyMap * TGCActionToKeyMap * TGCActionToKeyMap * TGCActionToKeyMap * TGCActionToKeyMap *
// * TGCActionToKeyMap * TGCActionToKeyMap * TGCActionToKeyMap * TGCActionToKeyMap * TGCActionToKeyMap * TGCActionToKeyMap * TGCActionToKeyMap *
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
template< typename TActionType >
TGCActionToKeyMap< TActionType >::TGCActionToKeyMap( const TActionType aeActions[], const Controller::Key aeCocosKeys[], u32 uArraySize )
{
	static_assert( ( true == std::is_enum< TActionType >::value ), "TActionType must be an enum" );

	for( u32 u = 0; u < uArraySize; ++u )
	{
		m_mapActionToKey[ aeActions[ u ] ] = aeCocosKeys[ u ];
	}
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
template< typename TActionType >
cocos2d::Controller::Key TGCActionToKeyMap< TActionType >::GetKeyForAction( TActionType eAction )	
{
	GCASSERT( ( m_mapActionToKey.find( eAction ) != m_mapActionToKey.end() ), "requested action not found in action map" );
	return m_mapActionToKey[ eAction ];
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// * TGCController * TGCController * TGCController * TGCController * TGCController * TGCController * TGCController * TGCController * TGCController *
// * TGCController * TGCController * TGCController * TGCController * TGCController * TGCController * TGCController * TGCController * TGCController *
// * TGCController * TGCController * TGCController * TGCController * TGCController * TGCController * TGCController * TGCController * TGCController *
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
template< typename TActionType >
TGCController< TActionType >::TGCController( CGCControllerManager::EControllerId eControllerId, TGCActionToKeyMap< TActionType >& rActionToKeyMap )
: CGCController		( eControllerId )
, m_rActionToKeyMap	( rActionToKeyMap )
{}


//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
template< typename TActionType >
TGCController< TActionType >::~TGCController()
{}


//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
template< typename TActionType >
inline bool TGCController< TActionType >::ButtonIsPressed( TActionType eAction )
{
	return CGCController::ButtonIsPressed( m_rActionToKeyMap.GetKeyForAction( eAction ) );
}


//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
template< typename TActionType >
inline bool TGCController< TActionType >::ButtonHasJustBeenPressed( TActionType eAction )
{
	return CGCController::ButtonHasJustBeenPressed( m_rActionToKeyMap.GetKeyForAction( eAction ) );
}


//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
template< typename TActionType >
inline bool TGCController< TActionType >::ButtonHasJustBeenReleased( TActionType eAction )
{
	return CGCController::ButtonHasJustBeenReleased( m_rActionToKeyMap.GetKeyForAction( eAction ) );
}


//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
template< typename TActionType >
inline f32 TGCController< TActionType >::GetCurrentAxisValueRaw( TActionType eAxisAction )
{
	return CGCController::GetCurrentAxisValueRaw( m_rActionToKeyMap.GetKeyForAction( eAxisAction ) );
}


//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
template< typename TActionType >
inline f32 TGCController< TActionType >::GetCurrentAxisDeadzoned( TActionType eAxisAction, float fDeadzone )
{
	return CGCController::GetCurrentAxisDeadzoned( m_rActionToKeyMap.GetKeyForAction( eAxisAction ), fDeadzone );
}


//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
template< typename TActionType >
inline cocos2d::Vec2 TGCController< TActionType >::GetCurrentStickValueRaw( TActionType eAxisActionX, TActionType eAxisActionY )
{
	return CGCController::GetCurrentStickValueRaw( m_rActionToKeyMap.GetKeyForAction( eAxisActionX ), m_rActionToKeyMap.GetKeyForAction( eAxisActionY ) );
}


//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
template< typename TActionType >
inline cocos2d::Vec2 TGCController< TActionType >::GetCurrentStickValueDeadzoned( TActionType eAxisActionX, TActionType eAxisActionY, float fDeadzone )
{
	return CGCController::GetCurrentStickValueDeadzoned( m_rActionToKeyMap.GetKeyForAction( eAxisActionX ), m_rActionToKeyMap.GetKeyForAction( eAxisActionY ), fDeadzone );
}


#endif // #if defined (WIN32)
#endif // #ifndef _GCINPUTCONTROLLER_H_