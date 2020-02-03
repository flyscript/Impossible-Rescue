#ifndef _GCENEMYR3_H_
#define _GCENEMYR3_H_

#include "GamerCamp/GCCocosInterface/GCFactory_ObjSpritePhysics.h"
#include "GamerCamp/GCCocosInterface/GCObjSpritePhysics.h"

class CGCObjGroupProjectile;
class CEnemyR3
	: public CGCObjSpritePhysics
	, public b2RayCastCallback
{
private:

	// Health
	float m_fHealth;

	CGCObjGroupProjectile*	m_pcProjectileManager;
	float					m_fFireRate;
	float					m_fTimeUntilCanFire;

	// Gravity
	float					m_fGravityScale;

	// Walking Speed
	float					m_fDirection;
	float					m_fWalkAcceleration;
	float					m_fWalkDrag;

	// Jump States
	float					m_fJumpAcceleration;
	int						m_iMaxJumpingFrames;
	bool					m_bCanJump;
	bool					m_bJumping;
	int						m_iJumpedFrames;
	float					m_fJumpDrag;
	float					m_fJumpAbility;

	// Attack Properties
	float m_fFireRange;

	// Flags
	bool m_bIsFullyDeactivated;
	float m_fImmobilisedTime;

public:
	//////////////////////////////////////////////////////////////////////////
	// Constructor and Destructor
	//////////////////////////////////////////////////////////////////////////
	CEnemyR3();
	virtual ~CEnemyR3();

	//////////////////////////////////////////////////////////////////////////
	// GCFactory Marcos
	//////////////////////////////////////////////////////////////////////////
	GCFACTORY_DECLARE_CREATABLECLASS( CEnemyR3 );

	//////////////////////////////////////////////////////////////////////////
	// CGCObjSpritePhysics virtual methods
	//////////////////////////////////////////////////////////////////////////
	virtual void VOnResourceAcquire() override;
	virtual void VOnResurrected() override;
	virtual void VOnUpdate( float fTimeStep ) override;
	virtual void VOnReset()	override;

	//////////////////////////////////////////////////////////////////////////
	// b2RayCastCallback virtual method
	//////////////////////////////////////////////////////////////////////////
	virtual float32 ReportFixture( b2Fixture* fixture, const b2Vec2& point, const b2Vec2& normal, float32 fraction ) override;

	//////////////////////////////////////////////////////////////////////////
	// Accessors
	//////////////////////////////////////////////////////////////////////////
	void SetFireRate( float fFirerate );
	void SetJumpAbility( float fJumpAbility );
	float GetJumpAbility();
	float Damage( float fDamage );

	//////////////////////////////////////////////////////////////////////////
	// Methods for Node Handling
	//////////////////////////////////////////////////////////////////////////
	void SwitchDirection();
	void Jump();
	void Jump( float fRequestedJumpAbility );

	//////////////////////////////////////////////////////////////////////////
	// Update Methods
	//////////////////////////////////////////////////////////////////////////
	void MovementUpdate();
	void FireUpdate( float fTimeStep );


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
