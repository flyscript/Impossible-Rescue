#ifndef _UIANIMATEDTEXT_H_
#define _UIANIMATEDTEXT_H_

#include "ui/CocosGUI.h"

//////////////////////////////////////////////////////////////////////////
// Enum for label animation types
//////////////////////////////////////////////////////////////////////////
// Animation Type for UILabel
enum class EUILabelIntroAnimationType
{
	EUIAnimType_None = 0,
	EUIAnimType_Bounce,
	EUIAnimType_Typing,
	EUIAnimType_SlideFromTop
};

class UIAnimatedText
	: public cocos2d::Node
{
private:
	// current active layer which are going to be added this UI component
	cocos2d::Layer* m_pcActiveLayer;

	// cocos2d ui text
	cocos2d::ui::Text* m_pcText;

	// flag for checking animation is playing or not
	bool m_bIsAnimationPlaying;

	//////////////////////////////////////////////////////////////////////////
	// Properties
	//////////////////////////////////////////////////////////////////////////
	// animation Type
	EUILabelIntroAnimationType m_eAnimationType;

	// the speed of UILabel animation
	float m_fAnimationSpeed;

	// the current index of current character which we want to add an action to it
	int m_iCurrentCharacter;

	//////////////////////////////////////////////////////////////////////////
	// Animation Scheduler Methods
	//////////////////////////////////////////////////////////////////////////
	// bounce animation behavior
	void BounceAnimation();

	// typing animation behavior
	void TypingAnimaton();

	// slide from top animation behavior
	void SlideFromTopAnimation();



public:

	//////////////////////////////////////////////////////////////////////////
	// Constructor and Destructor
	//////////////////////////////////////////////////////////////////////////
	UIAnimatedText( const std::string& rstrText, const std::string& rstrFontName, const float fFontSize, EUILabelIntroAnimationType eAnimationType, float fAnimationSpeed = 0.5f );
	~UIAnimatedText();
	static UIAnimatedText* create( const std::string& rstrText, const std::string& rstrFontName, float fFontSize, EUILabelIntroAnimationType eAnimationType = EUILabelIntroAnimationType::EUIAnimType_None, float fAnimationSpeed = 0.5f );

	//////////////////////////////////////////////////////////////////////////
	// Cocos2d Node Life Cycles Methods
	//////////////////////////////////////////////////////////////////////////
	virtual bool init();
	virtual void onEnter();
	virtual void onExit();
	virtual void update( float fTimeStep );

	//////////////////////////////////////////////////////////////////////////
	// Accessors
	/////////////////////////////////////////////////////////////////////////
	// A wrapper method for cocos2d::ui::Text::setText(std::string).
	// You can only use this method to change the label content.
	// (A capital S for SetText)
	void SetText( std::string strText );

	// Return current string value from label
	std::string GetText();

	// A wrapper method for cocos2d::ui::Text::setPosition(cocos2d::Vec2).
	// You can only use this method to change position for this Label.
	// (A capital S for SetPosition)
	void SetPosition( cocos2d::Vec2 vPosition );

	// Return current position from label
	cocos2d::Vec2 GetPosition();

	// A wrapper method for cocos2d::ui::Text::enableShadow
	// You can enable shadow effect to text.
	// (A capital E for EnableShadow)
	void EnableShadow( const cocos2d::Color4B& shadowColor, const cocos2d::Size &offset, int blurRadius );

	// A wrapper method for cocos2d::ui::Text::setTextAreaSize(cocos2d::Vec2).
	// You can only use this method to change position for this Label.
	// (A capital S for SetPosition)
	void SetTextAreaSize( const cocos2d::Size& rTextAreaSize );

	// Return text area size for the animated text
	const cocos2d::Size& GetTextAreaSize();

	// Sets how fast the text is animated
	void SetAnimationSpeed( float fAnimationSpeed );

	// Gets how fast the text is animated
	float GetAnimationSpeed();

	// A wrapper method for cocos2d::ui::Text::setTextColor(cocos2d::Color4B).
	// You can only use this method to change the text color.
	// (A capital S for SetTextColor)
	void SetTextColor( const cocos2d::Color4B color );

	// A wrapper method for cocos2d::ui::Text::setVisiblie(bool).
	// You can only use this method to change the label content.
	// (A capital S for SetText)
	void SetVisible( bool bIsVisible );

	// Setup the z index for the animated text
	void SetZIndex( int iZIndex );

	// A reset function for reseting this UI component
	void Reset();

	// A function to check if the animation has ended.
	bool HasAnimationEnded();

	void SkipAnimation();

};
#endif