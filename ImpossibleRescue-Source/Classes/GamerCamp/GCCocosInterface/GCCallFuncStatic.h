////////////////////////////////////////////////////////////////////////////////////////////////////////////
// (C) Gamer Camp / Alex Darby 2018
// Distributed under the MIT license - see readme.md
////////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef _CGCallFuncStatic_
#define _CGCallFuncStatic_


/** @brief Calls a 'callback' from a CCSequence without the requirement for it to be a member function of an instance derived from Ref
*/

#ifndef __CCINSTANT_ACTION_H__
#include "../2d/CCActionInstant.h"
#endif

#ifndef __CCPLATFORMDEFINE_H__
#include "../platform/win32/CCPlatformDefine-win32.h"
#endif


typedef void (*SEL_CallFuncStatic)( void* );

#define callfuncStatic_selector(_SELECTOR) (SEL_CallFuncStatic)(&_SELECTOR)


class CGCCallFuncStatic 
: public cocos2d::ActionInstant
{
public:
	CGCCallFuncStatic()
	: m_pCallFunc		(NULL)
	{
	}

	virtual ~CGCCallFuncStatic();

	// creates the action with the callback & data as void*
	static CGCCallFuncStatic* create( SEL_CallFuncStatic pfnStaticFunctionToCall, void* pDataToPassToFunctionWhenCalled );

	/** executes the callback */
	virtual void execute();
	//super methods
	virtual void update(float time);

protected:

	void* m_pDataUsedPassedToCallback;

	SEL_CallFuncStatic	m_pCallFunc;
};

#endif // !_CGCallFuncStatic_