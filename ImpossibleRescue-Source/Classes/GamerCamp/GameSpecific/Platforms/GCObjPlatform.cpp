////////////////////////////////////////////////////////////////////////////////////////////////////////////
// (C) Gamer Camp / Alex Darby 2018
// Distributed under the MIT license - see readme.md
////////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _GCOBJPLATFORMS_H_
#include "GCObjPlatform.h"
#endif

#include "../../GCCocosInterface/GB2ShapeCache-x.h"

//////////////////////////////////////////////////////////////////////////
// Constructor
//////////////////////////////////////////////////////////////////////////
CGCObjPlatform::CGCObjPlatform()
	: CGCObjSpritePhysics( GetGCTypeIDOf( CGCObjPlatform ) )
{
}

CGCObjPlatform::CGCObjPlatform( GCTypeID idDerivedType )
	: CGCObjSpritePhysics( idDerivedType )
{
}

//////////////////////////////////////////////////////////////////////////
// GCFactory Marcos
//////////////////////////////////////////////////////////////////////////
GCFACTORY_IMPLEMENT_CREATEABLECLASS( CGCObjPlatform );