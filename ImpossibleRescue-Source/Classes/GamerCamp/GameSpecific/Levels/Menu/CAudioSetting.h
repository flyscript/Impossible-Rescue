#ifndef _AUDIOSETTINGS_H
#define _AUDIOSETTINGS_H
#include "GamerCamp/GCCocosInterface/IGCGameLayer.h"
#include "Helper/GameConfig.h"
#include "GamerCamp/GameController/GCController.h"
#include "ui/UISlider.h"
#include "GamerCamp/GameController/AudioManager/CAudioManager.h"

class CAudioSetting
	: public IGCGameLayer
{
private:
	// List of UI item
	cocos2d::MenuItemImage*						m_apcButtonsList[ 3 ];
	// SFX volume slider
	ui::Slider*									m_pcAudioEffectsSlider;
	// BGM volume slider
	ui::Slider*									m_pcAudioBackgroundSlider;
	// Current selected item index
	int											m_iSelected;
	// Controller button state
	bool										m_bKeyReleased;
	// Controller X - axis button state
	bool										m_bKeyReleasedX;
	// Default BGM volume, read from local storage
	int											m_iBackgroundPercent;
	// Default SFX volume, read from local storage
	int											m_iEffectsPercent;

	// Action map for controllers
	TGCActionToKeyMap<GameConfig::EInputs >*	m_pcControllerActionToKeyMap;

	// Audio Manager
	CAudioManager*								m_pcAudioManager;
public:
	//////////////////////////////////////////////////////////////////////////
	// Constructor and Destructor
	//////////////////////////////////////////////////////////////////////////
	CAudioSetting();
	~CAudioSetting();

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
	void CB_OnEffectSliderChange( Ref* pSender, ui::Slider::EventType ctype );
	void CB_OnBackgroundSliderChange( Ref* pSender, ui::Slider::EventType ctype );

	//////////////////////////////////////////////////////////////////////////
	// Keyboard and Controller Update Methods
	//////////////////////////////////////////////////////////////////////////
	void KeyboardControls();
	void ControllerControls( TGCController< GameConfig::EInputs > cController );


};
#endif