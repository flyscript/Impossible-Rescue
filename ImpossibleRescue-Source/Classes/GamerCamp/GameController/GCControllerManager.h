////////////////////////////////////////////////////////////////////////////////////////////////////////////
// (C) Gamer Camp / Alex Darby 2019
// Distributed under the MIT license - see readme.md
////////////////////////////////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
#ifndef _GCINPUTCONTROLLERMANAGER_H_
#define _GCINPUTCONTROLLERMANAGER_H_

#include "GamerCamp/Core/GCTypes.h"

#include "platform/CCGLView.h"
#include "cocos2d/external/glfw3/include/win32/glfw3.h"
#include "cocos/base/CCController.h"


//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//#error read comment below!!
//					PLAN
// add action maps to CGCController 
// 
// 
//		
#if defined (WIN32)

using namespace cocos2d;


//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
class CGCControllerManager
{
public:
	const f32 k_fAxisMax		= 1.0f;
	const f32 k_fDeadzoneMax	= 0.3333f;

	enum EControllerId
	{
		eControllerOne		= GLFW_JOYSTICK_1, // the values are actually zero indexed but they could in principle change...
		eControllerTwo		= GLFW_JOYSTICK_2,
		eControllerThree	= GLFW_JOYSTICK_3,
		eControllerFour		= GLFW_JOYSTICK_4,
	};

						CGCControllerManager					();
						~CGCControllerManager					();

	void				Initialise								();
	void				Reset									();
	void				Update									();

	bool				ControllerIsActive						( EControllerId eControllerId );

	void				ControllerReset							( EControllerId eControllerId );

	bool				ControllerButtonIsPressed				( EControllerId eControllerId, Controller::Key eKeyCode );

	bool				ControllerButtonHasJustBeenPressed		( EControllerId eControllerId, Controller::Key eKeyCode );
	bool				ControllerButtonHasJustBeenReleased		( EControllerId eControllerId, Controller::Key eKeyCode );

	f32					ControllerGetCurrentAxisValueRaw		( EControllerId eControllerId, Controller::Key eAxisKeyCode );
	f32					ControllerGetCurrentAxisDeadzoned		( EControllerId eControllerId, Controller::Key eAxisKeyCode, float fDeadzone );

	cocos2d::Vec2		ControllerGetCurrentStickValueRaw		( EControllerId eControllerId, Controller::Key eAxisKeyCodeX, Controller::Key eAxisKeyCodeY );
	cocos2d::Vec2		ControllerGetCurrentStickValueDeadzoned	( EControllerId eControllerId, Controller::Key eAxisKeyCodeX, Controller::Key eAxisKeyCodeY, float fDeadzone );
};



#endif // #if defined (WIN32)
#endif // #ifndef _GCINPUTCONTROLLERMANAGER_H_