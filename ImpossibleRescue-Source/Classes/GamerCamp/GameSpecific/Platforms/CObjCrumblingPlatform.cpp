
#include "AppDelegate.h"
#include "GCObjPlatform.h"
#include "GamerCamp/GCCocosInterface/GCCocosHelpers.h"
#include "GamerCamp/GameController/AudioManager/CAudioManager.h"

#ifndef _GCOBJCRUMBLINGPLATFORMS_H_
#include "CObjCrumblingPlatform.h"
#endif


GCFACTORY_IMPLEMENT_CREATEABLECLASS( CObjCrumblingPlatform );

CObjCrumblingPlatform::CObjCrumblingPlatform()
	: CGCObjPlatform( GetGCTypeIDOf( CObjCrumblingPlatform ) )
	, m_bCrumbleInstantly( false )
	, m_bCrumbling( false )
	, m_bDead( false )
	, m_bDying( false )
	, m_fCrumbleTime( 2.0f )
	, m_fCrumbleProgress( 0.0f )
	, m_fDeathAnimationTime( 0.5f )
	, m_fDeathAnimationProgress( 0.0f )
{
	m_pcAudioManager = new CAudioManager;
}

//virtual 
void CObjCrumblingPlatform::VOnResourceAcquire()
{
	CGCObjSpritePhysics::VOnResourceAcquire();
}

// Triggers the crumble
void CObjCrumblingPlatform::PlayerEnter()
{
	m_bCrumbling = true;

	// Run crumble animation if appropriate
	if( !m_bDying )
	{
		const char* kcp_Animation = "Crumble_Crumbling";
		ValueMap pcDicPList = GCCocosHelpers::CreateDictionaryFromPlist( GetFactoryCreationParams()->strPlistFile );
		StopAction( m_pCurrentAction );
		m_pCurrentAction = ReturnAction( GCCocosHelpers::CreateAnimationActionLoop( GCCocosHelpers::CreateAnimation( pcDicPList, kcp_Animation ) ) );
	}
}

//Stops crumbling if possible for this platform
void CObjCrumblingPlatform::PlayerLeave()
{
	m_bCrumbling = m_bCrumbleInstantly;

	if( !m_bDying )
		StopAction( m_pCurrentAction );
}

// Runs every frame
void CObjCrumblingPlatform::VOnUpdate( float fDeltaTime )
{
	CGCObjSpritePhysics::VOnUpdate( fDeltaTime );

	if( m_bCrumbling )
	{
		// Progressively crumble
		m_fCrumbleProgress += fDeltaTime;

		// If platform has deteriorated
		if( m_fCrumbleTime <= m_fCrumbleProgress )
		{

			// Detract from death time
			m_fDeathAnimationProgress += fDeltaTime;

			// If finished dying animation
			if( m_fDeathAnimationTime <= m_fDeathAnimationProgress )
			{
				//Finish dying
				m_bDead = true;
				m_bCrumbling = false;
			}
			else if( !m_bDying )
			{
				// Play crumble platform destroy sfx
				m_pcAudioManager->PlayEffectsMusic( GameConfig::EMusic::EM_CrumblingPlatformBreak, false );

				// Run dying animation
				m_bDying = true;

				const char* kcp_Animation = "Crumble_Disintegrate";
				ValueMap pcDicPList = GCCocosHelpers::CreateDictionaryFromPlist( GetFactoryCreationParams()->strPlistFile );
				StopAction( m_pCurrentAction );

				CallFunc* pPlatformDestroyCallback = CallFunc::create( [ & ] ()
				{
					m_fDeathAnimationProgress = m_fDeathAnimationTime + 1;
					m_bDead = true;
					m_bCrumbling = false;
				} );

				Sequence* pSequence = Sequence::create(
					GCCocosHelpers::CreateAnimationActionOnce( GCCocosHelpers::CreateAnimation( pcDicPList, kcp_Animation ) ),
					pPlatformDestroyCallback,
					NULL );
				m_pCurrentAction = ReturnAction( pSequence );


			}
		}

	}
}

void CObjCrumblingPlatform::VOnReset()
{
	CGCObjSpritePhysics::VOnReset();

	// animate!
	const char* pszPlatformNormal = "Crumble_Normal";
	ValueMap dicPList = GCCocosHelpers::CreateDictionaryFromPlist( GetFactoryCreationParams()->strPlistFile );
	m_pCurrentAction = ReturnAction( GCCocosHelpers::CreateAnimationActionLoop( GCCocosHelpers::CreateAnimation( dicPList, pszPlatformNormal ) ) );

	m_bCrumbling = false;
	m_bDead = false;
	m_bDying = false;
	m_fCrumbleProgress = 0.0f;
	m_fDeathAnimationProgress = 0.0f;
}

