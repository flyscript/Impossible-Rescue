#ifndef _GCOBJCRUMBLINGPLATFORMS_H_
#define _GCOBJCRUMBLINGPLATFORMS_H_

#include "../../GCCocosInterface/GCObjSpritePhysics.h"

class CAudioManager;

class CObjCrumblingPlatform
	: public CGCObjPlatform
{
private:
	// Audio manager
	CAudioManager* m_pcAudioManager;

	// If this platform will crumble the instant it's been touched, or progressively degrades
	bool m_bCrumbleInstantly;

	// The time before this platform is no longer usable
	float m_fCrumbleTime;
	float m_fCrumbleProgress;

	// The time before this platform takes to run its destruction animation
	float m_fDeathAnimationTime;
	float m_fDeathAnimationProgress;

	// If the platform is currently crumbling
	bool m_bCrumbling;

	// If the platform has deteriorated
	bool m_bDying;

	// If the platform has finished crumbling
	bool m_bDead;


public:

	GCFACTORY_DECLARE_CREATABLECLASS( CObjCrumblingPlatform );
	CObjCrumblingPlatform();

	virtual void VOnResourceAcquire() override;

	virtual void VOnUpdate( float fDeltaTime ) override;

	virtual void VOnReset() override;

	void PlayerEnter();
	void PlayerLeave();

	inline void SetCrumbleTime( float fTime );
	inline float GetCrumbleTime();

	inline void SetCrumbleInstantly( bool bCrumbleInstantly );
	inline bool GetCrumbleInstantly();

	//Returns true if this crumbling platform has died
	inline bool IsDead();

	//Returns true if this crumbling platform is dying
	inline bool IsDying();
};


inline void CObjCrumblingPlatform::SetCrumbleTime( float fTime )
{
	m_fCrumbleTime = fTime;
}

inline float CObjCrumblingPlatform::GetCrumbleTime()
{
	return m_fCrumbleTime;
}

inline void CObjCrumblingPlatform::SetCrumbleInstantly( bool bCrumbleInstantly )
{
	m_bCrumbleInstantly = bCrumbleInstantly;
}

inline bool CObjCrumblingPlatform::GetCrumbleInstantly()
{
	return m_bCrumbleInstantly;
}

inline bool CObjCrumblingPlatform::IsDead()
{
	return m_bDead;
}

inline bool CObjCrumblingPlatform::IsDying()
{
	return m_bDying;
}
#endif