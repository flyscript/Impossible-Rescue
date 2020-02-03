#pragma once

#include "GamerCamp/GCCocosInterface/IGCGameLayer.h"
#include "CHackable.h"
#include "Helper/GameConfig.h"

/*
CQTEHack

A concrete implementation of the CHackable class.

Used for Quick-Time-Event-based hacking mechanics.

You are able to change the array of buttons that can be pressed, the number of
buttons needed to be pressed for a successful hack, and the time you have to
press the appropriate button before failing; in order to change the difficulty
of this hack.

"Buttons" for the QTE are actually player actions, so that it will work cleanly
on any platform.

Created by Will.

*/
class CQTEHack : public CHackable
{
private:

	////////////////////////////////
	//MODIFIERS
	////////////////////////////////

	// The amount of time (in seconds) the player has to press the displayed button before failing
	float m_fTimePerButton;

	// The number of buttons a player has to get right in order to succeed.
	int m_iNumberOfButtons;

	// An array to store the player actions that can pop up for this QTE
	std::vector<GameConfig::EPlayerActions>* m_aeButtons;


	////////////////////////////////
	//STATES
	////////////////////////////////

	// The time that the current button prompt started
	float m_fPromptTime;

	// Holds current progress towards QTE goal
	int m_iEventsComplete;

	// Holds the current QTE key needed to advance
	GameConfig::EPlayerActions m_eKeyToPress;

	// Sets a new QTE key
	void PickNewKey();

	void InitUI();

	IGCGameLayer* m_pActiveScene;
	cocos2d::Label* m_pPromptLabel;
	cocos2d::Label* m_pHackLabel;

public:

	// Constructor

	CQTEHack();

	// Destructor
	~CQTEHack();

	// Adds player actions to the std::vector of actions for this QTE
	void AddAction( GameConfig::EPlayerActions eAction );

	// Implementing the hack behavior for the QTE class
	virtual void VUpdate( float fDeltaTime ) override;

	// Implementing the trigger behavior for the QTE class
	virtual void VTrigger() override;

	// Implementing the entry behavior for the QTE class
	virtual void VEnter() override;

	// Implementing the exit behavior for the QTE class
	virtual void VExit() override;

	// Implementing the UI behavior for this class
	virtual void VShowUI() override;
	virtual void VHideUI() override;

	// Reset
	virtual void VReset() override;

	// Getter and setter for time player gets to press the button
	inline float GetTimePerButton()
	{
		return m_fTimePerButton;
	};
	inline void SetTimePerButton( float fTime )
	{
		m_fTimePerButton = fTime;
	};

	// Getter and setter for the number of times the player has to get the button right
	inline int GetNumberOfEvents()
	{
		return m_iNumberOfButtons;
	};
	inline void SetNumberOfEvents( int iEvents )
	{
		m_iNumberOfButtons = iEvents;
	};

};