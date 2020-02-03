#ifndef _Level3_H_
#define _Level3_H_

#include "GamerCamp/GameSpecific/Barrier/CObjBarrier.h"
#include "GamerCamp/GameSpecific/Levels/CLevelBase.h"


class CLevel3 : public CLevelBase
{
public:
	//////////////////////////////////////////////////////////////////////////
	// Constructor and Destructor
	//////////////////////////////////////////////////////////////////////////
	CLevel3();
	virtual ~CLevel3();

	//////////////////////////////////////////////////////////////////////////
	// CCNode Virtual Methods
	//////////////////////////////////////////////////////////////////////////
	virtual void onEnter() override;

	//////////////////////////////////////////////////////////////////////////
	// IGCGameLayer Virtual Methods Override from Parent
	//////////////////////////////////////////////////////////////////////////
	virtual	void VOnCreate() override;
	virtual void VOnUpdate( f32 fTimeStep ) override;
	virtual	void VOnDestroy() override;
	virtual void VOnReset() override;

	//////////////////////////////////////////////////////////////////////////
	// B2ContactListener Virtual Methods Override from Parent
	//////////////////////////////////////////////////////////////////////////
	virtual void BeginContact( b2Contact* pB2Contact ) override;
	virtual void EndContact( b2Contact* pB2Contact ) override;
	virtual void PreSolve( b2Contact* pB2Contact, const b2Manifold* pOldManifold ) override;
	virtual void PostSolve( b2Contact* pB2Contact, const b2ContactImpulse* pImpulse ) override;

	//////////////////////////////////////////////////////////////////////////
	// CLevelBase Virtual Method
	//////////////////////////////////////////////////////////////////////////
	virtual void PlayBackgroundMusic() override;
	// Needs to be overridden to go to next level
	virtual void NextLevel() override;
};

#endif
