#ifndef _SETTINGMENU_H
#define _SETTINGMENU_H
#include "GamerCamp/GCCocosInterface/IGCGameLayer.h"
#include "Helper/GameConfig.h"
#include "GamerCamp/GameController/GCController.h"
#include "GamerCamp/GameController/AudioManager/CAudioManager.h"

class CSettingsMenu
	: public IGCGameLayer
{
private:
	// UI item list
	cocos2d::MenuItemImage* m_pcButtonsList[ 3 ];
	// Current selected item index
	int						m_iSelected;
	// Controller button state
	bool					m_bKeyReleased;

	// Audio Manager
	CAudioManager*			m_pcAudioManager;

	// Action map for controllers
	TGCActionToKeyMap<GameConfig::EInputs >* m_pcControllerActionToKeyMap;

public:
	//////////////////////////////////////////////////////////////////////////
	// Constructor and Destructor
	//////////////////////////////////////////////////////////////////////////
	CSettingsMenu();
	~CSettingsMenu();

	//////////////////////////////////////////////////////////////////////////
	// IGCGameLayer Virtual Methods
	//////////////////////////////////////////////////////////////////////////
	virtual void onEnter();
	virtual	void VOnCreate();
	virtual void VOnUpdate( f32 fTimeStep );
	virtual	void VOnDestroy();

	//////////////////////////////////////////////////////////////////////////
	// UI Callbacks
	//////////////////////////////////////////////////////////////////////////
	void CB_OnControlButton( Ref* pSender );
	void CB_OnAudioSettingButton( Ref* pSender );
	void CB_OnBackButton( Ref* pSender );

	//////////////////////////////////////////////////////////////////////////
	// Keyboard and Controller Update Methods
	//////////////////////////////////////////////////////////////////////////
	void KeyboardControls();
	void ControllerControls( TGCController< GameConfig::EInputs > cController );
};

#endif
