#ifndef  _APP_DELEGATE_H_
#define  _APP_DELEGATE_H_

#include "cocos2d.h"

#ifndef _GCKEYBOARDMANAGER_H_
	#include "GamerCamp/Win32Input/GCKeyboardManager.h"
#endif

#ifndef _GCINPUTCONTROLLER_H_
	#include "GamerCamp/GameController/GCControllerManager.h"
#endif

#ifndef _GCKEYBOARDMANAGER_H_
	#include "GamerCamp/Win32Input/GCKeyboardManager.h"
#endif

#ifndef __cocos2d_libs__CCKeyboardEvent__
	#include "base/CCEventKeyboard.h"
#endif


//////////////////////////////////////////////////////////////////////////
// GamerCamp Edit - fwd declare
class CGCObjGroupDefault;


/**
@brief    The cocos2d Application.

The reason for implement as private inheritance is to hide some interface call by Director.
*/
class  AppDelegate : private cocos2d::Application
{
//////////////////////////////////////////////////////////////////////////
// GamerCamp Edit
private:
	static CGCKeyboardManager*		sm_pcKeyboardManager;	// handles keyboard input
	static CGCControllerManager*	sm_pcControllerManager;
// GamerCamp Edit
//////////////////////////////////////////////////////////////////////////

public:
    AppDelegate();
    virtual ~AppDelegate();

    /**
    @brief    Implement Director and Scene init code here.
    @return true    Initialize success, app continue.
    @return false   Initialize failed, app terminate.
    */
    virtual bool applicationDidFinishLaunching();

    /**
    @brief  The function be called when the application enter background
    @param  the pointer of the application
    */
    virtual void applicationDidEnterBackground();

    /**
    @brief  The function be called when the application enter foreground
    @param  the pointer of the application
    */
    virtual void applicationWillEnterForeground();

//////////////////////////////////////////////////////////////////////////
// GamerCamp Edit
	//////////////////////////////////////////////////////////////////////////
	// accessors for m_pcInputManager

		// initialise actions - see CGCKeyboardManager::Initialise() for details of arguments
		static void						InitialiseKeyboardManager( u32 uNumActions, cocos2d::EventKeyboard::KeyCode pauActionsAsKeycodes[] );
		static CGCKeyboardManager*		GetKeyboardManager();

		static void						InitialiseControllerManager();
		static CGCControllerManager*	GetControllerManager();

	// accessors for m_pcInputManager
	//////////////////////////////////////////////////////////////////////////
// GamerCamp Edit
//////////////////////////////////////////////////////////////////////////
};

//////////////////////////////////////////////////////////////////////////
// GamerCamp Edit

	//////////////////////////////////////////////////////////////////////////
	// initialise actions - see CGCKeyboardManager::Initialise() for details of arguments
	//////////////////////////////////////////////////////////////////////////
	// static
	inline void AppDelegate::InitialiseKeyboardManager( u32 uNumActions, cocos2d::EventKeyboard::KeyCode pauActionsAsKeycodes[] )
	{
		sm_pcKeyboardManager->Initialise( uNumActions, pauActionsAsKeycodes );
		sm_pcKeyboardManager->Reset();
	}
	

	//////////////////////////////////////////////////////////////////////////
	// accessor for keyboard manager
	//////////////////////////////////////////////////////////////////////////
	// static
	inline CGCKeyboardManager* AppDelegate::GetKeyboardManager()
	{
		return sm_pcKeyboardManager;
	}

	//////////////////////////////////////////////////////////////////////////
	// initialise actions - see CGCKeyboardManager::Initialise() for details of arguments
	//////////////////////////////////////////////////////////////////////////
	// static
	inline void AppDelegate::InitialiseControllerManager()
	{
		sm_pcControllerManager->Initialise();
	}


	//////////////////////////////////////////////////////////////////////////
	// accessor for keyboard manager
	//////////////////////////////////////////////////////////////////////////
	// static
	inline CGCControllerManager* AppDelegate::GetControllerManager()
	{
		return sm_pcControllerManager;
	}

	// GamerCamp Edit
//////////////////////////////////////////////////////////////////////////

#endif // _APP_DELEGATE_H_

