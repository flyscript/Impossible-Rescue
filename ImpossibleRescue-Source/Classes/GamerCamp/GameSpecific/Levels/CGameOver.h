#ifndef _GAMEOVER_H
#define _GAMEOVER_H
#include "GamerCamp/GCCocosInterface/IGCGameLayer.h"

class GCGameOver
	: public IGCGameLayer
{
private:
	// Back to menu timer
	float				m_fCurrentTime;
	// Back to menu second
	float				m_fTimeTillChange;
	// Background image
	cocos2d::Sprite*	m_pcBackground;
public:
	//////////////////////////////////////////////////////////////////////////
	// Constructor and Destructor
	//////////////////////////////////////////////////////////////////////////
	GCGameOver();
	~GCGameOver();

	//////////////////////////////////////////////////////////////////////////
	// IGCGameLayer Virtual Methods
	//////////////////////////////////////////////////////////////////////////
	virtual void VOnCreate();
	virtual void VOnDestroy();
	virtual void VOnUpdate( f32 fTimeStep );
	
};
#endif