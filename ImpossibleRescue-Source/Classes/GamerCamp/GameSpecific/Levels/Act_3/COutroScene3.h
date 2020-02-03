#ifndef _GCOUTROSCENE3_H_
#define _GCOUTROSCENE3_H_

#include "GamerCamp/GCCocosInterface/IGCGameLayer.h"
#include "GamerCamp/GameController/GCController.h"

class CAudioManager;
class UIAnimatedText;

class COutroScene3
	: public IGCGameLayer
{
private:
	// UI Stuff
	CGCObjSprite*			m_pcBackground;
	UIAnimatedText*			m_pcAnimatedText;
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
	// Constructor and Destructor
	//////////////////////////////////////////////////////////////////////////
	COutroScene3();
	~COutroScene3();

	//////////////////////////////////////////////////////////////////////////
	// IGCGameLayer Virtual Methods
	//////////////////////////////////////////////////////////////////////////
	virtual void onEnter();
	virtual void VOnCreate();
	virtual void VOnDestroy();
	virtual void VOnUpdate( f32 fTimeStep );

	//////////////////////////////////////////////////////////////////////////
	// Control Update Methods
	//////////////////////////////////////////////////////////////////////////
	void ControllerUpdate( TGCController< COutroScene3::EInput > cController );
	void KeyboardUpdate();

private:
	// Keyboard or controller mapping
	TGCActionToKeyMap< EInput >* m_pcControllerActionToKeyMap;
};
#endif
