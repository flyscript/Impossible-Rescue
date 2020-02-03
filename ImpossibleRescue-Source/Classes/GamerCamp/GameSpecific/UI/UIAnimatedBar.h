#ifndef _UIANIMATEDBAR_H_
#define _UIANIMATEDBAR_H_

#include "ui/UILoadingBar.h"

class UIAnimatedBar
	: public cocos2d::Node
{
private:
	// current active layer which are going to be added this UI component
	cocos2d::Layer* m_pcActiveLayer;

	// cocos2d ui loading bar
	cocos2d::ui::LoadingBar* m_pLoadingBar;

	//////////////////////////////////////////////////////////////////////////
	// Modifiers
	//////////////////////////////////////////////////////////////////////////
	// the target value of this loading bar
	float m_fTargetValue;

	// the current value of this loading bar, used to animate the progress
	float m_fCurrentValue;

	// the max value of this loading bar
	float m_fMaxValue;

	// the speed of the growing animation
	float m_fLoadingSpeed;


public:
	//////////////////////////////////////////////////////////////////////////
	// Constructor and Destructor
	//////////////////////////////////////////////////////////////////////////
	UIAnimatedBar();
	UIAnimatedBar( const std::string& rstrTextureName, float fPercentage );
	~UIAnimatedBar();
	CREATE_FUNC( UIAnimatedBar );
	static UIAnimatedBar* create( const std::string& rstrTextureName, float fPercentage );

	//////////////////////////////////////////////////////////////////////////
	// Cocos2d Node Life Cycles Methods
	//////////////////////////////////////////////////////////////////////////
	virtual bool init();
	virtual void onEnter();
	virtual void onExit();
	virtual void update( float fTimeStep );

	//////////////////////////////////////////////////////////////////////////
	// Accessors
	//////////////////////////////////////////////////////////////////////////
	// Setter method for setting the target loading bar value.
	void SetValue( float fValue );
	void SetValueWithoutAnimation( float fValue );
	// Getter method for getting the current value of the loading bar.
	float GetValue();

	// Setter method for setting the max value of this loading bar.
	void SetMaxValue( float fMaxValue );
	// Getter method for getting the max value of this loading bar.
	float GetMaxValue();

	// A wrapper method for cocos2d::ui::LoadingBar::setPosition(cocos2d::Vec2).
	// You can only use this method to change position for this LoadingBar.
	// (A capital S for SetPosition)
	void SetPosition( cocos2d::Vec2 vPosition );
	// Return current position of loading bar.
	cocos2d::Vec2 GetPosition();

	// Setter method for setting the loading bar animation speed.
	void SetLoadingSpeed( float fLoadingSpeed );

	// Reset function for reseting this UI component
	void Reset();

	// Visible accessor
	void SetVisible( bool bIsVisible );
};
#endif