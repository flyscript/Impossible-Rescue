////////////////////////////////////////////////////////////////////////////////////////////////////////////
// (C) Gamer Camp / Alex Darby 2018
// Distributed under the MIT license - see readme.md
////////////////////////////////////////////////////////////////////////////////////////////////////////////
#include "MenuScene.h"
#include "GamerCamp/GameController/AudioManager/CAudioManager.h"
#include "GamerCamp/GameController/DataPersistence/CDataManager.h"
#include "GamerCamp/GameSpecific/Levels/Menu/CMainCutscene.h"


USING_NS_CC;


//////////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////////
Scene* CMenuLayer::scene()
{
    // 'scene' is an autorelease object
    Scene *scene = Scene::create();
    
    // 'layer' is an autorelease object
    CMenuLayer *layer = CMenuLayer::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}



CMenuLayer::CMenuLayer()
	: m_fTime			(0.0f)
	, m_pSprite			(nullptr)
	, m_pcAudioManager	(nullptr)
	, m_bSoundPlaying	(false)
{
	m_pcAudioManager = new CAudioManager();
}

//////////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////////
bool CMenuLayer::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
	scheduleUpdate();
	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();
	
	m_pSprite = Sprite::create( "Loose/Backgrounds/Splash/Splash_GC_Logo.png" );

	// position the sprite on the center of the screen
	m_pSprite->setPosition( Vec2( visibleSize.width / 2 + origin.x , visibleSize.height / 2 + origin.y ) );

	// add the sprite as a child to this layer
	this->addChild( m_pSprite , 0 );
	m_fTime = 0;

	// Audio
	m_pcAudioManager->Preload();
	m_pcAudioManager->ChangeEffecstVolume( 0.0f );
	m_pcAudioManager->PlayEffectsMusic( GameConfig::EMusic::EM_Fade, false );
	m_pcAudioManager->ChangeEffecstVolume( 50.0f );
	m_pcAudioManager->ChangeBackgroundVolume( 50.0f );
	m_pcAudioManager->PlayBackgroundMusic( GameConfig::EBackgroundSounds::EBS_Menu , true );

	return true;
}


void CMenuLayer::update(float delta)
{
	if ( m_fTime > 3.0f )
	{
		m_pSprite->setTexture( "Loose/Backgrounds/Splash/Splash_C2DX_Logo.png" );
	}
	if ( m_fTime > 6.0f )
	{
		Director::getInstance()->replaceScene( TGCGameLayerSceneCreator< CCutsceneMain >::CreateScene() );
	}

	m_fTime += delta;
}
