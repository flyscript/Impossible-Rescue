////////////////////////////////////////////////////////////////////////////////////////////////////////////
// (C) Gamer Camp / Alex Darby 2018
// Distributed under the MIT license - see readme.md
////////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef _GCOBJPLAYER_H_
#define _GCOBJPLAYER_H_

#include "GamerCamp/GameSpecific/Projectiles/GCObjGroupProjectile.h"
#include "ui/UILoadingBar.h"
#include "Helper/GameConfig.h"
#include "GamerCamp/GCCocosInterface/IGCGameLayer.h"
#include "GamerCamp/GameController/AudioManager/CAudioManager.h"

#ifndef _GCOBJSPRITEPHYSICS_H_
#include "../../GCCocosInterface/GCObjSpritePhysics.h"
#endif

#ifndef _GCFACTORY_OBJSPRITEPHYSICS_H_
#include "../../GCCocosInterface/GCFactory_ObjSpritePhysics.h"
#endif


//////////////////////////////////////////////////////////////////////////
// forward declare
class CObjShadow;
class CHackable;

class CGCObjGroupProjectile;
template< typename TActionType > class TGCActionToKeyMap;

//////////////////////////////////////////////////////////////////////////
// This is a sample class derived from CGCObject.
// 
// It could be the basis of your player object, it's up to you really.
//
// The player will be the only gameplay object in the default object group
//
//////////////////////////////////////////////////////////////////////////
class CGCObjPlayer
	: public CGCObjSpritePhysics
{
private:

	//Audio Manager
	CAudioManager* m_pcAudioManager;

	CDataPersistence* m_pcDataPersistence;

	bool m_bControlIsEnabled;

	////////////////////////////////
	// HEALTH & LIVES
	////////////////////////////////

	// Number of lives that the player has - set to what they start with
	int m_iLives;

	// Amount of health that the player has - set to what they start with
	int m_iHealth;

	// When the player gets damaged, how long to debounce before they can be damaged again
	float m_fTimeUntilCanBeDamaged;

	// The countdown timer used to assess if the player can be damaged or not
	float m_fDamageCountDown;

	// Life bars
	cocos2d::Sprite* m_pHealthBar;
	cocos2d::Label* m_pLivesText;


	////////////////////////////////
	// FIRING
	////////////////////////////////

	// Used to fire projectiles
	CGCObjGroupProjectile* m_pProjectileManager;

	// Current Ammo Type
	GameConfig::EWeaponType m_eCurrentWeaponType;

	// Countdown for current ammo
	float m_fWeaponTimeLeft;
	float m_fWeaponTimeMax;

	// Fire rate variables
	float m_fFireRate; // time between shots (bullets/second)
	float m_fDefaultFireRate; // fire rate to revert to if an effect wears off
	float m_fTimeUntilCanFire;

	// Pointer to active scene for ui
	IGCGameLayer* m_pActiveScene;

	// Time bar for ammo
	cocos2d::ui::LoadingBar* m_pAmmoTimerBar;

	// Time bar for weapon
	cocos2d::Sprite* m_pWeaponTimerBar;

	//Defines movement characteristics of player
	//TODO: expand to character class which enemies and player can inherit from


	////////////////////////////////
	// MOVEMENT
	////////////////////////////////

	// "Thrust" and "drag" values for player movement
	f32 m_fWalkAcceleration;
	f32 m_fWalkDrag;

	// Amount of jump or drag force added each frame when jumping
	f32 m_fJumpForce;
	f32 m_fJumpDrag;

	// However fast the player presses jump button, the player will ALWAYS jump at LEAST this high
	float m_fMinJumpOffset;

	// When jump button is held for longer, the player can jump UPTO this height
	float m_fMaxJumpOffset;

	// Temporary values that change each jump
	float m_fMinJumpHeight;
	float m_fMaxJumpHeight;

	// Maximum number of jumps allowed (for expanding to double, triple jump)
	int m_iNumberOfJumpsLeft;


	////////////////////////////////
	// CONTROLLER
	////////////////////////////////

	// Used to stop input being taken EVERY frame when hacking
	bool m_bStickDebounce;

	// Used to stop input being taken EVERY frame when firing
	bool m_bFireDebounce;

	// Deadzone of the controller stick (where we get direction from)
	float m_fControllerDeadzone;

	// Movement deadzone (how far we need to press to move in a direction, not just face it)
	float m_fControllerMovementDeadzone;

	// 1D value of a 2D unit vector above which we begin clamping in 45 degree increments
	float m_fDirectionalFiringClampingFactor;

	// Action map for controllers
	TGCActionToKeyMap< GameConfig::EPlayerActions >* m_pcControllerActionToKeyMap;

	////////////////////////////////
	//SHADOW
	////////////////////////////////
	CObjShadow* m_pShadow;

	////////////////////////////////
	//ANIMATION
	////////////////////////////////
	cocos2d::Action*	m_pCurrentAction;
	cocos2d::Animation*	m_pCurrentAnimation;
	cocos2d::ValueMap*	m_pcDicPList;


	////////////////////////////////
	//HACKING
	////////////////////////////////
	//Pointer to currently interacting hackable object
	CHackable* m_pCHackingObject;

	////////////////////////////////
	//STATES
	////////////////////////////////
	enum AnimationState
	{
		AS_Standing = 0,
		AS_Running,
		AS_Jumping,
		AS_Falling,
		AS_Hacking
	};
	AnimationState m_eCurrentAnimState;
	AnimationState m_ePreviousAnimState;
	bool m_bCanJump;
	bool m_bJumping;
	int m_iGroundFlags;
	int m_iNumberOfJumps;
	bool m_bCanShadowChargePlay;

public:
	CGCObjPlayer();

	//////////////////////////////////////////////////////////////////////////
	// declare the factory method to enable this to be created via CGCFactory_ObjSpritePhysics 
	GCFACTORY_DECLARE_CREATABLECLASS( CGCObjPlayer );

	//////////////////////////////////////////////////////////////////////////
	// we need a virtual destructor since delete will be called on pointers of 
	// this class to delete derived types.
	virtual ~CGCObjPlayer()
	{
		delete m_pShadow;
	}


	//////////////////////////////////////////////////////////////////////////
	// overridden virtuals from the game object interface

		// This will be called exactly once for each CGCObject derived class instance 
		// registered with CGCObjectManager as soon as the TGB level file has stopped 
		// loading- it is an "explicit constructor".
	virtual void VOnResourceAcquire();

	// OnReset is called for each CGCObject derived class when CGCObjectManager is 
	// reset
	virtual void VOnReset();

	// OnUpdate is called for each CGCObject derived class when CGCObjectManager is 
	// updated in t2dSCeneGraph onProcessTick()
	virtual void VOnUpdate( f32 fTimeStep );

	// called immediately before the managing object group releases its own assets
	virtual void VOnResourceRelease();

	virtual void VOnResurrected() override;

	// overridden virtuals from the game object interface
	//////////////////////////////////////////////////////////////////////////

	// Updates the movement of the CCSprite
	void UpdateMovement( f32 fTimeStep );

	// Checks which animation should be running
	void UpdateAnimation( bool bOverride );

	void Animate( const char* kcAnimation, bool bLooped, float fFPS = 32.0f );

	// this function exists purely to better illustrate the EXAMPLE collision detection functionality in CGCGameLayerSpaceInvaders
	void NotifyOfCollisionWithInvader();

	// Allows player to jump again, usually called by collision handler
	void SetCanJump( bool a_bCanJump );

	//Stops the current jump (if any) when the player's head hits the ceiling
	void HeadHit();

	// Returns whether or not the player is jumping
	bool GetIsJumping();

	// Points the player to the hackable object they're currently looking at
	void SetHackableInteract( CHackable* pCHackableObject );

	// Tells the player that they've left a hackable object
	void ExitHackableInteract();

	// Sets the ammo type, default argument at the end (as this won't need to be expanded further) for time
	void ChangeWeaponType( GameConfig::EWeaponType eWeaponType, float fTime = 5.0f, float fFireRate = 0.0f );

	// Returns the number of lives after they've been decremented
	inline int DecrementLives();

	// Returns the number of lives
	inline int GetLives();

	// Sets the number of lives
	inline void SetLives( int iLives );

	// Returns the amount of health
	inline float GetHealth();

	// Negates the health by the amount of given damage and returns it
	inline float AddDamage( float fDamage );

	// Gets whether or not the player is on the ground
	inline bool GetIsOnGround();

	// Sets whether or not the player is on the ground
	inline void SetIsOnGround( bool bOnGround );
};

inline bool CGCObjPlayer::GetIsOnGround()
{
	return m_iGroundFlags > 0;
}

inline void CGCObjPlayer::SetIsOnGround( bool bOnGround )
{
	m_iGroundFlags += bOnGround ? 1 : -1;
	m_iNumberOfJumpsLeft = bOnGround ? GameConfig::k_iMaxNumberOfJumps : m_iNumberOfJumpsLeft;
}

inline int CGCObjPlayer::DecrementLives()
{
	return --m_iLives;
}

inline int CGCObjPlayer::GetLives()
{
	return m_iLives;
}

inline void CGCObjPlayer::SetLives( int iLives )
{
	m_iLives = iLives;
}

inline float CGCObjPlayer::GetHealth()
{
	return m_iHealth;
}
inline float CGCObjPlayer::AddDamage( float fDamage )
{
	// Only damage if the player can be damaged
	if( m_fDamageCountDown <= 0 )
	{
		// Reset timer
		m_fDamageCountDown = m_fTimeUntilCanBeDamaged;

		// Take the damage off the health
		m_iHealth -= fDamage;

		// Kill the player if their health is 0
		if( m_iHealth <= 0.0f )
		{
			// Take life off
			m_iLives -= 1;
			// Play kill sound
			m_pcAudioManager->PlayEffectsMusic( GameConfig::EMusic::EM_DanielleDie, false );
		}
		else
		{
			// Only play hurt sound if we're not playing kill sound
			m_pcAudioManager->PlayEffectsMusic( GameConfig::EMusic::EM_DanielleHurt, false );
		}


		//Set health bar percentage
		char cHealthImage[ 50 ];
		sprintf( cHealthImage, "Loose/UI/HUD/Health%d.png", ( int )m_iHealth );
		m_pHealthBar->setTexture( cHealthImage );
	}


	return m_iHealth;
}

#endif // #ifndef _GCOBJPLAYER_H_