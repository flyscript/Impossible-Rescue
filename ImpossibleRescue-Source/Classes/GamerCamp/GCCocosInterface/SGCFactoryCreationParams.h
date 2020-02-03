////////////////////////////////////////////////////////////////////////////////////////////////////////////
// (C) Gamer Camp / Alex Darby 2018
// Distributed under the MIT license - see readme.md
////////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef _SGCFACTORYCREATIONPARAMS_H_
#define _SGCFACTORYCREATIONPARAMS_H_

#include <string>

#ifndef _GCTYPES_H_
	#include "../Core/GCTypes.h"
#endif

#ifndef BOX2D_H
	#include "Box2D/Box2D.h"
#endif


//////////////////////////////////////////////////////////////////////////
// class used to initialise the data needed by a GCObjSpritePhysics
// to acquire its resources - it *is* technically possible to initialise 
// instances derived from GCObjSpritePhysics manually, however please use
// this method as it will make your life *much* easier in module 2 for 
// reasons which will become apparent in mopdule 2 :)
//////////////////////////////////////////////////////////////////////////
class CGCFactoryCreationParams
{
public:
	std::string strClassName;			// name of the CGCFactory_ObjSpritePhysics derived class to create
	std::string strPlistFile;			// path of the .plist file needed to load the object's sprite
	std::string strPhysicsShape;		// name of the physics shape used
	b2BodyType	eB2dBody_BodyType;		// body type of the physics body
	bool		bB2dBody_FixedRotation;	// determines whether the rotation of the object is fixed

	// Common Custom Properties
	bool bIsStartFlipped;				// A boolean that to flip direction when the object is spawned

	// Terminal Setup Properties
	bool bIsKeyTerminal;				// A terminal that reflects to the main objective in the level
	bool bIsTerminator;					// A terminal that ables to deactivate enemies
	bool bIsMover;						// A terminal that ables to activate moving platform
	std::string strHackType;			// A terminal hacking type - ButtonHack or QTE
	float fHackTime;					// ButtonHack - Hack Duration
	bool bIsHackResetable;				// ButtonHack - Hack Resetable flag
	float fHackTimePerButton;			// QTE - Time diff between 2 buttons
	int iHackNumberOfEvents;			// QTE - How many buttons that player need to press
	std::string strTerminalType;		// Terminal type for switching sprite group

	// Enemies Setup Properties
	bool bIsMovmentActivated;			// A boolean that make a non-walk enemy
	float fEnemyAttackRange;			// A value that reflects the enemy attack range / detection range

	// Moving Platform Properties
	bool bRequiredActivation;			// A moving platform that is required an activation to activate moving behaviour
	float fMovingPlatformTargetX;		// A target position X that the platform is moving to
	float fMovingPlatformTargetY;		// A target position Y that the platform is moving to

	// Env Object - Rock
	std::string strRockStyle;			// A string that represent the rock style

	// Floors Setup Properties
	std::string strFloorStyle;			// Name of the floor style, it will change the sprite frame name for floor

	// Perspective Platform Setup Properties
	std::string strPerspectivePlatformLocation;		// To choose the correct sprite by using this param - Low or High only, it will change the sprite name for this perspective platform.
	std::string strPerspectivePlatformStyle;		// Name of the perspective platform style, it will change the sprite name for this perspective platform.

	// Wall Setup Properties
	std::string strWallStyle;			// Name of the wall style, it will change the sprite frame name for wall


	//////////////////////////////////////////////////////////////////////////
	// default initialising constructor
	//////////////////////////////////////////////////////////////////////////
	CGCFactoryCreationParams( void )
	: eB2dBody_BodyType		( b2_dynamicBody )
	, bB2dBody_FixedRotation( false )
	{}


	//////////////////////////////////////////////////////////////////////////
	// explicit initialising constructor
	//////////////////////////////////////////////////////////////////////////
	CGCFactoryCreationParams(	const char* pszClassName, 
								const char* pszPlistFile, 
								const char*	pszPhysicsShape,
								b2BodyType	eb2BodyType, 
								bool		bRotationIsFixed )
	: strClassName			( pszClassName )
	, strPlistFile			( pszPlistFile )
	, strPhysicsShape		( pszPhysicsShape )
	, eB2dBody_BodyType		( eb2BodyType )
	, bB2dBody_FixedRotation( bRotationIsFixed )
	{}

	//////////////////////////////////////////////////////////////////////////
	// overloaded operator ==
	// N.B. compare least complex types first as an 'early-out'
	//////////////////////////////////////////////////////////////////////////
	bool operator==( const CGCFactoryCreationParams& rtRhs ) const
	{
		return(		( eB2dBody_BodyType			== rtRhs.eB2dBody_BodyType )
				&&	( bB2dBody_FixedRotation	== rtRhs.bB2dBody_FixedRotation )
				&&	( strClassName				== rtRhs.strClassName ) 
				&&	( strPlistFile				== rtRhs.strPlistFile )
				&&	( strPhysicsShape			== rtRhs.strPhysicsShape ) );
	}

	//////////////////////////////////////////////////////////////////////////
	// virtual destructor so it can be extended later and continue 
	// being used in CGCLevelLoader_Ogmo with the minimum of fuss
	virtual ~CGCFactoryCreationParams( void )
	{
	}

	//////////////////////////////////////////////////////////////////////////
	bool NameIs( const char* pszNameToTest ) const
	{
		return !( strClassName.compare( pszNameToTest ) );
	}
};


//////////////////////////////////////////////////////////////////////////
// macros to make declaring these a bit easier
// see GCObjectPlayer.cpp for usage examples
//////////////////////////////////////////////////////////////////////////
#define IN_CPP_CREATION_PARAMS_DECLARE( ClassName, SpritePlistPath, PhysicsShapeName, Box2dBodyType, BodyIsFixedRotation )\
	static CGCFactoryCreationParams s_cCreationParams_##ClassName( #ClassName, SpritePlistPath, PhysicsShapeName, Box2dBodyType, BodyIsFixedRotation )

#define IN_CPP_CREATION_PARAMS_AT_TOP_OF_VONRESOURCEACQUIRE( ClassName )	VHandleFactoryParams( s_cCreationParams_##ClassName, GetResetPosition() )



#endif // #ifndef _SCGFACTORYCREATIONPARAMS_H_ ...