////////////////////////////////////////////////////////////////////////////////////////////////////////////
// (C) Gamer Camp / Alex Darby 2018
// Distributed under the MIT license - see readme.md
////////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _GCOBJITEMS_H_
#include "GCObjItem.h"
#endif

#include "GamerCamp/GCCocosInterface/GCCocosHelpers.h"
#include "GamerCamp/GCObject/GCObjectManager.h"
#include "GamerCamp/GCCocosInterface/IGCGameLayer.h"
#include "../../GCCocosInterface/GB2ShapeCache-x.h"

USING_NS_CC;

//////////////////////////////////////////////////////////////////////////
// Constructor and Destructor
//////////////////////////////////////////////////////////////////////////
GCFACTORY_IMPLEMENT_CREATEABLECLASS( CGCObjItem );
CGCObjItem::CGCObjItem()
	: CGCObjSpritePhysics( GetGCTypeIDOf( CGCObjItem ) )
	, m_bIsCollected( false )
{
}

CGCObjItem::CGCObjItem( GCTypeID idDerivedType )
	: CGCObjSpritePhysics( idDerivedType )
{
}

CGCObjItem::~CGCObjItem()
{
}

//////////////////////////////////////////////////////////////////////////
// CGCObjSpritePhysics virtual methods
//////////////////////////////////////////////////////////////////////////
void CGCObjItem::VOnResourceAcquire( void )
{
	CGCObjSpritePhysics::VOnResourceAcquire();
}

void CGCObjItem::VOnReset()
{
	CGCObjSpritePhysics::VOnReset();
	m_bIsCollected = false;
}

//////////////////////////////////////////////////////////////////////////
// Accessors
//////////////////////////////////////////////////////////////////////////
void CGCObjItem::SetIsCollected( bool bIsCollected )
{
	m_bIsCollected = bIsCollected;
}

bool CGCObjItem::GetIsCollected()
{
	return m_bIsCollected;
}

