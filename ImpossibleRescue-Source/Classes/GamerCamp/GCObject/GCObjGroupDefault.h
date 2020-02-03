////////////////////////////////////////////////////////////////////////////////////////////////////////////
// (C) Gamer Camp / Alex Darby 2018
// Distributed under the MIT license - see readme.md
////////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef	_GCOBJECTGROUPDEFAULT_H_
#define	_GCOBJECTGROUPDEFAULT_H_

#ifndef _GCOBJECTGROUP_H_
	#include "GamerCamp/GCObject/GCObjectGroup.h"
#endif




//////////////////////////////////////////////////////////////////////////
// CGCObjGroupDefault is an CGCObjectGroup that will handle any CGCObject
// derived type.
// 
// If you need no special behaviour you can just use this to manage all
// the CGCObjects in your game.
//////////////////////////////////////////////////////////////////////////
class CGCObjGroupDefault
: public CGCObjectGroup
{
public:
	CGCObjGroupDefault();		
	virtual ~CGCObjGroupDefault();

	// returns true if this CGCObjectGroup handles idQueryType.
	// The behaviour of CGCObjGroupDefault is to handle anything.
	//
	// N.B. specifically: the default CObjectGroup in CGCObjectManager will 
	// be set when a CGCObjectGroup derived type registers which handles GCTYPEID_INVALID.
	virtual bool VHandlesThisTypeId( GCTypeID idQueryType );

 	// must return the typeid of *this* class
	virtual GCTypeID VGetTypeId( void );
};

#endif
