////////////////////////////////////////////////////////////////////////////////////////////////////////////
// (C) Gamer Camp / Alex Darby 2018
// Distributed under the MIT license - see readme.md
////////////////////////////////////////////////////////////////////////////////////////////////////////////
#include "IGCGameLayer.h"

#ifndef _APP_DELEGATE_H_
	#include "AppDelegate.h"
#endif

#ifndef	_GCOBJECTGROUP_H_
	#include "../GCObject/GCObjectGroup.h"
#endif

#ifndef	_GCOBJECTMANAGER_H_
	#include "../GCObject/GCObjectManager.h"
#endif

#ifndef	_GCOBJECTGROUPDEFAULT_H_
	#include "../GCObject/GCObjGroupDefault.h"
#endif

#ifndef _GCOBJSPRITE_H_
	#include "GCObjSprite.h"
#endif

#ifndef _GCOBJSPRITEPHYSICS_H_
	#include "GCObjSpritePhysics.h"
#endif

#ifndef __cocos2d_libs__CCTouchEventListener__
	#include "base/CCEventListenerTouch.h"
#endif

#include "GamerCamp/Debug/GLES-Render.h"


//////////////////////////////////////////////////////////////////////////
// using...
using namespace cocos2d;


//////////////////////////////////////////////////////////////////////////
// init static active layer
IGCGameLayer* IGCGameLayer::sm_ActiveGameLayer = NULL;


//////////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////////
IGCGameLayer::IGCGameLayer( GCTypeID idIGCGameLayerDerived )
: m_idTypeID				( idIGCGameLayerDerived )
, m_bOnTopOfSceneStack		( true )
, m_bSceneTransitionActive	( true )
, m_pcObjectManager			( NULL )
, m_pcGCObjGroupDefault		( NULL )
, m_bDebugDrawIsSet			( false )
, m_v2RawTouchPos			( 0.0f, 0.0f )
, m_v2TouchPos				( 0.0f, 0.0f )
, m_v2LastTouchPos			( 0.0f, 0.0f )
, m_bRawTouching			( false )
, m_bTouching				( false )
, m_bTouchingLastFrame		( false )
{
	// add self to static list of IGCGameLayer instances
	GetHiddenGameLayerList().InsertBack( this );

	schedule( schedule_selector( IGCGameLayer::InternalUpdate ) );
}


// virtual
IGCGameLayer::~IGCGameLayer()
{
	// clean default object group
	m_pcObjectManager->ObjectGroupUnRegister( m_pcGCObjGroupDefault );
	delete m_pcGCObjGroupDefault;
	m_pcGCObjGroupDefault = NULL;

	// and object manager
	delete m_pcObjectManager;
	m_pcObjectManager = NULL;

	// destroy b2dWorld
	B2dWorldDestroy();

	// remove self from	hidden IGCGameLayer instance list
	GetHiddenGameLayerList().Remove( this );

	// if this is the active layer set active layer to NULL
	if( sm_ActiveGameLayer == this )
	{
		sm_ActiveGameLayer = NULL;
	}
}


//////////////////////////////////////////////////////////////////////////
// CCNode interface...

	//////////////////////////////////////////////////////////////////////////
	// explicit constructor
	//virtual 
	bool IGCGameLayer::init()
	{
		// ensure base class behaviour works
		if( !CCLayer::init() )
		{
			return false;
		}		

		// create b2d world
		B2dWorldCreate();

		// create the CGCObjectManager
		m_pcObjectManager = new CGCObjectManager();

		// set self as active game layer and managed game manager as active game manager
		SetSelfAsActiveGameLayer();
		m_pcObjectManager->SetAsActiveObjectManager();

		InitTouchHandler();

		// create and register a default object group
		m_pcGCObjGroupDefault = new CGCObjGroupDefault();
		CGCObjectManager::ObjectGroupRegister( m_pcGCObjGroupDefault );

		// create everything in the game
		VOnCreate();

		// initialise everything in the game
		VOnResourceAcquire();

		return true;
	}



	//////////////////////////////////////////////////////////////////////////
	// called by cocos2d when layer is added to the active cocos 2d scenegraph
	//////////////////////////////////////////////////////////////////////////
	//virtual 
	void IGCGameLayer::onEnter()
	{
		// ensure base class behaviour works
		CCLayer::onEnter();

		// this bool is set to true by default,it gets set to false 
		// when IGCGameLayer::PushScene() is called.
		// this is a work-around for an issue that can't otherwise be fixed
		// without changing cocos 2d source
		if( m_bOnTopOfSceneStack )
		{
			// set self as active game layer and managed game manager as active game manager
			SetSelfAsActiveGameLayer();
			m_pcObjectManager->SetAsActiveObjectManager();

			// force an initial reset of everything
			VOnReset();
		}

		// a call to this function indicates that we are back on top of 
		// the scene stack, so set m_bOnTopOfSceneStack to true
		m_bOnTopOfSceneStack = true;

		// scene transition may be active until onEnterTransitionDidFinish() is called
		m_bSceneTransitionActive = true;
	}
								    

	//////////////////////////////////////////////////////////////////////////
	// called by cocos2d when the transition to the scene containing this finishes
	//////////////////////////////////////////////////////////////////////////
	//virtual 
	void IGCGameLayer::onEnterTransitionDidFinish()
	{
		CCLayer::onEnterTransitionDidFinish();
		// scene transition definitely not active
		m_bSceneTransitionActive = false;
	}


	//////////////////////////////////////////////////////////////////////////
	// called by cocos2d when the transition to the scene containing this starts
	//////////////////////////////////////////////////////////////////////////
	//virtual 
	void IGCGameLayer::onExitTransitionDidStart()
	{
		// from this point, a scene transition may be active until we get a call to onExit() 
		m_bSceneTransitionActive = true;
		CCLayer::onExitTransitionDidStart();
	}
	
		
	//////////////////////////////////////////////////////////////////////////
	// called by cocos2d when layer is removed from the active cocos 2d scenegraph
	//////////////////////////////////////////////////////////////////////////
	//virtual 
	void IGCGameLayer::onExit()
	{
		// scene transition definitely no longer active
		m_bSceneTransitionActive = false;

		// this bool is set to true by default,it gets set to false 
		// when IGCGameLayer::PushScene() is called.
		// this is a work-around for an issue that can't otherwise be fixed
		// without changing cocos 2d source
		if( m_bOnTopOfSceneStack )
		{
			// set self as active game layer and managed game manager as active game manager
			SetSelfAsActiveGameLayer();
			m_pcObjectManager->SetAsActiveObjectManager();

			// N.B. can't call these in the destructor because they're virtual
			// shut everything down
			VOnResourceRelease();

			// destroy everything
			VOnDestroy();
		}

		// ensure base class behaviour works
		CCLayer::onExit();
	}
// CCNode interface...
//////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////
// update function, called via the CCNode 'schedule' interface
void IGCGameLayer::InternalUpdate( f32 fTimeStep )
{
	// set self as active game layer and managed game manager as active game manager
	SetSelfAsActiveGameLayer();
	m_pcObjectManager->SetAsActiveObjectManager();

	// only update when not in a scene transition
	if( !ASceneTransitionIsActive() )
	{
		VOnUpdate( fTimeStep );
	}
}


//////////////////////////////////////////////////////////////////////////
// create all CGCObjectGroups and then all CGCObjects
//
// N.B. this base class version creates the default object group
// N.N.B. if you override this fn. and want to rely on its behaviour call 
// it before your code
//virtual 
void IGCGameLayer::VOnCreate()
{
}


//////////////////////////////////////////////////////////////////////////
// destroy all CGCObjects and then all CGCObjectGroups
//
// N.B. this base class version destroys the default object group 
// N.N.B. if you override this fn. and want to rely on its behaviour call 
// it after your code
//virtual 
void IGCGameLayer::VOnDestroy()
{
}


//////////////////////////////////////////////////////////////////////////
// allocate dynamic memory and load resources
//virtual 
void IGCGameLayer::VOnResourceAcquire()
{
	m_pcObjectManager->OnResourceAcquire();
}


//////////////////////////////////////////////////////////////////////////
// reset state 
//virtual 
void IGCGameLayer::VOnReset()
{
	ResetTouchState();
	AppDelegate::GetKeyboardManager()->Reset();
	AppDelegate::GetControllerManager()->Reset();
	CGCObjectManager::OnReset();
}


//////////////////////////////////////////////////////////////////////////
// update
//virtual 
void IGCGameLayer::VOnUpdate( f32 fTimeStep )
{
	AppDelegate::GetKeyboardManager()->Update();
	UpdateTouchState();
	VB2dWorldUpdate();
	GCASSERT( nullptr != B2dGetWorld(), "b2d physics world is null" );
	m_cGCCollisionManager.HandleCollisions( *B2dGetWorld() );
	CGCObjectManager::OnUpdate( fTimeStep );
}


//////////////////////////////////////////////////////////////////////////
// unload resources and free all dynamic memory  
//virtual 
void IGCGameLayer::VOnResourceRelease( void)
{
	CGCObjectManager::OnResourceRelease();

	// DON'T DO THIS!!
	// Will break when you use a transition with ReplaceScene()
	//GB2ShapeCache::sharedGB2ShapeCache()->reset();
}


//////////////////////////////////////////////////////////////////////////
// box 2d

//////////////////////////////////////////////////////////////////////////
// constants
const b2Vec2	k_v2B2dGravity( 0.0f, -50.0f );	

//////////////////////////////////////////////////////////////////////////
// comment this IN to draw debug physics shapes
//#define GC_DRAW_DEBUG_PHYSICS

#if defined( GC_DRAW_DEBUG_PHYSICS )
	// special layer class just for rendering the physics stuff
	class _GCGameLayerPhysicsDebugRenderer 
		: public Layer
	{
		GLESDebugDraw*	m_b2dDebugDrawer; 
		b2World*		m_pWorldToRender;

	public:
		void InitialiseDebugDrawing( b2World* pWorldToRender )
		{
			m_pWorldToRender = pWorldToRender;

			uint32 uFlags = 0;
			uFlags += b2Draw::e_shapeBit;
			//uFlags += b2Draw::e_jointBit;
			//uFlags += b2Draw::e_aabbBit;
			//uFlags += b2Draw::e_pairBit;
			//uFlags += b2Draw::e_centerOfMassBit;

			m_b2dDebugDrawer = new GLESDebugDraw( 20 ); // pixels to meters
			m_b2dDebugDrawer->SetFlags( uFlags );

			m_pWorldToRender->SetDebugDraw( m_b2dDebugDrawer );
		}

		// used to override rendering and to draw debug physics shapes
		virtual void draw( cocos2d::Renderer* pRenderer, const cocos2d::Mat4& rmatTransform, uint32_t uFlags )
		{
			cocos2d::Layer::draw( pRenderer, rmatTransform, uFlags );

			GL::enableVertexAttribs( GL::VERTEX_ATTRIB_FLAG_POSITION );
			Director::getInstance()->pushMatrix( MATRIX_STACK_TYPE::MATRIX_STACK_MODELVIEW );
			m_pWorldToRender->DrawDebugData();
			Director::getInstance()->popMatrix( MATRIX_STACK_TYPE::MATRIX_STACK_MODELVIEW );
		}

		// called by cocos2d when layer is removed from the active cocos 2d scenegraph
		virtual void onExit	()
		{
			delete m_b2dDebugDrawer;
		}
	};
#endif


//////////////////////////////////////////////////////////////////////////
// creates box 2d world
//////////////////////////////////////////////////////////////////////////
void IGCGameLayer::B2dWorldCreate()
{
	m_pBox2DWorld = new b2World( k_v2B2dGravity );
	m_pBox2DWorld->SetContinuousPhysics( true );

	#if defined( GC_DRAW_DEBUG_PHYSICS )
		
		m_bDebugDrawIsSet = true;

		_GCGameLayerPhysicsDebugRenderer* pDebugLayer = new _GCGameLayerPhysicsDebugRenderer();
		if( pDebugLayer && pDebugLayer->init() )
		{
			pDebugLayer->InitialiseDebugDrawing( m_pBox2DWorld );
			this->addChild( pDebugLayer );
		}	

	#endif
}


//////////////////////////////////////////////////////////////////////////
// destroys box 2d world
//////////////////////////////////////////////////////////////////////////
void IGCGameLayer::B2dWorldDestroy()
{
	delete m_pBox2DWorld;
	m_pBox2DWorld = NULL;
}


//////////////////////////////////////////////////////////////////////////
// updates box 2d world
f32 k_fTimeStep				= 1.0f / 60.f;
i32 k_iVelocityIterations	= 8;
i32 k_iPositionIterations	= 3;
//////////////////////////////////////////////////////////////////////////
//virtual 
void IGCGameLayer::VB2dWorldUpdate()
{
	// step the world
	m_pBox2DWorld->Step( k_fTimeStep, k_iVelocityIterations, k_iPositionIterations );
	m_pBox2DWorld->ClearForces();

	// update graphical positions based on body movement
	for(	const b2Body* pB2Body	= m_pBox2DWorld->GetBodyList(); 
			NULL					!= pB2Body; 
			pB2Body					= pB2Body->GetNext() )
	{
		CGCObjSpritePhysics* pcObjPhys = (CGCObjSpritePhysics*)pB2Body->GetUserData();
		// if( this is an alex object )
		if (pcObjPhys != NULL)
		{
			pcObjPhys->VUpdateSpriteFromBody( pB2Body );
		}
	}
}
// box 2d
//////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////////
// private
void IGCGameLayer::InitTouchHandler()
{
	EventListenerTouchAllAtOnce* pTouchEventListener = cocos2d::EventListenerTouchAllAtOnce::create();

	// all of these are proxied via a lambda as the callback type doesn't expect a this pointer
	pTouchEventListener->onTouchesBegan = [this](const std::vector< cocos2d::Touch* >& vecTouches, cocos2d::Event* pEvent)
	{ 
		OnTouchesBegan( vecTouches, pEvent ); 
	};
	pTouchEventListener->onTouchesMoved = [this](const std::vector< cocos2d::Touch* >& vecTouches, cocos2d::Event* pEvent)
	{ 
		OnTouchesMoved( vecTouches, pEvent ); 
	};
	pTouchEventListener->onTouchesEnded = [this](const std::vector< cocos2d::Touch* >& vecTouches, cocos2d::Event* pEvent)
	{ 
		OnTouchesEnded( vecTouches, pEvent ); 
	};

	getEventDispatcher()->addEventListenerWithFixedPriority( pTouchEventListener, 1 );
}

//////////////////////////////////////////////////////////////////////////
// 
//////////////////////////////////////////////////////////////////////////
void IGCGameLayer::ResetTouchState()
{
	m_v2RawTouchPos		= 
	m_v2TouchPos		=
	m_v2LastTouchPos	= Vec2( 0.0f, 0.0f );
	m_bRawTouching		=
	m_bTouching			=
	m_bTouchingLastFrame= false;
}


//////////////////////////////////////////////////////////////////////////
// N.B. always set current touch to 0.0 ifno current touch
//////////////////////////////////////////////////////////////////////////
// private
void IGCGameLayer::UpdateTouchState()
{
	m_bTouchingLastFrame	= m_bTouching;
	m_bTouching				= m_bRawTouching;

	m_v2LastTouchPos		= m_v2TouchPos;
	m_v2TouchPos			= ( m_bTouching ) ? m_v2RawTouchPos : Vec2( 0.0f, 0.0f );
}


//////////////////////////////////////////////////////////////////////////
// uber simple touch manager interface
// called when one or more touches start
//////////////////////////////////////////////////////////////////////////
void IGCGameLayer::OnTouchesBegan( const std::vector< cocos2d::Touch* >& vecTouches, cocos2d::Event* pEvent )
{
	m_bRawTouching = true;

	cocos2d::Touch* pFirstTouch = ( vecTouches.size() > 0 ) ? vecTouches[ 0 ] : nullptr;
	if( pFirstTouch )
	{
		cocos2d::Vec2 touchLocation = pFirstTouch->getLocationInView();
		touchLocation = cocos2d::Director::getInstance()->convertToGL(touchLocation);

		m_v2RawTouchPos.x = touchLocation.x;
		m_v2RawTouchPos.y = touchLocation.y;
	}
}


//////////////////////////////////////////////////////////////////////////
// uber simple touch manager interface
// called when one or more touches moves
//////////////////////////////////////////////////////////////////////////
//virtual 
void IGCGameLayer::OnTouchesMoved( const std::vector< cocos2d::Touch* >& vecTouches, cocos2d::Event* pEvent )
{
	cocos2d::Touch* pFirstTouch = ( vecTouches.size() > 0 ) ? vecTouches[ 0 ] : nullptr;
	if( pFirstTouch )
	{
		cocos2d::Vec2 touchLocation = pFirstTouch->getLocationInView();
		touchLocation = cocos2d::Director::getInstance()->convertToGL(touchLocation);

		m_v2RawTouchPos.x = touchLocation.x;
		m_v2RawTouchPos.y = touchLocation.y;

		//CCLOG( "%f,%f", m_v2RawTouchPos.x, m_v2RawTouchPos.y );
	}
}


//////////////////////////////////////////////////////////////////////////
// uber simple touch manager interface
// called when one or more touches stops
//////////////////////////////////////////////////////////////////////////
//virtual 
void IGCGameLayer::OnTouchesEnded( const std::vector< cocos2d::Touch* >& vecTouches, cocos2d::Event* pEvent )
{
	cocos2d::Touch* pFirstTouch = ( vecTouches.size() > 0 ) ? vecTouches[ 0 ] : nullptr;
	if( pFirstTouch )
	{
		cocos2d::Vec2 touchLocation = pFirstTouch->getLocationInView();
		touchLocation = cocos2d::Director::getInstance()->convertToGL(touchLocation);

		m_v2RawTouchPos.x = touchLocation.x;
		m_v2RawTouchPos.y = touchLocation.y;
	}

	if( vecTouches.size() == 1 )
	{
		m_bRawTouching = false;
	}
}
// uber simple touch manager interface
//////////////////////////////////////////////////////////////////////////
