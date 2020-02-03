#ifndef _GCCUTSCENEMAIN_H_
#define _GCCUTSCENEMAIN_H_

#include "GamerCamp/GCCocosInterface/IGCGameLayer.h"
#include "GamerCamp/GameController/GCController.h"

class CAudioManager;
class UIAnimatedText;
class CCutsceneMain
	: public IGCGameLayer
{
private:
	// Animated story text line
	UIAnimatedText*	m_pcAnimatedText;
	// Skip Label
	cocos2d::Label*	m_pcSkipLabel;
	// Flag to check if the cutscene text has been skipped
	bool			m_bTextSkipped;
	// Skip Label animation timer
	float			m_fTime;

	// Audio Manager
	CAudioManager*	m_pcAudioManager;
public:
	// Input options for this scene
	enum EInput
	{
		EI_Skip
	};

	//////////////////////////////////////////////////////////////////////////
	// Constructor and Destructor
	//////////////////////////////////////////////////////////////////////////
	CCutsceneMain();
	~CCutsceneMain();
	
	//////////////////////////////////////////////////////////////////////////
	// IGCGameLayer Virtual Methods
	//////////////////////////////////////////////////////////////////////////
	virtual void onEnter();
	virtual void VOnCreate();
	virtual void VOnDestroy();
	virtual void VOnUpdate( f32 fTimeStep );
	
	//////////////////////////////////////////////////////////////////////////
	// Keyboard and Controller Update Methods
	//////////////////////////////////////////////////////////////////////////
	void ControllerUpdate( TGCController< CCutsceneMain::EInput > cController );
	void KeyboardUpdate();

private:
	// Keyboard or Contoller key mapping
	TGCActionToKeyMap< EInput >* m_pcControllerActionToKeyMap;
};
#endif