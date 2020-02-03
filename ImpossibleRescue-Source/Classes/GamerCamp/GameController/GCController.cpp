////////////////////////////////////////////////////////////////////////////////////////////////////////////
// (C) Gamer Camp / Alex Darby 2019
// Distributed under the MIT license - see readme.md
////////////////////////////////////////////////////////////////////////////////////////////////////////////
#if defined (WIN32)

#ifndef _APP_DELEGATE_H_
	#include "Classes/AppDelegate.h"
#endif

#include "GCController.h"

#include "2d/CCNode.h"
#include "base/CCEventDispatcher.h"
#include "base/CCDirector.h"
#include "base/ccMacros.h"
#include "base/CCEventListenerController.h"
#include "platform/CCGLView.h"


using namespace cocos2d;



//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
CGCController::CGCController( CGCControllerManager::EControllerId eControllerId )
: m_eControllerId ( eControllerId )
{}


//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
CGCController::~CGCController()
{}


//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
bool CGCController::IsActive()
{
	return AppDelegate::GetControllerManager()->ControllerIsActive( m_eControllerId );
}


//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
void CGCController::Reset()
{
	CGCControllerManager* pcConMan = AppDelegate::GetControllerManager();
	GCASSERT( pcConMan->ControllerIsActive( m_eControllerId ), "controller is inactive!!" );
	pcConMan->ControllerReset( m_eControllerId );
}


//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
bool CGCController::ButtonIsPressed( Controller::Key eKeyCode )
{
	CGCControllerManager* pcConMan = AppDelegate::GetControllerManager();
	GCASSERT( pcConMan->ControllerIsActive( m_eControllerId ), "controller is inactive!!" );
	return pcConMan->ControllerButtonIsPressed( m_eControllerId, eKeyCode );
}


//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
bool CGCController::ButtonHasJustBeenPressed( Controller::Key eKeyCode )
{
	CGCControllerManager* pcConMan = AppDelegate::GetControllerManager();
	GCASSERT( pcConMan->ControllerIsActive( m_eControllerId ), "controller is inactive!!" );
	return pcConMan->ControllerButtonHasJustBeenPressed( m_eControllerId, eKeyCode );
}


//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
bool CGCController::ButtonHasJustBeenReleased( Controller::Key eKeyCode )
{
	CGCControllerManager* pcConMan = AppDelegate::GetControllerManager();
	GCASSERT( pcConMan->ControllerIsActive( m_eControllerId ), "controller is inactive!!" );
	return pcConMan->ControllerButtonHasJustBeenReleased( m_eControllerId, eKeyCode );
}


//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
f32 CGCController::GetCurrentAxisValueRaw( Controller::Key eAxisKeyCode )
{
	CGCControllerManager* pcConMan = AppDelegate::GetControllerManager();
	GCASSERT( pcConMan->ControllerIsActive( m_eControllerId ), "controller is inactive!!" );
	return pcConMan->ControllerGetCurrentAxisValueRaw( m_eControllerId, eAxisKeyCode );
}


//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
f32 CGCController::GetCurrentAxisDeadzoned( Controller::Key eAxisKeyCode, float fDeadzone )
{
	CGCControllerManager* pcConMan = AppDelegate::GetControllerManager();
	GCASSERT( pcConMan->ControllerIsActive( m_eControllerId ), "controller is inactive!!" );
	return pcConMan->ControllerGetCurrentAxisDeadzoned( m_eControllerId, eAxisKeyCode, fDeadzone );
}


//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
Vec2 CGCController::GetCurrentStickValueRaw( Controller::Key eAxisKeyCodeX, Controller::Key eAxisKeyCodeY )
{
	CGCControllerManager* pcConMan = AppDelegate::GetControllerManager();
	GCASSERT( pcConMan->ControllerIsActive( m_eControllerId ), "controller is inactive!!" );
	return pcConMan->ControllerGetCurrentStickValueRaw( m_eControllerId, eAxisKeyCodeX, eAxisKeyCodeY );
}


//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
Vec2 CGCController::GetCurrentStickValueDeadzoned( Controller::Key eAxisKeyCodeX, Controller::Key eAxisKeyCodeY, float fDeadzone )
{
	CGCControllerManager* pcConMan = AppDelegate::GetControllerManager();
	GCASSERT( pcConMan->ControllerIsActive( m_eControllerId ), "controller is inactive!!" );
	return pcConMan->ControllerGetCurrentStickValueDeadzoned( m_eControllerId, eAxisKeyCodeX, eAxisKeyCodeY, fDeadzone );
}


#endif //#if defined (WIN32)
