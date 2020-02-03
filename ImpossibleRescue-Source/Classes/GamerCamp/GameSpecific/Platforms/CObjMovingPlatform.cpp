
#include "AppDelegate.h"
#include "GCObjPlatform.h"
#include "GamerCamp/GCCocosInterface/GCCocosHelpers.h"
#include "GamerCamp/EventManager/CEventManager.h"

#ifndef _GCOBJMOVINGPLATFORMS_H_
#include "CObjMovingPlatform.h"
#endif


GCFACTORY_IMPLEMENT_CREATEABLECLASS( CObjMovingPlatform );

//virtual 
void CObjMovingPlatform::VOnResourceAcquire()
{
	CGCObjSpritePhysics::VOnResourceAcquire();

	const char* pszPlatformNormal = "Platform_Idle";

	// animate!
	ValueMap dicPList = GCCocosHelpers::CreateDictionaryFromPlist( GetFactoryCreationParams()->strPlistFile );
	m_pCurrentAction = ReturnAction( GCCocosHelpers::CreateAnimationActionLoop( GCCocosHelpers::CreateAnimation( dicPList, pszPlatformNormal ) ) );

	// read factory parameters
	m_bIsActivated = GetFactoryCreationParams()->bRequiredActivation ? false : true;
}

void CObjMovingPlatform::VOnReset()
{
	CGCObjSpritePhysics::VOnReset();
	m_bIsActivated = GetFactoryCreationParams()->bRequiredActivation ? false : true;
}


// Runs every frame
void CObjMovingPlatform::VOnUpdate( float fDeltaTime )
{
	if( m_bIsActivated )
	{
		if( m_v2Origin.isZero() )
		{
			SetOrigin( GetSpritePosition() );
			SetTarget( m_v2Origin + Vec2( GetFactoryCreationParams()->fMovingPlatformTargetX, GetFactoryCreationParams()->fMovingPlatformTargetY ) );
		}

		if( m_fTimeLeftAtPosition <= 0.0f )
		{
			if( GetSpritePosition().fuzzyEquals( m_bMovingToTarget ? m_v2Target : m_v2Origin, 1.0f ) )
			{
				// If we've reached the target, then revert course
				m_bMovingToTarget = !m_bMovingToTarget;
				m_fTimeLeftAtPosition = m_fTimeSpentAtTargets;
				SetVelocity( Vec2::ZERO );
			}
			else
			{
				// Get the direction we need to go to reach the target
				Vec2 v2Direction = ( m_bMovingToTarget ? m_v2Target : m_v2Origin ) - GetSpritePosition();
				v2Direction.normalize();

				// Do some basic thrust-drag to get us there
				//ApplyForceToCenter((v2Direction * m_fThrust) - (GetVelocity() * m_fDrag));
				SetVelocity( v2Direction * m_fSpeed );
			}
		}
		else
		{
			m_fTimeLeftAtPosition -= fDeltaTime;
		}
	}
}

// Sets gravity to 0 for this object when it's created
void CObjMovingPlatform::VOnResurrected()
{
	CGCObjSpritePhysics::VOnResurrected();

	//Hotfix for rotation when it's disabled
	GetPhysicsBody()->SetFixedRotation( true );

	GetPhysicsBody()->SetGravityScale( 0.0f );

	// listen to an event - activate moving platform event
	CEventManager::Manager().AddListener( GameConfig::EEventType::EET_ActivateMovingPlatform, [ & ] ( void* )
	{
		m_bIsActivated = true;
	} );
}

