////////////////////////////////////////////////////////////////////////////////////////////////////////////
// (C) Gamer Camp / Alex Darby 2018
// Distributed under the MIT license - see readme.md
////////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef _GCFACTORY_OBJSPRITEPHYSICS_H_
#define _GCFACTORY_OBJSPRITEPHYSICS_H_

#ifndef _GCTYPES_H_
	#include "../Core/GCTypes.h"
#endif

#ifndef BOX2D_H
	#include "Box2D/Box2D.h"
#endif

#ifndef _GCOBJSPRITE_H_
	#include "GCObjSpritePhysics.h"
#endif

#ifndef _SGCFACTORYCREATIONPARAMS_H_
	#include "SGCFactoryCreationParams.h"
#endif

//////////////////////////////////////////////////////////////////////////
// proof of concept factory class to simplify code that ties the game into
// the editor and maximise the extendibility of it
//
//////////////////////////////////////////////////////////////////////////
class CGCFactory_ObjSpritePhysics
{
public:
	// enum for now, this may need extending later
	enum
	{
		EMaxRegisteredClasses = 64
	};

private:
	// typedef for the function pointer signature required to use the factory
	typedef CGCObjSpritePhysics* (*pfnFactoryFunction)( const CGCFactoryCreationParams& rCreateParams, cocos2d::Vec2 v2InitialPosition );

	// struct used to store the data needed for the factory
	struct SCreatableClass
	{
		const char*			pszClassName;		// name of the class
		pfnFactoryFunction	pfnClassFactory;	// factory function to use to create it
	};


	// member variables
	SCreatableClass m_asRegisteredClasses[ EMaxRegisteredClasses ];
	u32				m_uNumRegisteredClasses;


	// private constructor - see GetFactory()
	CGCFactory_ObjSpritePhysics( void );

public:

	// this class is *completely* a singleton all access is via this function
	static CGCFactory_ObjSpritePhysics& GetFactory( void );
																										

	// register a class for construction
	void RegisterCreatableClass( const char* pszClassName, pfnFactoryFunction pfnClassFactoryFunction );


	// factory function
	CGCObjSpritePhysics* CreateInstance( const CGCFactoryCreationParams& rCreationParams, cocos2d::Vec2 v2InitialPosition );
};


//////////////////////////////////////////////////////////////////////////
// macros

// must be put in a public: section of your class declaration in the classes .h file
#define GCFACTORY_DECLARE_CREATABLECLASS( ClassName	) \
	static CGCObjSpritePhysics* GCFactoryFunction_##ClassName( const CGCFactoryCreationParams& rCreationParams, cocos2d::Vec2 v2InitialPosition )


// must be put in the corresponding .cpp file to that where GCFACTORY_DECLARE_CREATABLECLASS() was used
#define GCFACTORY_IMPLEMENT_CREATEABLECLASS( ClassName ) \
	CGCObjSpritePhysics* ClassName::GCFactoryFunction_##ClassName( const CGCFactoryCreationParams& rCreationParams, cocos2d::Vec2 v2InitialPosition )\
	{\
		ClassName* pReturn = new ClassName();\
		pReturn->VHandleFactoryParams( rCreationParams, v2InitialPosition );\
		return static_cast< CGCObjSpritePhysics* >( pReturn );\
	}\
	\
	struct SFactoryFunctionRegistraar_##ClassName\
	{\
		SFactoryFunctionRegistraar_##ClassName( void )\
		{\
			CGCFactory_ObjSpritePhysics::GetFactory().RegisterCreatableClass( #ClassName, ClassName::GCFactoryFunction_##ClassName );\
		}\
	};\
	static SFactoryFunctionRegistraar_##ClassName g_GlobalFactoryFunctionRegistraar_##ClassName


#endif//#ifndef _IGCGAMELAYER_H_