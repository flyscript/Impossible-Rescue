#ifndef _MAINMENU_H
#define _MAINMENU_H

#include "GamerCamp/GCCocosInterface/IGCGameLayer.h"
#include "Helper/GameConfig.h"
#include "GamerCamp/GameController/GCController.h"
#include "GamerCamp/GameController/AudioManager/CAudioManager.h"

class CMainMenu
	: public IGCGameLayer
{
private:
	// List of UI item
	cocos2d::MenuItemImage* m_pcButtonsList[ 4 ];
	// Current selected item index
	int						m_iSelected;
	// Controller button state
	bool					m_bKeyReleased;

	// Audio manager
	CAudioManager*			m_pcAudioManager;

	// Action map for controllers
	TGCActionToKeyMap<GameConfig::EInputs >* m_pcControllerActionToKeyMap;
public:
	//////////////////////////////////////////////////////////////////////////
	// Constructor and Destructor
	//////////////////////////////////////////////////////////////////////////
	CMainMenu();
	~CMainMenu();

	//////////////////////////////////////////////////////////////////////////
	// IGCGameLayer Virtual Methods
	//////////////////////////////////////////////////////////////////////////
	virtual void onEnter();
	virtual	void VOnCreate();
	virtual void VOnUpdate( f32 fTimeStep );
	virtual	void VOnDestroy();
	virtual void VOnReset() override;

	//////////////////////////////////////////////////////////////////////////
	// Keyboard and Controller Update Methods
	//////////////////////////////////////////////////////////////////////////
	void KeyboardControls();
	void ControllerControls( TGCController< GameConfig::EInputs > cController );

	//////////////////////////////////////////////////////////////////////////
	// UI Callbacks
	//////////////////////////////////////////////////////////////////////////
	void CB_OnGameQuitButton( Ref* pSender );
	void CB_OnGameStartButton( Ref* pSender );
	void CB_OnGameLoadButton( Ref* pSender );
	void CB_OnGameOptionButton( Ref* pSender );
};
#endif

