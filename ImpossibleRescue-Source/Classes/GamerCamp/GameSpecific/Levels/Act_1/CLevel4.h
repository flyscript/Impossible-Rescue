#ifndef _Level4_H_
#define _Level4_H_

#include "GamerCamp/GameSpecific/Barrier/CObjBarrier.h"
#include "GamerCamp/GameSpecific/Levels/CLevelBase.h"


class CLevel4 : public CLevelBase
{
private:

	// Barrier
	CObjBarrier*					m_pcGCOBarrier;
	CGCFactoryCreationParams		m_sBarrierCreateParams;

	// Flags
	bool							m_bIsBarrierActivated;

public:
	//////////////////////////////////////////////////////////////////////////
	// Constructor and Destructor
	//////////////////////////////////////////////////////////////////////////
	CLevel4();
	virtual ~CLevel4();

	//////////////////////////////////////////////////////////////////////////
	// CCNode Virtual Methods
	//////////////////////////////////////////////////////////////////////////
	virtual void onEnter() override;

	//////////////////////////////////////////////////////////////////////////
	// IGCGameLayer Virtual Methods Override from Parent
	//////////////////////////////////////////////////////////////////////////
	virtual	void VOnCreate( void ) override;
	virtual void VOnUpdate( f32 fTimeStep ) override;
	virtual	void VOnDestroy( void ) override;
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

	// Update method for barrier movement
	void UpdateBarrier();
};

#endif
