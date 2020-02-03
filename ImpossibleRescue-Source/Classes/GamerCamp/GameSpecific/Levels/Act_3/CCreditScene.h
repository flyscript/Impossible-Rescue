#ifndef _GCCREDITSCENE_H_
#define _GCCREDITSCENE_H_

#include "GamerCamp/GCCocosInterface/IGCGameLayer.h"
#include "ui/CocosGUI.h"
#include "GamerCamp/GameController/GCController.h"

//////////////////////////////////////////////////////////////////////////
// Forward Declaration
//////////////////////////////////////////////////////////////////////////
class CAudioManager;

class CCreditScene
	: public IGCGameLayer
{
private:
	// Timer to back to main menu
	f32							m_fCurrentTime;
	f32							m_fTimeLeftToMenu;
	bool						m_bIsTimerEnded;
	CAudioManager*				m_pcAudioManager;
	// Credit Text Container
	cocos2d::ui::ScrollView*	m_pCreditContainerScrollView;
	cocos2d::Label*				m_pcSkipLabel;

	// Press to continue animation timer
	float						m_fPressToContinueAnimationTime;

public:
	enum EInput
	{
		EI_Skip
	};
	
	//////////////////////////////////////////////////////////////////////////
	// Constructor and Destructor
	//////////////////////////////////////////////////////////////////////////
	CCreditScene();
	~CCreditScene();

	//////////////////////////////////////////////////////////////////////////
	// IGCGameLayer Virtual Methods
	//////////////////////////////////////////////////////////////////////////
	virtual void onEnter();
	virtual void VOnCreate();
	virtual void VOnReset();
	virtual void VOnDestroy();
	virtual void VOnUpdate( f32 fTimeStep );

	//////////////////////////////////////////////////////////////////////////
	// Keyboard and Controller Update Methods
	//////////////////////////////////////////////////////////////////////////
	void ControllerUpdate( TGCController< CCreditScene::EInput > cController );
	void KeyboardUpdate();
	
	//////////////////////////////////////////////////////////////////////////
	// Callback to menu
	//////////////////////////////////////////////////////////////////////////
	void BackToMenu();

private:
	// Keyboard or Controller key mapping
	TGCActionToKeyMap< EInput >* m_pcControllerActionToKeyMap;
};

#endif
