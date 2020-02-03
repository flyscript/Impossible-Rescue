////////////////////////////////////////////////////////////////////////////////////////////////////////////
// (C) Gamer Camp / Alex Darby 2019
// Distributed under the MIT license - see readme.md
////////////////////////////////////////////////////////////////////////////////////////////////////////////
#if defined (WIN32)
#include "GCControllerManager.h"

#include "2d/CCNode.h"
#include "base/CCEventDispatcher.h"
#include "base/CCDirector.h"
#include "base/ccMacros.h"
#include "base/CCEventListenerController.h"
#include "platform/CCGLView.h"


using namespace cocos2d;



//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
CGCControllerManager::CGCControllerManager()
{}


//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
CGCControllerManager::~CGCControllerManager()
{}


//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
void CGCControllerManager::Initialise()
{
	Controller::startDiscoveryController();
}


//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
void CGCControllerManager::Reset()
{
	for( Controller* pcController : Controller::getAllController() )
	{
		pcController->reset();
	}
}


//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
void CGCControllerManager::Update()
{}


//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
bool CGCControllerManager::ControllerIsActive( EControllerId eControllerId )
{
	const Controller* pcController = Controller::getControllerByDeviceId( (int) eControllerId );
	return ( nullptr != pcController );
}


//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
void CGCControllerManager::ControllerReset( EControllerId eControllerId )
{
	Controller* pcController = Controller::getControllerByDeviceId( (int) eControllerId );
	GCASSERT( ( nullptr != pcController ), "Aiieee!! You should check the controller is valid before calling this!" );

	pcController->reset();
}


//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
bool CGCControllerManager::ControllerButtonIsPressed( EControllerId eControllerId, Controller::Key eKeyCode )
{
	Controller* pcController = Controller::getControllerByDeviceId( (int) eControllerId );
	GCASSERT( ( nullptr != pcController ), "Aiieee!! You should check the controller is valid before calling this!" );

	const Controller::KeyStatus& rcKeyStatus = pcController->getKeyStatus( (int) eKeyCode );

	return rcKeyStatus.isPressed;
}


//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
bool CGCControllerManager::ControllerButtonHasJustBeenPressed( EControllerId eControllerId, Controller::Key eKeyCode )
{
	Controller* pcController = Controller::getControllerByDeviceId( (int) eControllerId );
	GCASSERT( ( nullptr != pcController ), "Aiieee!! You should check the controller is valid before calling this!" );

	const Controller::KeyStatus& rcKeyStatus		= pcController->getKeyStatus		( (int) eKeyCode );
	const Controller::KeyStatus& rcLastKeyStatus	= pcController->getLastKeyStatus	( (int) eKeyCode );

	return ( ( ! rcLastKeyStatus.isPressed ) && rcKeyStatus.isPressed );
}


//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
bool CGCControllerManager::ControllerButtonHasJustBeenReleased( EControllerId eControllerId, Controller::Key eKeyCode )
{
	Controller* pcController = Controller::getControllerByDeviceId( (int) eControllerId );
	GCASSERT( ( nullptr != pcController ), "Aiieee!! You should check the controller is valid before calling this!" );

	const Controller::KeyStatus& rcKeyStatus		= pcController->getKeyStatus		( (int) eKeyCode );
	const Controller::KeyStatus& rcLastKeyStatus	= pcController->getLastKeyStatus	( (int) eKeyCode );

	return ( rcLastKeyStatus.isPressed && ( ! rcKeyStatus.isPressed ) );
}


//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
f32 CGCControllerManager::ControllerGetCurrentAxisValueRaw( EControllerId eControllerId, Controller::Key eAxisKeyCode )
{
	Controller* pcController = Controller::getControllerByDeviceId( (int) eControllerId );
	GCASSERT( ( nullptr != pcController ), "Aiieee!! You should check the controller is valid before calling this!" );

	const Controller::KeyStatus& rcKeyStatus = pcController->getKeyStatus( (int) eAxisKeyCode );

	if( rcKeyStatus.isAnalog )
	{
		return rcKeyStatus.value;
	}

	return 0.0f;
}


//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
f32 CGCControllerManager::ControllerGetCurrentAxisDeadzoned( EControllerId eControllerId, Controller::Key eAxisKeyCode, float fDeadzone )
{
	f32 fRawAxisValue	= ControllerGetCurrentAxisValueRaw( eControllerId, eAxisKeyCode );
	fRawAxisValue		= GCMaths::Clamp( fRawAxisValue, ( -k_fAxisMax ), k_fAxisMax );

	f32 fSignCorrecter	= ( ( fRawAxisValue < 0.0f ) ? -1.0f : 1.0f );
	f32 fAbsAxisValue	= std::min( std::fabsf( fRawAxisValue ),	k_fAxisMax );		// these limits prevent div0 later
	f32 fAbsDeadzone	= std::min( std::fabsf( fDeadzone ),		k_fDeadzoneMax );	// these limits prevent div0 later

	f32 fAvailableRange			= ( k_fAxisMax - fAbsDeadzone );
	f32 fAbsRawWRTDeadzone		= std::max( ( fAbsAxisValue - fAbsDeadzone ), 0.0f );
	f32 fAbsRawAsPropOfAvailble = ( fAbsRawWRTDeadzone / fAvailableRange );

	f32 fFinalAxisValue = ( fAbsRawAsPropOfAvailble * fSignCorrecter );

	return fFinalAxisValue;
}


//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
Vec2 CGCControllerManager::ControllerGetCurrentStickValueRaw( EControllerId eControllerId, Controller::Key eAxisKeyCodeX, Controller::Key eAxisKeyCodeY )
{
	return ControllerGetCurrentStickValueDeadzoned( eControllerId, eAxisKeyCodeX, eAxisKeyCodeY, 0.0f );
}


//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
Vec2 CGCControllerManager::ControllerGetCurrentStickValueDeadzoned( EControllerId eControllerId, Controller::Key eAxisKeyCodeX, Controller::Key eAxisKeyCodeY, float fDeadzone )
{
	f32 fRawAxisX = ControllerGetCurrentAxisValueRaw( eControllerId, eAxisKeyCodeX );
	f32 fRawAxisY = ControllerGetCurrentAxisValueRaw( eControllerId, eAxisKeyCodeY );

	fRawAxisX = GCMaths::Clamp( fRawAxisX, ( -k_fAxisMax ), k_fAxisMax );
	fRawAxisY = GCMaths::Clamp( fRawAxisY, ( -k_fAxisMax ), k_fAxisMax );

	// get magnitude & normalise
	Vec2	v2Raw			= Vec2( fRawAxisX, fRawAxisY );
	f32		fMagnitude		= v2Raw.length();	
	
	// early out if very small
	if( fMagnitude < 0.0001f )
	{
		return Vec2::ZERO;
	}

	Vec2	v2Normalised	= ( v2Raw / fMagnitude );


	// clamp and deadzone magnitude
	f32		fValidatedDeadzone			= std::min( std::fabsf( fDeadzone ), k_fDeadzoneMax );	
	f32		fMaxMagnitudeWRTDeadzone	= ( k_fAxisMax - fValidatedDeadzone );

	f32		fMagnitudeClamped			= std::min( fMagnitude, k_fAxisMax );
	f32		fMagnitudeWRTDeadzone		= std::max( ( fMagnitudeClamped - fValidatedDeadzone ), 0.0f );
	
	f32		fDeadzonedMagnitudeAsPropOfAvailableRange	= ( fMagnitudeWRTDeadzone / fMaxMagnitudeWRTDeadzone );
	Vec2	v2ClampedToMaxAndDeadZoned					= ( v2Normalised * fDeadzonedMagnitudeAsPropOfAvailableRange );

	return v2ClampedToMaxAndDeadZoned;
}


#endif //#if defined (WIN32)
