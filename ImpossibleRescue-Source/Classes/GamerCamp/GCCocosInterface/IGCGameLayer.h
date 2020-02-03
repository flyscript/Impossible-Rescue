////////////////////////////////////////////////////////////////////////////////////////////////////////////
// (C) Gamer Camp / Alex Darby 2018
// Distributed under the MIT license - see readme.md
////////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef _IGCGAMELAYER_H_
#define _IGCGAMELAYER_H_

#ifndef __COCOS2D_H__
	#include "cocos2d.h"
#endif

#ifndef MATH_VEC2_H
	#include "cocos2d/cocos/math/Vec2.h"
#endif

#ifndef _GCTYPES_H_
	#include "GamerCamp/Core/GCTypes.h"
#endif

#ifndef _GCOBJECTMANAGER_H_
	#include "GamerCamp/GCObject/GCObjectManager.h"
#endif

#ifndef _GCOBJECTGROUPDEFAULT_H_
	#include "GamerCamp/GCObject/GCObjGroupDefault.h"
#endif

#ifndef	_GCCOLLISIONMANAGER_H_
	#include "GamerCamp/GCCocosInterface/GCCollisionManager.h"
#endif

#ifndef B2_MATH_H
	#include "Box2D/Box2D.h" 
#endif

#ifndef GB2ShapeCache_x_h
	#include "../GCCocosInterface/GB2ShapeCache-x.h"
#endif

#ifndef __CC_TOUCH_H__
	#include "base/CCTouch.h"
#endif

#include <vector>



//////////////////////////////////////////////////////////////////////////
// forward declare


//////////////////////////////////////////////////////////////////////////
// IGCGameLayer - root class for connecting CGCObjectManager (and 
// therefore other gamercamp game logic types) into the cocos2d-x engine.
//
//
//
//////////////////////////////////////////////////////////////////////////
class IGCGameLayer 
: public cocos2d::Layer
, public CNoAllocListable
{
private:
	GCTypeID				m_idTypeID;					// GCTypeid of this type
	
	bool					m_bOnTopOfSceneStack;		// set to false when a new scene is pushed over the top of this scene
	bool					m_bSceneTransitionActive;	// set to true when a scene transition is active (see onAdd / onExit etc.) 
	
	CGCObjectManager*		m_pcObjectManager;			// root of GCObject update code
	CGCObjGroupDefault*		m_pcGCObjGroupDefault;		// default object group needed by GCObjectManager

	// prevent construction other than by derivation
	IGCGameLayer	();

	// revoke copy
	IGCGameLayer( const IGCGameLayer& );
	IGCGameLayer& operator=( const IGCGameLayer& );

	// internal update function - called via CCNode schedule system
	void InternalUpdate( f32 fTimeStep );

protected:
	// protected constructor - can only be created by deriving from this class
	IGCGameLayer( GCTypeID idDerivedType );

	// protected accessor for m_bSceneTransitionActive
	inline bool ASceneTransitionIsActive();


public:
	// virtual destructor
	virtual			~IGCGameLayer	();

	// accessor for TypeID - set by deriving classes in constructor
	inline GCTypeID GetTypeID();

	//////////////////////////////////////////////////////////////////////////
	// CCNode interface...
		virtual bool	init						(); // explicit constructor
		virtual void	onEnter						(); // called by cocos2d when layer is added to the active cocos 2d scenegraph
		virtual void	onEnterTransitionDidFinish	(); // called by cocos2d when the transition to the scene containing this finishes
		virtual void	onExitTransitionDidStart	(); // called by cocos2d when the transition to the scene containing this starts
		virtual void	onExit						(); // called by cocos2d when layer is removed from the active cocos 2d scenegraph

	// CCNode interface...
	//////////////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////////////
	// IGCGameLayer interface 
				
		// create everything: CGCObjectGroups before CGCObjects 
		virtual void			VOnCreate					();
		
		// destroy everything: CGCObjects before CGCObjectGroups
		virtual void			VOnDestroy					();
		
		// allocate dynamic memory and load resources
		virtual void			VOnResourceAcquire			();		
		
		// reset state 
		virtual void			VOnReset					();		
		
		// update
		virtual void			VOnUpdate					( f32 fTimeStep );
		
		// unload resources and free all dynamic memory  
		virtual void			VOnResourceRelease			( void);

	// IGCGameLayer interface 
	//////////////////////////////////////////////////////////////////////////


	//////////////////////////////////////////////////////////////////////////
	// accessor for CGCObjectManager
		 
		CGCObjectManager* ObjectManager();

	// accessor for CGCObjectManager
	//////////////////////////////////////////////////////////////////////////


	//////////////////////////////////////////////////////////////////////////
	// wrapped functions on CCScene to prevent IGCGameLayer getting destroyed 
	// inadvertently when a scene containing one is removed from the top of 
	// CCScene's scene stack by a 'pushScene' operation.

		void PushScene		( cocos2d::Scene* pNewScene );
		void ReplaceScene	( cocos2d::Scene* pNewScene );

	// wrapped functions on CCScene 
	//////////////////////////////////////////////////////////////////////////


	//////////////////////////////////////////////////////////////////////////
	// static interface that allows > 1 of this type to exist and be accessed statically
	private:
		// static function to hide away a static list of IGCGameLayer instances
		// they add themselves to it on construction and remove themselves on deletion
		// only one instance of any specific IGCGameLayer derived type should exist at one time
		inline static TNoAllocList< IGCGameLayer >& GetHiddenGameLayerList();
	public:
		// accessor for IGCGameLayer instances
		// N.B. since this class is derived from CCLayer we can add
		// other CCNode derived types to it just like a normal CCLayer 
		inline static IGCGameLayer* GetGameLayerByID( const GCTypeID kidOfGameLayer );
		
	// static interface that allows > 1 of this type to exist and be accessed statically
	//////////////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////////////
	// "active layer" singleton-esque interface

	// purpose of this is to provide singleton-esque functionality, but allow multiple
	// instances of the class to exist.
	private:
		static IGCGameLayer* sm_ActiveGameLayer;

	protected:
		// sets instance as the active game layer
		inline void SetSelfAsActiveGameLayer();

	public:
		// singleton-esque function that returns the active game layer pointer
		inline static IGCGameLayer* ActiveInstance();

	// "active layer" singleton-esque interface
	//////////////////////////////////////////////////////////////////////////



	//////////////////////////////////////////////////////////////////////////
	// box 2d
	private:
		b2World*	m_pBox2DWorld;				// box 2d world 
		f32			m_fPixelsToMetres;			// constant set when a shape file is loaded
		bool		m_bDebugDrawIsSet;			// set when phsyics debug drawing is enabled

		// create / destroy box 2d world
		void B2dWorldCreate	();
		void B2dWorldDestroy();


	protected:
		virtual void VB2dWorldUpdate();

	public:
		// scale factor from meters to pixels - box2d units are all meters
		#define IGCGAMELAYER_B2D_PIXELS_PER_METER	(20.0f)

		inline b2World*	B2dGetWorld				();
		inline void		B2dLoadShapesFromPlist	( const char* pszPlistFile ); 
		inline bool		B2dIsDebugDrawing		();

		// conversion between world and pixel spaces
		template< typename Ttype > static inline Ttype B2dWorldToPixels( Ttype tArgWorld );
		template< typename Ttype > static inline Ttype B2dPixelsToWorld( Ttype tArgPixels );


	// box 2d
	//////////////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////////////
	// collision manager
	private:
			
		CGCCollisionManager		m_cGCCollisionManager;

	public:
		CGCCollisionManager& GetCollisionManager()
		{
			return m_cGCCollisionManager;
		}

	// collision manager
	//////////////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////////////
	// uber simple touch manager interface
	private:
		cocos2d::Vec2	m_v2RawTouchPos;
		cocos2d::Vec2	m_v2TouchPos;
		cocos2d::Vec2	m_v2LastTouchPos;

		bool	m_bRawTouching;
		bool	m_bTouching;
		bool	m_bTouchingLastFrame;

		void	InitTouchHandler();		
		void	ResetTouchState	();
		void	UpdateTouchState();	

	public:

		// external touch accessors
		inline cocos2d::Vec2	GetTouchPos			();
		inline cocos2d::Vec2	GetLastTouchPos		();
		inline bool				ATouchIsActive		();
		inline bool				TouchWasJustStarted	();
		inline bool				TouchWasJustReleased();

		// touch functions
		void OnTouchesBegan( const std::vector< cocos2d::Touch* >& vecTouches, cocos2d::Event* pEvent );
		void OnTouchesMoved( const std::vector< cocos2d::Touch* >& vecTouches, cocos2d::Event* pEvent );
		void OnTouchesEnded( const std::vector< cocos2d::Touch* >& vecTouches, cocos2d::Event* pEvent );
		// uber simple touch manager interface
		//////////////////////////////////////////////////////////////////////////
};


//////////////////////////////////////////////////////////////////////////
// Templated function to take effort out of correctly creating a scene
// containing an IGCGameLayer derived type.
//
// Use this to create a scene containing an IGCGameLayer derived layer 
// when CCDirector Scene management functions, eg:
//
//		Director::getInstance()->runWithScene( TGCGameLayerSceneCreator<>::CreateScene() );
//		Director::getInstance()->replaceScene( TGCGameLayerSceneCreator<>::CreateScene() );
//
//////////////////////////////////////////////////////////////////////////
template < typename TGameLayerDerived >
class TGCGameLayerSceneCreator
{
private:
	TGCGameLayerSceneCreator	();
	~TGCGameLayerSceneCreator	();

public:
	static cocos2d::Scene* CreateScene()
	{
		STATIC_ASSERT_CONVERSION_VALID( IGCGameLayer, TGameLayerDerived, TGameLayerDerived_must_be_derived_from_IGCGameLayer );

		// CCScene::create returns an autorelease pointer
		cocos2d::Scene* pReturnScene = cocos2d::Scene::create();

		// need to do this long-hand here rather than using the 'create' idiom
		IGCGameLayer* pIGCLayer = new TGameLayerDerived();
		if( pIGCLayer && pIGCLayer->init() )
		{
			pIGCLayer->autorelease();
		}
		else
		{
			CCAssert( false, "Failed to allocate IGCGameLayer, or IGCGameLayer->init() failed" );
		}

		// add IGCGameLayer to the scene
		pReturnScene->addChild( pIGCLayer, -1 );

		return pReturnScene;
	}
};


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// inline functions inline functions inline functions inline functions inline functions inline functions inline functions inline functions
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
// protected accessor for m_bSceneTransitionActive
//////////////////////////////////////////////////////////////////////////
// protected
inline bool IGCGameLayer::ASceneTransitionIsActive()
{
	return m_bSceneTransitionActive;
}


//////////////////////////////////////////////////////////////////////////
// accessor for TypeID - set by deriving classes in constructor
//////////////////////////////////////////////////////////////////////////
inline GCTypeID IGCGameLayer::GetTypeID()
{
	return m_idTypeID;
}


//////////////////////////////////////////////////////////////////////////
// wrapper to enable pushing of other CCScenes over the top of one 
// containing this class without causing any issues
// m_bNewSceneWasPushed is set to true so that IGCGameLayer::onExit()
// doesn't do any unnecessary shutdown and the next call to 
// IGCGameLayer::onAdd() deosn't do any unnecessary startup tasks
////////////////////////////////////////////////////////////////////////// 
inline void IGCGameLayer::PushScene( cocos2d::Scene* pNewScene )
{
	SetSelfAsActiveGameLayer();
	CCAssert(	cocos2d::Director::getInstance()->getRunningScene() == ActiveInstance()->getParent(), 
				"must only call IGCGameLayer::PushScene when a scen containing an IGCGameLayer is currently running" ); 
	m_bOnTopOfSceneStack = false;
	cocos2d::Director::getInstance()->pushScene( pNewScene );
}


//////////////////////////////////////////////////////////////////////////
// for completeness - additional wrapper to replace a scene containing
// this class (no known issues to fix)
////////////////////////////////////////////////////////////////////////// 
inline void IGCGameLayer::ReplaceScene( cocos2d::Scene* pNewScene )
{
	SetSelfAsActiveGameLayer();
	CCAssert(	cocos2d::Director::getInstance()->getRunningScene() == ActiveInstance()->getParent(), 
				"must only call IGCGameLayer::PushScene when a scen containing an IGCGameLayer is currently running" );
	cocos2d::Director::getInstance()->replaceScene( pNewScene );
}


//////////////////////////////////////////////////////////////////////////
// sets instance as the active game layer
//////////////////////////////////////////////////////////////////////////
// protected
inline void IGCGameLayer::SetSelfAsActiveGameLayer()
{
	sm_ActiveGameLayer = this;
}


//////////////////////////////////////////////////////////////////////////
// singleton-esque function that returns the active game layer pointer 
////////////////////////////////////////////////////////////////////////// 
// static 
inline IGCGameLayer* IGCGameLayer::ActiveInstance()
{
	return sm_ActiveGameLayer;
}


//////////////////////////////////////////////////////////////////////////
// static function to hide away a static list of IGCGameLayer instances
// they add themselves to it on construction and remove themselves on deletion
// only one instance of any specific IGCGameLayer derived type should exist at one time
//////////////////////////////////////////////////////////////////////////
// static 
inline TNoAllocList< IGCGameLayer >& IGCGameLayer::GetHiddenGameLayerList()
{
	static TNoAllocList< IGCGameLayer > s_MagicalHiddenList;
	return s_MagicalHiddenList;
}


//////////////////////////////////////////////////////////////////////////
// accessor for IGCGameLayer instances
// N.B. since this class is derived from CCLayer we can add
// other CCNode derived types to it just like a normal CCLayer 
//////////////////////////////////////////////////////////////////////////
// static 
inline IGCGameLayer* IGCGameLayer::GetGameLayerByID( const GCTypeID kidOfGameLayer )
{
	TNoAllocList< IGCGameLayer >& rcHiddenList = GetHiddenGameLayerList();
	for(	IGCGameLayer* pGameLayer = rcHiddenList.GetFirst();
			pGameLayer != 0;
			pGameLayer = rcHiddenList.GetNext( pGameLayer ) )
	{
		if( kidOfGameLayer == pGameLayer->GetTypeID() )
		{
			return pGameLayer;
		}
	}
	return NULL;
}


//////////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////////
inline b2World* IGCGameLayer::B2dGetWorld()
{
	return m_pBox2DWorld;
}


//////////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////////
inline void IGCGameLayer::B2dLoadShapesFromPlist( const char* pszPlistFile )
{
	cocos2d::GB2ShapeCache::sharedGB2ShapeCache()->addShapesWithFile( pszPlistFile );
}


//////////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////////
inline bool IGCGameLayer::B2dIsDebugDrawing()
{
	return m_bDebugDrawIsSet;
}


//////////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////////
// static 
template< typename Ttype > 
inline Ttype IGCGameLayer::B2dWorldToPixels( Ttype tArgWorld )
{
	// n.b. ordering important! this will only work for b2Vec2 if we 
	// put the scale factor as the left hand value
	return ( IGCGAMELAYER_B2D_PIXELS_PER_METER * tArgWorld );
}

//////////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////////
// static 
template< typename Ttype > 
inline Ttype IGCGameLayer::B2dPixelsToWorld( Ttype tArgPixels )
{
	return ((1.0f/IGCGAMELAYER_B2D_PIXELS_PER_METER) * tArgPixels );
}


//////////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////////
inline cocos2d::Vec2 IGCGameLayer::GetTouchPos()
{
	return m_v2TouchPos;
}


//////////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////////
inline cocos2d::Vec2 IGCGameLayer::GetLastTouchPos()
{
	return m_v2LastTouchPos;
}


//////////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////////
inline bool IGCGameLayer::ATouchIsActive()
{
	return m_bTouching;
}


//////////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////////
inline bool IGCGameLayer::TouchWasJustStarted()
{
	return ( m_bTouching && (!m_bTouchingLastFrame) );
}


//////////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////////
inline bool IGCGameLayer::TouchWasJustReleased()
{
	return ( (!m_bTouching)	&& m_bTouchingLastFrame );
}


#endif//#ifndef _IGCGAMELAYER_H_