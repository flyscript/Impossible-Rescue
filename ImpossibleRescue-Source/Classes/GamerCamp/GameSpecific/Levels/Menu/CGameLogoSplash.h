#ifndef _GCGAMELOGO_H_
#define _GCGAMELOGO_H_

#include "GamerCamp/GCCocosInterface/IGCGameLayer.h"
#include "GamerCamp/GameController/GCController.h"

class CAudioManager;
class UIAnimatedText;
class CGameLogo
	: public IGCGameLayer
{
private:
	// Skip label
	cocos2d::Label* m_pcSkipLabel;
	// Skip label animation timer
	f32			m_fTime;

	// Audio Manager
	CAudioManager*	m_pcAudioManager;

public:
	// Input options in this scene
	enum EInput
	{
		EI_Skip
	};

	//////////////////////////////////////////////////////////////////////////
	// Constructor and Destructor
	//////////////////////////////////////////////////////////////////////////
	CGameLogo();
	~CGameLogo();
	
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
	void ControllerUpdate( TGCController< CGameLogo::EInput > cController );
	void KeyboardUpdate();

private:
	// Action map for controllers
	TGCActionToKeyMap< EInput >* m_pcControllerActionToKeyMap;
};
#endif