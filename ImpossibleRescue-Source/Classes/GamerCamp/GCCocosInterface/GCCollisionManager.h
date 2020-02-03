////////////////////////////////////////////////////////////////////////////////////////////////////////////
// (C) Gamer Camp / Alex Darby 2018
// Distributed under the MIT license - see readme.md
////////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef	_GCCOLLISIONMANAGER_H_
#define	_GCCOLLISIONMANAGER_H_

#include <functional>


#ifndef BOX2D_H
	#include "Box2D/Box2D.h"
#endif

#ifndef _GCTYPES_H_
	#include "GamerCamp/Core/GCTypes.h"
#endif

#ifndef _GCOBJSPRITEPHYSICS_H_
	#include "GamerCamp/GCCocosInterface/GCObjSpritePhysics.h"
#endif


//////////////////////////////////////////////////////////////////////////
class CGCCollisionPairHandler
{
	GCTypeID	m_tidTypeOne;
	GCTypeID	m_tidTypeTwo;
	
protected:

	// this is constructed with the type Ids in the order they are passed to the handle collision function
	CGCCollisionPairHandler( GCTypeID tidOne, GCTypeID tidTwo )
	: m_tidTypeOne( tidOne )
	, m_tidTypeTwo( tidTwo )
	{}

	inline GCTypeID GetTypeIdOf1stParam()
	{
		return m_tidTypeOne;
	}

	inline GCTypeID GetTypeIdOf2ndParam()
	{
		return m_tidTypeTwo;
	}

	virtual void VInternalHandleCollision( CGCObjSpritePhysics& pcOSPOne, CGCObjSpritePhysics& pcOSPTwo, const b2Contact& rcContact ) = 0;


public:

	virtual ~CGCCollisionPairHandler()
	{}

	// this function doesn't check the order, just whether the pair matches 
	bool CanHandleCollisionsFor( CGCObjSpritePhysics& rcOSP_A, CGCObjSpritePhysics& rcOSP_B )
	{
		if(		(		( m_tidTypeOne == rcOSP_A.GetGCTypeID() )
					&&	( m_tidTypeTwo == rcOSP_B.GetGCTypeID() ) )
			||	(		( m_tidTypeTwo == rcOSP_A.GetGCTypeID() )
					&&	( m_tidTypeOne == rcOSP_B.GetGCTypeID() ) ) )
		{
			return true;
		}
		return false;
	}

	// this function flips the order to match the order expected by VHandleCollision
	void HandleCollision( CGCObjSpritePhysics& rcOSP_A, CGCObjSpritePhysics& rcOSP_B, const b2Contact& rcContact )
	{
		GCASSERT( CanHandleCollisionsFor( rcOSP_A, rcOSP_B ), "types don't match, can't handle this collision!" );
		
		if(		( GetTypeIdOf1stParam() == rcOSP_A.GetGCTypeID() )
			&&	( GetTypeIdOf2ndParam() == rcOSP_B.GetGCTypeID() ) )
		{
			VInternalHandleCollision( rcOSP_A, rcOSP_B, rcContact );
		}
		else
		{
			VInternalHandleCollision( rcOSP_B, rcOSP_A, rcContact );
		}
	}
};


//////////////////////////////////////////////////////////////////////////
template < typename TOSPDerivedOne, typename TOSPDerivedTwo >
class TGCCollisionPairHandler 
: public CGCCollisionPairHandler
{
	std::function< void( TOSPDerivedOne&, TOSPDerivedTwo&, const b2Contact& ) > m_funcOnHandleCollision;

	virtual void VInternalHandleCollision( CGCObjSpritePhysics& rcOSPOne, CGCObjSpritePhysics& rcOSPTwo, const b2Contact& rcContact ) override
	{
		CCASSERT( ( GetTypeIdOf1stParam() == rcOSPOne.GetGCTypeID() ), "type Id of pcOSPOne does not match GetTypeIdOf1stParam()" );
		CCASSERT( ( GetTypeIdOf2ndParam() == rcOSPTwo.GetGCTypeID() ), "type Id of pcOSPTwo does not match GetTypeIdOf2ndParam()" );
		m_funcOnHandleCollision( static_cast< TOSPDerivedOne& >( rcOSPOne ), static_cast< TOSPDerivedTwo& >( rcOSPTwo ), rcContact );
	}

public:

	TGCCollisionPairHandler( GCTypeID tidOne, GCTypeID tidTwo, std::function< void( TOSPDerivedOne&, TOSPDerivedTwo&, const b2Contact& ) > funcHandleCollision )
	: CGCCollisionPairHandler( tidOne, tidTwo )
	, m_funcOnHandleCollision( funcHandleCollision )
	{
		static_assert( std::is_base_of< CGCObjSpritePhysics, TOSPDerivedOne >::value,	"TOSPDerivedOne must be derived from CGCObjSpritePhysics" );
		static_assert( std::is_base_of< CGCObjSpritePhysics, TOSPDerivedTwo >::value,	"TOSPDerivedTwo must be derived from CGCObjSpritePhysics" );
		GCASSERT( tidOne == GetGCTypeIDOf( TOSPDerivedOne ),							"type id mismatch!" );
		GCASSERT( tidTwo == GetGCTypeIDOf( TOSPDerivedTwo ),							"type id mismatch!" );
	}
};


//////////////////////////////////////////////////////////////////////////
//
// this header 1is a particularly modern C++ solution to the need to 
// simply find pout about collisions between different CGCobjSpritePhysics
// instances which are knocking about in the game.
// 
// The dark magic is partly in the template class TGCCollisionPairHandler 
// which is the only way to create instances of CGCCollisionPairHandler
// 
// It essentially enforces a "type safe" conversion from the base 
// CGCObjSpritePhysics pointer accessible via the collision data into 
// the CGCObjSpritePhysics derived type it's instantiated with.
// 
// The actual darkest magic happens in CGCCollisionManager::AddCollisionHandler
// this template function uses some non-standard template stuff to create 
// & register the correct instantiation of TGCCollisionPairHandler based 
// on the parameters of the function passed to it (thus avoiding the potential 
// human errors whiuch could come from manually typing it al out - not to 
// mention it's abut a billion times more elegant & cleaner looking!
// 
// Actually it would also be worth passing through the b2dcontact structure 
// to the collision handler or there's loads of useful context entirely missing
// 
//////////////////////////////////////////////////////////////////////////
class CGCCollisionManager
{
	std::vector< CGCCollisionPairHandler* > m_vecCollisionPairHandlers;

public:
	// can be created and deleted freely
	CGCCollisionManager()
	{}

	// cleans up anything in the vector
	~CGCCollisionManager()
	{
		u32 uNumHandlers = m_vecCollisionPairHandlers.size();
		for( u32 u = 0; u < uNumHandlers; ++u )
		{
			delete m_vecCollisionPairHandlers[ u ];
			m_vecCollisionPairHandlers[ u ] = nullptr;
		}
		m_vecCollisionPairHandlers.clear();
	}

	// this could do with extending to return an id / pointer / similar which can be used to remove the collision handler later
	// wouls also need the RemoveCollisionHandler function added
	template< typename TFunctor > 
	void AddCollisionHandler( TFunctor funcHandleCollision )
	{
		using TFunctorParam0 = std::remove_reference< GCHelpers::function_traits< TFunctor >::argument< 0 > >::type;
		using TFunctorParam1 = std::remove_reference< GCHelpers::function_traits< TFunctor >::argument< 1 > >::type;
		m_vecCollisionPairHandlers.push_back( new TGCCollisionPairHandler< TFunctorParam0, TFunctorParam1 >( GetGCTypeIDOf( TFunctorParam0 ), GetGCTypeIDOf( TFunctorParam1 ), funcHandleCollision ) );
	}

	void HandleCollisions( b2World& pcb2World );
};

#endif
