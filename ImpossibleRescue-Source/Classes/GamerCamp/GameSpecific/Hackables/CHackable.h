#ifndef _GCHACKABLE_H_
#define _GCHACKABLE_H_

#include "GamerCamp/GameSpecific/Terminals/CObjTerminal.h"
#include "GamerCamp/GameController/AudioManager/CAudioManager.h"
#include "Helper/GameConfig.h"
#include <functional>

// Predefines
class CObjTerminal;
class CAudioManager;

/*
CHackable

A "non-concrete" class used as a basis for the different hacking mechanics.

Not actually abstract, because then we'd get incomplete class type errors when making
pointers to a derived hackable of unknown child class type.

Uses function pointers for callbacks, so that case-specific behaviours for
a successful or failed hacking attempt can be defined for each hackable
object in the game.

Has an Update(deltaTime) function for stepped behavioral updates, and a
trigger	function to let the hackable know that it should start. These are
to be implemented by the inherited classes.

Outlined by Stanley and Will. Cleaned up, implemented, and documented by Will.

*/
class CHackable
{
private:

	// Pointers to methods to be run if a hack succeeds or fails
	std::function<void()> m_pfnCallbackOnFailed;
	std::function<void()> m_pfnCallbackOnSucceed;
	std::function<void()> m_pfnCallbackOnStarted;

	// Whether or not we're already triggered
	bool m_bTriggered;

	// Whether or not this hackble has already been started hacking
	bool m_bIsHacking;

	// Whether or not this hackable has already been hacked
	bool m_bHacked;

	// Whether or not the player can move whilst this is being hacked
	bool m_bCanMoveWhileHacking;

	// Holds the key the player has pressed
	GameConfig::EPlayerActions m_ePlayerInput;

	// UI virtual methods to be defined in inherited classes
	virtual void VShowUI();
	virtual void VHideUI();

	// The terminal that this hackable belongs to
	CObjTerminal* m_pcTerminal;

protected:

	// Audio Manager
	bool m_bIsPlayingHackingSFX;
	CAudioManager* m_pcAudioManager;

	// Methods to invoke that will run the behavior for a successful or
	// failed hack, as defined by the relevant function pointers.
	void HackSucceeded();
	void HackFailed();

	// Inline setter for m_bTriggered, used only by inherited classes
	// Classes that interact with a hackable should onl;y be able to Trigger()
	inline void SetTriggered( bool bTriggered )
	{
		m_bTriggered = bTriggered;
	}


public:

	// Constructor
	CHackable();

	// Destructor
	virtual ~CHackable();


	// Setter methods for the function pointers
	void SetOnFailMethod( std::function<void()> pfnCallback );
	void SetOnSucceedMethod( std::function<void()> pfnCallback );
	void SetOnStartedMethod( std::function<void()> pfnCallback );

	// Inline setter and getter methods for m_bCanPlayerMoveWhilstHacking
	inline void SetCanMoveWhilstHacking( bool bCanMove )
	{
		m_bCanMoveWhileHacking = bCanMove;
	};
	inline bool GetCanMoveWhilstHacking()
	{
		return m_bCanMoveWhileHacking;
	};

	// Inline setter and getter methods for the terminal that this belongs to
	inline void SetTerminal( CObjTerminal* pcTerminal )
	{
		m_pcTerminal = pcTerminal;
	};
	inline CObjTerminal* GetTerminal()
	{
		return m_pcTerminal;
	};

	// Inline function for setting and getting hacking status flag
	inline void SetIsHacking( bool bFlag )
	{
		m_bIsHacking = bFlag;
	}
	inline bool GetIsHacking()
	{
		return m_bIsHacking;
	}

	// Inline function returns true if this hackable has already been hacked
	inline bool IsHacked()
	{
		return m_bHacked;
	};

	// Inline function for callback getters
	inline void TriggerOnStartedCallback()
	{
		if( m_pfnCallbackOnStarted )
		{
			m_pfnCallbackOnStarted();
		}
	};
	inline void TriggerOnFailedCallback()
	{
		if( m_pfnCallbackOnFailed )
		{
			m_pfnCallbackOnFailed();
		}
	};
	inline void TriggerOnSucceedCallback()
	{
		if( m_pfnCallbackOnSucceed )
		{
			m_pfnCallbackOnSucceed();
		}
	}

	// Called by the player to tell us a key has been pressed
	void SetPlayerInput( GameConfig::EPlayerActions eAction );

	// Called by inherited classes to find out what the player has pressed
	GameConfig::EPlayerActions GetPlayerInput();

	// Update method to be defined in inherited classes that defines hacking behavior
	virtual void VUpdate( float fDeltaTime );

	// Trigger method to be defined in inherited classes that starts the hack
	virtual void VTrigger();

	// Reset method can be defined in inherited classes, which should call this method first
	virtual void VReset();

	// Inline getter for m_bTriggered
	inline bool GetTriggered()
	{
		return m_bTriggered;
	}

	// Trigger method to be defined in inherited classes that shows prompt
	virtual void VEnter();

	// Exit method to be defined in inherited classes that defines behavior for leaving the hackable object
	virtual void VExit();

};

#endif