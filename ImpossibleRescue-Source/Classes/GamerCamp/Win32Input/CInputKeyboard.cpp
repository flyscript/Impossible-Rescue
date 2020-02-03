//////////////////////////////////////////////////////////////////////////
// original code credits:
//
// file name:	cinput.h
// author:		Victor Saar
// e-mail:		vsaar@web.de
// web site:	http://www.two-kings.de
//
// Updated for Cocos2d-x by Alex Darby @ Gamer Camp 09/2012
//////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////////////////////////////////
// (C) Gamer Camp / Alex Darby 2018
// Distributed under the MIT license - see readme.md
////////////////////////////////////////////////////////////////////////////////////////////////////////////
#if defined (WIN32)
#include "CInputKeyboard.h"

#include "2d/CCNode.h"
#include "base/CCEventDispatcher.h"
#include "base/CCDirector.h"
#include "base/ccMacros.h"
#include "platform/CCGLView.h"

                                      
using namespace cocos2d;


//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
CInputKeyboard::CInputKeyboard( void )
{
	Reset();
	m_pcKeyboardListener = EventListenerKeyboard::create();

	// when a pressed event is received we set the flag saying the button is pressed...
	m_pcKeyboardListener->onKeyPressed = [this]( EventKeyboard::KeyCode keyCode, Event* event )
	{
		m_abKeyBufferWrite[ (int) keyCode ] = true;
	};

	// and set t to false when a released event is received 
	m_pcKeyboardListener->onKeyReleased = [this]( EventKeyboard::KeyCode keyCode, Event* event )
	{
		m_abKeyBufferWrite[ (int) keyCode ] = false;
	};

	// attach this to the director's dispatcher
	Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority( m_pcKeyboardListener, 1 );
}



//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
CInputKeyboard::~CInputKeyboard(void)
{
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
void CInputKeyboard::Reset( void )
{
	memset ( &(m_abKeyBufferWrite[ 0 ]), 0, sizeof(m_abKeyBufferWrite) );
	memset ( &(m_abKeyBufferRead[ 0 ]),	 0, sizeof(m_abKeyBufferRead) );
}


//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
void CInputKeyboard::Update( void )
{
	memcpy( &( m_abKeyBufferRead[ 0 ] ), &( m_abKeyBufferWrite[ 0 ] ), sizeof( m_abKeyBufferRead ) );
}


//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
bool CInputKeyboard::KeyIsPressed( EventKeyboard::KeyCode eKeyCode )
{
	return m_abKeyBufferRead[ (int) eKeyCode ];
}


#endif //#if defined (WIN32)
