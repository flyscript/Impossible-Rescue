////////////////////////////////////////////////////////////////////////////////////////////////////////////
// (C) Gamer Camp / Alex Darby 2018
// Distributed under the MIT license - see readme.md
////////////////////////////////////////////////////////////////////////////////////////////////////////////
#include <memory.h>

#include "AppDelegate.h"
#include "GamerCamp/GameController/GCController.h"
#include "GamerCamp/GameSpecific/Hackables/CHackable.h"
#include "GamerCamp/GameSpecific/Projectiles/GCObjGroupProjectile.h"
#include "GamerCamp/GameSpecific/Player/CObjShadow.h"
#include "GamerCamp/GCCocosInterface/GCCocosHelpers.h"
#include "GamerCamp/GCCocosInterface/IGCGameLayer.h"
#include "GamerCamp/GCObject/GCObjectManager.h"
#include <iostream>

#include "cocos2d/cocos/2d/CCAnimation.h"
#include "GCObjPlayer.h"
#include "GamerCamp/GameSpecific/Levels/Menu/CPauseMenu.h"
#include "GamerCamp/EventManager/CEventManager.h"

USING_NS_CC;


// action map arrays must match in length - in the templated controller class we use they map from the user define enum to cocos2d::Controller::Key 
static GameConfig::EPlayerActions			s_aePlayerActions[] = { GameConfig::EPlayerActions::EPA_NULL,		GameConfig::EPlayerActions::EPA_Up,			GameConfig::EPlayerActions::EPA_Down,			GameConfig::EPlayerActions::EPA_Left,			GameConfig::EPlayerActions::EPA_Right,				GameConfig::EPlayerActions::EPA_Jump,		GameConfig::EPlayerActions::EPA_Fire_Axis,   GameConfig::EPlayerActions::EPA_Fire,					GameConfig::EPlayerActions::EPA_Hack,		GameConfig::EPlayerActions::EPA_Options,	GameConfig::EPlayerActions::EPA_AxisMove_X,		GameConfig::EPlayerActions::EPA_AxisMove_Y };
static cocos2d::Controller::Key				s_aeKeys[] = { cocos2d::Controller::Key::KEY_NONE,			cocos2d::Controller::Key::BUTTON_DPAD_UP,	cocos2d::Controller::Key::BUTTON_DPAD_DOWN,		cocos2d::Controller::Key::BUTTON_DPAD_LEFT,		cocos2d::Controller::Key::BUTTON_DPAD_RIGHT,	    cocos2d::Controller::Key::BUTTON_A,			cocos2d::Controller::Key::AXIS_RIGHT_TRIGGER, cocos2d::Controller::Key::BUTTON_X,			cocos2d::Controller::Key::BUTTON_B,		cocos2d::Controller::Key::BUTTON_OPTIONS,		cocos2d::Controller::Key::JOYSTICK_LEFT_X,		cocos2d::Controller::Key::JOYSTICK_LEFT_Y };



//////////////////////////////////////////////////////////////////////////
// implement the factory method to enable this to be created via CGCFactory_ObjSpritePhysics 
GCFACTORY_IMPLEMENT_CREATEABLECLASS( CGCObjPlayer );


//////////////////////////////////////////////////////////////////////////
// GetGCTypeIDOf uses the template in GCTypeID to generate a unique ID for 
// this type - need this to construct our base type
CGCObjPlayer::CGCObjPlayer()
	: CGCObjSpritePhysics( GetGCTypeIDOf( CGCObjPlayer ) )
	, m_iLives( GameConfig::k_iPlayerLives )
	, m_iHealth( GameConfig::k_iPlayerMaxHealth )
	, m_fTimeUntilCanBeDamaged( 0.2f )
	, m_fDamageCountDown( 0.0f )
	, m_pHealthBar( nullptr )
	, m_pLivesText( nullptr )
	, m_pProjectileManager( nullptr )
	, m_fWeaponTimeLeft( 0.0f )
	, m_fWeaponTimeMax( 0.0f )
	, m_fFireRate( GameConfig::k_fStunFireRate )
	, m_fDefaultFireRate( GameConfig::k_fStunFireRate )
	, m_fTimeUntilCanFire( 0.0f )
	, m_pActiveScene( nullptr )
	, m_pAmmoTimerBar( nullptr )
	, m_fWalkAcceleration( GameConfig::k_fPlayerWalkForce )
	, m_fWalkDrag( GameConfig::k_fPlayerWalkDragForce )
	, m_fJumpForce( GameConfig::k_fPlayerJumpForce )
	, m_fJumpDrag( GameConfig::k_fPlayerJumpDragForce )
	, m_fMinJumpOffset( GameConfig::k_fPlayerMinJumpHeight )
	, m_fMaxJumpOffset( GameConfig::k_fPlayerMaxJumpHeight )
	, m_fMaxJumpHeight( 0.0f )
	, m_fMinJumpHeight( 0.0f )
	, m_bStickDebounce( false )
	, m_bFireDebounce( false )
	, m_iGroundFlags( 0 )
	, m_iNumberOfJumpsLeft( GameConfig::k_iMaxNumberOfJumps )
	, m_fControllerDeadzone( GameConfig::k_fLookDeadzone )
	, m_fControllerMovementDeadzone( GameConfig::k_fMovementDeadzone )
	, m_fDirectionalFiringClampingFactor( GameConfig::k_f45DegreeClampingFactor )
	, m_pCurrentAction( nullptr )
	, m_pCurrentAnimation( nullptr )
	, m_eCurrentAnimState( AnimationState::AS_Standing )
	, m_ePreviousAnimState( AnimationState::AS_Standing )
	, m_bJumping( false )
	, m_iNumberOfJumps( GameConfig::k_iPlayerJumps )
	, m_pcControllerActionToKeyMap( nullptr )
	, m_pCHackingObject( nullptr )
	, m_pShadow( nullptr )
	, m_pcAudioManager( nullptr )
	, m_bControlIsEnabled( false )
	, m_pcDataPersistence( nullptr )
	, m_bCanShadowChargePlay( false )
{
}


//////////////////////////////////////////////////////////////////////////
// 
//////////////////////////////////////////////////////////////////////////
//virtual 
void CGCObjPlayer::VOnResourceAcquire()
{
	SetName( "Danielle" );

	CGCObjSpritePhysics::VOnResourceAcquire();

	// animate!
	Animate( "Stand", true );

	// find the player projectile group 
	// N.B. we know this cast is safe because we're checking the typeID
	m_pProjectileManager = static_cast< CGCObjGroupProjectile* >
		( CGCObjectManager::FindObjectGroupByID( GetGCTypeIDOf( CGCObjGroupProjectile ) ) );

	// because we're just storing a vanilla pointer we must call delete on it in VOnResourceRelease or leak memory 
	// 
	// n.b. m_pcControllerActionToKeyMap is a "perfect use case" for std::unique_ptr...
	// 
	// n.n.b. ... however if we did use std::unique_ptr we'd need to use std::unique_ptr::reset in VOnResourceRelease if we wanted the memory allocate / free behaviour to be the same...
	m_pcControllerActionToKeyMap = TCreateActionToKeyMap( s_aePlayerActions, s_aeKeys );

	// Default jump at start of level
	SetCanJump( true );

	// Get active scene
	m_pActiveScene = IGCGameLayer::ActiveInstance();

	// Get useful information from the scene
	cocos2d::Size visibleSize = cocos2d::Director::getInstance()->getVisibleSize();
	cocos2d::Point origin = cocos2d::Director::getInstance()->getVisibleOrigin();

	// Initialise Shadow
	m_pShadow = new CObjShadow();
	m_pShadow->CreateSprite( "TexturePacker/Sprites/Characters/Player/Shadow/Shadow.plist" );
	m_pShadow->SetResetPosition( GetResetPosition() + Vec2( 100.0f, 30.0f ) );
	m_pShadow->SetFlippedX( IsFlippedX() );
	m_pShadow->SetPlayer( this );


	// Initialize health bar
	m_pHealthBar = Sprite::create( "Loose/UI/HUD/Health3.png" );
	m_pHealthBar->setPosition( cocos2d::Vec2( 100.f + m_pHealthBar->getContentSize().width / 2, 1030.f ) );
	m_pHealthBar->setVisible( true );
	m_pActiveScene->addChild( m_pHealthBar, int( GameConfig::EZIndexLayer::EZIL_HUD_Top ) );

	// Initialize lives bar
	m_pLivesText = Label::createWithTTF( "x3", "fonts/space_age.ttf", 36 );
	m_pLivesText->setAlignment( TextHAlignment::CENTER );
	m_pLivesText->setTextColor( cocos2d::Color4B( 0, 222, 255, 255 ) );
	// m_pLivesText->enableShadow( cocos2d::Color4B::BLACK, cocos2d::Size( -2, -2 ), 5.f );
	m_pLivesText->setPosition( cocos2d::Vec2( 60.f, 1030.f ) );
	m_pLivesText->setVisible( true );
	m_pActiveScene->addChild( m_pLivesText, int( GameConfig::EZIndexLayer::EZIL_HUD_Top ) );

	// Initialize weapon bar
	m_pWeaponTimerBar = Sprite::create( "Loose/UI/HUD/Charge5.png" );
	m_pWeaponTimerBar->setPosition( cocos2d::Vec2( 115.f + m_pHealthBar->getContentSize().width / 2, 980.f - m_pHealthBar->getContentSize().height / 2 ) );
	m_pWeaponTimerBar->setVisible( true );
	m_pActiveScene->addChild( m_pWeaponTimerBar );

	// Initialise audio manager
	m_pcAudioManager = new CAudioManager();
	m_pcDataPersistence = new CDataPersistence();
	m_iHealth = m_pcDataPersistence->GetHealth();
	m_iLives = m_pcDataPersistence->GetLives();
}



//////////////////////////////////////////////////////////////////////////
// 
//////////////////////////////////////////////////////////////////////////
//virtual 
void CGCObjPlayer::VOnReset()
{
	CGCObjSpritePhysics::VOnReset();

	m_pcAudioManager->PlayFootstep();

	// Reset health to default
	if( m_iLives < m_pcDataPersistence->GetLives() )
	{
		m_iHealth = GameConfig::k_iPlayerMaxHealth;
	}
	else
	{
		m_iHealth = m_pcDataPersistence->GetHealth();
	}


	//Set health bar percentage
	char cHealthImage[ 50 ];
	sprintf( cHealthImage, "Loose/UI/HUD/Health%d.png", ( int )m_iHealth );
	m_pHealthBar->setTexture( cHealthImage );


	// Set lives text
	char cLives[ 3 ];
	sprintf( cLives, "x%d", m_iLives );
	m_pLivesText->setString( cLives );

	// reset velocity and flip state
	SetFlippedX( false );
	SetFlippedY( false );

	SetCanJump( true );

	//Set back to taser
	m_eCurrentWeaponType = GameConfig::EWeaponType::EWT_Stun;

	// reset
	if( GetPhysicsBody() )
	{
		Vec2 v2SpritePos = GetSpritePosition();
		GetPhysicsBody()->SetLinearVelocity( b2Vec2( 0.0f, 0.0f ) );
		GetPhysicsBody()->SetTransform( IGCGameLayer::B2dPixelsToWorld( b2Vec2( v2SpritePos.x, v2SpritePos.y ) ), 0.0f );
		GetPhysicsBody()->SetFixedRotation( true );
	}
}



//////////////////////////////////////////////////////////////////////////
// 
//////////////////////////////////////////////////////////////////////////
//virtual 
void CGCObjPlayer::VOnUpdate( f32 fTimeStep )
{
	// Handle movement
	UpdateMovement( fTimeStep );

	// Handle animations
	UpdateAnimation( false );
}


//////////////////////////////////////////////////////////////////////////
// 
//////////////////////////////////////////////////////////////////////////
//virtual
void CGCObjPlayer::VOnResourceRelease()
{
	m_pcAudioManager->StopFootstep();
	CGCObjSpritePhysics::VOnResourceRelease();
	delete m_pcControllerActionToKeyMap;
	m_pcControllerActionToKeyMap = nullptr;
}

void CGCObjPlayer::VOnResurrected()
{
	CGCObjSpritePhysics::VOnResurrected();
	// Hotfix for fixing rotation
	GetPhysicsBody()->SetFixedRotation( true );

	CEventManager::Manager().RemoveAllListenersOfType( GameConfig::EET_TimerFinished );

	CEventManager::Manager().AddListener( GameConfig::EEventType::EET_ControlDisable, [ & ] ( void* )
	{
		m_bControlIsEnabled = false;
	} );
	CEventManager::Manager().AddListener( GameConfig::EEventType::EET_ControlEnable, [ & ] ( void* )
	{
		m_bControlIsEnabled = true;
	} );
	CEventManager::Manager().AddListener( GameConfig::EEventType::EET_ControlEnable, [ & ] ( void* )
	{
		m_bControlIsEnabled = true;
	} );

	CEventManager::Manager().AddListener( GameConfig::EEventType::EET_TimerFinished, [ & ] ( void* )
	{
		if( m_iLives == 1 )
		{
			CEventManager::Manager().Trigger( GameConfig::EEventType::EET_LivesOver, nullptr );
		}
		else
		{
			m_iLives--;
		}

	} );
}


//////////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////////
void CGCObjPlayer::UpdateMovement( f32 fTimeStep )
{


	////////////////////////////////
	// DEFAULTS
	////////////////////////////////

	// we accumulate total force over the frame and apply it at the end
	Vec2 v2TotalForce( 0.0f, 0.0f );

	// * calculate the control force direction
	Vec2 v2ControlForceDirection( 0.0f, 0.0f );

	// * calculate the look direction
	Vec2 v2LookDirection( 0.0f, 0.0f );

	// this float is used to add / remove the effect of various terms 
	// in equations based on whether input has been applied this frame
	f32 fIsInputInactive = 1.0f;

	// instantiating templates is one of the few use cases where auto is a big improvement & arguably the best thing to do
	// e.g.
	//	auto cController = ... ;
	const CGCKeyboardManager*		pKeyManager = AppDelegate::GetKeyboardManager();
	TGCController< GameConfig::EPlayerActions > cController = TGetActionMappedController( CGCControllerManager::eControllerOne, ( *m_pcControllerActionToKeyMap ) );







	////////////////////////////////
	// UI & HEALTH
	////////////////////////////////

	// Decrement timer if it's still counting down (prevent buffer underflow errors)
	m_fDamageCountDown -= ( m_fDamageCountDown > 0.0f ) * fTimeStep;
	m_fWeaponTimeLeft -= ( m_fWeaponTimeLeft > 0.0f ) * fTimeStep;
	m_fTimeUntilCanFire = m_fTimeUntilCanFire > 0.0f ? m_fTimeUntilCanFire - fTimeStep : 0.0f;

	// Reset weapon if we ran out of time
	if( m_eCurrentWeaponType != GameConfig::EWeaponType::EWT_Stun && m_fWeaponTimeLeft <= 0.0f )
	{
		m_eCurrentWeaponType = GameConfig::EWeaponType::EWT_Stun;
		m_fFireRate = m_fDefaultFireRate;
		m_fTimeUntilCanFire = 0.0f;
	}

	// Update the loading bar progress by calculating the progress in percentage
	char cAmmoImage[ 50 ];
	if( m_eCurrentWeaponType == GameConfig::EWeaponType::EWT_Stun )
	{
		sprintf( cAmmoImage, "Loose/UI/HUD/Charge%d.png", ( int )( ( m_fFireRate - m_fTimeUntilCanFire ) / m_fFireRate * 5.0f ) );
	}
	else
	{
		sprintf( cAmmoImage, "Loose/UI/HUD/Charge%d.png", ( int )( m_fWeaponTimeLeft / m_fWeaponTimeMax * 5.0f ) );
	}
	if( ( ( ( int )m_fWeaponTimeLeft / m_fWeaponTimeMax * 5.0f ) == 5 ) || ( ( ( int )( ( m_fFireRate - m_fTimeUntilCanFire ) / m_fFireRate * 5.0f ) ) == 5 ) )
	{
		if( m_bCanShadowChargePlay )
		{
			m_pcAudioManager->PlayEffectsMusic( GameConfig::EMusic::EM_ShadowCharge, false );
			m_bCanShadowChargePlay = false;
		}
	}
	else
	{
		m_bCanShadowChargePlay = true;
	}
	m_pWeaponTimerBar->setTexture( cAmmoImage );


	////////////////////////////////
	// INPUT
	////////////////////////////////

	bool bJumpPressed = false;
	bool bJumpHeld = false;
	bool bHackPressed = false;
	GameConfig::EPlayerActions eAction = GameConfig::EPlayerActions::EPA_NULL;

	if( cController.IsActive() )
	{
		Vec2 v2LeftStickLookDeadzoned = cController.GetCurrentStickValueDeadzoned( GameConfig::EPlayerActions::EPA_AxisMove_X, GameConfig::EPlayerActions::EPA_AxisMove_Y, m_fControllerDeadzone );
		v2LookDirection.x = v2LeftStickLookDeadzoned.x;
		v2LookDirection.y = v2LeftStickLookDeadzoned.y;

		v2ControlForceDirection.x = v2LeftStickLookDeadzoned.x >= m_fControllerMovementDeadzone || v2LeftStickLookDeadzoned.x <= -m_fControllerMovementDeadzone ? v2LeftStickLookDeadzoned.x : 0.0f;

		/*if ( cController.ButtonHasJustBeenPressed( GameConfig::EPlayerActions::EPA_Options ) )
		{
			IGCGameLayer::ActiveInstance()->PushScene( TGCGameLayerSceneCreator< CGCPauseMenu >::CreateScene() );
		}
		*/
		//Hack
		if( cController.ButtonIsPressed( GameConfig::EPlayerActions::EPA_Hack ) )
		{
			eAction = GameConfig::EPlayerActions::EPA_Hack;
			bHackPressed = true;
		}

		//Jump
		if( cController.ButtonHasJustBeenPressed( GameConfig::EPlayerActions::EPA_Jump ) )
		{
			bJumpPressed = true;
		}
		else
			if( cController.ButtonIsPressed( GameConfig::EPlayerActions::EPA_Jump ) )
			{
				bJumpHeld = true;
			}


		//if thumb stick is not at rest 
		if( v2ControlForceDirection.length() > 0.0f )
		{
			//note that we have taken input by setting fIsInputInactive to 0
			fIsInputInactive = 0.0f;
		}



		if( !m_bStickDebounce && (
			cController.GetCurrentAxisDeadzoned( GameConfig::EPlayerActions::EPA_AxisMove_Y, m_fControllerMovementDeadzone ) >= m_fControllerMovementDeadzone
			||
			cController.ButtonHasJustBeenPressed( GameConfig::EPlayerActions::EPA_Up )
			) )
			eAction = GameConfig::EPlayerActions::EPA_Up;

		if( !m_bStickDebounce && (
			cController.GetCurrentAxisDeadzoned( GameConfig::EPlayerActions::EPA_AxisMove_Y, m_fControllerMovementDeadzone ) <= -m_fControllerMovementDeadzone
			||
			cController.ButtonHasJustBeenPressed( GameConfig::EPlayerActions::EPA_Down )
			) )
			eAction = GameConfig::EPlayerActions::EPA_Down;

		if( !m_bStickDebounce && (
			cController.GetCurrentAxisDeadzoned( GameConfig::EPlayerActions::EPA_AxisMove_X, m_fControllerMovementDeadzone ) <= -m_fControllerMovementDeadzone
			||
			cController.ButtonHasJustBeenPressed( GameConfig::EPlayerActions::EPA_Left )
			) )
		{
			eAction = GameConfig::EPlayerActions::EPA_Left;
		}


		if( !m_bStickDebounce && (
			cController.GetCurrentAxisDeadzoned( GameConfig::EPlayerActions::EPA_AxisMove_X, m_fControllerMovementDeadzone ) >= m_fControllerMovementDeadzone
			||
			cController.ButtonHasJustBeenPressed( GameConfig::EPlayerActions::EPA_Right )
			) )
		{
			eAction = GameConfig::EPlayerActions::EPA_Right;
		}


		m_bStickDebounce = !cController.GetCurrentStickValueDeadzoned( GameConfig::EPlayerActions::EPA_AxisMove_X, GameConfig::EPlayerActions::EPA_AxisMove_Y, 0.99f ).isZero();

	}

	// Keyboard
	else
	{
		//Hack
		if( pKeyManager->ActionIsPressed( u32( GameConfig::EPlayerActions::EPA_Hack ) ) )
		{
			eAction = GameConfig::EPlayerActions::EPA_Hack;
			bHackPressed = true;
		}

		//Jump
		if( pKeyManager->ActionIsPressed( u32( GameConfig::EPlayerActions::EPA_Jump ) ) )
		{
			bJumpPressed = true;
		}
		if( pKeyManager->ActionHasJustBeenPressed( u32( GameConfig::EPlayerActions::EPA_Jump ) ) )
		{
			bJumpHeld = true;
		}

		if( pKeyManager->ActionIsPressed( u32( GameConfig::EPlayerActions::EPA_Up ) ) )
		{
			v2LookDirection.y = 1.0f;
			fIsInputInactive = 0.0f;
		}
		if( pKeyManager->ActionIsPressed( u32( GameConfig::EPlayerActions::EPA_Down ) ) )
		{
			v2LookDirection.y = -1.0f;
			fIsInputInactive = 0.0f;
		}

		//Move
		if( pKeyManager->ActionIsPressed( u32( GameConfig::EPlayerActions::EPA_Left ) ) )
		{
			v2ControlForceDirection.x = -1.0f;
			v2LookDirection.x = -1.0f;
			fIsInputInactive = 0.0f;
		}
		if( pKeyManager->ActionIsPressed( u32( GameConfig::EPlayerActions::EPA_Right ) ) )
		{
			v2ControlForceDirection.x = 1.0f;
			v2LookDirection.x = 1.0f;
			fIsInputInactive = 0.0f;
		}

		if( pKeyManager->ActionHasJustBeenPressed( u32( GameConfig::EPlayerActions::EPA_Jump ) ) )
			eAction = GameConfig::EPlayerActions::EPA_Jump;
		if( pKeyManager->ActionHasJustBeenPressed( u32( GameConfig::EPlayerActions::EPA_Up ) ) )
			eAction = GameConfig::EPlayerActions::EPA_Up;
		if( pKeyManager->ActionHasJustBeenPressed( u32( GameConfig::EPlayerActions::EPA_Down ) ) )
			eAction = GameConfig::EPlayerActions::EPA_Down;
		if( pKeyManager->ActionHasJustBeenPressed( u32( GameConfig::EPlayerActions::EPA_Left ) ) )
		{

			eAction = GameConfig::EPlayerActions::EPA_Left;
		}

		if( pKeyManager->ActionHasJustBeenPressed( u32( GameConfig::EPlayerActions::EPA_Right ) ) )
		{

			eAction = GameConfig::EPlayerActions::EPA_Right;
		}


	}







	////////////////////////////////
	// JUMP
	////////////////////////////////

	// Starting a jump
	if( !m_bJumping && bJumpPressed && m_bCanJump && m_bControlIsEnabled )
	{
		m_pcAudioManager->PlayEffectsMusic( GameConfig::EMusic::EM_DanielleJump, false );
		// Set jumping to true
		m_bJumping = true;

		// Can no longer jump (remove for double jumps etc)
		m_bCanJump = false;

		// Input was taken
		fIsInputInactive = 0.0f;

		// Jump upward
		v2ControlForceDirection.y = 1.0f;

		// Set jump targets
		m_fMinJumpHeight = GetSpritePosition().y + m_fMinJumpOffset;
		m_fMaxJumpHeight = GetSpritePosition().y + m_fMaxJumpOffset;

		// Run jumping animation
		m_eCurrentAnimState = AS_Jumping;
	}

	// If jumping, but the minimum height hasn't been reached
	// OR 
	// Keep jumping if the player is holding jump and maximum frames hasn't been hit
	else if( m_bJumping
		&&
		(
		( GetSpritePosition().y < m_fMinJumpHeight )
			||
			( bJumpHeld && GetSpritePosition().y < m_fMaxJumpHeight )
			)
		)
	{

		// Jump upward but fade off as we approach the target (which we'll set as being the maximum height)
		float fInversePercentOfJumpComplete = (
			( m_fMaxJumpHeight - GetSpritePosition().y )
			/
			m_fMaxJumpHeight
			);

		v2ControlForceDirection.y = 0.5f + fInversePercentOfJumpComplete;

	}

	// Stop jumping
	else
	{
		m_bJumping = false;
	}




	////////////////////////////////
	// HACK
	////////////////////////////////

	if( m_pCHackingObject )
	{

		if( bHackPressed )
		{
			m_pCHackingObject->VTrigger();
		}

		m_pCHackingObject->SetPlayerInput( eAction );

		m_pCHackingObject->VUpdate( fTimeStep );
	}





	////////////////////////////////
	// MOVEMENT
	////////////////////////////////

	// Only move if it's allowed - stops accidental movement whilst hacking
	if( m_bControlIsEnabled )
	{
		if( !m_pCHackingObject ||
			m_pCHackingObject->IsHacked() ||
			m_pCHackingObject->GetCanMoveWhilstHacking() ||
			!m_pCHackingObject->GetTriggered()
			)
		{
			// Apply appropriate forces to the directions
			v2ControlForceDirection.x *= m_fWalkAcceleration;
			v2ControlForceDirection.y *= m_fJumpForce;
		}
		else
		{
			v2ControlForceDirection.x = 0.0f;
			v2ControlForceDirection.y = 0.0f;
		}
	}



	// Get original velocity
	Vec2 v2CurrentVelocity = GetVelocity();

	// Calculate pseudo-drag forces
	Vec2 v2Drag( v2CurrentVelocity.x*m_fWalkDrag, v2CurrentVelocity.y*m_fJumpDrag );
	v2Drag.y *= v2ControlForceDirection.y > 0.0f;

	// Overall force is equal to the directional forces minus the "drag" forces
	v2TotalForce += ( v2ControlForceDirection - v2Drag );

	// Physics calculations handled by box 2d based on force applied
	ApplyForceToCenter( v2TotalForce );






	////////////////////////
	//  ANIMATION
	////////////////////////

	if( !m_bJumping )
	{
		if( GetPhysicsBody()->GetLinearVelocity().x < 0.5f //TODO: make not magic
			&& GetPhysicsBody()->GetLinearVelocity().x > -0.5f )
		{
			// if this runs we're now standing
			m_pcAudioManager->PauseFootStep();
			m_eCurrentAnimState = AS_Standing;
		}
		else
		{
			m_pcAudioManager->ResumeFootstep();
			// if this runs we're now running
			m_eCurrentAnimState = AS_Running;
		}

		if( m_pCHackingObject && m_pCHackingObject->GetTriggered() )
		{
			m_pcAudioManager->PauseFootStep();
			m_eCurrentAnimState = AS_Hacking;
		}

		if( GetPhysicsBody()->GetLinearVelocity().y < -10.0f )
		{
			m_bCanJump = false;
			m_pcAudioManager->PauseFootStep();
			m_eCurrentAnimState = AS_Falling;
		}
	}


	// * set sprite flip based on velocity
	// N.B. the else-if looks redundant, but we want the sprite's flip 
	// state to stay the same if its velocity is set to (0.0f, 0.0f)
	if( v2LookDirection.x > 0.0f )
	{
		SetFlippedX( true );
	}
	else if( v2LookDirection.x < 0.0f )
	{
		SetFlippedX( false );
	}
	else if( v2LookDirection.y == 0.0f )
	{
		//Set look direction if look input wasn't registered this frame
		v2LookDirection.x = ( -1 + ( 2 * IsFlippedX() ) );
	}





	//////////////////////////////
	// FIRING
	//////////////////////////////

	// Boolean helps us at the end of these checks to decide if we should fire or not
	bool bFireWasPressed = false;

	// If using a controller
	if( cController.IsActive() )
	{
		//If we've just pressed, or it's held but we're on auto
		if( cController.GetCurrentAxisValueRaw( GameConfig::EPlayerActions::EPA_Fire_Axis ) > 0.0f && !m_bFireDebounce )
		{

			bFireWasPressed = true;
		}

		//If we've just pressed, or it's held but we're on auto
		if( cController.ButtonHasJustBeenPressed( GameConfig::EPlayerActions::EPA_Fire ) )
		{

			bFireWasPressed = true;
		}

		m_bFireDebounce = cController.GetCurrentAxisValueRaw( GameConfig::EPlayerActions::EPA_Fire_Axis ) > 0.0f;
	}

	// If using keyboard
	else
	{
		//If we've just pressed, or it's held but we're on auto
		if(
			pKeyManager->ActionHasJustBeenPressed( u32( GameConfig::EPlayerActions::EPA_Fire ) ) )
		{
			bFireWasPressed = true;

			/*
			// Old code for firing only left or right
			// Fire in direction we're facing
			float fDirection = (-1 + (2.f * IsFlippedX()));
			m_pProjectileManager->SpawnProjectile(
			GetSpritePosition() + Vec2(fDirection * (5.0f + (GetContentSize().width / 2.f)), 0.0f), //initial position
			Vec2(fDirection * 20.0f, 0.0f), //velocity
			3.0f //lifetime
			);
			*/
		}
	}

	// Check for fire button being pressed and we're within the fire rate - and that SHADOW exists
	if( bFireWasPressed && m_fTimeUntilCanFire <= 0.0f && m_pShadow && m_bControlIsEnabled )
	{
		// Set m_fTimeUntilCanFire to the fire rate, to be decremented every frame
		m_fTimeUntilCanFire = m_fFireRate;

		// Get a unit vector of where the player is looking
		Vec2 v2DirectionUnit = v2LookDirection;
		v2DirectionUnit.normalize();

		// Comment out below code to disable 45 degree firing clamping and go back to 360 firing:
		v2DirectionUnit.x =
			(	//whether or not the stick direction is far enough one way or the other enough to trigger clamping
				v2DirectionUnit.x > -m_fDirectionalFiringClampingFactor
				&&
				v2DirectionUnit.x < m_fDirectionalFiringClampingFactor
				)
			?
			0.0f //clamp to 0 if not large enough to trigger clamping
			:
			( v2DirectionUnit.x < 0.f ? -1.f : 1.f ) //set to -1 or 1, depending on direction, if large enough to trigger clamping
			;

		// Same as above
		v2DirectionUnit.y = ( v2DirectionUnit.y > -m_fDirectionalFiringClampingFactor && v2DirectionUnit.y < m_fDirectionalFiringClampingFactor ) ? 0.0f : ( v2DirectionUnit.y < 0.f ? -1.f : 1.f );

		// Set projectile offset from shadow dependent on the direction it will be fired in
		Vec2 v2Offset(
			40.0f * v2DirectionUnit.x,
			15.0f
		);

		// Ensure that shadow is facing the right way to fire this projectile
		m_pShadow->LockLookToPlayer();

		// Shoot the projectile in the calculated direction
		m_pProjectileManager->Fire(
			m_eCurrentWeaponType,						//weapon type
			m_pShadow->GetSpritePosition() + v2Offset,	//initial position
			v2DirectionUnit,							//direction
			3.0f										//lifetime - overridden by manager but may be expanded for additional life
		);
		m_pcAudioManager->PlayEffectsMusic( GameConfig::EMusic::EM_ShadowShoot, false );
	}
}

////////////////////////
//  ANIMATION
////////////////////////
void CGCObjPlayer::UpdateAnimation( bool bOverride )
{
	// Only run new animations if we're not already doing them - unless overridden
	if( m_eCurrentAnimState != m_ePreviousAnimState || bOverride )
	{
		// Will hold our animation name
		char cAnimation[ 50 ];

		// The framerate for animation - can be different per animation
		float fFPS;

		// If the animation should loop - true by default
		bool bLoop = true;

		// Run appropriate animation
		switch( m_eCurrentAnimState )
		{
		case AS_Standing:

			sprintf( cAnimation, "Stand" );
			fFPS = 16.0f;

			break;

		case AS_Running:

			sprintf( cAnimation, "Run" );
			fFPS = 64.0f;

			break;

		case AS_Jumping:

			sprintf( cAnimation, "Jump" );
			bLoop = false;
			fFPS = 120.0f;

			break;

		case AS_Hacking:

			sprintf( cAnimation, "Hack" );
			fFPS = 30.f;

			break;

		case AS_Falling:

			sprintf( cAnimation, "Fall" );
			fFPS = 30.0f;

			break;

		default:

			sprintf( cAnimation, "Stand" );
			fFPS = 16.0f;

			break;
		}

		// Play the selected animation
		Animate( cAnimation, bLoop, fFPS );
	}

	// Any updates that need to be made to the animation every frame
	switch( m_eCurrentAnimState )
	{
	case AS_Standing:
		break;

	case AS_Running:
		GCCocosHelpers::SetAnimationFPS( m_pCurrentAnimation, GetVelocity().x*1.75f );
		break;

	case AS_Jumping:
		break;

	case AS_Hacking:
		break;

	case AS_Falling:
		break;

	default:
		break;
	}

	// End of frame; next frame's previous animation state is this frame's current animation state
	m_ePreviousAnimState = m_eCurrentAnimState;

}

// Sets the player's animation to the given parameters
void CGCObjPlayer::Animate( const char* kcAnimation, bool bLooped, float fFPS /*= 32.0f */ )
{
	// Value map of animations
	ValueMap dicPList = GCCocosHelpers::CreateDictionaryFromPlist( GetFactoryCreationParams()->strPlistFile );

	// Stop current ACTION
	if( m_pCurrentAction )
		StopAction( m_pCurrentAction );

	// Create a new ANIMATION and store a pointer to it - we can use this to alter running framerate later

	m_pCurrentAnimation = GCCocosHelpers::CreateAnimation( dicPList, kcAnimation, fFPS );

	//  Create an ACTION out of the animation and store a pointer to it - we can use this for comparisons or stopping the "animation" later
	m_pCurrentAction = ReturnAction( bLooped ? GCCocosHelpers::CreateAnimationActionLoop( m_pCurrentAnimation ) : GCCocosHelpers::CreateAnimationActionOnce( m_pCurrentAnimation ) );
}


//////////////////////////////////////////////////////////////////////////
// this function exists purely to better illustrate the EXAMPLE collision 
// detection functionality in CGCGameLayerPlatformer
//////////////////////////////////////////////////////////////////////////
void CGCObjPlayer::NotifyOfCollisionWithInvader()
{
	CCLOG( "CGCObjPlayer::NotifyOfCollisionWithInvader - Aiee! I have collided with an invader!" );
}

void CGCObjPlayer::SetCanJump( bool bCanJump )
{
	m_bCanJump = bCanJump;
	m_bJumping = !bCanJump;

	m_iNumberOfJumps = bCanJump ? 1 : GameConfig::k_iMaxNumberOfJumps;
}

void CGCObjPlayer::HeadHit()
{
	m_bJumping = false;
}

bool CGCObjPlayer::GetIsJumping()
{
	return m_bJumping;
}

//////////////////////////////////////////////////////////////////////
// Point player to hacking interface - Will & Stanley
//////////////////////////////////////////////////////////////////////
void CGCObjPlayer::SetHackableInteract( CHackable* pCHackableObject )
{
	m_pCHackingObject = pCHackableObject;
	pCHackableObject->VEnter();
}

void CGCObjPlayer::ExitHackableInteract()
{
	//If we're looking at something then let it know we're leaving
	if( m_pCHackingObject )
	{
		m_pCHackingObject->VExit();
	}

	m_pCHackingObject = nullptr;
}

void CGCObjPlayer::ChangeWeaponType( GameConfig::EWeaponType eWeaponType, float fTime /*= 5.0f*/, float fFireRate /*= 0.0f*/ )
{
	//CCLOG("CHANGING WEAPON TYPE");

	m_eCurrentWeaponType = eWeaponType;

	m_fWeaponTimeLeft = fTime;
	m_fWeaponTimeMax = fTime;

	m_fTimeUntilCanFire = 0.0f;

	//If the fire rate argument isn't 0, then set our new one to it
	m_fFireRate = fFireRate > 0.0f ? fFireRate : m_fFireRate;

}

