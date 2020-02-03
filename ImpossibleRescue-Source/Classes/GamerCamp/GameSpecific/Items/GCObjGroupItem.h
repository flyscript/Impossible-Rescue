////////////////////////////////////////////////////////////////////////////////////////////////////////////
// (C) Gamer Camp / Alex Darby 2018
// Distributed under the MIT license - see readme.md
////////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef	_GCOBJECTGROUPITEM_H_
#define	_GCOBJECTGROUPITEM_H_


#ifndef BOX2D_H
	#include "Box2d/Box2D.h"
#endif

#ifndef	_GCOBJECTGROUP_H_
	#include "GamerCamp/GCObject/GCObjectGroup.h"
#endif


//////////////////////////////////////////////////////////////////////////
// forward declare
class CGCObjSprite;
class CGCObjItem;


//////////////////////////////////////////////////////////////////////////
// 
class CGCObjGroupItem
: public CGCObjectGroup
{
	// destroy managed items
	void DestroyItems( void );

public:
	//// number of items
	//static const u32 k_uNumInvaders = 16;

	CGCObjGroupItem();		
	virtual ~CGCObjGroupItem();


//////////////////////////////////////////////////////////////////////////
// overrides for CGCObjectGroup public interface

	// handles GCObjItem
	virtual bool		VHandlesThisTypeId					( GCTypeID idQueryType );

	// must return the typeid of the CGCObjectGroup derived class
	virtual GCTypeID	VGetTypeId							( void );

	virtual void		VOnGroupResourceAcquire_PostObject	( void );	

	virtual void		VOnGroupResourceRelease				( void );	

// CGCObjectGroup public interface
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
};

#endif
