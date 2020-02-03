#include "CLevelBase.h"
#include "GamerCamp/GameSpecific/Items/CWeaponPickup.h"
#include "MenuScene.h"
#include "Helper/HeaderLinker.h"
#include "AppDelegate.h"
#include "Menu/CPauseMenu.h"
#include "Menu/CMainMenu.h"
#include "CGameOver.h"

static GameConfig::ELevelInput	s_aePlayerActions[] = { GameConfig::ELevelInput::ELI_Option, GameConfig::ELevelInput::ELI_ShadowSkip };
static cocos2d::Controller::Key	s_aeKeys[] = { cocos2d::Controller::Key::BUTTON_OPTIONS, cocos2d::Controller::Key::BUTTON_A };

///////////////////////////////////////////////////////////////////////////////
// Constructors
///////////////////////////////////////////////////////////////////////////////
CLevelBase::CLevelBase( GCTypeID idIGCGameLayerDerived, std::string sOgmoLevel, char* psBackground, int iLevelGoals, int iLevel )
	: IGCGameLayer( idIGCGameLayerDerived )
	, m_sOgmoLevel( sOgmoLevel )
	, m_pcAudioManager( nullptr )
	, m_pcDataManager( nullptr )
	, m_pcGCGroupProjectile( nullptr )
	, m_pcSprBackground( nullptr )
	, m_pWeaponIcon( nullptr )
	, m_pLevelLabel( nullptr )
	, m_pTimerPercentage( nullptr )
	, m_pObjectiveLabel( nullptr )
	, m_iLevelGoals( iLevelGoals )
	, m_iGoalsAcheived( 0 )
	, m_bLevelIsTimed( false )
	, m_fLevelTime( INT_MAX )
	, m_fLevelTimeElapsed( 0.0f )
	, m_bCanGoToNextLevel( false )
	, m_bResetWasRequested( false )
	, m_bQuitWasRequested( false )
	, m_pcShadowTutorial( nullptr )
	, m_bTutorialIsSkipped( false )
	, m_bHasPlayerDied( false )
	, m_iLevel( iLevel )
{
	sprintf( m_psBackground, psBackground );

	// Get audio manager
	m_pcAudioManager = new CAudioManager;
}

CLevelBase::CLevelBase( GCTypeID idIGCGameLayerDerived, std::string sOgmoLevel, char* psBackground, int iLevelGoals, int iLevel, f32 fLevelTime )
	: IGCGameLayer( idIGCGameLayerDerived )
	, m_sOgmoLevel( sOgmoLevel )
	, m_pcAudioManager( nullptr )
	, m_pcDataManager( nullptr )
	, m_pcGCGroupProjectile( nullptr )
	, m_pcSprBackground( nullptr )
	, m_pTimerPercentage( nullptr )
	, m_pWeaponIcon( nullptr )
	, m_pLevelLabel( nullptr )
	, m_pObjectiveLabel( nullptr )
	, m_iLevelGoals( iLevelGoals )
	, m_iGoalsAcheived( 0 )
	, m_bLevelIsTimed( true )
	, m_fLevelTime( fLevelTime )
	, m_fLevelTimeElapsed( 0.0f )
	, m_bCanGoToNextLevel( false )
	, m_bResetWasRequested( false )
	, m_bQuitWasRequested( false )
	, m_bHasPlayerDied( false )
	, m_iLevel( iLevel )
{
	sprintf( m_psBackground, psBackground );

	// Get audio manager
	m_pcAudioManager = new CAudioManager;
}

//////////////////////////////////////////////////////////////////////////
// Destructor
//////////////////////////////////////////////////////////////////////////
CLevelBase::~CLevelBase()
{
}

void CLevelBase::onEnter()
{
	IGCGameLayer::onEnter();

	//// load the physics shapes from the plist created with PhysicsEditor
	//B2dLoadShapesFromPlist( "PhysicsEditor/GameShapes.plist" );

	//////////////////////////////////////////////////////////////////////////
	// init the actions
	// N.B. these 
	cocos2d::EventKeyboard::KeyCode aeKeyCodesForActions[] =
	{
		EventKeyboard::KeyCode::KEY_NONE,			// EPA_NULL,
		EventKeyboard::KeyCode::KEY_UP_ARROW,		// EPA_Up
		EventKeyboard::KeyCode::KEY_DOWN_ARROW,		// EPA_Down,
		EventKeyboard::KeyCode::KEY_LEFT_ARROW,		// EPA_Left,		
		EventKeyboard::KeyCode::KEY_RIGHT_ARROW,	// EPA_Right,	
		EventKeyboard::KeyCode::KEY_SPACE,			// EPA_Jump,
		EventKeyboard::KeyCode::KEY_SHIFT,			// EPA_Fire,	
		EventKeyboard::KeyCode::KEY_CTRL,			// EPA_Hack,
		EventKeyboard::KeyCode::KEY_ESCAPE,			// EPA_Options,
	};


	u32 uSizeOfActionArray = ( sizeof( aeKeyCodesForActions ) / sizeof( cocos2d::EventKeyboard::KeyCode ) );

	// call base class function	to init the keyboard manager
	AppDelegate::InitialiseKeyboardManager( uSizeOfActionArray, aeKeyCodesForActions );
}

void CLevelBase::VOnCreate()
{
	// Get active scene
	auto pActiveScene = IGCGameLayer::ActiveInstance();

	// Screen dimensional values
	Size visibleSize = Director::getInstance()->getVisibleSize();
	Point origin = Director::getInstance()->getVisibleOrigin();

	// Create the default object group
	IGCGameLayer::VOnCreate();

	// Create and register the object group for the player projectile objects
	m_pcGCGroupProjectile = new CGCObjGroupProjectile();
	CGCObjectManager::ObjectGroupRegister( m_pcGCGroupProjectile );

	// Create data manager
	m_pcDataManager = new CDataPersistence();

	// Populate the menu
	PopulateMenu();

	// Create and initialize timer label
	if( m_bLevelIsTimed )
	{
		char chTimeLeft[ 100 ];
		sprintf( chTimeLeft, " Time left: %d", static_cast< int >( m_fLevelTime - m_fLevelTimeElapsed ) );
		m_pTimerPercentage = Label::createWithTTF( chTimeLeft, "fonts/space_age.ttf", 32 );
		m_pTimerPercentage->setTextColor( cocos2d::Color4B( GLubyte( 0 ), GLubyte( 222 ), GLubyte( 255 ), GLubyte( 255 ) ) );
		m_pTimerPercentage->setPosition( Vec2( visibleSize.width / 2, visibleSize.height - 50 ) );

		this->addChild( m_pTimerPercentage, 1 );
	}

	// Create and initialize goals label
	m_pObjectiveLabel = Label::createWithTTF( "Test", "fonts/arial.ttf", 24 );
	m_pObjectiveLabel->setTextColor( cocos2d::Color4B( 0.0f, 255, 222, 255 ) );
	m_pObjectiveLabel->setPosition( Vec2( visibleSize.width / 2, visibleSize.height - 50 - ( m_bLevelIsTimed * 50 ) ) );
	m_pObjectiveLabel->setVisible( false );
	this->addChild( m_pObjectiveLabel, 1 );

	m_pLevelLabel = Label::createWithTTF( "Level a-b", "fonts/space_age.ttf", 32 );
	m_pLevelLabel->setTextColor( cocos2d::Color4B( GLubyte( 0 ), GLubyte( 222 ), GLubyte( 255 ), GLubyte( 255 ) ) );
	m_pLevelLabel->setVisible( true );
	m_pLevelLabel->setPosition( Vec2( visibleSize.width * 0.915f, visibleSize.height - 50 ) );
	this->addChild( m_pLevelLabel, 1 );

	// Background
	m_pcSprBackground = Sprite::create( m_psBackground );
	m_pcSprBackground->setPosition( Vec2( visibleSize.width / 2, visibleSize.height / 2 ) );
	addChild( m_pcSprBackground, -99 );

	// Physics
	B2dLoadShapesFromPlist( "PhysicsEditor/GameShapes.plist" );
	B2dGetWorld()->SetContactListener( this );

	// Populate the ground
	PopulateGround();

	// Load the level
	m_cLevelLoader.LoadLevelFile( FileUtils::getInstance()->fullPathForFilename( m_sOgmoLevel ).c_str() );
	m_cLevelLoader.CreateObjects( CGCFactory_ObjSpritePhysics::GetFactory() );

	m_pcControllerActionToKeyMap = TCreateActionToKeyMap( s_aePlayerActions, s_aeKeys );

	m_pcAudioManager->StopBackgroundMusic();
	//PlayBackgroundMusic();

	if( m_iLevel )
	{
		m_pcShadowTutorial = new CShadowTutorial();
		m_pcShadowTutorial->Create( m_iLevel );
	}

	// Initialise taser icon
	m_pWeaponIcon = Sprite::create( "Loose/UI/HUD/Taser.png" );
	m_pWeaponIcon->setPosition( cocos2d::Vec2( 60.f, 960.f ) );
	m_pWeaponIcon->setVisible( true );
	pActiveScene->addChild( m_pWeaponIcon, int( GameConfig::EZIndexLayer::EZIL_HUD_Top ) );


	// Initialise corners
	m_pTopLeftCorner = Sprite::create( "Loose/UI/HUD/Border.png" );
	m_pTopLeftCorner->setPosition( Vec2( 15.f + m_pTopLeftCorner->getContentSize().width / 2, 1065.f - m_pTopLeftCorner->getContentSize().height / 2 ) );
	m_pTopLeftCorner->setVisible( true );
	pActiveScene->addChild( m_pTopLeftCorner, int( GameConfig::EZIndexLayer::EZIL_HUD_Top ) );
	m_pTopRightCorner = Sprite::create( "Loose/UI/HUD/Border.png" );
	m_pTopRightCorner->setRotation( 90.f );
	m_pTopRightCorner->setPosition( cocos2d::Vec2( 1905.f - m_pTopRightCorner->getContentSize().width / 2, 1065.f - m_pTopRightCorner->getContentSize().height / 2 ) );
	m_pTopRightCorner->setVisible( true );
	pActiveScene->addChild( m_pTopRightCorner, int( GameConfig::EZIndexLayer::EZIL_HUD_Top ) );
	m_pBottomLeftCorner = Sprite::create( "Loose/UI/HUD/Border.png" );
	m_pBottomLeftCorner->setRotation( 270.f );
	m_pBottomLeftCorner->setPosition( Vec2( 15.f + m_pBottomLeftCorner->getContentSize().width / 2, 15.f + m_pBottomLeftCorner->getContentSize().height / 2 ) );
	m_pBottomLeftCorner->setVisible( true );
	pActiveScene->addChild( m_pBottomLeftCorner, int( GameConfig::EZIndexLayer::EZIL_HUD_Top ) );
	m_pBottomRightCorner = Sprite::create( "Loose/UI/HUD/Border.png" );
	m_pBottomRightCorner->setPosition( Vec2( 1920.f - 15.f - m_pBottomRightCorner->getContentSize().width / 2, 15.f + m_pBottomRightCorner->getContentSize().height / 2 ) );
	m_pBottomRightCorner->setRotation( 180.f );
	m_pBottomRightCorner->setVisible( true );
	pActiveScene->addChild( m_pBottomRightCorner, int( GameConfig::EZIndexLayer::EZIL_HUD_Top ) );

	// Initialise level title

}

void CLevelBase::PopulateMenu()
{
	Size visibleSize = Director::getInstance()->getVisibleSize();
	Point origin = Director::getInstance()->getVisibleOrigin();

	///////////////////////////////////////////////////////////////////////////
	// add menu
	///////////////////////////////////////////////////////////////////////////

	// add a "close" icon to exit the progress. it's an autorelease object
	MenuItemImage* pResetItem
		= MenuItemImage::create( "Loose/CloseNormal.png",
			"Loose/CloseSelected.png",
			CC_CALLBACK_1( CLevelBase::Callback_OnResetButton, this ) );

	pResetItem->setPosition( Vec2( ( ( visibleSize.width - ( pResetItem->getContentSize().width * 0.5f ) ) + origin.x ),
		( ( ( pResetItem->getContentSize().height * 0.5f ) + origin.y ) ) ) );

	MenuItemImage* pQuitItem
		= MenuItemImage::create( "Loose/CloseNormal.png",
			"Loose/CloseSelected.png",
			CC_CALLBACK_1( CLevelBase::Callback_OnQuitButton, this ) );

	pQuitItem->setPosition( Vec2( ( ( visibleSize.width - ( pQuitItem->getContentSize().width * 0.5f ) ) + origin.x ),
		( ( visibleSize.height - ( pQuitItem->getContentSize().height * 0.5f ) ) + origin.y ) ) );
}

void CLevelBase::PopulateGround()
{
	Size visibleSize = Director::getInstance()->getVisibleSize();
	Point origin = Director::getInstance()->getVisibleOrigin();

	// PTM_RATIO
	f32 PTM_RATIO = IGCGAMELAYER_B2D_PIXELS_PER_METER;

	b2Vec2	b2v2ScreenCentre_Pixels( ( origin.x + ( visibleSize.width * 0.5f ) ), ( origin.y + ( visibleSize.height * 0.5f ) ) );
	Vec2	v2ScreenCentre_Pixels( ( origin.x + ( visibleSize.width * 0.5f ) ), ( origin.y + ( visibleSize.height * 0.5f ) ) );



	// define the ground body
	b2BodyDef groundBodyDef;
	groundBodyDef.position = IGCGameLayer::B2dPixelsToWorld( b2v2ScreenCentre_Pixels );
	groundBodyDef.type = b2_kinematicBody;

	// Call the body factory which allocates memory for the ground body
	// from a pool and creates the ground box shape (also from a pool).
	// The body is also added to the world.
	b2Body* groundBody = B2dGetWorld()->CreateBody( &groundBodyDef );

	// Define the ground box shape.
	b2PolygonShape groundBox;

	// bottom
	groundBox.SetAsBox( ( ( visibleSize.width * 0.5f ) / PTM_RATIO ), 0.5f, b2Vec2( 0.0f, -( ( visibleSize.height * 0.5f ) / PTM_RATIO ) ), 0.0f );
	groundBody->CreateFixture( &groundBox, 0 );

	// top
	//groundBox.SetAsBox( ( ( visibleSize.width * 0.5f ) / PTM_RATIO ), 0.5f, b2Vec2( 0.0f, ( ( visibleSize.height * 0.5f ) / PTM_RATIO ) ), 0.0f );
	//groundBody->CreateFixture( &groundBox, 0 );

	// left
	groundBox.SetAsBox( 0.5f, ( ( visibleSize.height * 2.0f * 0.5f ) / PTM_RATIO ), b2Vec2( -( ( visibleSize.width * 0.5f ) / PTM_RATIO ), 0.0f ), 0.0f );
	groundBody->CreateFixture( &groundBox, 0 );

	// right
	groundBox.SetAsBox( 0.5f, ( ( visibleSize.height * 2.0f * 0.5f ) / PTM_RATIO ), b2Vec2( ( ( visibleSize.width * 0.5f ) / PTM_RATIO ), 0.0f ), 0.0f );
	groundBody->CreateFixture( &groundBox, 0 );

}

void CLevelBase::VOnUpdate( f32 fTimeStep )
{
	IGCGameLayer::VOnUpdate( fTimeStep );

	if( m_bHasPlayerDied )
	{
		m_pcDataManager->SaveLevel( 1 );
		ReplaceScene( TGCGameLayerSceneCreator< GCGameOver >::CreateScene() );
	}

	TGCController< GameConfig::ELevelInput > cController = TGetActionMappedController( CGCControllerManager::eControllerOne, ( *m_pcControllerActionToKeyMap ) );
	if( cController.IsActive() )
	{
		ControllerControls( cController );
	}
	else
	{
		KeyboardControls();
	}
	// Advances to next level if the flag is true
	if( m_bCanGoToNextLevel )
	{
		NextLevel();
	}

	// Updates the timed level functionality, if any
	if( m_bLevelIsTimed && m_bTutorialIsSkipped )
	{
		UpdateTimer( fTimeStep );
	}

	// Updates the goals counter UI
	char acGoalsText[ 100 ];
	sprintf( acGoalsText, "Goals Complete: %d/%d", m_iGoalsAcheived, m_iLevelGoals );
	m_pObjectiveLabel->setString( acGoalsText );

	if( ResetWasRequested() )
	{
		m_pcAudioManager->StopBackgroundMusic();

		VOnReset();

		ResetRequestWasHandled();
	}

	if( QuitWasRequested() )
	{
		QuitRequestWasHandled();
		ReplaceScene( TGCGameLayerSceneCreator< CMainMenu >::CreateScene() );
	}
}

void CLevelBase::VOnDestroy()
{
	///////////////////////////////////////////////////////////////////////////
	// clean up anything we allocated in opposite order to creation
	///////////////////////////////////////////////////////////////////////////	

	m_pcAudioManager->StopBackgroundMusic();

	// clean all event listeners
	m_cEventManager.RemoveAllListeners();

	IGCGameLayer::VOnDestroy();
}

void CLevelBase::VOnReset()
{
	// Add the Listener for Gained objective event
	CEventManager::Manager().AddListener( GameConfig::EEventType::EET_GainedObjective, [ & ] ( void* )
	{
		IncrementLevelGoals();
	} );

	// Add the Listener for lives over event
	CEventManager::Manager().AddListener( GameConfig::EEventType::EET_LivesOver, [ & ] ( void* )
	{
		m_bHasPlayerDied = true;
	} );

	// Add the Listener for return to menu event
	CEventManager::Manager().AddListener( GameConfig::EEventType::EET_ReturnToMenu, [ & ] ( void* )
	{
		RequestQuit();
	} );

	//Trigger the disable controls event 
	CEventManager::Manager().Trigger( GameConfig::EEventType::EET_ControlDisable, nullptr );
	// Reset level goals
	ResetLevelGoals();

	m_bTutorialIsSkipped = false;
	// Reset time passed
	m_fLevelTimeElapsed = 0.0f;

	IGCGameLayer::VOnReset();
}

void CLevelBase::UpdateTimer( f32 fTimeStep )
{

	if( m_fLevelTimeElapsed <= m_fLevelTime )
	{
		m_fLevelTimeElapsed += fTimeStep;
		//timer percentage.
		char acTimerPercentage[ 100 ];
		sprintf( acTimerPercentage, " Time left: %d", static_cast< int >( m_fLevelTime - m_fLevelTimeElapsed + 1 ) );
		m_pTimerPercentage->setString( acTimerPercentage );
	}

	else
	{
		m_fLevelTimeElapsed += fTimeStep;
		if( ( int )m_fLevelTimeElapsed > ( int )m_fLevelTime + 1 )
		{
			CEventManager::Manager().Trigger( GameConfig::EEventType::EET_TimerFinished, nullptr );
			RequestReset();
		}

	}
}

///////////////////////////////////////////////////////////////////////////////
// on quit button
///////////////////////////////////////////////////////////////////////////////
void CLevelBase::Callback_OnQuitButton( Ref* pSender )
{
	RequestQuit();
}

///////////////////////////////////////////////////////////////////////////////
// on reset button
///////////////////////////////////////////////////////////////////////////////
void CLevelBase::Callback_OnResetButton( Ref* pSender )
{
	RequestReset();
}

void CLevelBase::AdvanceToNextLevel()
{
	m_bCanGoToNextLevel = true;
}

///////////////////////////////////////////////////////////////////////////////
// begin contact
// insert any logic that relies on detecting the first frame where a 
// contact exists
///////////////////////////////////////////////////////////////////////////////
//virtual 
void CLevelBase::BeginContact( b2Contact* pB2Contact )
{
	const b2Fixture* pFixtureA = pB2Contact->GetFixtureA();
	const b2Fixture* pFixtureB = pB2Contact->GetFixtureB();

	const b2Body* pBodyA = pFixtureA->GetBody();
	const b2Body* pBodyB = pFixtureB->GetBody();

	CGCObjSpritePhysics * pGcSprPhysA = ( CGCObjSpritePhysics* )pBodyA->GetUserData();
	// if( this is not a GC object )
	if( pGcSprPhysA == NULL )
	{
		return;
	}

	CGCObjSpritePhysics * pGcSprPhysB = ( CGCObjSpritePhysics* )pBodyB->GetUserData();
	// if( this is not a GC object )
	if( pGcSprPhysB == NULL )
	{
		return;
	}

	const std::string*	psFixtureIdA = cocos2d::GB2ShapeCache::getFixtureIdText( pFixtureA );
	const std::string*	psFixtureIdB = cocos2d::GB2ShapeCache::getFixtureIdText( pFixtureB );


	if( pB2Contact->IsEnabled() )
	{
		HandlePlayerAndCoinCollision( pGcSprPhysA, pGcSprPhysB );
		HandleTurnNodeAndEnemyR1Collision( pGcSprPhysA, pGcSprPhysB );
		HandleTurnNodeAndEnemyR3Collision( pGcSprPhysA, pGcSprPhysB );
		HandlePlayerAndExitNode( pGcSprPhysA, pGcSprPhysB );
		HandlePlayerAndTerminalCollision( pGcSprPhysA, pGcSprPhysB );
	}


	///////////////////////////
	//	PLAYER COLLISIONS
	///////////////////////////

	CGCObjPlayer* pPlayerA = CGCObject::SafeCastToDerived< CGCObjPlayer* >( pGcSprPhysA );
	CGCObjPlayer* pPlayerB = CGCObject::SafeCastToDerived< CGCObjPlayer* >( pGcSprPhysB );

	// at least one of them is a player
	if( pPlayerA || pPlayerB )
	{
		CGCObjPlayer*	pPlayer = ( pPlayerA ? pPlayerA : pPlayerB );
		GCTypeID		tidNotPlayer = ( pPlayerA ? pGcSprPhysB->GetGCTypeID() : pGcSprPhysA->GetGCTypeID() );

		CObjCrumblingPlatform* pCrumblingPlatform = CGCObject::SafeCastToDerived< CObjCrumblingPlatform* >( pPlayerA ? pGcSprPhysB : pGcSprPhysA );
		CObjMovingPlatform* pMovingPlatform = CGCObject::SafeCastToDerived< CObjMovingPlatform* >( pPlayerA ? pGcSprPhysB : pGcSprPhysA );

		// Set player to be able to jump again
		if( psFixtureIdB
			&& ( 0 == psFixtureIdB->compare( "bottom" ) )
			&& pFixtureB->IsSensor()
			&& ( !pCrumblingPlatform || !pCrumblingPlatform->IsDead() ) )
		{
			if( !pPlayer->GetIsJumping() )
			{
				pPlayer->SetCanJump( true );
			}
		}

		// Tell the player if their head clipped something
		if( psFixtureIdB
			&& ( 0 == psFixtureIdB->compare( "top" ) )
			&& pFixtureB->IsSensor()
			&& !pMovingPlatform
			&& ( !pCrumblingPlatform || !pCrumblingPlatform->IsDead() ) )
		{
			pPlayer->HeadHit();
		}

		// Weapon Pickup
		if( pB2Contact->IsEnabled()
			&& ( pGcSprPhysA->GetGCTypeID() != pGcSprPhysB->GetGCTypeID() ) )
		{
			if( ( ( pGcSprPhysA->GetGCTypeID() == GetGCTypeIDOf( CGCObjPlayer ) )
				&& ( pGcSprPhysB->GetGCTypeID() == GetGCTypeIDOf( CObjWeaponPickup ) ) )
				|| ( ( pGcSprPhysA->GetGCTypeID() == GetGCTypeIDOf( CObjWeaponPickup ) )
					&& ( pGcSprPhysB->GetGCTypeID() == GetGCTypeIDOf( CGCObjPlayer ) ) ) )
			{
				CObjWeaponPickup* pPickupA = CGCObject::SafeCastToDerived< CObjWeaponPickup* >( pGcSprPhysA );
				CObjWeaponPickup* pPickupB = CGCObject::SafeCastToDerived< CObjWeaponPickup* >( pGcSprPhysB );

				CObjWeaponPickup* pPickup = pPickupA ? pPickupA : pPickupB;

				// Change weapon type
				pPlayer->ChangeWeaponType( pPickup->GetWeaponType(), pPickup->GetTime(), pPickup->GetFireRate() );

				// Get rid of the pickup
				CGCObjectManager::ObjectKill( pPickup );

				// Ensure the player doesn't receive any counter force from the collision
				pB2Contact->SetEnabled( false );

			}
		}

	}



	//Crumble Platforms
	if( pB2Contact->IsEnabled()
		&& ( pGcSprPhysA->GetGCTypeID() != pGcSprPhysB->GetGCTypeID() ) )
	{
		if( ( ( pGcSprPhysA->GetGCTypeID() == GetGCTypeIDOf( CGCObjPlayer ) )
			&& ( pGcSprPhysB->GetGCTypeID() == GetGCTypeIDOf( CObjCrumblingPlatform ) ) )
			|| ( ( pGcSprPhysA->GetGCTypeID() == GetGCTypeIDOf( CObjCrumblingPlatform ) )
				&& ( pGcSprPhysB->GetGCTypeID() == GetGCTypeIDOf( CGCObjPlayer ) ) ) )
		{

			const std::string* pstrFixtureASensorId = cocos2d::GB2ShapeCache::getFixtureIdText( pFixtureA );
			const std::string* pstrFixtureBSensorId = cocos2d::GB2ShapeCache::getFixtureIdText( pFixtureB );

			if( ( pFixtureA->IsSensor() && pstrFixtureASensorId->compare( "crumble_area" ) == 0 ) || ( pFixtureB->IsSensor() && pstrFixtureBSensorId->compare( "crumble_area" ) == 0 ) )
			{
				CObjCrumblingPlatform* pPlatformA = CGCObject::SafeCastToDerived< CObjCrumblingPlatform* >( pGcSprPhysA );
				CObjCrumblingPlatform* pPlatformB = CGCObject::SafeCastToDerived< CObjCrumblingPlatform* >( pGcSprPhysB );

				CObjCrumblingPlatform* pPlatform = pPlatformA ? pPlatformA : pPlatformB;

				pPlatform->PlayerEnter();
			}

		}
	}
}

///////////////////////////////////////////////////////////////////////////////
// end contact
// insert any logic that relies on detecting the last frame where a 
// contact exists
///////////////////////////////////////////////////////////////////////////////
void CLevelBase::EndContact( b2Contact* pB2Contact )
{
	const b2Fixture* pFixtureA = pB2Contact->GetFixtureA();
	const b2Fixture* pFixtureB = pB2Contact->GetFixtureB();

	const b2Body* pBodyA = pFixtureA->GetBody();
	const b2Body* pBodyB = pFixtureB->GetBody();

	CGCObjSpritePhysics * pGcSprPhysA = ( CGCObjSpritePhysics* )pBodyA->GetUserData();
	// if( this is not a GC object )
	if( pGcSprPhysA == NULL )
	{
		return;
	}

	CGCObjSpritePhysics * pGcSprPhysB = ( CGCObjSpritePhysics* )pBodyB->GetUserData();
	// if( this is not a GC object )
	if( pGcSprPhysB == NULL )
	{
		return;
	}

	const std::string*	psFixtureIdA = cocos2d::GB2ShapeCache::getFixtureIdText( pFixtureA );
	const std::string*	psFixtureIdB = cocos2d::GB2ShapeCache::getFixtureIdText( pFixtureB );

	if( pB2Contact->IsEnabled() )
	{
		HandlePlayerLeaveTerminalCollision( pGcSprPhysA, pGcSprPhysB );
	}

	CGCObjPlayer* pPlayerA = CGCObject::SafeCastToDerived< CGCObjPlayer* >( pGcSprPhysA );
	CGCObjPlayer* pPlayerB = CGCObject::SafeCastToDerived< CGCObjPlayer* >( pGcSprPhysB );

	// at least one of them is a player
	if( pPlayerA || pPlayerB )
	{
		CGCObjPlayer*	pPlayer = ( pPlayerA ? pPlayerA : pPlayerB );
		GCTypeID		tidNotPlayer = ( pPlayerA ? pGcSprPhysB->GetGCTypeID() : pGcSprPhysA->GetGCTypeID() );

		// Set player to be able to jump again
		if( psFixtureIdB
			&& ( 0 == psFixtureIdB->compare( "bottom" ) )
			&& pFixtureB->IsSensor() )
		{
			if( !pPlayer->GetIsJumping() )
			{
				pPlayer->SetIsOnGround( false );
			}
		}
	}

	// Crumble Platforms
	if( pB2Contact->IsEnabled()
		&& ( pGcSprPhysA->GetGCTypeID() != pGcSprPhysB->GetGCTypeID() ) )
	{
		if( ( ( pGcSprPhysA->GetGCTypeID() == GetGCTypeIDOf( CGCObjPlayer ) )
			&& ( pGcSprPhysB->GetGCTypeID() == GetGCTypeIDOf( CObjCrumblingPlatform ) ) )
			|| ( ( pGcSprPhysA->GetGCTypeID() == GetGCTypeIDOf( CObjCrumblingPlatform ) )
				&& ( pGcSprPhysB->GetGCTypeID() == GetGCTypeIDOf( CGCObjPlayer ) ) ) )
		{
			CObjCrumblingPlatform* pPlatformA = CGCObject::SafeCastToDerived< CObjCrumblingPlatform* >( pGcSprPhysA );
			CObjCrumblingPlatform* pPlatformB = CGCObject::SafeCastToDerived< CObjCrumblingPlatform* >( pGcSprPhysB );

			CObjCrumblingPlatform* pPlatform = pPlatformA ? pPlatformA : pPlatformB;

			pPlatform->PlayerLeave();

		}
	}
}

///////////////////////////////////////////////////////////////////////////////
// pre solve
// insert any logic that needs to be done before a contact is resolved
///////////////////////////////////////////////////////////////////////////////
void CLevelBase::PreSolve( b2Contact* pB2Contact, const b2Manifold* pOldManifold )
{
	const b2Fixture* pFixtureA = pB2Contact->GetFixtureA();
	const b2Fixture* pFixtureB = pB2Contact->GetFixtureB();

	const b2Body* pBodyA = pFixtureA->GetBody();
	const b2Body* pBodyB = pFixtureB->GetBody();

	CGCObjSpritePhysics* pGcSprPhysA = ( CGCObjSpritePhysics* )pBodyA->GetUserData();
	// if( this is not a GC object )
	if( pGcSprPhysA == NULL )
	{
		return;
	}

	CGCObjSpritePhysics* pGcSprPhysB = ( CGCObjSpritePhysics* )pBodyB->GetUserData();
	// if( this is not a GC object )
	if( pGcSprPhysB == NULL )
	{
		return;
	}

	const std::string*	psFixtureIdA = cocos2d::GB2ShapeCache::getFixtureIdText( pFixtureA );
	const std::string*	psFixtureIdB = cocos2d::GB2ShapeCache::getFixtureIdText( pFixtureB );


	//////////////////////////
	//	PLAYER
	//////////////////////////

	CGCObjPlayer* pPlayerA = CGCObject::SafeCastToDerived< CGCObjPlayer* >( pGcSprPhysA );
	CGCObjPlayer* pPlayerB = CGCObject::SafeCastToDerived< CGCObjPlayer* >( pGcSprPhysB );

	CGCObjPlayer* pPlayer = pPlayerA ? pPlayerA : pPlayerB;

	// Enemy knock-back
	if( pB2Contact->IsEnabled()
		&& ( pGcSprPhysA->GetGCTypeID() != pGcSprPhysB->GetGCTypeID() ) )
	{
		if( pGcSprPhysA->GetGCTypeID() == GetGCTypeIDOf( CGCObjPlayer ) || pGcSprPhysB->GetGCTypeID() == GetGCTypeIDOf( CGCObjPlayer ) )
		{
			CGCObjSpritePhysics* pOther = pPlayerA ? pGcSprPhysB : pGcSprPhysA;

			// Detect if an enemy was hit
			if( 0 == ( pPlayerA ? psFixtureIdB->compare( "enemy" ) : psFixtureIdA->compare( "enemy" ) ) )
			{
				float fDamage = 0.0f;

				if( CGCObject::SafeCastToDerived< CEnemyR1* >( pOther ) ) fDamage = GameConfig::k_iR1ContactDamage;
				if( CGCObject::SafeCastToDerived< CEnemyR2* >( pOther ) ) fDamage = GameConfig::k_iR2ContactDamage;
				if( CGCObject::SafeCastToDerived< CEnemyR3* >( pOther ) ) fDamage = GameConfig::k_iR3ContactDamage;

				// Damage the player
				if( pPlayer->AddDamage( fDamage ) <= 0.0f )
				{
					if( pPlayer->GetLives() > 0 )
					{
						// Reset the level if they run out of health
						RequestReset();
					}
					else
					{
						// Game over if they run out of lives
						m_bHasPlayerDied = true;
					}
				}

				// Get direction to knock back
				Vec2 v2Direction = Vec2( pPlayer->GetSpritePosition().x - pOther->GetSpritePosition().x < 0.0f ? -1.f : 1.f, 0.75f );

				// Knock back the player
				pPlayer->HeadHit();
				pPlayer->SetCanJump( false );
				pPlayer->SetVelocity( Vec2::ZERO );
				pPlayer->SetVelocity( v2Direction*GameConfig::k_fEnemyKnockbackForce );

				// Ensure the enemy doesn't get nudged
				pB2Contact->SetEnabled( false );
			}
		}
	}


	// Crumble Platforms
	if( pB2Contact->IsEnabled()
		&& ( pGcSprPhysA->GetGCTypeID() != pGcSprPhysB->GetGCTypeID() ) )
	{
		if( ( ( pGcSprPhysA->GetGCTypeID() == GetGCTypeIDOf( CGCObjPlayer ) )
			&& ( pGcSprPhysB->GetGCTypeID() == GetGCTypeIDOf( CObjCrumblingPlatform ) ) )
			|| ( ( pGcSprPhysA->GetGCTypeID() == GetGCTypeIDOf( CObjCrumblingPlatform ) )
				&& ( pGcSprPhysB->GetGCTypeID() == GetGCTypeIDOf( CGCObjPlayer ) ) ) )
		{
			CObjCrumblingPlatform* pPlatformA = CGCObject::SafeCastToDerived< CObjCrumblingPlatform* >( pGcSprPhysA );
			CObjCrumblingPlatform* pPlatformB = CGCObject::SafeCastToDerived< CObjCrumblingPlatform* >( pGcSprPhysB );

			CObjCrumblingPlatform* pPlatform = pPlatformA ? pPlatformA : pPlatformB;

			if( !pPlatform->IsDead() )
			{
#if _DEBUG
				CCLOG( "Crumbling platform contact" );
#endif
				pB2Contact->SetEnabled( true );
			}
			else
			{
#if _DEBUG
				CCLOG( "Crumbling platform no contact" );
#endif
				pB2Contact->SetEnabled( false );
			}

		}
	}

	// Moving Platforms
	if( pB2Contact->IsEnabled()
		&& ( pGcSprPhysA->GetGCTypeID() != pGcSprPhysB->GetGCTypeID() ) )
	{
		if( ( ( pGcSprPhysA->GetGCTypeID() == GetGCTypeIDOf( CGCObjPlayer ) )
			&& ( pGcSprPhysB->GetGCTypeID() == GetGCTypeIDOf( CObjMovingPlatform ) ) )
			|| ( ( pGcSprPhysA->GetGCTypeID() == GetGCTypeIDOf( CObjMovingPlatform ) )
				&& ( pGcSprPhysB->GetGCTypeID() == GetGCTypeIDOf( CGCObjPlayer ) ) ) )
		{
			CObjMovingPlatform* pPlatformA = CGCObject::SafeCastToDerived< CObjMovingPlatform* >( pGcSprPhysA );
			CObjMovingPlatform* pPlatformB = CGCObject::SafeCastToDerived< CObjMovingPlatform* >( pGcSprPhysB );

			CObjMovingPlatform* pPlatform = pPlatformA ? pPlatformA : pPlatformB;
			CGCObjPlayer* pPlayer = pPlatformA ? CGCObject::SafeCastToDerived< CGCObjPlayer* >( pGcSprPhysB ) : CGCObject::SafeCastToDerived< CGCObjPlayer* >( pGcSprPhysA );

			pPlayer->ApplyForceToCenter( pPlatform->GetVelocity()*51.f );

			// Let the player through if they're going upwards
			if( 0 != ( pPlayerA ? psFixtureIdA->compare( "foot" ) : psFixtureIdB->compare( "foot" ) ) )
			{
				pB2Contact->SetEnabled( false );
			}
			else
			{
				if( pB2Contact->GetManifold()->localPoint.y < 0.0f )
				{
					pB2Contact->SetEnabled( false );
				}
			}
		}
	}


	//////////////////////////
	//	PROJECTILES
	//////////////////////////

	// returns a valid ptr if instance's EXACT type matches or nullptr if not
	CGCObjProjectile* pProjectileA = CGCObject::SafeCastToDerived< CGCObjProjectile* >( pGcSprPhysA );
	CGCObjProjectile* pProjectileB = CGCObject::SafeCastToDerived< CGCObjProjectile* >( pGcSprPhysB );

	// at least one of them is an projectile:
	if( pProjectileA || pProjectileB )
	{
		CGCObjProjectile*	pProjectile = ( pProjectileA ? pProjectileA : pProjectileB );
		GCTypeID		 tidNotProjectile = ( pProjectileA ? pGcSprPhysB->GetGCTypeID() : pGcSprPhysA->GetGCTypeID() );

		CGCObjSpritePhysics* pOther = static_cast< CGCObjSpritePhysics* >( pProjectileA ? pGcSprPhysB : pGcSprPhysA );

		// Disable collision if this is the player's projectile hitting a player
		if( tidNotProjectile == GetGCTypeIDOf( CGCObjPlayer ) )
		{
			// Causes a return if the bullet shouldn't hurt the player. If that doesn't happen, knock them back
			switch( pProjectile->GetWeaponType() )
			{
				// All fall through to the same thing
			case GameConfig::EWeaponType::EWT_Stun:

				//case GameConfig::EWeaponType::EWT_Normal:

			case GameConfig::EWeaponType::EWT_Overcharge:

				pB2Contact->SetEnabled( false );
				return;
				break;
			}

			// Damage the player
			if( pPlayer->AddDamage( pProjectile->GetDamage() ) <= 0.0f )
			{
				if( pPlayer->GetLives() > 0 )
				{
					// Reset the level if they run out of health
					RequestReset();
				}
				else
				{
					// Game over if they run out of lives
					m_bHasPlayerDied = true;
				}
			}

			// Get direction to knock back
			Vec2 v2Direction =
				(
					pPlayer->GetSpritePosition()
					-
					pProjectile->GetSpritePosition()
					)
				+
				Vec2( 0.0f, 25.0f );

			v2Direction.normalize();

			// Knock back the player
			pPlayer->ApplyForceToCenter( v2Direction * GameConfig::k_fEnemyKnockbackForce );

			//Kill projectile
			pProjectile->SetLifeTime( 0.0f );
		}
		// Disable collision if this is an enemy projectile hitting an enemy
		else if( pOther != nullptr && ( pProjectile->GetWeaponType() == GameConfig::EWeaponType::EWT_R2 || pProjectile->GetWeaponType() == GameConfig::EWeaponType::EWT_R3 ) )
		{
			pB2Contact->SetEnabled( false );
		}

		// Hurt enemy if they're hit by the projectile
		else if( pOther != nullptr && ( psFixtureIdB->compare( "enemy" ) || psFixtureIdA->compare( "enemy" ) ) )
		{
			CEnemyR1* r1 = CGCObject::SafeCastToDerived< CEnemyR1* >( pOther );
			CEnemyR2* r2 = CGCObject::SafeCastToDerived< CEnemyR2* >( pOther );
			CEnemyR3* r3 = CGCObject::SafeCastToDerived< CEnemyR3* >( pOther );

			// Immobilise the enemy temporarily if it's a taser
			if( pProjectile->GetWeaponType() == GameConfig::EWeaponType::EWT_Stun )
			{
				if( r1 )
				{
					r1->Immobilise( GameConfig::k_fR1TaseImmobiliseTime );
				}
				if( r2 )
				{
					r2->Immobilise( GameConfig::k_fR2TaseImmobiliseTime );
				}
				if( r3 )
				{
					r3->Immobilise( GameConfig::k_fR3TaseImmobiliseTime );
				}	
			}

			// Otherwise it must be an "overcharge", so damage
			else
			{
				if( r1 && 0 >= r1->Damage( pProjectile->GetDamage() ) )
				{
					CGCObjectManager::ObjectKill( r1 );
				}

				if( r2 && 0 >= r2->Damage( pProjectile->GetDamage() ) )
				{
					CGCObjectManager::ObjectKill( r2 );
				}

				if( r3 && 0 >= r3->Damage( pProjectile->GetDamage() ) )
				{
					CGCObjectManager::ObjectKill( r3 );
				}
			}


			// ignore the collision!
			pB2Contact->SetEnabled( false );

			//Kill projectile
			pProjectile->SetLifeTime( 0.0f );
		}
	}


	// ignore contact between player projectile and item for collision resolution purposes
	if( pGcSprPhysA->GetGCTypeID() != pGcSprPhysB->GetGCTypeID() )
	{
		if( ( ( pGcSprPhysA->GetGCTypeID() == GetGCTypeIDOf( CGCObjProjectile ) )
			&& ( pGcSprPhysB->GetGCTypeID() == GetGCTypeIDOf( CGCObjItem ) ) )
			|| ( ( pGcSprPhysA->GetGCTypeID() == GetGCTypeIDOf( CGCObjItem ) )
				&& ( pGcSprPhysB->GetGCTypeID() == GetGCTypeIDOf( CGCObjProjectile ) ) ) )
		{
			// ignore the collision!
			pB2Contact->SetEnabled( false );
		}
	}
}

///////////////////////////////////////////////////////////////////////////////
// post solve
// insert any logic that needs to be done after a contact is resolved
// e.g. check the types and double the impulse
///////////////////////////////////////////////////////////////////////////////
void CLevelBase::PostSolve( b2Contact* pB2Contact, const b2ContactImpulse* pImpulse )
{
}

//////////////////////////////////////////////////////////////////////////
// Collision Handling Methods
//////////////////////////////////////////////////////////////////////////
void CLevelBase::HandleTurnNodeAndEnemyR1Collision( CGCObjSpritePhysics* pGcSprPhysA, CGCObjSpritePhysics* pGcSprPhysB )
{
	if( pGcSprPhysA->GetGCTypeID() != pGcSprPhysB->GetGCTypeID() )
	{
		// returns a valid ptr if instance's EXACT type matches or nullptr if not
		CEnemyR1* pEnemyR1A = CGCObject::SafeCastToDerived< CEnemyR1* >( pGcSprPhysA );
		CEnemyR1* pEnemyR1B = CGCObject::SafeCastToDerived< CEnemyR1* >( pGcSprPhysB );

		// at least one of them is an invader?
		if( pEnemyR1A || pEnemyR1B )
		{
			CEnemyR1*	pEnemyR1 = ( pEnemyR1A ? pEnemyR1A : pEnemyR1B );
			GCTypeID		tidNotEnemyR1 = ( pEnemyR1A ? pGcSprPhysB->GetGCTypeID() : pGcSprPhysA->GetGCTypeID() );

			if( GetGCTypeIDOf( CTurnNode ) == tidNotEnemyR1 )
			{
				pEnemyR1->SwitchDirection();
			}
		}
	}
}

void CLevelBase::HandleTurnNodeAndEnemyR3Collision( CGCObjSpritePhysics* pGcSprPhysA, CGCObjSpritePhysics* pGcSprPhysB )
{
	if( pGcSprPhysA->GetGCTypeID() != pGcSprPhysB->GetGCTypeID() )
	{
		// returns a valid ptr if instance's EXACT type matches or nullptr if not
		CEnemyR3* pEnemyR3A = CGCObject::SafeCastToDerived< CEnemyR3* >( pGcSprPhysA );
		CEnemyR3* pEnemyR3B = CGCObject::SafeCastToDerived< CEnemyR3* >( pGcSprPhysB );

		// at least one of them is an invader?
		if( pEnemyR3A || pEnemyR3B )
		{
			CEnemyR3*	pEnemyR3 = ( pEnemyR3A ? pEnemyR3A : pEnemyR3B );
			GCTypeID	tidNotEnemyR1 = ( pEnemyR3A ? pGcSprPhysB->GetGCTypeID() : pGcSprPhysA->GetGCTypeID() );

			if( GetGCTypeIDOf( CTurnNode ) == tidNotEnemyR1 )
			{
				pEnemyR3->SwitchDirection();
			}
		}
	}
}

void CLevelBase::HandlePlayerAndCoinCollision( CGCObjSpritePhysics* pGcSprPhysA, CGCObjSpritePhysics* pGcSprPhysB )
{
	if( pGcSprPhysA->GetGCTypeID() != pGcSprPhysB->GetGCTypeID() )
	{
		// returns a valid ptr if instance's EXACT type matches or nullptr if not
		CGCObjItem* pItemA = CGCObject::SafeCastToDerived< CGCObjItem* >( pGcSprPhysA );
		CGCObjItem* pItemB = CGCObject::SafeCastToDerived< CGCObjItem* >( pGcSprPhysB );

		// at least one of them is an invader?
		if( pItemA || pItemB )
		{
			CGCObjItem*	pItem = ( pItemA ? pItemA : pItemB );
			GCTypeID		tidNotItem = ( pItemA ? pGcSprPhysB->GetGCTypeID() : pGcSprPhysA->GetGCTypeID() );

			if( GetGCTypeIDOf( CGCObjPlayer ) == tidNotItem )
			{
				CGCObjectManager::ObjectKill( pItem );
				IncrementLevelGoals();
			}
		}
	}
}

void CLevelBase::HandlePlayerAndExitNode( CGCObjSpritePhysics* pGcSprPhysA, CGCObjSpritePhysics* pGcSprPhysB )
{
	if( pGcSprPhysA->GetGCTypeID() != pGcSprPhysB->GetGCTypeID() )
	{
		if( ( pGcSprPhysA->GetGCTypeID() == GetGCTypeIDOf( CExitNode ) )
			&& ( pGcSprPhysB->GetGCTypeID() == GetGCTypeIDOf( CGCObjPlayer ) )
			|| ( ( pGcSprPhysA->GetGCTypeID() == GetGCTypeIDOf( CGCObjPlayer ) )
				&& ( pGcSprPhysB->GetGCTypeID() == GetGCTypeIDOf( CExitNode ) ) ) )
		{
			if( m_iGoalsAcheived >= m_iLevelGoals )
			{
				CGCObjPlayer* pPlayer = ( pGcSprPhysA->GetGCTypeID() == GetGCTypeIDOf( CGCObjPlayer ) ) ?
					CGCObject::SafeCastToDerived< CGCObjPlayer* >( pGcSprPhysA ) : CGCObject::SafeCastToDerived< CGCObjPlayer* >( pGcSprPhysB );
				m_pcDataManager->SaveHealth( pPlayer->GetHealth() );
				m_pcDataManager->SaveLives( pPlayer->GetLives() );
				m_pcAudioManager->PlayEffectsMusic( GameConfig::EMusic::EM_DoorClosed, false );
				AdvanceToNextLevel();
			}

		}
	}
}

void CLevelBase::HandlePlayerAndTerminalCollision( CGCObjSpritePhysics * pGcSprPhysA, CGCObjSpritePhysics * pGcSprPhysB )
{
	if( pGcSprPhysA->GetGCTypeID() != pGcSprPhysB->GetGCTypeID() )
	{
		// returns a valid ptr if instance's EXACT type matches or nullptr if not
		CObjTerminal* pTerminalA = CGCObject::SafeCastToDerived< CObjTerminal* >( pGcSprPhysA );
		CObjTerminal* pTerminalB = CGCObject::SafeCastToDerived< CObjTerminal* >( pGcSprPhysB );

		CGCObjPlayer* pPlayerA = CGCObject::SafeCastToDerived< CGCObjPlayer* >( pGcSprPhysA );
		CGCObjPlayer* pPlayerB = CGCObject::SafeCastToDerived< CGCObjPlayer* >( pGcSprPhysB );

		// at least one of them is an invader?
		if( pTerminalA || pTerminalB && pPlayerA || pPlayerB )
		{
			CObjTerminal*	pTerminal = ( pTerminalA ? pTerminalA : pTerminalB );
			CGCObjPlayer*	pPlayer = ( pPlayerA ? pPlayerA : pPlayerB );
			GCTypeID		tidNotTerminal = ( pTerminalA ? pGcSprPhysB->GetGCTypeID() : pGcSprPhysA->GetGCTypeID() );

			if( GetGCTypeIDOf( CGCObjPlayer ) == tidNotTerminal )
			{
				// TODO:: Set interacting hacking object reference to player maybe...?

				// Assign hackable interface to player
				pPlayer->SetHackableInteract( &pTerminal->GetHackableInterface() );

			}
		}
	}
}

void CLevelBase::HandlePlayerLeaveTerminalCollision( CGCObjSpritePhysics* pGcSprPhysA, CGCObjSpritePhysics* pGcSprPhysB )
{
	if( pGcSprPhysA->GetGCTypeID() != pGcSprPhysB->GetGCTypeID() )
	{
		// returns a valid ptr if instance's EXACT type matches or nullptr if not
		CObjTerminal* pTerminalA = CGCObject::SafeCastToDerived< CObjTerminal* >( pGcSprPhysA );
		CObjTerminal* pTerminalB = CGCObject::SafeCastToDerived< CObjTerminal* >( pGcSprPhysB );

		CGCObjPlayer* pPlayerA = CGCObject::SafeCastToDerived< CGCObjPlayer* >( pGcSprPhysA );
		CGCObjPlayer* pPlayerB = CGCObject::SafeCastToDerived< CGCObjPlayer* >( pGcSprPhysB );

		// at least one of them is an invader?
		if( pTerminalA || pTerminalB && pPlayerA || pPlayerB )
		{
			CObjTerminal*	pTerminal = ( pTerminalA ? pTerminalA : pTerminalB );
			CGCObjPlayer*	pPlayer = ( pPlayerA ? pPlayerA : pPlayerB );
			GCTypeID		tidNotTerminal = ( pTerminalA ? pGcSprPhysB->GetGCTypeID() : pGcSprPhysA->GetGCTypeID() );

			if( GetGCTypeIDOf( CGCObjPlayer ) == tidNotTerminal )
			{
				// Remove hackable interface from player
				pPlayer->ExitHackableInteract();
			}
		}
	}
}

void CLevelBase::IncrementLevelGoals()
{
	m_pcAudioManager->PlayEffectsMusic( GameConfig::EMusic::EM_DoorOpen, false );
	m_iGoalsAcheived++;

	// Firing level complete event when the current acheived goals reache the target
	if( m_iGoalsAcheived >= m_iLevelGoals )
	{
		CEventManager::Manager().Trigger( GameConfig::EET_LevelCompleted, nullptr );
	}
}

void CLevelBase::ResetLevelGoals()
{
	m_iGoalsAcheived = 0;
	m_fLevelTimeElapsed = 0.0f;
}

void CLevelBase::KeyboardControls()
{
	const CGCKeyboardManager*		pKeyManager = AppDelegate::GetKeyboardManager();
	//Handles actions when the Escape key has been pressed 
	if( pKeyManager->ActionHasJustBeenPressed( u32( GameConfig::EPlayerActions::EPA_Options ) ) )
	{
		//pause the current background music 
		m_pcAudioManager->PauseBackground();
		//trigger the disable controls event 
		CEventManager::Manager().Trigger( GameConfig::EEventType::EET_ControlDisable , nullptr );
		//Push the pause menu screen on top of this one
		PushScene( TGCGameLayerSceneCreator< CPauseMenu >::CreateScene() );
	}
	//Handles actions when the space bar is pressed and the tutorial is not skipped 
	if ( m_pcShadowTutorial && pKeyManager->ActionHasJustBeenPressed( u32( GameConfig::EPlayerActions::EPA_Jump ) ) && !m_bTutorialIsSkipped )
	{
		//play the menu ok button 
		m_pcAudioManager->PlayEffectsMusic( GameConfig::EMusic::EM_MenuOK , false );
		//set the flag
		m_bTutorialIsSkipped = true;
		//turn the shadow tutorials invisible 
		m_pcShadowTutorial->SetVisible( false );
		//trigger the enable controls event
		CEventManager::Manager().Trigger( GameConfig::EEventType::EET_ControlEnable , nullptr );
		//Call the derived class play background music method
		PlayBackgroundMusic();
	}

}

void CLevelBase::ControllerControls( TGCController<GameConfig::ELevelInput> cController )
{
	//Handles the actions when the options button has been pressed 
	if( cController.ButtonHasJustBeenPressed( GameConfig::ELevelInput::ELI_Option ) )
	{
		//pause the current background music 
		m_pcAudioManager->PauseBackground();
		//trigger the disable controls event 
		CEventManager::Manager().Trigger( GameConfig::EEventType::EET_ControlDisable, nullptr );
		//Push the pause menu screen on top of this one
		PushScene( TGCGameLayerSceneCreator< CPauseMenu >::CreateScene() );
	}

	//Handles the actions when the X button is pressed and the shadow tutorial is not skipped 
	if( m_pcShadowTutorial && cController.ButtonHasJustBeenPressed( GameConfig::ELevelInput::ELI_ShadowSkip ) && !m_bTutorialIsSkipped )
	{
		//play the menu ok button 
		m_pcAudioManager->PlayEffectsMusic( GameConfig::EMusic::EM_MenuOK, false );
		//set the flag
		m_bTutorialIsSkipped = true;
		//turn the shadow tutorials invisible 
		m_pcShadowTutorial->SetVisible( false );
		//trigger the enable controls event
		CEventManager::Manager().Trigger( GameConfig::EEventType::EET_ControlEnable, nullptr );
		//Call the derived class play background music method
		PlayBackgroundMusic();
	}

}

// Method to be implemented by the derived levels
void CLevelBase::PlayBackgroundMusic()
{
}