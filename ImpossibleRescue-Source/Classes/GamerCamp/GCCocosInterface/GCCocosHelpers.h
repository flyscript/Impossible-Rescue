////////////////////////////////////////////////////////////////////////////////////////////////////////////
// (C) Gamer Camp / Alex Darby 2018
// Distributed under the MIT license - see readme.md
////////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef _GCCOCOSHELPERS_H_
#define _GCCOCOSHELPERS_H_

#ifndef _GCTYPES_H_
	#include "../Core/GCTypes.h"
#endif

#ifndef __cocos2d_libs__CCValue__
	#include "base/CCValue.h"
#endif

//////////////////////////////////////////////////////////////////////////
// forward declare
namespace cocos2d
{
	class Sprite;
	class Animation;
	class ActionInterval;
}

//////////////////////////////////////////////////////////////////////////
// GCCocosHelpers
namespace GCCocosHelpers
{
	// N.B. the returned dictionary is NOT autoreleased
	// N.N.B. you must call release on it when finished or it will leak
	cocos2d::ValueMap CreateDictionaryFromPlist( const std::string& rstrPlist );

	// creates a dictionary from the passed .plist file
	// creates a sprite using a .plist file - just uses 1st frame in the .plist
	// N.B. only use this if you just need to load a single frame of a sprite
	cocos2d::Sprite* CreateSpriteFromPlist( const std::string& rstrPlist );

	//////////////////////////////////////////////////////////////////////////
	// functions that can be called to load sprites more efficiently
	// CreateSpriteFromPlist is implemented in terms of these functions
	
		// this loads the plist, adds the texture and frames to relevant 
		// caches and then returns the dictionary loaded from the plist 
		// N.B. returned dictionary in NOT autoreleased
		cocos2d::ValueMap		Sprite_LoadTextureAndFramesToCachesAndGetDictionary	( const std::string& rstrPlist );

		// once you have done the above 2 functions call this as many times
		// as you need to to create the sprites you want
		cocos2d::Sprite*		Sprite_CreateSpriteFrom1stFrame						( cocos2d::ValueMap& dicSpriteInfo );

	// N.B. don't forget to release your dictionary!
	//////////////////////////////////////////////////////////////////////////


	// iterates the dictionary (created by Sprite_CreateDictionaryFromPlist) 
	// finding all the frames matching the animation name and adding them 
	// to the animation via the shared sprite cache
	// 
	// N.B. all the frames must be added to the frame cache first by calling 
	// SpriteFrameCache::getInstance()->addSpriteFramesWithFile( pszPlist );
	// (which is called during CreateSpriteFromPlist)
	// 
	// N.N.B. alternatively you could call Sprite_LoadTextureAndFramesToCaches
	// which does the same thing
	cocos2d::Animation*		CreateAnimation					(	cocos2d::ValueMap&		rcSpritePlistAsDict, 
																const std::string&		rstrAnimationName );
	
	// Below overload allows for a custom animation framerate to be set - Will C
	cocos2d::Animation*		CreateAnimation					(	cocos2d::ValueMap&		rcSpritePlistAsDict,
																const std::string&		rstrAnimationName,
			
													float					fAnimationFramesPerSecond );
	
	
	// Take an animation and adjust its frames per second - Will C
	void SetAnimationFPS( cocos2d::Animation* pAnim, float fFPS );


	// create animation actions given an animation
	cocos2d::ActionInterval*	CreateAnimationActionOnce	(	cocos2d::Animation*	pAnimation );
	cocos2d::ActionInterval*	CreateAnimationActionLoop	(	cocos2d::Animation*	pAnimation );
}

#endif