////////////////////////////////////////////////////////////////////////////////////////////////////////////
// (C) Gamer Camp / Alex Darby 2018
// Distributed under the MIT license - see readme.md
////////////////////////////////////////////////////////////////////////////////////////////////////////////
#include <string>

#include "GCFactory_ObjSpritePhysics.h"

//////////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////////
CGCFactory_ObjSpritePhysics::CGCFactory_ObjSpritePhysics( void )
: m_uNumRegisteredClasses( 0 )
{}


//////////////////////////////////////////////////////////////////////////
// this class is *completely* a singleton all access is via this function
//////////////////////////////////////////////////////////////////////////
// static 																			
CGCFactory_ObjSpritePhysics& CGCFactory_ObjSpritePhysics::GetFactory( void )
{
	static CGCFactory_ObjSpritePhysics s_SingleInstance;
	return s_SingleInstance;
}


//////////////////////////////////////////////////////////////////////////
// register a class for construction
//////////////////////////////////////////////////////////////////////////
void CGCFactory_ObjSpritePhysics::RegisterCreatableClass( const char* pszClassName, pfnFactoryFunction pfnClassFactoryFunction )
{
	CCAssert( m_uNumRegisteredClasses < EMaxRegisteredClasses,	"too many classes registered with the factory!" );
	CCAssert( pszClassName,										"pszClassName is NULL" );
	CCAssert( pfnClassFactoryFunction,							"pfnClassFactoryFunction is NULL" );

	m_asRegisteredClasses[ m_uNumRegisteredClasses ].pszClassName		= pszClassName;
	m_asRegisteredClasses[ m_uNumRegisteredClasses ].pfnClassFactory	= pfnClassFactoryFunction;
	++m_uNumRegisteredClasses;
}


//////////////////////////////////////////////////////////////////////////
// factory function
//////////////////////////////////////////////////////////////////////////
CGCObjSpritePhysics* CGCFactory_ObjSpritePhysics::CreateInstance( const CGCFactoryCreationParams& rCreationParams, cocos2d::Vec2 v2InitialPosition )
{
	CGCObjSpritePhysics*	pReturn			= NULL;

	// find the class creator corresponding to the class name passed and use it to create the instance to return
	for( u32 uLoop = 0; uLoop < m_uNumRegisteredClasses; ++uLoop )
	{
		const SCreatableClass& rCreatableClass = m_asRegisteredClasses[ uLoop ];
		if( 0 == rCreationParams.strClassName.compare( rCreatableClass.pszClassName ) )
		{
			pReturn = rCreatableClass.pfnClassFactory( rCreationParams, v2InitialPosition );
			break;
		}
	}

	return pReturn;
}
