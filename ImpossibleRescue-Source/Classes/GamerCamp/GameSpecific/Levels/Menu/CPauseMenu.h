#ifndef _PAUSEMENU_H
#define _PAUSEMENU_H
#include "GamerCamp/GCCocosInterface/IGCGameLayer.h"
#include "Helper/GameConfig.h"
#include "GamerCamp/GameController/GCController.h"
#include "GamerCamp/GameController/AudioManager/CAudioManager.h"

class CPauseMenu
	: public IGCGameLayer
{
private:
	// List of UI item
	cocos2d::MenuItemImage* m_apcButtonsList[ 4 ];
	// Current selected item index
	int						m_iSelected;
	// Controller button state
	bool					m_bKeyReleased;
	// Control button
	MenuItemImage*			m_pItemControls;
	// Audio button
	MenuItemImage*			m_pItemAudio;
	// Resume button
	MenuItemImage*			m_pItemResume;
	// Back to menu button
	MenuItemImage*			m_pItemMainMenu;
	// Buttons container
	Menu*					m_pMenu;

	// Audio manager
	CAudioManager*			m_pcAudioManager;

	// Action map for controllers
	TGCActionToKeyMap<GameConfig::EInputs >* m_pcControllerActionToKeyMap;

public:
	//////////////////////////////////////////////////////////////////////////
	// Constructor and Destructor
	//////////////////////////////////////////////////////////////////////////
	CPauseMenu();
	~CPauseMenu();

	//////////////////////////////////////////////////////////////////////////
	// IGCGameLayer Virtual Methods
	//////////////////////////////////////////////////////////////////////////
	virtual void onEnter();
	virtual	void VOnCreate( void );
	virtual void VOnUpdate( f32 fTimeStep );
	virtual	void VOnDestroy( void );

	//////////////////////////////////////////////////////////////////////////
	// Keyboard and Controller Update Methods
	//////////////////////////////////////////////////////////////////////////
	void KeyboardControls();
	void ControllerControls( TGCController< GameConfig::EInputs > cController );

	//////////////////////////////////////////////////////////////////////////
	// UI Callbacks
	//////////////////////////////////////////////////////////////////////////
	void CB_OnControlsButton( Ref* pSender );
	void CB_OnAudioSettingsButton( Ref* pSender );
	void CB_OnGameResumeButton( Ref* pSender );
	void CB_OnMainMenuButton( Ref* pSender );
};
#endif
