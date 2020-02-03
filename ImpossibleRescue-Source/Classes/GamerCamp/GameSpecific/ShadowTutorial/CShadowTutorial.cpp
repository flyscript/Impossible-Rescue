#include "CShadowTutorial.h"
#include "GamerCamp/GCCocosInterface/IGCGameLayer.h"
#include "GamerCamp/GCCocosInterface/GCCocosHelpers.h"

//////////////////////////////////////////////////////////////////////////
// Constructor and Destructor
//////////////////////////////////////////////////////////////////////////
CShadowTutorial::CShadowTutorial()
	: m_pcShadowImage( nullptr )
{
}

CShadowTutorial::~CShadowTutorial()
{
}

//////////////////////////////////////////////////////////////////////////
// Accessors
//////////////////////////////////////////////////////////////////////////
void CShadowTutorial::Create( int iLevel )
{
	cocos2d::Size visibleSize = cocos2d::Director::getInstance()->getVisibleSize();
	cocos2d::Point origin = cocos2d::Director::getInstance()->getVisibleOrigin();

	//Create the shadow tutorial sprite 
	m_pcShadowImage = new CGCObjSprite();
	m_pcShadowImage->CreateSprite( "TexturePacker/Sprites/Characters/Player/Shadow_Tutorial/Shadow_Tutorial.plist" );
	m_pcShadowImage->SetParent( IGCGameLayer::ActiveInstance() );
	m_pcShadowImage->SetResetPosition( cocos2d::Vec2( origin.x + ( visibleSize.width*0.4 ), origin.y + ( visibleSize.width*0.1 ) + 65.0f ) );
	m_pcShadowImage->SetZIndex( 2 );

	//Change the Sprite based on which level
	cocos2d::ValueMap pcDicPList = GCCocosHelpers::CreateDictionaryFromPlist( "TexturePacker/Sprites/Characters/Player/Shadow_Tutorial/Shadow_Tutorial.plist" );
	switch( iLevel )
	{
	case 1:
		m_pcShadowImage->RunAction( GCCocosHelpers::CreateAnimationActionLoop( GCCocosHelpers::CreateAnimation( pcDicPList, "Level_1" ) ) );
		break;
	case 2:
		m_pcShadowImage->RunAction( GCCocosHelpers::CreateAnimationActionLoop( GCCocosHelpers::CreateAnimation( pcDicPList, "Level_2" ) ) );
		break;
	case 3:
		m_pcShadowImage->RunAction( GCCocosHelpers::CreateAnimationActionLoop( GCCocosHelpers::CreateAnimation( pcDicPList, "Level_3" ) ) );
		break;
	case 4:
		m_pcShadowImage->RunAction( GCCocosHelpers::CreateAnimationActionLoop( GCCocosHelpers::CreateAnimation( pcDicPList, "Level_4" ) ) );
		break;
	}
}

//Function to change the visibility of the sprite 
void CShadowTutorial::SetVisible( bool bVisibility )
{
	m_pcShadowImage->SetVisible( bVisibility );
}

