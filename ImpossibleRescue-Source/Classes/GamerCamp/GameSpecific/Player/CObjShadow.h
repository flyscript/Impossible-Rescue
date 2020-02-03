/*
CGObjShadow.h

William Chapman



*/

#ifndef _GCOBJSHADOW_H_
#define _GCOBJSHADOW_H_

#include "Helper/GameConfig.h"
#include "GCObjPlayer.h"

#ifndef _GCOBJSPRITEPHYSICS_H_
#include "../../GCCocosInterface/GCObjSpritePhysics.h"
#endif


class CObjShadow
	: public CGCObjSpritePhysics
{
private:

	// Movement variables
	float m_fThrust;
	float m_fDrag;

	// Pointer to the player SHADOW follows
	CGCObjPlayer* m_pPlayer;

	// Forces shadow to face the same direction as the player (used for firing)
	bool m_bLockLookToPlayer;

public:
	CObjShadow();

	virtual ~CObjShadow()
	{
	}

	//////////////////////////////////////////////////////////////////////////
	// overridden virtuals from the game object interface
	virtual void VOnResourceAcquire() override;

	// OnReset is called for each CGCObject derived class when CGCObjectManager is 
	// reset
	virtual void VOnReset() override;

	// OnUpdate is called for each CGCObject derived class when CGCObjectManager is 
	// updated in t2dSCeneGraph onProcessTick()
	virtual void VOnUpdate( f32 fTimeStep );


	// called immediately before the managing object group releases its own assets
	virtual void VOnResourceRelease();

	virtual void VOnResurrected();

	void SetPlayer( CGCObjPlayer* pPlayer );

	// Locks where SHADOW looks to match the player rather than where SHADOW is moving
	void LockLookToPlayer();

};

#endif