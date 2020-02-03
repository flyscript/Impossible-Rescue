#ifndef _GCCUTSCENE1_H_
#define _GCCUTSCENE1_H_

#include "GamerCamp/GCCocosInterface/IGCGameLayer.h"
#include "GamerCamp/GameController/GCController.h"

class CAudioManager;
class UIAnimatedText;
class CCutscene1
	: public IGCGameLayer
{
private:
	// UI Stuff
	CGCObjSprite*			m_pcBackground;
	UIAnimatedText*			m_pAnimatedText;
	cocos2d::Label*			m_pcSkipLabel;
	bool					m_bTextSkipped;

	// Press to continue animation timer
	float m_fPressToContinueAnimationTime;

	// Audio Manager
	CAudioManager*			m_pcAudioManager;
public:
	enum EInput
	{
		EI_Skip
	};

	//////////////////////////////////////////////////////////////////////////
	// Constructor and Desctructor
	//////////////////////////////////////////////////////////////////////////
	CCutscene1();
	~CCutscene1();

	//////////////////////////////////////////////////////////////////////////
	// IGCGameLayer Virtual Methods
	//////////////////////////////////////////////////////////////////////////
	virtual void onEnter();
	virtual void VOnCreate();
	virtual void VOnDestroy();
	virtual void VOnUpdate( f32 fTimeStep );

	//////////////////////////////////////////////////////////////////////////
	// Other Update Methods
	//////////////////////////////////////////////////////////////////////////
	void ControllerUpdate( TGCController< CCutscene1::EInput > cController );
	void KeyboardUpdate();

private:
	TGCActionToKeyMap< EInput >* m_pcControllerActionToKeyMap;
};

#endif
