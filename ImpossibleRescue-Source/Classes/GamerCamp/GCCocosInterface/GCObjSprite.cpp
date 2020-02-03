////////////////////////////////////////////////////////////////////////////////////////////////////////////
// (C) Gamer Camp / Alex Darby 2018
// Distributed under the MIT license - see readme.md
////////////////////////////////////////////////////////////////////////////////////////////////////////////
#include "GCObjSprite.h"

#ifndef _GCCOCOSHELPERS_H_
	#include "../GCCocosInterface/GCCocosHelpers.h"
#endif

#ifndef _IGCGAMELAYER_H_
	#include "IGCGameLayer.h"
#endif

#ifndef __COCOS2D_H__
	#include "cocos2d.h"
#endif


//////////////////////////////////////////////////////////////////////////
// save ourselves some typing later
using namespace cocos2d;



//////////////////////////////////////////////////////////////////////////
// protected version to pass through id to GCObject
//////////////////////////////////////////////////////////////////////////
CGCObjSprite::CGCObjSprite( GCTypeID idDerivedType )
: CGCObject		( idDerivedType )
, m_pcSprite	( NULL )
{
}


//////////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////////
CGCObjSprite::CGCObjSprite()
: CGCObject				( GetGCTypeIDOf( CGCObjSprite ) )
, m_pcSprite			( NULL )
, m_v2InitialPosition	( Vec2( 0.0f, 0.0f ) )
{
}


//////////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////////
//virtual 
CGCObjSprite::~CGCObjSprite()
{
	CCAssert( NULL == m_pcSprite,	"CGCObjSprite::~CGCObjSprite -it appears that DestroySprite() "
									"was not called on this instance of CGCObjSprite" );
}


//////////////////////////////////////////////////////////////////////////
//
// N.B. GCCocosHelpers::CreateSpriteFromPlist() returns an autoreleased 
// CCsprite, so we must retain() it to prevent it from being cleaned up
//////////////////////////////////////////////////////////////////////////
void CGCObjSprite::CreateSprite( const char* pszPlist )
{
	m_pcSprite = GCCocosHelpers::CreateSpriteFromPlist( pszPlist );
	CCAssert( m_pcSprite, "m_pcSprite was not created - is pszPlist correct path" );
	m_pcSprite->retain();
}


//////////////////////////////////////////////////////////////////////////
// This is faster because the user must load a __Dictionary from a plist
// before calling it - will only be faster if creating many sprites.
// 
// See Sprite_LoadTextureAndFramesToCachesAndGetDictionary()
// 
// N.B. GCCocosHelpers::CreateSpriteFromPlist() returns an autoreleased 
// CCsprite, so we must retain() it to prevent it from being cleaned up
//////////////////////////////////////////////////////////////////////////
void CGCObjSprite::CreateSpriteFast( ValueMap dicSpriteInfo )
{
	m_pcSprite = GCCocosHelpers::Sprite_CreateSpriteFrom1stFrame( dicSpriteInfo );
	CCAssert( m_pcSprite, "m_pcSprite was not created - is pszPlist correct path" );
	m_pcSprite->retain();
}

//////////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////////
void CGCObjSprite::DestroySprite()
{
	// this macro calls release on m_pcSprite then sets it to NULL
	CC_SAFE_RELEASE_NULL( m_pcSprite );
}



//////////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////////
void CGCObjSprite::SetParent( cocos2d::Node* pNewParent )
{
	CCAssert( m_pcSprite, "m_pcSprite is NULL. Have you called CreateSprite?" );
	CCAssert( pNewParent, "pNewParent is NULL" );
	// n.b. this does nothing if the CCNode derived type has no parent...
	if( m_pcSprite->getParent() )
	{
		m_pcSprite->removeFromParentAndCleanup( false );
	}
	pNewParent->addChild( m_pcSprite );
}


//////////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////////
CGCObjSprite::EActionState CGCObjSprite::RunAction( Action* pAction )
{
	CCAssert( m_pcSprite, "m_pcSprite is NULL. Have you called CreateSprite?" );
	CCAssert( pAction, "pAction is NULL" );
	m_pcSprite->runAction( pAction );
	return EAS_Running;
}

cocos2d::Action* CGCObjSprite::ReturnAction( Sequence* pSequence )
{
	CCAssert( m_pcSprite, "m_pcSprite is NULL. Have you called CreateSprite?" );
	CCAssert( pSequence, "pSequence is NULL" );
	return m_pcSprite->runAction( pSequence );
}

//////////////////////////////////////////////////////////////////////////
//Runs an action and then returns a pointer to the action
//Will Chapman
//////////////////////////////////////////////////////////////////////////
cocos2d::Action* CGCObjSprite::ReturnAction( Action* pAction )
{
	CCAssert( m_pcSprite, "m_pcSprite is NULL. Have you called CreateSprite?" );
	CCAssert( pAction, "pAction is NULL" );
	return m_pcSprite->runAction( pAction );
}

//////////////////////////////////////////////////////////////////////////
//Uses a pointer to an action to stop it
//Will Chapman
//////////////////////////////////////////////////////////////////////////
CGCObjSprite::EActionState CGCObjSprite::StopAction( Action* pAction )
{
	CCAssert( m_pcSprite, "m_pcSprite is NULL. Have you called CreateSprite?" );
	CCAssert( pAction, "pAction is NULL" );
	m_pcSprite->stopAction( pAction );
	return EAS_Running;
}


//////////////////////////////////////////////////////////////////////////
DEBUG_ONLY( static int s_iRespourceAcquiredCount = 0 );
// default behaviour is to add the managed sprite to the game layer 
//////////////////////////////////////////////////////////////////////////
//virtual 
void CGCObjSprite::VOnResourceAcquire()
{
	DEBUG_ONLY( ++s_iRespourceAcquiredCount );
}


//////////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////////
//virtual 
void CGCObjSprite::VOnReset()
{
	SetSpriteRotation( 0.0f );
	SetSpritePosition( m_v2InitialPosition );
	SetVisible( false ); // N.B. set to true in VOnResurrect()

	// if the physics world is being rendered in debug mode we need to render sprites semi-trans to make the rendering visible
	if( IGCGameLayer::ActiveInstance()->B2dIsDebugDrawing() )
	{
		m_pcSprite->setOpacity( 128 ); 
	}
}


//////////////////////////////////////////////////////////////////////////
// don't need to do anything - shutting down the parent will reduce the 
// refcount on m_pcSprite 
//////////////////////////////////////////////////////////////////////////
//virtual 
void CGCObjSprite::VOnResourceRelease()
{
	DEBUG_ONLY( if( --s_iRespourceAcquiredCount == 0 ) )
	DEBUG_ONLY( { )
	DEBUG_ONLY(		CCLOG( "CGCObjectSprite - all acquired resources have been freed" ); )
	DEBUG_ONLY( } )
	DestroySprite();
}


//////////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////////
//virtual 
void CGCObjSprite::VOnKilled()
{
	SetVisible( false );
}


//////////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////////
//virtual 
void CGCObjSprite::VOnResurrected()
{
	SetVisible( true );
}
