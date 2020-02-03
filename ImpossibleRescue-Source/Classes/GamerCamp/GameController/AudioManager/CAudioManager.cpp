#ifndef _AUDIOMANAGER_H
#include "CAudioManager.h"
#endif
#include "Helper/GameConfig.h"
#include "AudioEngine.h"
#include <iostream>

// Initialising the values of some static variables.
std::vector<CAudioManager::SSoundIdPair>	CAudioManager::m_vsSoundEffectsPair;
u32											CAudioManager::m_iBackgroundId = 0;
u32											CAudioManager::m_iOldBackgroundId = 0;
int											CAudioManager::m_iEffectsVolumePercentage;
int											CAudioManager::m_iBackgroundVolumePercentage;
u32											CAudioManager::m_iFootstepId = 0;
bool										CAudioManager::m_bIsFootstepPlaying = false;
bool										CAudioManager::m_bIsMenuMusicPlaying = false;

CAudioManager::CAudioManager()
{
	m_pcDataPersistence = new CDataPersistence();
	// get the voulume saved using data persistence 
	m_iBackgroundVolumePercentage = m_pcDataPersistence->GetBackgroundVolume();
	m_iEffectsVolumePercentage = m_pcDataPersistence->GetEffectsVolume();
}

// Method to retrieve the file path for different background music
const char* CAudioManager::GetBackGroundFilePath( GameConfig::EBackgroundSounds eBackgroundSound )
{
	switch( eBackgroundSound )
	{
	case GameConfig::EBackgroundSounds::EBS_Level1:
		return "Music/Level_Music/Music_Level_1.mp3";
		break;
	case  GameConfig::EBackgroundSounds::EBS_Level2:
		return "Music/Level_Music/Music_Level_2.mp3";
		break;
	case  GameConfig::EBackgroundSounds::EBS_Level3:
		return "Music/Level_Music/Music_Level_3.mp3";
		break;
	case  GameConfig::EBackgroundSounds::EBS_Level4:
		return "Music/Level_Music/Music_Level_4.mp3";
		break;
	case GameConfig::EBackgroundSounds::EBS_Menu:
		return "Music/Front_End/Main_Menu/Music/Menu_Music.mp3";
		break;
	case GameConfig::EBackgroundSounds::EBS_Credits:
		return "Music/Credits/CreditsMusic.mp3";
		break;
	default:
		return "Music/Level_Music/Music_Level_1.mp3";
		break;
	};
}

// Method to retrieve the file path for different Sound effects  
const char* CAudioManager::GetFilePath( GameConfig::EMusic eMusic )
{
	switch( eMusic )
	{
	case GameConfig::EMusic::EM_DanielleMove:
		return "Music/Danielle/Movement/Continuous_Walking.mp3";
		break;
	case GameConfig::EMusic::EM_DanielleHack:
		return "Music/Danielle/Hacking/Hacking_process.mp3";
		break;
	case GameConfig::EMusic::EM_DanielleHackFail:
		return "Music/Danielle/Hacking/Hacking_failure.mp3";
		break;
	case GameConfig::EMusic::EM_DanielleHackPass:
		return "Music/Danielle/Hacking/Hacking_complete.mp3";
		break;
	case GameConfig::EMusic::EM_DanielleJump:
		return "Music/Danielle/Movement/Jump/Jump_3.mp3";
		break;
	case GameConfig::EMusic::EM_DanielleJumpLand:
		return "Music/Danielle/Movement/Jump/Jumping_land_1.mp3";
		break;
	case GameConfig::EMusic::EM_ShadowShoot:
		return "Music/Shadow/Guns/Gun_5.mp3";
		break;
	case GameConfig::EMusic::EM_DanielleHurt:
		return "Music/Danielle/Health/Hurt.mp3";
		break;
	case GameConfig::EMusic::EM_DanielleDie:
		return "Music/Danielle/Health/Die.mp3";
		break;
	case GameConfig::EMusic::EM_MenuBack:
		return "Music/Front_End/Main_Menu/Navigation/Back.mp3";
		break;
	case GameConfig::EMusic::EM_MenuDown:
		return "Music/Front_End/Main_Menu/Navigation/Down.mp3";
		break;
	case GameConfig::EMusic::EM_MenuOK:
		return "Music/Front_End/Main_Menu/Navigation/Ok.mp3";
		break;
	case GameConfig::EMusic::EM_MenuUp:
		return "Music/Front_End/Main_Menu/Navigation/Up.mp3";
		break;
	case GameConfig::EMusic::EM_Geiger:
		return "Music/Front_End/Text/geiger.mp3";
		break;
	case GameConfig::EMusic::EM_Fade:
		return "Music/Front_End/Main_Menu/Navigation/Fade.mp3";
		break;
	case GameConfig::EMusic::EM_ShadowCharge:
		return "Music/Shadow/StunChargeFull.mp3";
		break;
	case GameConfig::EMusic::EM_DoorClosed:
		return "Music/Door/DoorClose.mp3";
		break;
	case GameConfig::EMusic::EM_DoorOpen:
		return "Music/Door/DoorOpen.mp3";
		break;
	case GameConfig::EMusic::EM_CrumblingPlatformBreak:
		return "Music/Platform/Platform_break.mp3";
		break;
	default:
		return "Music/Level_Music/Music_Level_1.mp3";
		break;
	};
}

// Method to preload all the Music needed in the game, so that it does not cause lag later on. 
void CAudioManager::Preload()
{

	for( int iIterator = 0; iIterator != ( int )GameConfig::EBackgroundSounds::EBS_End; iIterator++ )
	{
		cocos2d::experimental::AudioEngine::preload( GetBackGroundFilePath( ( GameConfig::EBackgroundSounds )iIterator ) );

	}
	for( int iIterator = 0; iIterator != ( int )GameConfig::EMusic::EM_End; iIterator++ )
	{
		cocos2d::experimental::AudioEngine::preload( GetFilePath( ( GameConfig::EMusic )iIterator ) );
	}

}

// Method to play Sound Effects.
void CAudioManager::PlayEffectsMusic( GameConfig::EMusic eMusic, bool bIsLooped )
{
	//the play 2d function return a id which we can use to make changes to the playing sound effect 
	u32 uiId = cocos2d::experimental::AudioEngine::play2d( GetFilePath( eMusic ), bIsLooped ); 
	if( m_iEffectsVolumePercentage >= 0.0f )
	{
		cocos2d::experimental::AudioEngine::setVolume( uiId, m_pcDataPersistence->GetEffectsVolume() / 100.f );
	}
	// If the Sound effect is looped store the Id in the vector along with what type of sound effect it is.
	if( bIsLooped )
	{
		SSoundIdPair sPair;
		sPair.eSFX = eMusic;
		sPair.uiSFXId = uiId;
		m_vsSoundEffectsPair.push_back( sPair );
	}
}

// Method to stop the Sound Effects that are looped.
void CAudioManager::StopEffectsMusic( GameConfig::EMusic eMusic )
{
	// Flag to see if we have found the Sound effect.
	bool bFlag = false;
	//Iterator to store the position of the Sound effect in the Vector.
	int iIterator = -1;
	for( SSoundIdPair spair : m_vsSoundEffectsPair )
	{
		iIterator++;
		if( spair.eSFX == eMusic )
		{
			cocos2d::experimental::AudioEngine::stop( spair.uiSFXId );
			// Removing the Sound Effect and its id from the vector.
			m_vsSoundEffectsPair.erase( m_vsSoundEffectsPair.begin() + iIterator );
			bFlag = true;
			return;
		}
	}
	if( bFlag )
	{
		std::cout << "File not found" << std::endl;

	}
}

// Takes a 0-100 value for volume, where 0 is muted and 100 is full volume, I.E. Percentage.
void CAudioManager::ChangeEffecstVolume( int fVolume )
{

	m_iEffectsVolumePercentage = fVolume;

	//Loop through the Vector containing all the playing Sound effects and change their volume to the new one.
	for( SSoundIdPair spair : m_vsSoundEffectsPair )
	{
		cocos2d::experimental::AudioEngine::setVolume( spair.uiSFXId, m_iEffectsVolumePercentage / 100.f );

	}
	// Save the new Volume percentage using data persistence
	m_pcDataPersistence->SaveEffectsVolume( m_iEffectsVolumePercentage );

	//Update the Footstep volume
	if( m_bIsFootstepPlaying )
	{
		cocos2d::experimental::AudioEngine::setVolume( m_iFootstepId, m_iEffectsVolumePercentage / 100.f );
	}

}

// Takes a 0-100 value for volume, where 0 is muted and 100 is full volume
void CAudioManager::ChangeBackgroundVolume( int fVolume )
{
	m_iBackgroundVolumePercentage = fVolume;

	//If there is a background music playing change its volume
	if( m_iBackgroundId != 0 )
	{
		cocos2d::experimental::AudioEngine::setVolume( m_iBackgroundId, m_iBackgroundVolumePercentage / 100.0f );

	}
	// Save the new Volume percentage using data persistence
	m_pcDataPersistence->SaveBackgroundVolume( m_iBackgroundVolumePercentage );
}

//Returns the current Sound Effects Volume percentage
int CAudioManager::GetEffectsVolune()
{
	return m_iEffectsVolumePercentage;
}

//Return the current Background Music Volume Percentage
int CAudioManager::GetBackgroundVolume()
{
	return m_iBackgroundVolumePercentage;
}

// Method to Pause the current background music and save its ID, Mainly use for when we bring up the pause menu.
void CAudioManager::PauseBackground()
{
	if( m_iBackgroundId != 0 )
	{
		cocos2d::experimental::AudioEngine::pause( m_iBackgroundId );
		m_iOldBackgroundId = m_iBackgroundId;
		m_iBackgroundId = 0;
	}

}

// Method to resume the Paused Background Music and Change its Volume to the new Volume levels.
void CAudioManager::ResumeBackground()
{
	if( m_iOldBackgroundId != 0 )
	{
		cocos2d::experimental::AudioEngine::resume( m_iOldBackgroundId );
		m_iBackgroundId = m_iOldBackgroundId;
		m_iOldBackgroundId = 0;
		cocos2d::experimental::AudioEngine::setVolume( m_iBackgroundId, m_iBackgroundVolumePercentage / 100.0f );
	}

}

// Method to play the footstep sound
void CAudioManager::PlayFootstep()
{
	if( m_iFootstepId == 0 )
	{
		m_iFootstepId = cocos2d::experimental::AudioEngine::play2d( GetFilePath( GameConfig::EMusic::EM_DanielleMove ), true );
		cocos2d::experimental::AudioEngine::setVolume( m_iFootstepId, m_iEffectsVolumePercentage / 100.f );
		m_bIsFootstepPlaying = true;
	}
}

// Method to pause the footstep sound
void CAudioManager::PauseFootStep()
{
	if( m_bIsFootstepPlaying )
	{
		cocos2d::experimental::AudioEngine::pause( m_iFootstepId );
		m_bIsFootstepPlaying = false;
	}
}

// Method to resume the footstep sound
void CAudioManager::ResumeFootstep()
{
	if( !m_bIsFootstepPlaying )
	{
		cocos2d::experimental::AudioEngine::resume( m_iFootstepId );
		m_bIsFootstepPlaying = true;
	}
}

// Method to stop the footstep sound
void CAudioManager::StopFootstep()
{
	if( m_iFootstepId != 0 )
	{
		cocos2d::experimental::AudioEngine::stop( m_iFootstepId );
		m_iFootstepId = 0;
		m_bIsFootstepPlaying = false;
	}

}

// Method to check if the footstep sound is playing 
bool CAudioManager::IsFootstepPlaying()
{
	return m_bIsFootstepPlaying;
}

// Method to Play the background Music
void CAudioManager::PlayBackgroundMusic( GameConfig::EBackgroundSounds eBackgroundSound, bool bIsLooped )
{
	//If the there is some background music already playing, then storethe id of the already playing Music
	if( m_iBackgroundId != 0 )
	{
		m_iOldBackgroundId = m_iBackgroundId;
	}
	
	u32 uiId = cocos2d::experimental::AudioEngine::play2d( GetBackGroundFilePath( eBackgroundSound ), bIsLooped );
	if( m_iEffectsVolumePercentage >= 0 )
	{
		cocos2d::experimental::AudioEngine::setVolume( uiId, m_pcDataPersistence->GetBackgroundVolume() / 100.f );
	}

	//If the background music we want to play is the menu music, raise the flag 
	if( eBackgroundSound == GameConfig::EBackgroundSounds::EBS_Menu )
	{
		m_bIsMenuMusicPlaying = true;
	}
	m_iBackgroundId = uiId;
}

// Method to stop the background music
void CAudioManager::StopBackgroundMusic()
{
	if( m_iBackgroundId != 0 )
	{
		cocos2d::experimental::AudioEngine::stop( m_iBackgroundId );
		m_iBackgroundId = 0;
	}
	// If it is the menu music that we are stopping set the flag to false
	if( m_bIsMenuMusicPlaying )
	{
		m_bIsMenuMusicPlaying = false;
	}
}

// Method to return the value of the flag which tells us if the menu music is playing.
bool CAudioManager::MenuMusicIsPlaying()
{
	return m_bIsMenuMusicPlaying;
}