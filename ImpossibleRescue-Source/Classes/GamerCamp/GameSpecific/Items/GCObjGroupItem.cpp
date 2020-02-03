////////////////////////////////////////////////////////////////////////////////////////////////////////////
// (C) Gamer Camp / Alex Darby 2018
// Distributed under the MIT license - see readme.md
////////////////////////////////////////////////////////////////////////////////////////////////////////////
#include <string.h>

#include "GamerCamp/GameSpecific/Items/GCObjGroupItem.h"

#include "GamerCamp/GCObject/GCObjectManager.h"
#include "GamerCamp/GCCocosInterface/IGCGameLayer.h"
#include "GamerCamp/Core/GCTypes.h"
#include "GamerCamp/GCObject/GCObject.h"
#include "GamerCamp/GameSpecific/Items/GCObjItem.h"
#include "GamerCamp/GCCocosInterface/GCCocosHelpers.h"


//////////////////////////////////////////////////////////////////////////
// using
using namespace cocos2d;



//////////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////////
CGCObjGroupItem::CGCObjGroupItem()
{
}



//////////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////////
// virtual
CGCObjGroupItem::~CGCObjGroupItem()
{}



//////////////////////////////////////////////////////////////////////////
// only handle items
//////////////////////////////////////////////////////////////////////////
//virtual 
bool CGCObjGroupItem::VHandlesThisTypeId( GCTypeID idQueryType )
{
	return( GetGCTypeIDOf( CGCObjItem ) == idQueryType );
}



//////////////////////////////////////////////////////////////////////////
// must return the typeid of the CGCObjectGroup derived class
//////////////////////////////////////////////////////////////////////////
//virtual 
GCTypeID CGCObjGroupItem::VGetTypeId( void )
{
	return GetGCTypeIDOf( CGCObjGroupItem );
}



//////////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////////
//virtual 
void CGCObjGroupItem::VOnGroupResourceAcquire_PostObject( void )
{
	// parent class version
	CGCObjectGroup::VOnGroupResourceAcquire_PostObject();

	// set up animations for all items
	const char* pszPlist_Coin	        = "TexturePacker/Sprites/Coin/Coin.plist";
	const char* pszAnim_Coin_Rotate		= "Rotate";

	// make an animation
	// N.B. pdictPList is returned autoreleased - will clean itself at end of frame if not retained
	ValueMap&	rdicPList	= GCCocosHelpers::CreateDictionaryFromPlist( pszPlist_Coin );
	Animation*	pAnimation	= GCCocosHelpers::CreateAnimation( rdicPList, pszAnim_Coin_Rotate );

	ForEachObject( [&] ( CGCObject* pcItemAsObject ) -> bool
	{
		// this check is essentially redundant, but they say assumption is the mother of all something or others...
		CCAssert(	( GetGCTypeIDOf( CGCObjItem ) == pcItemAsObject->GetGCTypeID() ), 
			"CGCObject derived type mismatch!" );

		CGCObjSprite* pItemSprite = (CGCObjSprite*) pcItemAsObject;
		pItemSprite->RunAction( GCCocosHelpers::CreateAnimationActionLoop( pAnimation ) );
		return true;
	} );
}


//////////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////////
//virtual 
void CGCObjGroupItem::VOnGroupResourceRelease( void )
{
	// n.b. this must happen first, as will fail if objects destroyed before 
	CGCObjectGroup::VOnGroupResourceRelease(); 
}


