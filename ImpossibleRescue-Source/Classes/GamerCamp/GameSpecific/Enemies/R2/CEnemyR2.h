#ifndef _GCENEMYR2_H_
#define _GCENEMYR2_H_

#include "GamerCamp/GCCocosInterface/GCFactory_ObjSpritePhysics.h"
#include "GamerCamp/GCCocosInterface/GCObjSpritePhysics.h"
#include "Helper/GameConfig.h"

class CGCObjGroupProjectile;
class CEnemyR2
	: public CGCObjSpritePhysics
	, public b2RayCastCallback
{
private:

	// Health
	float m_fHealth;

	// Shoot config
	CGCObjGroupProjectile*	m_pcProjectileManager;
	float					m_fFireRate;
	float					m_fTimeUntilCanFire;
	float m_fFireRange;

	// States
	int m_iDirection;
	bool m_bIsFullyDeactivated;
	float m_fImmobilisedTime;

public:
	//////////////////////////////////////////////////////////////////////////
	// Constructor and Destructor
	//////////////////////////////////////////////////////////////////////////
	CEnemyR2();
	virtual ~CEnemyR2();

	//////////////////////////////////////////////////////////////////////////
	// GCFactory Marcos
	//////////////////////////////////////////////////////////////////////////
	GCFACTORY_DECLARE_CREATABLECLASS( CEnemyR2 );

	//////////////////////////////////////////////////////////////////////////
	// CGCObjSpritePhysics virtual methods
	//////////////////////////////////////////////////////////////////////////
	virtual void VOnResourceAcquire() override;
	virtual void VOnUpdate( float fTimeStep ) override;
	virtual void VOnResurrected() override;
	virtual void VOnReset()	override;

	//////////////////////////////////////////////////////////////////////////
	// Box2D Raycast virtual method for callback
	//////////////////////////////////////////////////////////////////////////
	virtual float32 ReportFixture( b2Fixture* fixture, const b2Vec2& point, const b2Vec2& normal, float32 fraction ) override;

	//////////////////////////////////////////////////////////////////////////
	// Accessors
	//////////////////////////////////////////////////////////////////////////
	void SetFireRate( float fFirerate );
	float Damage( float fDamage );

	/*
	Immobilises the enemy for a given period of time
	Default of -1.0f seconds causes permanent immobilisation
	Thus, a call with 0.0f seconds would re-mobilise the enemy
	*/
	virtual void Immobilise( float fTime = -1.0f );
};


#endif
