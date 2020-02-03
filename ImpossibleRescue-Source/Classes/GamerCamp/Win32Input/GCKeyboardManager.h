////////////////////////////////////////////////////////////////////////////////////////////////////////////
// (C) Gamer Camp / Alex Darby 2018
// Distributed under the MIT license - see readme.md
////////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef _GCKEYBOARDMANAGER_H_
#define _GCKEYBOARDMANAGER_H_

#ifndef _GCTYPES_H_
	#include "../Core/GCTypes.h"
#endif

#ifndef __cocos2d_libs__CCKeyboardEvent__
	#include "base/CCEventKeyboard.h"
#endif

using namespace cocos2d;

//////////////////////////////////////////////////////////////////////////
// forward declare
class CInputKeyboard;


//////////////////////////////////////////////////////////////////////////
//
// GCKeyboardManager
//
// This handles the tedium of translating from keyboard input into the 
// much more game logic friendly concept of an 'action' 
//
// The value auActionValuesAsDIK passed to Initialise() is assumed to be 
// composed of valid windows virtual key codes (as defined in WinUser.h)
//
// The correspondence between these and your game side actions should
// be managed by using an enum like this:
//
// enum EGameActions
// {
//	EGA_UP		= 0,
// 	EGA_DOWN,
// 	EGA_LEFT,
//	EGA_RIGHT
//	EGA_ACTION
//	// add new actions before this
//	EGA_MAXACTION
// };
// 
// Then you would define the array to pass to Initialise like this:
// 
// u32 auVKInputValuesForActions[] = 
// {
// 	VK_UP,		//EGA_UP	= 0,
// 	VK_DOWN,	//EGA_DOWN	= 1,
//	VK_LEFT,	//EGA_LEFT	= 2,
// 	VK_RIGHT,	//EGA_RIGHT	= 3
// 	'A',		//EGA_ACTION = 4 - Note: the VK codes for alpha numeric keys are the same as their ASCII values (use CAPITAL letters!)
// };
//
// int iSizeOfVKArray = (sizeof(auVKInputValuesForActions) / sizeof(u32) );
//
// Note that the enum values that we're using for the actions are the same 
// as the array indices of the corresponding DIK_xxxxxx that we're passing 
// to the function CGCKeyboardManager::Initialise( ... )
//
// Then create and Initialise the input manager like this:
// 
//	CGCKeyboardManager* pInputManager = new CGCKeyboardManager();
// 	pInputManager->Initialise( iSizeOfVKArray, auVKInputValuesForActions );
// 
//
// CGCKeyboardManager::Reset() should be called at game reset
// CGCKeyboardManager::Update() should be called at the start of the update loop
//
// Assuming the update is being called every frame then these functions are 
// used to check the current state of the actions like so:
//
// if( CGCKeyboardManager::ActionHasJustBeenPressed( EGA_UP ) )
// {
//    // react to up being pressed once on the first frame it is pressed
// }
//
// if( CGCKeyboardManager::ActionIsPressed( EGA_UP ) )
// {
//    // react to up being pressed on all frames whilst it is pressed
// }
//
// if( CGCKeyboardManager::ActionHasJustBeenReleased( EGA_UP )	)
// {
//    // react after up has been pressed once on the first frame after it is released
// }
//
//////////////////////////////////////////////////////////////////////////
class CGCKeyboardManager
{
public:
	enum
	{
		EMaxActions = 32
	};

private:
	CInputKeyboard*	m_pKeyboardInput;					// This is the class that contains the dinput code

	u32						m_uNumActions;						// Number of actions managed
	bool*					m_pabActionStates_CurrentFrame;		// Pointer to whichever of m_abActionBuffer_One or m_abActionBuffer_Two is the current frame's buffer
	bool*					m_pabActionStates_LastFrame;		// Pointer to whichever of m_abActionBuffer_One or m_abActionBuffer_Two is the last frame's buffer

	bool					m_abActionBuffer_One[ EMaxActions ];// These two arrays are used to double buffer input over two frames so this code
	bool					m_abActionBuffer_Two[ EMaxActions ];// can reason about when actions are pressed / released snce last frame

	EventKeyboard::KeyCode	m_auActionKeycodeValues[ EMaxActions ];// Contains the cocos2d::EventKeyboard::KeyCode values that correspond to the actions


public:
	// default initializing constructor
	CGCKeyboardManager();

	// destructor does nothing
	~CGCKeyboardManager();


	// lifetime and logic management - these correspond with the semantically equivalent functions of CGCObjectManager et al.	
	// n.b. explicit constructor and destructor in initialise and shutdown
	void Initialise	( u32 uNumActions, const EventKeyboard::KeyCode auActionValuesAsKeyCodes[] );
	void Reset		( void );
	void Update		( void );
	void ShutDown	( void );

	// accessing actions
	inline bool ActionHasJustBeenPressed	( u32 uAction )const;	// will return true if uAction was not pressed last frame and is now pressed
	inline bool ActionIsPressed				( u32 uAction )const;	// will return true if uAction is currently being pressed
	inline bool ActionHasJustBeenReleased	( u32 uAction )const;	// will return true if uAction was pressed last frame, but is now not pressed
};



//////////////////////////////////////////////////////////////////////////
// will return true if uAction was not pressed last frame and is now pressed
//////////////////////////////////////////////////////////////////////////
inline bool CGCKeyboardManager::ActionHasJustBeenPressed( u32 uAction ) const
{
	CCAssert( ( uAction < m_uNumActions ), "uAction is out of range - must be less than uNumActions passed to CGCKeyboardManager::Initialise( ... )" );
	// n.b. && returns bool
	return( m_pabActionStates_CurrentFrame[ uAction ] && (!m_pabActionStates_LastFrame[ uAction ] ) );
}	


//////////////////////////////////////////////////////////////////////////
// will return true if uAction is currently being pressed
//////////////////////////////////////////////////////////////////////////
inline bool CGCKeyboardManager::ActionIsPressed( u32 uAction )	const
{
	CCAssert( ( uAction < m_uNumActions ), "uAction is out of range - must be less than uNumActions passed to CGCKeyboardManager::Initialise( ... )" );
	return m_pabActionStates_CurrentFrame[ uAction ];
}


//////////////////////////////////////////////////////////////////////////
// will return true if uAction was pressed last frame, but is now not pressed
//////////////////////////////////////////////////////////////////////////
inline bool CGCKeyboardManager::ActionHasJustBeenReleased( u32 uAction ) const
{
	CCAssert( ( uAction < m_uNumActions ), "uAction is out of range - must be less than uNumActions passed to CGCKeyboardManager::Initialise( ... )" );
	// n.b. && returns bool
	return( ( !m_pabActionStates_CurrentFrame[ uAction ] ) && m_pabActionStates_LastFrame[ uAction ] );
}
#endif //#ifndef _GCKEYBOARDMANAGER_H_
