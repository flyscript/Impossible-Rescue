#ifndef _GCENEMYR1_H_
#define _GCENEMYR1_H_

#include "GamerCamp/GCCocosInterface/GCFactory_ObjSpritePhysics.h"
#include "GamerCamp/GCCocosInterface/GCObjSpritePhysics.h"
#include "GamerCamp/EventManager/CEventManager.h"

class CGCEvent;

class CEnemyR1
	: public CGCObjSpritePhysics
{
private:
	// Health
	float m_fHealth;

	// Gravity
	float m_fGravityScale;

	// Walking Speed
	float m_fDirection;
	float m_fWalkAcceleration;
	float m_fWalkDrag;

	// Jump States
	float m_fJumpAcceleration;
	int m_iMaxJumpingFrames;
	bool m_bCanJump;
	bool m_bJumping;
	int m_iJumpedFrames;
	float m_fJumpDrag;
	float m_fJumpAbility;

	// Flags
	bool m_bIsFullyDeactivated;
	bool m_bIsMovementActivated;
	float m_fImmobilisedTime;

	// Event Manager reference
	CEventManager&	m_cEventManager = CEventManager::Manager();


public:
	//////////////////////////////////////////////////////////////////////////
	// Constructor and Destructor
	//////////////////////////////////////////////////////////////////////////
	CEnemyR1();
	virtual ~CEnemyR1();

	//////////////////////////////////////////////////////////////////////////
	// GCFactory Marcos
	//////////////////////////////////////////////////////////////////////////
	GCFACTORY_DECLARE_CREATABLECLASS( CEnemyR1 );

	//////////////////////////////////////////////////////////////////////////
	// Movement related methods
	//////////////////////////////////////////////////////////////////////////
	void SwitchDirection();
	void Jump();
	void Jump( float fRequestedJumpAbility );

	//////////////////////////////////////////////////////////////////////////
	// overridden virtuals from the game object interface
	//////////////////////////////////////////////////////////////////////////
	virtual void VOnResourceAcquire( void ) override;
	virtual void VOnResurrected( void ) override;
	virtual void VOnUpdate( f32 fTimeStep );
	virtual void VOnReset();

	//////////////////////////////////////////////////////////////////////////
	// Accessors
	//////////////////////////////////////////////////////////////////////////
	void SetJumpAbility( float fJumpAbility );
	float GetJumpAbility();
	float Damage( float fDamage );

	//////////////////////////////////////////////////////////////////////////
	// Event System
	//////////////////////////////////////////////////////////////////////////
	/*
	Immobilises the enemy for a given period of time
	Default of -1.0f seconds causes permanent immobilisation
	Thus, a call with 0.0f seconds would re-mobilise the enemy
	*/
	virtual void Immobilise( float fTime = -1.0f );
};


#endif
