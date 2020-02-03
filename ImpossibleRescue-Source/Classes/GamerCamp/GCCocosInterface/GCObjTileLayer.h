////////////////////////////////////////////////////////////////////////////////////////////////////////////
// (C) Gamer Camp / Alex Darby 2018
// Distributed under the MIT license - see readme.md
////////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef _GCOBJTILELAYER_H_
#define _GCOBJTILELAYER_H_

#ifndef MATH_VEC2_H
	#include "cocos2d/cocos/math/Vec2.h"
#endif

#ifndef __CCTMX_TILE_MAP_H__
	#include "2d/CCTMXTiledMap.h"
#endif

#ifndef _GCOBJECT_H_
	#include "GamerCamp/GCObject/GCObject.h"
#endif




// 
// N.B. since this shares a lot of common code with CGCObjSprite the
// common interface should probably be factored into a base class they can 
// both derive from...
//////////////////////////////////////////////////////////////////////////
class CGCObjTileLayer
: public CGCObject
{
private:
	// cocos2d TMXTiledMap used to display the .tmx file
	cocos2d::TMXTiledMap* m_pTMXMap;

	// used to reposition the sprite at its original position on reset
	cocos2d::Vec2			m_v2InitialPosition;

protected:
							CGCObjTileLayer		( GCTypeID idDerivedType );

public:
							CGCObjTileLayer		();
	virtual					~CGCObjTileLayer	();

	// explicit construction / destruction for tiel layer
	void					CreateTileLayer		( const char* pszPlist );
	void					DestroyTileLayer	();

	// position accessors
	inline void				SetPosition			( cocos2d::Vec2 v2Position );
	inline cocos2d::Vec2	GetPosition			() const;
	// reset position is where the sprite will go back to when reset
	inline void				SetResetPosition	( cocos2d::Vec2 v2Position );
	
	// adds the sprite as a child of the passed CCNode derived type
	void					SetParent			( cocos2d::Node* pNewParent );

	//////////////////////////////////////////////////////////////////////////
	// accessors for the TMX data
	
		// return values for GetObjectPosition
		enum EGetObjPosErr
		{
			EGetObjPosErr_OK,				// returned if ok
			EGetObjPosErr_GroupNotFound,	// returned if named object group not found
			EGetObjPosErr_ObjectNotFound	// returned if named object not found
		};
		// n.b. position is returned by reference
		EGetObjPosErr GetObjectPosition( const char* pszObjectGroupName, const char* pszObjectName, cocos2d::Vec2& rv2ReturnPos );

	// accessors for the TMX data
	//////////////////////////////////////////////////////////////////////////
	
	//////////////////////////////////////////////////////////////////////////
	// CGCObject Interface - see CGCObject for explanation of the purpose
	// and responsibilities of these functions
		virtual void VOnResourceAcquire	();
		virtual void VOnReset		    ();              // must be implemented as pure virtual in GCObject
		virtual void VOnUpdate		    ( float fTimeStep );   // must be implemented as pure virtual in GCObject
		virtual void VOnResourceRelease	();
	// CGCObject Interface
	//////////////////////////////////////////////////////////////////////////
};


//////////////////////////////////////////////////////////////////////////
// 
//////////////////////////////////////////////////////////////////////////
inline void CGCObjTileLayer::SetResetPosition( cocos2d::Vec2 bArgPosition )
{
	m_v2InitialPosition = bArgPosition;
}


//////////////////////////////////////////////////////////////////////////
// 
//////////////////////////////////////////////////////////////////////////
inline void CGCObjTileLayer::SetPosition( cocos2d::Vec2 bArgPosition )
{
	CCAssert( m_pTMXMap, "m_pTMXMap is NULL. Have you called CreateSprite?" );
	m_pTMXMap->setPosition( cocos2d::Point( bArgPosition.x, bArgPosition.y ) );
}


//////////////////////////////////////////////////////////////////////////
// 
//////////////////////////////////////////////////////////////////////////
inline cocos2d::Vec2 CGCObjTileLayer::GetPosition() const
{
	CCAssert( m_pTMXMap, "m_pcSpm_pTMXMaprite is NULL. Have you called CreateSprite?" );	
	return m_pTMXMap->getPosition();
}

#endif // _GCOBJTILELAYER_H_