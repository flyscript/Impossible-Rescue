//
//  GB2ShapeCache-x.cpp
//  
//  Loads physics sprites created with http://www.PhysicsEditor.de
//  To be used with cocos2d-x
//
//  Generic Shape Cache for box2d
//
//  Created by Thomas Broquist
//
//      http://www.PhysicsEditor.de
//      http://texturepacker.com
//      http://www.code-and-web.de
//  
//  All rights reserved.
//
//  Permission is hereby granted, free of charge, to any person obtaining a copy
//  of this software and associated documentation files (the "Software"), to deal
//  in the Software without restriction, including without limitation the rights
//  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
//  copies of the Software, and to permit persons to whom the Software is
//  furnished to do so, subject to the following conditions:
//  
//  The above copyright notice and this permission notice shall be included in
//  all copies or substantial portions of the Software.
//  
//  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
//  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
//  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
//  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
//  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
//  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
//  THE SOFTWARE.
//

#include "GB2ShapeCache-x.h"
#include "Box2d/Box2D.h"
#include "base/CCNS.h"

using namespace cocos2d;

/**
 * Internal class to hold the fixtures
 */
class FixtureDef
{
public:
	static const int k_iMagicNumber = 0x0C0FFEE0;

	struct SUserData
	{
		int iMagicNumber;
		std::string strIdText;
	};


	FixtureDef( const b2FixtureDef& rFixDef, const char* pszIdText )
		: next( NULL )
		, fixture( rFixDef )
	{
		sUserData.iMagicNumber = k_iMagicNumber;
		sUserData.strIdText = pszIdText;
		fixture.userData = reinterpret_cast<void*>( &sUserData );
	}

	~FixtureDef()
	{
		delete next;
		delete fixture.shape;
	}

	static const std::string* RetrieveIdText( const b2Fixture* pb2Fixture )
	{
		const std::string*	pstrReturn = NULL;
		const SUserData*	pUserData = reinterpret_cast<const SUserData*>( pb2Fixture->GetUserData() );

		if( pUserData->iMagicNumber == k_iMagicNumber )
		{
			pstrReturn = &( pUserData->strIdText );
		}

		return pstrReturn;
	}

	FixtureDef*		next;
	b2FixtureDef	fixture;
	SUserData		sUserData;
};

class BodyDef
{
public:
	BodyDef()
		: fixtures( NULL )
	{}

	~BodyDef()
	{
		if( fixtures )
			delete fixtures;
	}

	FixtureDef*		fixtures;
	cocos2d::Vec2	anchorPoint;
};

static GB2ShapeCache *_sharedGB2ShapeCache = NULL;

GB2ShapeCache* GB2ShapeCache::sharedGB2ShapeCache( void )
{
	if( !_sharedGB2ShapeCache ) {
		_sharedGB2ShapeCache = new GB2ShapeCache();
		_sharedGB2ShapeCache->init();
	}

	return _sharedGB2ShapeCache;
}

//static 
const std::string* GB2ShapeCache::getFixtureIdText( const b2Fixture* pb2Fixture )
{
	return FixtureDef::RetrieveIdText( pb2Fixture );
}


bool GB2ShapeCache::init()
{
	return true;
}

void GB2ShapeCache::reset()
{
	std::map<std::string, BodyDef *>::iterator iter;
	for( iter = shapeObjects.begin(); iter != shapeObjects.end(); ++iter ) {
		delete iter->second;
	}
	shapeObjects.clear();
}

void GB2ShapeCache::addFixturesToBody( b2Body *body, const std::string &shape )
{
	std::map<std::string, BodyDef *>::iterator pos = shapeObjects.find( shape );
	assert( pos != shapeObjects.end() );

	BodyDef *so = ( *pos ).second;

	FixtureDef *fix = so->fixtures;
	while( fix ) {
		body->CreateFixture( &fix->fixture );
		fix = fix->next;
	}
}

cocos2d::Vec2 GB2ShapeCache::anchorPointForShape( const std::string &shape )
{
	std::map<std::string, BodyDef *>::iterator pos = shapeObjects.find( shape );
	assert( pos != shapeObjects.end() );

	BodyDef *bd = ( *pos ).second;
	return bd->anchorPoint;
}


void GB2ShapeCache::addShapesWithFile( const std::string& plist )
{

	ValueMap dict = FileUtils::getInstance()->getValueMapFromFile( plist );
	// not triggered - cocos2dx delivers empty dict if non was found

	CCAssert( !dict.empty(), "Shape-file not found, plist file empty, or not existing" );

	ValueMap& metadataDict = dict[ "metadata" ].asValueMap();

	//int format	= static_cast<CCString *>(metadataDict->objectForKey("format"))->intValue();
	//ptmRatio = static_cast<CCString *>(metadataDict->objectForKey("ptm_ratio"))->floatValue();
	int format = metadataDict[ "format" ].asInt();
	ptmRatio = metadataDict[ "ptm_ratio" ].asFloat();
	CCLOG( "ptmRatio = %f", ptmRatio );
	CCAssert( format == 1, "Format not supported" );

	//CCDictionary *bodyDict = (CCDictionary *)dict->objectForKey("bodies");
	ValueMap& bodyDict = dict[ "bodies" ].asValueMap();

	b2Vec2 vertices[ b2_maxPolygonVertices ];

	// new code!
	// cKeyPair is tuple< string, value >
	for( auto cKeyPair : bodyDict )
	{
		// get top level body data
		ValueMap&	rdicBodyData = cKeyPair.second.asValueMap();
		std::string	strBodyName = cKeyPair.first;

		BodyDef* pBodyDef = new BodyDef();
		pBodyDef->anchorPoint = PointFromString( rdicBodyData[ "anchorpoint" ].asString() );

		ValueVector vvFixtureList = rdicBodyData[ "fixtures" ].asValueVector();

		// iterate & create fixtures - n.b. this is manuall;y creating a linked list of fixtures
		FixtureDef **ppNextFixtureDef = &( pBodyDef->fixtures );

		for( auto cvalFixture : vvFixtureList )
		{
			b2FixtureDef	cFixtureDef;
			ValueMap&		rdicFixtureData = cvalFixture.asValueMap();

			cFixtureDef.filter.categoryBits = rdicFixtureData[ "filter_categoryBits" ].asInt();
			cFixtureDef.filter.maskBits = rdicFixtureData[ "filter_maskBits" ].asInt();
			cFixtureDef.filter.groupIndex = rdicFixtureData[ "filter_groupIndex" ].asInt();
			cFixtureDef.friction = rdicFixtureData[ "friction" ].asFloat();
			cFixtureDef.density = rdicFixtureData[ "density" ].asFloat();
			cFixtureDef.restitution = rdicFixtureData[ "restitution" ].asFloat();
			cFixtureDef.isSensor = ( rdicFixtureData[ "isSensor" ].asInt() != 0 );

			std::string& rstrFixtureId = rdicFixtureData[ "id" ].asString();
			std::string& rstrFixtureType = rdicFixtureData[ "fixture_type" ].asString();

			if( rstrFixtureType == "POLYGON" )
			{
				ValueVector vvPolygons = rdicFixtureData[ "polygons" ].asValueVector();

				for( auto cPolyValue : vvPolygons )
				{
					FixtureDef* pFixture = new FixtureDef( cFixtureDef, rstrFixtureId.c_str() );

					b2PolygonShape* pPolyShape = new b2PolygonShape();
					int				iVIndex = 0;

					ValueVector vvVertices = cPolyValue.asValueVector();

					assert( vvVertices.size() <= b2_maxPolygonVertices );

					for( auto cVertValue : vvVertices )
					{
						cocos2d::Vec2 v2Offset = PointFromString( cVertValue.asString() );
						vertices[ iVIndex ].x = ( v2Offset.x / ptmRatio );
						vertices[ iVIndex ].y = ( v2Offset.y / ptmRatio );
						iVIndex++;
					}

					pPolyShape->Set( vertices, iVIndex );
					pFixture->fixture.shape = pPolyShape;

					// add pFixture to the list
					*ppNextFixtureDef = pFixture;
					ppNextFixtureDef = &( pFixture->next );
				}
			}
			else if( rstrFixtureType == "CIRCLE" )
			{
				FixtureDef*		pCircleFixture = new FixtureDef( cFixtureDef, rstrFixtureId.c_str() );

				ValueMap&		rdicCircleShape = rdicFixtureData[ "circle" ].asValueMap();

				b2CircleShape*	pCircleShape = new b2CircleShape();

				pCircleShape->m_radius = ( rdicCircleShape[ "radius" ].asFloat() / ptmRatio );
				cocos2d::Vec2 v2Pos = PointFromString( rdicCircleShape[ "position" ].asString() );
				pCircleShape->m_p = b2Vec2( ( v2Pos.x / ptmRatio ), ( v2Pos.y / ptmRatio ) );

				pCircleFixture->fixture.shape = pCircleShape;

				// add pFixture to the list
				*ppNextFixtureDef = pCircleFixture;
				ppNextFixtureDef = &( pCircleFixture->next );
			}
			else
			{
				CCAssert( 0, "Unknown fixtureType" );
			}

		}// for( auto cValue : vvFixtureList )

		shapeObjects[ strBodyName ] = pBodyDef;

	}//for( auto cKeyPair : bodyDict )

}