////////////////////////////////////////////////////////////////////////////////////////////////////////////
// (C) Gamer Camp / Alex Darby 2018
// Distributed under the MIT license - see readme.md
////////////////////////////////////////////////////////////////////////////////////////////////////////////
#include "base/ccMacros.h"

#include "GamerCamp/GCCocosInterface/GCCollisionManager.h"

 
//////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////// 
void CGCCollisionManager::HandleCollisions( b2World& rcb2World )
{
	u32 uNumHandlers = m_vecCollisionPairHandlers.size();

	for( const b2Contact* pB2Contact = rcb2World.GetContactList();
		NULL != pB2Contact;
		pB2Contact = pB2Contact->GetNext() )
	{
		const b2Fixture* pFixtureA = CGCObjSpritePhysics::FromB2DContactGetFixture_A( pB2Contact );
		const b2Fixture* pFixtureB = CGCObjSpritePhysics::FromB2DContactGetFixture_B( pB2Contact );

		// return if either physics body has null user data
		CGCObjSpritePhysics* pGcSprPhysA = CGCObjSpritePhysics::FromB2DFixtureGetSpritePhysics( pFixtureA );
		if( !pGcSprPhysA )
		{
			continue;
		}

		CGCObjSpritePhysics* pGcSprPhysB = CGCObjSpritePhysics::FromB2DFixtureGetSpritePhysics( pFixtureB );
		if( !pGcSprPhysB )
		{
			continue;
		}

		for( u32 u = 0; u < uNumHandlers; ++u )
		{
			CGCCollisionPairHandler* pcHandler = m_vecCollisionPairHandlers[ u ];
			
			if( pcHandler->CanHandleCollisionsFor( ( *pGcSprPhysA ), ( *pGcSprPhysB ) ) )
			{
				pcHandler->HandleCollision( ( *pGcSprPhysA ), ( *pGcSprPhysB ), ( *pB2Contact ) );
			}
		}
	}
}