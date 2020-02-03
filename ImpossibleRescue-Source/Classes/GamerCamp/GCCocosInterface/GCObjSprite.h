////////////////////////////////////////////////////////////////////////////////////////////////////////////
// (C) Gamer Camp / Alex Darby 2018
// Distributed under the MIT license - see readme.md
////////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef _GCOBJSPRITE_H_
#define _GCOBJSPRITE_H_

#include "base/CCValue.h"
#include "2d/CCActionInterval.h"

#ifndef MATH_VEC2_H
#include "cocos2d/cocos/math/Vec2.h"
#endif

#ifndef __SPITE_NODE_CCSPRITE_H__
#include "2d/CCSprite.h"
#endif

#ifndef __CCGEMETRY_H__
#include "math/CCGeometry.h"
#endif 

#ifndef BOX2D_H
#include "Box2D/Box2D.h"
#endif

#ifndef _GCOBJECT_H_
#include "GamerCamp/GCObject/GCObject.h"
#endif

//////////////////////////////////////////////////////////////////////////
// CGCObjSprite is intended for use as a base class for any CGCObject 
// derived types that need to render a sprite. 
// 
// It wraps the CCSprite class, taking responsibility for managing the 
// lifetime of the managed CCSprite.
//
// The default behaviour is to make the sprite visible when it is put into 
// the live list of the containing group, and to make it invisible when it 
// is put into the dead list of the containing group.
//////////////////////////////////////////////////////////////////////////
class CGCObjSprite
: public CGCObject
{
private:
	// cocos2d sprite used to display this object
	cocos2d::Sprite*	m_pcSprite;

	// used to reposition the sprite at its original position on reset
	cocos2d::Vec2				m_v2InitialPosition;

protected:
	CGCObjSprite( GCTypeID idDerivedType );

public:
	CGCObjSprite();
	virtual ~CGCObjSprite();

	// explicit constructor & destructor 
	void			CreateSprite( const char* pszPlist );
	void			CreateSpriteFast( cocos2d::ValueMap dicSpriteInfo );
	void			DestroySprite();

	// control visibility
	inline void		SetVisible( bool bIsVisible );
	inline bool		IsVisible();

	// control flip of sprite
	inline void		SetFlippedX( bool bIsFlipped );
	inline bool		IsFlippedX();
	inline void		SetFlippedY( bool bIsFlipped );
	inline bool		IsFlippedY();

	// position accessors
	inline void				SetSpritePosition( cocos2d::Vec2 v2Position );
	inline cocos2d::Vec2	GetSpritePosition() const;

	// reset position is where the sprite will go back to when reset
	inline void				SetResetPosition( cocos2d::Vec2 v2Position );
	inline cocos2d::Vec2	GetResetPosition();

	// rotation accessors
	inline void		SetSpriteRotation( float fRotationRadians );
	inline float	GetSpriteRotation() const;

	// content size accessors
	inline cocos2d::Size GetContentSize();

	// set z index to sprite
	inline void SetZIndex( int iZIndex );

	// accessor for the sprite's rect
	inline cocos2d::Rect GetBoundingBox() const;

	// adds the sprite as a child of the passed CCNode derived type
	void			SetParent( cocos2d::Node* pNewParent );


	// run actions on the sprite (e.g. animations)
	enum EActionState
	{
		EAS_Error,
		EAS_Running,
		EAS_Finished,
	};
	EActionState		RunAction( cocos2d::Action* pAction );
	cocos2d::Action*	ReturnAction( cocos2d::Sequence* pSequence );
	cocos2d::Action*	ReturnAction( cocos2d::Action* pAction );
	EActionState		StopAction( cocos2d::Action* pAction );


	//////////////////////////////////////////////////////////////////////////
	// CGCObject Interface - see CGCObject for explanation of the purpose
	// and responsibilities of these functions
	virtual void VOnResourceAcquire();
	virtual void VOnReset();
	virtual void VOnResourceRelease();
	virtual void VOnKilled();
	virtual void VOnResurrected();
	// CGCObject Interface
	//////////////////////////////////////////////////////////////////////////
};


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// inline functions inline functions inline functions inline functions inline functions inline functions inline functions inline functions
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
// 
//////////////////////////////////////////////////////////////////////////
inline void CGCObjSprite::SetVisible( bool bIsVisible )
{
	CCAssert( m_pcSprite, "m_pcSprite is NULL. Have you called CreateSprite?" );
	m_pcSprite->setVisible( bIsVisible );
}


//////////////////////////////////////////////////////////////////////////
// 
//////////////////////////////////////////////////////////////////////////
inline bool CGCObjSprite::IsVisible()
{
	CCAssert( m_pcSprite, "m_pcSprite is NULL. Have you called CreateSprite?" );
	return m_pcSprite->isVisible();
}


//////////////////////////////////////////////////////////////////////////
// 
//////////////////////////////////////////////////////////////////////////
inline void CGCObjSprite::SetFlippedX( bool bIsFlipped )
{
	CCAssert( m_pcSprite, "m_pcSprite is NULL. Have you called CreateSprite?" );
	m_pcSprite->setFlippedX( bIsFlipped );
}


//////////////////////////////////////////////////////////////////////////
// 
//////////////////////////////////////////////////////////////////////////
inline bool CGCObjSprite::IsFlippedX()
{
	CCAssert( m_pcSprite, "m_pcSprite is NULL. Have you called CreateSprite?" );
	return m_pcSprite->isFlippedX();
}


//////////////////////////////////////////////////////////////////////////
// 
//////////////////////////////////////////////////////////////////////////
inline void CGCObjSprite::SetFlippedY( bool bIsFlipped )
{
	CCAssert( m_pcSprite, "m_pcSprite is NULL. Have you called CreateSprite?" );
	m_pcSprite->setFlippedY( bIsFlipped );
}


//////////////////////////////////////////////////////////////////////////
// 
//////////////////////////////////////////////////////////////////////////
inline bool CGCObjSprite::IsFlippedY()
{
	CCAssert( m_pcSprite, "m_pcSprite is NULL. Have you called CreateSprite?" );
	return m_pcSprite->isFlippedY();
}


//////////////////////////////////////////////////////////////////////////
// 
//////////////////////////////////////////////////////////////////////////
inline void CGCObjSprite::SetResetPosition( cocos2d::Vec2 bArgPosition )
{
	m_v2InitialPosition = bArgPosition;
}


//////////////////////////////////////////////////////////////////////////
// 
//////////////////////////////////////////////////////////////////////////
inline cocos2d::Vec2 CGCObjSprite::GetResetPosition()
{
	return m_v2InitialPosition;
}


//////////////////////////////////////////////////////////////////////////
// 
//////////////////////////////////////////////////////////////////////////
inline void CGCObjSprite::SetSpriteRotation( float fRotationRadians )
{
	m_pcSprite->setRotation( CC_RADIANS_TO_DEGREES( ( fRotationRadians < b2_pi ) ? fRotationRadians : ( fRotationRadians - ( 2 * b2_pi ) ) ) );
}


//////////////////////////////////////////////////////////////////////////
// 
//////////////////////////////////////////////////////////////////////////
inline float CGCObjSprite::GetSpriteRotation() const
{
	return CC_DEGREES_TO_RADIANS( m_pcSprite->getRotation() );
}

inline cocos2d::Size CGCObjSprite::GetContentSize()
{
	CCAssert( m_pcSprite, "m_pcSprite is NULL. Have you called CreateSprite?" );
	return m_pcSprite->getContentSize();
}

inline void CGCObjSprite::SetZIndex( int iZIndex )
{
	CCAssert( m_pcSprite, "m_pcSprite is NULL. Have you called CreateSprite?" );
	m_pcSprite->getParent()->reorderChild( m_pcSprite, iZIndex );
}

//////////////////////////////////////////////////////////////////////////
// 
//////////////////////////////////////////////////////////////////////////
inline void CGCObjSprite::SetSpritePosition( cocos2d::Vec2 bArgPosition )
{
	CCAssert( m_pcSprite, "m_pcSprite is NULL. Have you called CreateSprite?" );
	m_pcSprite->setPosition( cocos2d::Point( bArgPosition.x, bArgPosition.y ) );
}


//////////////////////////////////////////////////////////////////////////
// 
//////////////////////////////////////////////////////////////////////////
inline cocos2d::Vec2 CGCObjSprite::GetSpritePosition() const
{
	CCAssert( m_pcSprite, "m_pcSprite is NULL. Have you called CreateSprite?" );
	cocos2d::Point cPosition = m_pcSprite->getPosition();
	return cocos2d::Vec2( cPosition.x, cPosition.y );
}


//////////////////////////////////////////////////////////////////////////
// accessor for the sprite's bounding box
//////////////////////////////////////////////////////////////////////////
inline cocos2d::Rect CGCObjSprite::GetBoundingBox() const
{
	CCAssert( m_pcSprite, "m_pcSprite is NULL. Have you called CreateSprite?" );
	return m_pcSprite->getBoundingBox();
}

#endif