////////////////////////////////////////////////////////////////////////////////////////////////////////////
// (C) Gamer Camp / Alex Darby 2018
// Distributed under the MIT license - see readme.md
////////////////////////////////////////////////////////////////////////////////////////////////////////////
#include "GCCocosHelpers.h"

#include "cocos2d.h"
#include <algorithm>
#include <stdlib.h> 
#include <vector>
#include <tuple>

using namespace cocos2d;

//////////////////////////////////////////////////////////////////////////
// namespace to wrap our cocos2d-x helper functions
//////////////////////////////////////////////////////////////////////////
namespace GCCocosHelpers
{
	//////////////////////////////////////////////////////////////////////////
	// creates a __Dictionary from a plist
	// N.B. the returned __Dictionary IS autoreleased, so you'll need to 
	// retain it if you want it there later
	//////////////////////////////////////////////////////////////////////////
	ValueMap CreateDictionaryFromPlist( const std::string& rstrPlist )
	{
		// addSpriteFramesWithFile() actually does this internally - caching this could represent a big reduction in 
		// disc access when initialising sprites with a lots of animations (see CreateSpriteFromPlist() ) 
		std::string		strPath		= FileUtils::getInstance()->fullPathForFilename( rstrPlist );
		ValueMap		dicPList	= FileUtils::getInstance()->getValueMapFromFile( strPath.c_str() );
		return dicPList;
	}


	//////////////////////////////////////////////////////////////////////////
	// creates a sprite given a plist file
	// n.b. the returned sprite is set to autorelease
	//////////////////////////////////////////////////////////////////////////
	Sprite* CreateSpriteFromPlist( const std::string& rstrPlist ) 
	{
		// n.b. pdictSpriteInfo is set to autorelease so we don't need to release it manually
		ValueMap	dicSpriteInfo	= Sprite_LoadTextureAndFramesToCachesAndGetDictionary( rstrPlist );
		Sprite*		pSprite			= Sprite_CreateSpriteFrom1stFrame( dicSpriteInfo );
		return pSprite;
	}

	//////////////////////////////////////////////////////////////////////////
	// once you have the dictionary call this to load the texture and
	// sprite frames that are specified in the dictionary
	// 
	// n.b. the returned dictionary is set to autorelease so you should call
	// retain if you wish to keep it around
	//////////////////////////////////////////////////////////////////////////
	ValueMap Sprite_LoadTextureAndFramesToCachesAndGetDictionary( const std::string& rstrPlist )
	{
		return SpriteFrameCache::getInstance()->addSpriteFramesWithFileAndReturnValueMap( rstrPlist ); 
	}

	//////////////////////////////////////////////////////////////////////////
	// once you have done the above 2 functions call this as many times
	// as you need to to create the sprites you want
	//////////////////////////////////////////////////////////////////////////
	Sprite* Sprite_CreateSpriteFrom1stFrame( ValueMap& dicSpriteInfo )
	{
		ValueMap&		dicFrames	= dicSpriteInfo[ "frames" ].asValueMap();

		// elements are tuple< string, value >
		auto			itr1stFrame	= dicFrames.begin();
		Sprite*			pSprite		= Sprite::createWithSpriteFrameName( itr1stFrame->first );

		return pSprite;
	}



	//////////////////////////////////////////////////////////////////////////
	// operator <  equivalent function used by std::sort in CreateAnimationAction()
		static bool _AlphaCompare( const Ref* p1, const Ref* p2 )
		{
			const char* pszp1 = (((std::string*)p1)->c_str());
			const char* pszp2 = (((std::string*)p2)->c_str());
			// must emulate operator<()
			return (strcmp( pszp1, pszp2 ) < 0 )?true:false;
		}
	//
	// use the function above to sort the keyframes in the dictionary 
	//////////////////////////////////////////////////////////////////////////

	Animation* CreateAnimation( ValueMap& rcSpritePlistAsDict, const std::string& rstrAnimationName )
	{
		return CreateAnimation( rcSpritePlistAsDict, rstrAnimationName, 32.0f );
	}

	// Will C
	void SetAnimationFPS( cocos2d::Animation* pAnim, float fFPS )
	{
		CCAssert( pAnim, "ANIMATION DOESN'T EXIST" );
		pAnim->setDelayPerUnit( 1.0f / fFPS );
	}

	Animation* CreateAnimation( ValueMap& rcSpritePlistAsDict, const std::string& rstrAnimationName, float fAnimationFramesPerSecond )
	{
		// find the first frame name in the plist and use that
		ValueMap&	rdicFrames = rcSpritePlistAsDict[ "frames" ].asValueMap();

		std::vector< const std::string* > vpstrFrameNames;

		// dictionary contains tuple< string, value >
		// couldn't work out how to use the range based for to iterate and not copy the strings
		// this looks horrible, but it's storing pointer to the std::String already allocated for each frame
		for( auto itr = rdicFrames.begin(); itr != rdicFrames.end(); ++itr )
		{
			vpstrFrameNames.push_back( &((*itr).first) );
		}

		std::sort(	vpstrFrameNames.begin(), 
					vpstrFrameNames.end(), 
					[]( const std::string* pstrOne, const std::string* pstrTwo )
					{
						return( pstrOne->compare( *pstrTwo ) < 0 );
					}
		);

		// create an animation from the name
		Animation* pReturnAnim = Animation::create(); // N.B. auto released!
		pReturnAnim->setDelayPerUnit			( 1.0f / fAnimationFramesPerSecond );
		pReturnAnim->setRestoreOriginalFrame	( false );

		// iterate the array of animation frames (now sorted) and add all frames that match the animation name
		// N.B. uFramesFound is used to check whether any frames were added
		const char* pszAnimationName = rstrAnimationName.c_str();
		u32			uFramesFound = 0;

		for( unsigned int i = 0; i < vpstrFrameNames.size(); ++i )
		{
			// get locals relating to iteration
			const std::string&	rstrFrame		= *vpstrFrameNames[ i ];
			const char*		pszFrameName	= rstrFrame.c_str();

			// try to find the string
			const char* pstrFoundAt			= strstr( pszFrameName, pszAnimationName );
			int			iAnimNameLength		= strlen( pszAnimationName );
			int			iFrameNameLength	= strlen( pszFrameName );

			// check that pstrFoundAt == start of frame name...
			if( pstrFoundAt && ( pstrFoundAt == pszFrameName ) )
			{
				// ...and that pstrFoundAt reaches to the last \ or / in the path (i.e. we have a complete match)
				char*	pchLastPathDelim	= NULL;
				char*	pchCheck			= ((char*) pstrFoundAt) + 1;
				int		iMaxCharsToCheck	= iFrameNameLength - 1; // N.B. can't get to this unless anim name is at 1st char of frame name...
				for( int i = 0; i < iMaxCharsToCheck; ++i, ++pchCheck )
				{
					if(		( (*pchCheck) == '\\' )
						||	( (*pchCheck) == '/' ))
					{
						pchLastPathDelim = pchCheck;
					}
				}

				// if this is true the name goes from the 1st char to the last delimiter
				if( ( pchLastPathDelim - pstrFoundAt ) == iAnimNameLength )
				{
					SpriteFrame* pFrame = SpriteFrameCache::getInstance()->getSpriteFrameByName( pszFrameName );
					pReturnAnim->addSpriteFrame( pFrame );
					++uFramesFound;
				}

			}// found anim name in frame name

		}//for( all frames ){ ... check if in anim ...

		// If no frames were added we return NULL, else the animation
		// N.B. pReturnAnim is created auto released so it will self delete if not retained
		return ( 0 == uFramesFound ) ? NULL : pReturnAnim;
	}


	//////////////////////////////////////////////////////////////////////////
	// helper function to create a play-once action from an animation
	//////////////////////////////////////////////////////////////////////////
	ActionInterval* CreateAnimationActionOnce( Animation* pAnimation )
	{
		CCAssert( pAnimation, "pAnimation passed as NULL" );
		return Animate::create( pAnimation ); // N.B. Auto release
	}


	//////////////////////////////////////////////////////////////////////////
	// helper function to create a looped action from an animation
	//////////////////////////////////////////////////////////////////////////
	ActionInterval* CreateAnimationActionLoop( Animation* pAnimation )
	{
		CCAssert( pAnimation, "pAnimation passed as NULL" );
		return RepeatForever::create( Animate::create( pAnimation ) ); // N.B. Auto release
	}
}