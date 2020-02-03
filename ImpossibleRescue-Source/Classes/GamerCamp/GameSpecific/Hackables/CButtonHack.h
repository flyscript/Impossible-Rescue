#ifndef _GCBUTTONHACK_H_
#define _GCBUTTONHACK_H_

#include "cocos2d.h"

////////////////////////////////////////////////
// CGCButtonHack Status
////////////////////////////////////////////////
class IGCGameLayer;
class UIAnimatedBar;

class CButtonHack : public CHackable
{
public:
	//////////////////////////////////////////////////////////////////////////
	// Constructors and Destructor
	//////////////////////////////////////////////////////////////////////////

	CButtonHack();

	// Overloaded constructors for customization
	CButtonHack( float fMaxHackTime, bool bResetable );

	virtual ~CButtonHack();

	//////////////////////////////////////////////////////////////////////////
	// Overriding the parent methods - State methods
	//////////////////////////////////////////////////////////////////////////

	// Implementing the hack logic and updating UI stuff
	virtual void VUpdate( float fDeltaTime ) override;

	// Implementing the trigger event when someone interacted with
	virtual void VTrigger() override;

	// Trigger method to be defined in inherited classes that shows prompt
	virtual void VEnter() override;

	// Implementing the exit event when someone left from
	virtual void VExit() override;

	// Reset
	virtual void VReset() override;

private:
	// Pointer to active scene
	IGCGameLayer* m_pActiveScene;

	// Reset the progress when player leave from hackable item or not
	bool m_bResetable;

	// Required time to hack this item
	float m_fMaxHackTime;

	// Current hacking progress
	float m_fHackProgress;

	// Pointer to UI stuff
	cocos2d::Label* m_pHackLabel;
	UIAnimatedBar* m_pHackingProgressBar;

	//////////////////////////////////////////////////////////////////////////
	// Overriding the parent methods - UI methods
	//////////////////////////////////////////////////////////////////////////
	void InitUI();
	void UpdateUI();
	virtual void VShowUI() override;
	virtual void VHideUI() override;
};
#endif