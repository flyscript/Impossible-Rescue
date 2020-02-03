#include "CObjShadow.h"
#include "GamerCamp/GCCocosInterface/GCCocosHelpers.h"


USING_NS_CC;

CObjShadow::CObjShadow()
	: CGCObjSpritePhysics( GetGCTypeIDOf( CObjShadow ) )
	, m_fThrust( 5.0f )
	, m_fDrag( 60.0f )
	, m_pPlayer( nullptr )
{

}

IN_CPP_CREATION_PARAMS_DECLARE( CObjShadow, "TexturePacker/Sprites/Characters/Player/Shadow/Shadow.plist", "shadow", b2_dynamicBody, true );
void CObjShadow::VOnResourceAcquire()
{
	IN_CPP_CREATION_PARAMS_AT_TOP_OF_VONRESOURCEACQUIRE( CObjShadow );

	CGCObjSpritePhysics::VOnResourceAcquire();

	const char* pszAnim_shadowFloat = "Hover";

	// animate!
	ValueMap dicPList = GCCocosHelpers::CreateDictionaryFromPlist( GetFactoryCreationParams()->strPlistFile );
	RunAction( GCCocosHelpers::CreateAnimationActionLoop( GCCocosHelpers::CreateAnimation( dicPList, pszAnim_shadowFloat, 12.f ) ) );
}

void CObjShadow::VOnResurrected()
{
	CGCObjSpritePhysics::VOnResurrected();

	GetPhysicsBody()->SetGravityScale( 0.0f );
}

void CObjShadow::VOnUpdate( f32 fTimeStep )
{
	if( m_pPlayer )
	{
		Vec2 v2Target( m_pPlayer->GetSpritePosition() + Vec2( ( -1 + ( 2 * m_pPlayer->IsFlippedX() ) ) * -50.0f, 10.0f ) );
		Vec2 v2Dif( v2Target - GetSpritePosition() );
		Vec2 v2Drag( GetVelocity() * m_fDrag );
		Vec2 v2Thrust( v2Dif * m_fThrust );
		Vec2 v2Force( v2Thrust - v2Drag );

		ApplyForceToCenter( v2Force );

		// Set sprite flip based on velocity
		if( GetVelocity().x < 2.5f && GetVelocity().x > -2.5f )
		{
			m_bLockLookToPlayer = false;
			SetFlippedX( m_pPlayer->IsFlippedX() );
		}
		else if( m_bLockLookToPlayer )
		{
			SetFlippedX( m_pPlayer->IsFlippedX() );
		}
		else
		{
			if( GetVelocity().x > 0.0f )
			{
				SetFlippedX( true );
			}
			else if( GetVelocity().x < 0.0f )
			{
				SetFlippedX( false );
			}
		}
	}
}

void CObjShadow::VOnResourceRelease()
{
	CGCObjSpritePhysics::VOnResourceRelease();
}


void CObjShadow::VOnReset()
{
	CGCObjSpritePhysics::VOnReset();

	// reset velocity and flip state
	SetFlippedX( false );
	SetFlippedY( false );

	m_bLockLookToPlayer = false;

	// reset
	if( GetPhysicsBody() )
	{
		Vec2 v2SpritePos = GetSpritePosition();
		GetPhysicsBody()->SetLinearVelocity( b2Vec2( 0.0f, 0.0f ) );
		GetPhysicsBody()->SetTransform( IGCGameLayer::B2dPixelsToWorld( b2Vec2( v2SpritePos.x, v2SpritePos.y ) ), 0.0f );
		GetPhysicsBody()->SetFixedRotation( true );
	}
}

void CObjShadow::SetPlayer( CGCObjPlayer* pPlayer )
{
	m_pPlayer = pPlayer;
}

void CObjShadow::LockLookToPlayer()
{
	m_bLockLookToPlayer = true;
}
