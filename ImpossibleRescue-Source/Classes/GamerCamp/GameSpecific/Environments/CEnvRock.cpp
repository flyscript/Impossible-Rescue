#ifndef _CENVROCK_H_
#include "GamerCamp/GameSpecific/Environments/CEnvRock.h"
#endif

#include "GamerCamp/GCCocosInterface/GCCocosHelpers.h"
#include "GamerCamp/GCCocosInterface/GB2ShapeCache-x.h"

//////////////////////////////////////////////////////////////////////////
// Constructor and Destructor
//////////////////////////////////////////////////////////////////////////
CEnvRock::CEnvRock()
	: CGCObjSpritePhysics( GetGCTypeIDOf( CEnvRock ) )
{
}

//////////////////////////////////////////////////////////////////////////
// GCFactory Marcos
//////////////////////////////////////////////////////////////////////////
GCFACTORY_IMPLEMENT_CREATEABLECLASS( CEnvRock );

//////////////////////////////////////////////////////////////////////////
// CGCObjSpritePhysics Virtual Method
//////////////////////////////////////////////////////////////////////////
void CEnvRock::VOnResourceAcquire()
{
	CGCObjSpritePhysics::VOnResourceAcquire();

	// Switch to different rock sprite asset by getting value from GetFactoryCreationParams()->strRockStyle
	cocos2d::ValueMap pcDicPList = GCCocosHelpers::CreateDictionaryFromPlist( GetFactoryCreationParams()->strPlistFile );
	RunAction( GCCocosHelpers::CreateAnimationActionLoop( GCCocosHelpers::CreateAnimation( pcDicPList, GetFactoryCreationParams()->strRockStyle ) ) );
}
