////////////////////////////////////////////////////////////////////////////////////////////////////////////
// (C) Gamer Camp / Alex Darby 2018
// Distributed under the MIT license - see readme.md
////////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef __CCMACROS_H__ 
	#include "base/ccMacros.h"
#endif

#ifndef _GCKEYBOARDMANAGER_H_
	#include "GCKeyboardManager.h"
#endif

#ifndef _CINPUT_H_
	#include "CInputKeyboard.h"
#endif


// default initiaising constructor
CGCKeyboardManager::CGCKeyboardManager()
: m_pKeyboardInput				( 0 )
, m_uNumActions					( 0 )
, m_pabActionStates_CurrentFrame( m_abActionBuffer_One )
, m_pabActionStates_LastFrame	( m_abActionBuffer_Two )
{
	m_pKeyboardInput = new CInputKeyboard();
}

// destructor does nothing
CGCKeyboardManager::~CGCKeyboardManager()			  
{}


//////////////////////////////////////////////////////////////////////////
// explicit constructor
// see header for a detailed description of how to use this
// N.B. do not call more than once on the same instance!
//////////////////////////////////////////////////////////////////////////
void CGCKeyboardManager::Initialise( u32 uNumActions, const cocos2d::EventKeyboard::KeyCode auActionValuesAsKeyCodes[] )
{
	CCAssert( ( uNumActions < EMaxActions ), "uNumActions is out of range - must be less than CGCKeyboardManager::EMaxActions"	);
	m_uNumActions = uNumActions;
	for( u32 uLoop = 0; uLoop < m_uNumActions; ++uLoop )
	{
		m_auActionKeycodeValues[ uLoop ] = auActionValuesAsKeyCodes[ uLoop ];
	}
}


//////////////////////////////////////////////////////////////////////////
// Empty the content of the two buffers and reset the current / last
// frame pointers
//////////////////////////////////////////////////////////////////////////
void CGCKeyboardManager::Reset( void )
{
	memset( m_abActionBuffer_One, 0, sizeof( m_abActionBuffer_One ) );
	memset( m_abActionBuffer_Two, 0, sizeof( m_abActionBuffer_Two ) );

	m_pabActionStates_CurrentFrame	= m_abActionBuffer_One;
	m_pabActionStates_LastFrame		= m_abActionBuffer_Two;
}


//////////////////////////////////////////////////////////////////////////
// Swap action buffer pointers so that previous frame's 'current action 
// buffer' is this frames 'last action buffer', then get this frames input
// off dinput into the current buffer
//////////////////////////////////////////////////////////////////////////
void CGCKeyboardManager::Update( void )
{
	// update cinput
	m_pKeyboardInput->Update();

	// swap action buffer pointers
	bool* abSwapTemp				= m_pabActionStates_LastFrame;
	m_pabActionStates_LastFrame		= m_pabActionStates_CurrentFrame;
	m_pabActionStates_CurrentFrame	= abSwapTemp;

	// iterate action array and test for key presses in cinput
	// store return values directly into m_pabActionStates_CurrentFrame
	for( u32 uLoop = 0; uLoop < m_uNumActions; ++uLoop )
	{
		m_pabActionStates_CurrentFrame[ uLoop ] = m_pKeyboardInput->KeyIsPressed( m_auActionKeycodeValues[ uLoop ] );
	}

}


//////////////////////////////////////////////////////////////////////////
// explicit destructor
//////////////////////////////////////////////////////////////////////////
void CGCKeyboardManager::ShutDown( void )
{
	delete m_pKeyboardInput;
	m_pKeyboardInput = NULL;
}

