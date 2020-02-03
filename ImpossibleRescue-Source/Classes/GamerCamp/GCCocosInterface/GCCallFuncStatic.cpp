/****************************************************************************
 Copyright (c) 2013 Gamer Camp
 ****************************************************************************/

#ifndef _CGCallFuncStatic_
#include "GCCallFuncStatic.h"
#endif

#ifndef __SCRIPT_SUPPORT_H__
#include "../cocos2dx/script_support/CCScriptSupport.h"
#endif

#ifndef __CC_ZONE_H__
#include "../deprecated/CCDeprecated.h"
#endif


CGCCallFuncStatic* CGCCallFuncStatic::create( SEL_CallFuncStatic pfnStaticFunctionToCall, void* pDataToPassToFunctionWhenCalled ) 
{
	CGCCallFuncStatic *pRet = new CGCCallFuncStatic();

	pRet->m_pDataUsedPassedToCallback	= pDataToPassToFunctionWhenCalled;
	pRet->m_pCallFunc					= pfnStaticFunctionToCall;
	pRet->autorelease();
	return pRet;
	
}

CGCCallFuncStatic::~CGCCallFuncStatic(void)
{
}


void CGCCallFuncStatic::update(float time)
{
	CC_UNUSED_PARAM(time);
	this->execute();
}

void CGCCallFuncStatic::execute() 
{
	if( m_pCallFunc ) 
	{
		m_pCallFunc( m_pDataUsedPassedToCallback );
	}
}
