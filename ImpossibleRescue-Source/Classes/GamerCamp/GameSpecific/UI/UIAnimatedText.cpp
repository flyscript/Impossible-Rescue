#include "cocos2d.h"
#include "GamerCamp/GCCocosInterface/IGCGameLayer.h";

#include "UIAnimatedText.h"

//////////////////////////////////////////////////////////////////////////
// Constructor and Destructor
//////////////////////////////////////////////////////////////////////////
UIAnimatedText::UIAnimatedText( const std::string& rstrText, const std::string& rstrFontName, const float fFontSize, EUILabelIntroAnimationType eAnimationType, float fAnimationSpeed /*= 0.5f*/ )
	: m_pcActiveLayer( nullptr )
	, m_eAnimationType( eAnimationType )
	, m_fAnimationSpeed( fAnimationSpeed )
	, m_iCurrentCharacter( 0 )
{
	m_pcText = cocos2d::ui::Text::create( rstrText, rstrFontName, fFontSize );
}

UIAnimatedText::~UIAnimatedText()
{
}

UIAnimatedText* UIAnimatedText::create( const std::string& rstrText, const std::string& rstrFontName, float fFontSize, EUILabelIntroAnimationType eAnimationType, float fAnimationSpeed /*= 0.5f*/ )
{
	UIAnimatedText *pRet = new( std::nothrow ) UIAnimatedText( rstrText, rstrFontName, fFontSize, eAnimationType, fAnimationSpeed );
	if( pRet && pRet->init() )
	{
		// setup autorelease for this object
		pRet->autorelease();
		return pRet;
	}
	else
	{
		// if the object failed on init, delete it right now
		delete pRet;
		pRet = nullptr;
		return nullptr;
	}
}

//////////////////////////////////////////////////////////////////////////
// Animation Scheduler Methods
//////////////////////////////////////////////////////////////////////////
void UIAnimatedText::BounceAnimation()
{
	// get the current character as sprite
	cocos2d::Sprite* pCharacterSprite = ( cocos2d::Sprite * )m_pcText->getLetter( m_iCurrentCharacter );
	if( pCharacterSprite )
	{
		// create jump action
		cocos2d::JumpBy* jump = cocos2d::JumpBy::create( 0.5f, cocos2d::Vec2::ZERO, 60, 1 );
		cocos2d::RepeatForever* jump_4ever = cocos2d::RepeatForever::create( jump );
		pCharacterSprite->setOpacity( 255 );

		// apply action to character
		pCharacterSprite->runAction( jump );
	}

	// increase current character index
	m_iCurrentCharacter++;

	// check all the characters have been applied jump action
	if( m_iCurrentCharacter >= m_pcText->getStringLength() )
	{
		// once the all characters have been applied jump action, unschedule the action
		cocos2d::Director::getInstance()->getScheduler()->unschedule( "BounceAnimation", this );
		// change the flag for notifying this animation has been ended
		m_bIsAnimationPlaying = false;
	}
}

void UIAnimatedText::TypingAnimaton()
{
	// get the current character as sprite
	cocos2d::Sprite* pCharacterSprite = ( cocos2d::Sprite * )m_pcText->getLetter( m_iCurrentCharacter );
	if( pCharacterSprite )
	{
		// set the character opacity
		pCharacterSprite->setOpacity( 255 );
	}

	// increase current character index
	m_iCurrentCharacter++;

	// check all the characters have been applied jump action
	if( m_iCurrentCharacter >= m_pcText->getStringLength() )
	{
		// once the all characters have been applied jump action, unschedule the action
		cocos2d::Director::getInstance()->getScheduler()->unschedule( "TypingAnimation", this );
		// change the flag for notifying this animation has been ended
		m_bIsAnimationPlaying = false;
	}
}

void UIAnimatedText::SlideFromTopAnimation()
{
	// get the current character as sprite
	cocos2d::Sprite* pCharacterSprite = ( cocos2d::Sprite * )m_pcText->getLetter( m_iCurrentCharacter );
	if( pCharacterSprite )
	{
		// set the origin position for the current character
		pCharacterSprite->setPosition( cocos2d::Vec2( pCharacterSprite->getPosition().x, pCharacterSprite->getPosition().y + 20.0f ) );

		// run actions
		pCharacterSprite->runAction( cocos2d::Sequence::create( cocos2d::MoveBy::create( m_fAnimationSpeed * 0.2f, cocos2d::Vec2( 0.0f, -20.0f ) ), NULL ) );

		// set visible
		pCharacterSprite->setOpacity( 255 );
	}

	// increase current character index
	m_iCurrentCharacter++;

	// check all the characters have been applied jump action
	if( m_iCurrentCharacter >= m_pcText->getStringLength() )
	{
		// once the all characters have been applied jump action, unschedule the action
		cocos2d::Director::getInstance()->getScheduler()->unschedule( "SlideFromTopAnimation", this );
		// change the flag for notifying this animation has been ended
		m_bIsAnimationPlaying = false;
	}
}

bool UIAnimatedText::HasAnimationEnded()
{
	return !m_bIsAnimationPlaying;
}

void UIAnimatedText::EnableShadow( const cocos2d::Color4B& shadowColor /* = Color4B::BLACK */,
	const cocos2d::Size &offset /* = Size(2 ,-2)*/,
	int blurRadius /*= 0 */ )
{
	m_pcText->enableShadow( shadowColor, offset, blurRadius );
}

//////////////////////////////////////////////////////////////////////////
// Cocos2d Node Life Cycles Methods
//////////////////////////////////////////////////////////////////////////
bool UIAnimatedText::init()
{
	if( !Node::init() )
	{
		return false;
	}

	// get active scene
	m_pcActiveLayer = IGCGameLayer::ActiveInstance();

	// add child to scene
	m_pcActiveLayer->addChild( m_pcText, 1 );

	// reset the animation
	this->Reset();

	// registration for the update function
	cocos2d::Director::getInstance()->getScheduler()->scheduleUpdate( this, 0, false );

	return true;
}

void UIAnimatedText::onEnter()
{
}

void UIAnimatedText::onExit()
{
}

void UIAnimatedText::update( float fTimeStep )
{
}

//////////////////////////////////////////////////////////////////////////
// Accessors
//////////////////////////////////////////////////////////////////////////
// Set the content of the label
void UIAnimatedText::SetText( std::string strText )
{
	if( m_pcText )
	{
		m_pcText->setString( strText );
	}
}
// Get the content of the label
std::string UIAnimatedText::GetText()
{
	if( m_pcText )
	{
		return m_pcText->getString();
	}
	return NULL;
}

// Set the label position
void UIAnimatedText::SetPosition( cocos2d::Vec2 vPosition )
{
	if( m_pcText )
	{
		m_pcText->setPosition( vPosition );
	}
}

// Get the label position
cocos2d::Vec2 UIAnimatedText::GetPosition()
{
	if( m_pcText )
	{
		return m_pcText->getPosition();
	}
	return cocos2d::Vec2::ZERO;
}

void UIAnimatedText::SetTextAreaSize( const cocos2d::Size & rTextAreaSize )
{
	m_pcText->setTextAreaSize( rTextAreaSize );
}

const cocos2d::Size & UIAnimatedText::GetTextAreaSize()
{
	return m_pcText->getTextAreaSize();
}

void UIAnimatedText::SetAnimationSpeed( float fAnimationSpeed )
{
	m_fAnimationSpeed = fAnimationSpeed;
}

float UIAnimatedText::GetAnimationSpeed()
{
	return m_fAnimationSpeed;
}

// Set label text color
void UIAnimatedText::SetTextColor( const cocos2d::Color4B color )
{
	if( m_pcText )
	{
		m_pcText->setTextColor( color );
	}
}

void UIAnimatedText::SetVisible( bool bIsVisible )
{
	if( m_pcText )
	{
		m_pcText->setVisible( bIsVisible );
	}
}

void UIAnimatedText::SetZIndex( int iZIndex )
{
	m_pcText->getParent()->reorderChild( m_pcText, iZIndex );
}

void UIAnimatedText::Reset()
{
	// init the current character index pointer
	m_iCurrentCharacter = 0;

	// setup animation scheduler
	switch( m_eAnimationType )
	{
	case EUILabelIntroAnimationType::EUIAnimType_Bounce:
		m_pcText->setOpacity( 0 );
		cocos2d::Director::getInstance()->getScheduler()->schedule( std::bind( &UIAnimatedText::BounceAnimation, this ), this, m_fAnimationSpeed, false, "BounceAnimation" );
		m_bIsAnimationPlaying = true;
		break;
	case EUILabelIntroAnimationType::EUIAnimType_Typing:
		m_pcText->setOpacity( 0 );
		cocos2d::Director::getInstance()->getScheduler()->schedule( std::bind( &UIAnimatedText::TypingAnimaton, this ), this, m_fAnimationSpeed * 0.2f, false, "TypingAnimation" );
		m_bIsAnimationPlaying = true;
		break;
	case EUILabelIntroAnimationType::EUIAnimType_SlideFromTop:
		m_pcText->setOpacity( 0 );
		cocos2d::Director::getInstance()->getScheduler()->schedule( std::bind( &UIAnimatedText::SlideFromTopAnimation, this ), this, m_fAnimationSpeed * 0.2f, false, "SlideFromTopAnimation" );
		m_bIsAnimationPlaying = true;
		break;
	default:
		// show all text when there is no any cases match the condition
		m_pcText->setOpacity( 255 );
		// set no animation is playing by default
		m_bIsAnimationPlaying = false;
		break;
	}
}

void UIAnimatedText::SkipAnimation()
{
	// stop animation by type
	switch( m_eAnimationType )
	{
	case EUILabelIntroAnimationType::EUIAnimType_Bounce:
		cocos2d::Director::getInstance()->getScheduler()->unschedule( "BounceAnimation", this );
		break;
	case EUILabelIntroAnimationType::EUIAnimType_Typing:
		cocos2d::Director::getInstance()->getScheduler()->unschedule( "TypingAnimation", this );
		break;
	case EUILabelIntroAnimationType::EUIAnimType_SlideFromTop:
		cocos2d::Director::getInstance()->getScheduler()->unschedule( "SlideFromTopAnimation", this );
		break;
	}

	m_pcText->setOpacity( 255 );
}