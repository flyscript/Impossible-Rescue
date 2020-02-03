#include "CGameOver.h"

#include "AppDelegate.h"
#include "Act_1/CLevel1.h"

//////////////////////////////////////////////////////////////////////////
// Constructor and Destructor
//////////////////////////////////////////////////////////////////////////
GCGameOver::GCGameOver()
	: IGCGameLayer( GetGCTypeIDOf( GCGameOver ) )
    , m_fCurrentTime ( 0.0f )
    , m_fTimeTillChange ( 1.0f )
{
}

GCGameOver::~GCGameOver()
{
}

//////////////////////////////////////////////////////////////////////////
// IGCGameLayer Virtual Methods
//////////////////////////////////////////////////////////////////////////
void GCGameOver::VOnCreate()
{
	IGCGameLayer::VOnCreate();

	cocos2d::Vec2 origin = Director::getInstance()->getVisibleOrigin();
	cocos2d::Size visibleSize = Director::getInstance()->getVisibleSize();

	m_pcBackground = Sprite::create( "Loose/Backgrounds/Splash/GameOverScreen.png" );
	this->addChild( m_pcBackground , 0 );
	// position the sprite on the center of the screen
	m_pcBackground->setPosition( Vec2( visibleSize.width / 2 + origin.x , visibleSize.height / 2 + origin.y ) );
	
}

void GCGameOver::VOnDestroy()
{
	IGCGameLayer::VOnDestroy();
}

void GCGameOver::VOnUpdate( f32 fTimeStep )
{
	if( m_fCurrentTime > m_fTimeTillChange )
	{
		ReplaceScene( TGCGameLayerSceneCreator< CLevel1 >::CreateScene() );
	}
	else
	{
		m_fCurrentTime += fTimeStep;
	}
}
