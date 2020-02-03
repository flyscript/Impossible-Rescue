#ifndef _GCOBJMOVINGPLATFORMS_H_
#define _GCOBJMOVINGPLATFORMS_H_

#include "../../GCCocosInterface/GCObjSpritePhysics.h"

/*

*/

class CObjMovingPlatform
	: public CGCObjPlatform
{
private:

	// If this platform is a conveyor or not
	bool m_bIsConveyor;

	// Time it should spend at target and origin positions before moving again
	float m_fTimeSpentAtTargets;

	// Countdown for how long it still needs to spend at this position
	float m_fTimeLeftAtPosition;

	// Where the platform originates (should probably be equal to reset position)
	cocos2d::Vec2 m_v2Origin;

	// Where the platform tries to get to
	cocos2d::Vec2 m_v2Target;

	// The speed at which the platform moves
	float m_fSpeed;

	// If we are currently going towards the target or not (i.e: the origin)
	bool m_bMovingToTarget;

	// This moving platform needs to wait for a mover terminal is hacked
	bool m_bIsActivated;

public:

	GCFACTORY_DECLARE_CREATABLECLASS( CObjMovingPlatform );
	CObjMovingPlatform()
		: CGCObjPlatform( GetGCTypeIDOf( CObjMovingPlatform ) )
		, m_bIsConveyor( false )
		, m_v2Origin( cocos2d::Vec2::ZERO )
		, m_v2Target( cocos2d::Vec2::ZERO )
		, m_fSpeed( 5.0f )
		, m_bMovingToTarget( true )
		, m_fTimeSpentAtTargets( 1.25f )
		, m_fTimeLeftAtPosition( 1.25f )
		, m_bIsActivated( true )

	{
	}

	virtual void VOnResurrected() override;

	virtual void VOnResourceAcquire() override;

	virtual void VOnReset() override;

	virtual void VOnUpdate( float fDeltaTime );

	inline void SetIsConveyor( bool bIsConveyor );
	inline bool GetIsConveyor();

	inline void SetOrigin( cocos2d::Vec2 v2Origin );
	inline cocos2d::Vec2 GetOrigin();

	inline void SetTarget( cocos2d::Vec2 v2Target );
	inline cocos2d::Vec2 GetTarget();

	inline void SetTimeAtTargets( float fTime );
	inline float GetTimeAtTargets();

	inline void SetSpeed( float fThrust );
	inline float GetSpeed();

};


inline void CObjMovingPlatform::SetIsConveyor( bool bIsConveyor )
{
	m_bIsConveyor = bIsConveyor;
}

inline bool CObjMovingPlatform::GetIsConveyor()
{
	return m_bIsConveyor;
}

inline void CObjMovingPlatform::SetOrigin( cocos2d::Vec2 v2Origin )
{
	m_v2Origin = v2Origin;
}

inline cocos2d::Vec2 CObjMovingPlatform::GetOrigin()
{
	return m_v2Origin;
}

inline void CObjMovingPlatform::SetTarget( cocos2d::Vec2 v2Target )
{
	m_v2Target = v2Target;
}

inline cocos2d::Vec2 CObjMovingPlatform::GetTarget()
{
	return m_v2Target;
}

inline void CObjMovingPlatform::SetTimeAtTargets( float fTime )
{
	m_fTimeSpentAtTargets = fTime;
	m_fTimeLeftAtPosition = fTime;
}

inline float CObjMovingPlatform::GetTimeAtTargets()
{
	return m_fTimeSpentAtTargets;
}

inline void CObjMovingPlatform::SetSpeed( float fSpeed )
{
	m_fSpeed = fSpeed;
}

inline float CObjMovingPlatform::GetSpeed()
{
	return m_fSpeed;
}



#endif