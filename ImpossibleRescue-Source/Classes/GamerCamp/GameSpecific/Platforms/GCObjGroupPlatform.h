////////////////////////////////////////////////////////////////////////////////////////////////////////////
// (C) Gamer Camp / Alex Darby 2018
// Distributed under the MIT license - see readme.md
////////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef	_GCOBJECTGROUPPLATFORM_H_
#define	_GCOBJECTGROUPPLATFORM_H_


#ifndef	_GCOBJECTGROUP_H_
#include "GamerCamp/GCObject/GCObjectGroup.h"
#endif


//////////////////////////////////////////////////////////////////////////
// forward declare
class CGCObjSprite;
class CGCObjPlatform;


//////////////////////////////////////////////////////////////////////////
// 
class CGCObjGroupPlatform
	: public CGCObjectGroup
{

public:
	//// number of platforms
	//static const u32 k_uNumInvaders = 16;

	CGCObjGroupPlatform();
	virtual ~CGCObjGroupPlatform();

	//////////////////////////////////////////////////////////////////////////
	// overrides for CGCObjectGroup public interface

		// handles GCObjPlatform
	virtual bool		VHandlesThisTypeId( GCTypeID idQueryType );

	// must return the typeid of the CGCObjectGroup derived class
	virtual GCTypeID	VGetTypeId();

	virtual void		VOnGroupResourceRelease();

	// CGCObjectGroup public interface
	//////////////////////////////////////////////////////////////////////////
};
#endif
