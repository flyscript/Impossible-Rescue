#ifndef _CONTROLSMENU_H
#define _CONTROLSMENU_H
#include "GamerCamp/GCCocosInterface/IGCGameLayer.h"
#include "Helper/GameConfig.h"
#include "GamerCamp/GameController/GCController.h"
#include "GamerCamp/GameController/AudioManager/CAudioManager.h"

class CControlsMenu
	: public IGCGameLayer
{
private:
	// List of UI item
	cocos2d::MenuItemImage*  m_apcButtonsList[ 1 ];
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
	CControlsMenu();
	~CControlsMenu();

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
	void CB_OnBackButton( Ref* pSender );

	//////////////////////////////////////////////////////////////////////////
	// Keyboard and Controller Update Methods
	//////////////////////////////////////////////////////////////////////////
	void KeyboardControls();
	void ControllerControls( TGCController< GameConfig::EInputs > cController );
};
#endif
