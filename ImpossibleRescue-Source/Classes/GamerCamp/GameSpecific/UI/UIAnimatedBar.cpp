#include "cocos2d.h"
#include "ui/UILoadingBar.h"
#include "GamerCamp/GCCocosInterface/IGCGameLayer.h";

#include "UIAnimatedBar.h"

//////////////////////////////////////////////////////////////////////////
// Constructor and Destructor
//////////////////////////////////////////////////////////////////////////
UIAnimatedBar::UIAnimatedBar()
	: m_pcActiveLayer( nullptr )
	, m_pLoadingBar( nullptr )
	, m_fCurrentValue( 0.0f )
	, m_fTargetValue( 0.0f )
	, m_fMaxValue( 0.0f )
	, m_fLoadingSpeed( 10.0f )
{
	// init pointer here
	m_pLoadingBar = cocos2d::ui::LoadingBar::create( "Loose/UI/Bars/loadingBar.png", 0.0f );
	m_pLoadingBar->setDirection( cocos2d::ui::LoadingBar::Direction::LEFT );
	m_pLoadingBar->setVisible( true );
}

UIAnimatedBar::UIAnimatedBar( const std::string& rstrTextureName, float fPercentage )
	: m_pcActiveLayer( nullptr )
	, m_pLoadingBar( nullptr )
	, m_fCurrentValue( 0.0f )
	, m_fTargetValue( 0.0f )
	, m_fMaxValue( 0.0f )
	, m_fLoadingSpeed( 10.0f )
{
	// init pointer here
	m_pLoadingBar = cocos2d::ui::LoadingBar::create( rstrTextureName, fPercentage );
	m_pLoadingBar->setDirection( cocos2d::ui::LoadingBar::Direction::LEFT );
	m_pLoadingBar->setVisible( true );
}

UIAnimatedBar::~UIAnimatedBar()
{
	m_pcActiveLayer = nullptr;
}

UIAnimatedBar * UIAnimatedBar::create( const std::string & rstrTextureName, float fPercentage )
{
	UIAnimatedBar *pRet = new( std::nothrow ) UIAnimatedBar( rstrTextureName, fPercentage );
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
// Init
//////////////////////////////////////////////////////////////////////////
bool UIAnimatedBar::init()
{
	if( !Node::init() )
	{
		return false;
	}

	// get active scene and add the UI component to scene
	m_pcActiveLayer = IGCGameLayer::ActiveInstance();
	m_pcActiveLayer->addChild( m_pLoadingBar, 1 );

	Reset();

	// schedule the update function to this object
	cocos2d::Director::getInstance()->getScheduler()->scheduleUpdate( this, 0, false );

	return true;
}

//////////////////////////////////////////////////////////////////////////
// Cocos2d Node Life Cycles Methods
//////////////////////////////////////////////////////////////////////////
void UIAnimatedBar::onEnter()
{
}

void UIAnimatedBar::onExit()
{
}

void UIAnimatedBar::update( float fDeltaTime )
{
	if( m_pLoadingBar && m_pcActiveLayer )
	{
		if( m_fCurrentValue < m_fTargetValue )
		{
			m_fCurrentValue += m_fLoadingSpeed * fDeltaTime;
			// round current value to target value
			if( m_fCurrentValue > m_fTargetValue )
			{
				m_fCurrentValue = m_fTargetValue;
			}
		}
		else if( m_fCurrentValue > m_fTargetValue )
		{
			m_fCurrentValue -= m_fLoadingSpeed * fDeltaTime;
			// floor current value to target value
			if( m_fCurrentValue < m_fTargetValue )
			{
				m_fCurrentValue = m_fTargetValue;
			}
		}

		m_pLoadingBar->setPercent( m_fCurrentValue / m_fMaxValue * 100 );
	}
}

//////////////////////////////////////////////////////////////////////////
// Accessors
//////////////////////////////////////////////////////////////////////////
// target value
float UIAnimatedBar::GetValue()
{
	return m_fTargetValue;
}

void UIAnimatedBar::SetValue( float fValue )
{
	m_fTargetValue = fValue;
}

void UIAnimatedBar::SetValueWithoutAnimation( float fValue )
{
	m_fTargetValue = fValue;
	m_fCurrentValue = fValue;
}

// max value
float UIAnimatedBar::GetMaxValue()
{
	return m_fMaxValue;
}

void UIAnimatedBar::SetMaxValue( float fMaxValue )
{
	m_fMaxValue = fMaxValue;
}

// bar location
cocos2d::Vec2 UIAnimatedBar::GetPosition()
{
	if( m_pLoadingBar )
	{
		return m_pLoadingBar->getPosition();
	}
	else
	{
		return cocos2d::Vec2::ZERO;
	}
}

void UIAnimatedBar::SetPosition( cocos2d::Vec2 vPosition )
{
	if( m_pLoadingBar && m_pcActiveLayer )
	{
		m_pLoadingBar->setPosition( vPosition );
	}
}

// bar loading speed
void UIAnimatedBar::SetLoadingSpeed( float fLoadingSpeed )
{
	m_fLoadingSpeed = fLoadingSpeed;
}

void UIAnimatedBar::Reset()
{
	// reset loading bar animation values
	m_fCurrentValue = 0.0f;
	m_fTargetValue = 0.0f;
}

void UIAnimatedBar::SetVisible( bool bIsVisible )
{
	if( m_pLoadingBar )
	{
		m_pLoadingBar->setVisible( bIsVisible );
	}
}
