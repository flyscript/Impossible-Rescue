#ifndef _AUDIOMANAGER_H
#define _AUDIOMANAGER_H

#include "vector"
#include "GamerCamp/Core/GCTypes.h"
#include "Helper/GameConfig.h"
#include "GamerCamp/GameController/DataPersistence/CDataManager.h"
#include "AudioEngine.h"

class CAudioManager
{
private:
	// Get background music filepath by levels
	const char* GetBackGroundFilePath( GameConfig::EBackgroundSounds eBackgroundSound );

	// Get sound effect filepath by sfx types
	const char* GetFilePath( GameConfig::EMusic eMusic );

	CDataPersistence*			m_pcDataPersistence;

public:

	// Key pair value structure for storing active sound effect
	// Cocos2d audio engine returns an unsigned integer after the sfx is triggered to play
	// To pause or stop the active sfx, we need that value
	struct SSoundIdPair
	{
		GameConfig::EMusic	eSFX;
		u32					uiSFXId;
	};

	// Some of the value for bgm and sfx that we don't want to append twice
	// As a result, here we use static value to avoid it

	// List of active sfx
	static std::vector<CAudioManager::SSoundIdPair>	m_vsSoundEffectsPair;
	// Unsigned int from audio engine for bgm
	static u32										m_iBackgroundId;
	// Previous screen bgm unsigned int from audio engine
	static u32										m_iOldBackgroundId;
	// Unsigned int for footstep sfx
	static u32										m_iFootstepId;

	// Another static values that we use it to handle audio manager volume and states
	static int										m_iEffectsVolumePercentage;
	static int										m_iBackgroundVolumePercentage;
	static bool										m_bIsFootstepPlaying;
	static bool										m_bIsMenuMusicPlaying;

	//////////////////////////////////////////////////////////////////////////
	// Constructor & Destructor
	//////////////////////////////////////////////////////////////////////////
	CAudioManager();
	~CAudioManager()
	{
	}

	// Preload all sfx and bgm from the specify filepath
	void Preload();

	//////////////////////////////////////////////////////////////////////////
	// SFX Accessors
	//////////////////////////////////////////////////////////////////////////
	// Play SFX by SFX type, can be loop or not
	void PlayEffectsMusic( GameConfig::EMusic eMusic, bool bIsLooped );
	// Stop SFX by SFX type
	void StopEffectsMusic( GameConfig::EMusic eMusic );

	//////////////////////////////////////////////////////////////////////////
	// BGM Accessors
	//////////////////////////////////////////////////////////////////////////
	// Play BGM by BGM type, can be loop or not
	void PlayBackgroundMusic( GameConfig::EBackgroundSounds eBackgroundSound, bool bIsLooped );
	// Stop BGM by BGM type
	void StopBackgroundMusic();
	// Pause BGM
	void PauseBackground();
	// Resume BGM
	void ResumeBackground();


	//////////////////////////////////////////////////////////////////////////
	// Volume Accessors
	//////////////////////////////////////////////////////////////////////////
	// Takes a 0-100 value for volume, where 0 is muted and 100 is full volume
	void ChangeEffecstVolume( int fVolume );
	// Takes a 0-100 value for volume, where 0 is muted and 100 is full volume
	void ChangeBackgroundVolume( int fVolume );
	// Return SFX volume, 0 - 100
	int GetEffectsVolune();
	// Return BGM volume, 0 - 100
	int GetBackgroundVolume();

	//////////////////////////////////////////////////////////////////////////
	// Footstep SFX Accessors
	//////////////////////////////////////////////////////////////////////////
	void PlayFootstep();
	void PauseFootStep();
	void ResumeFootstep();
	void StopFootstep();
	bool IsFootstepPlaying();
	
	//////////////////////////////////////////////////////////////////////////
	// Method to check if the menu music is playing or not.
	//////////////////////////////////////////////////////////////////////////
	bool MenuMusicIsPlaying();
};
#endif
