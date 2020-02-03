#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <iostream>
#include "GCLevelLoader_Ogmo.h"

#include "cocos2d.h"

#include "GamerCamp/GCCocosInterface/GCFactory_ObjSpritePhysics.h"

#include "GCLevelLoader_Ogmo.h"

using namespace std;
USING_NS_CC;



//////////////////////////////////////////////////////////////////////////
// constants defining the names of the xml expected attributes in the oel
// level file
//////////////////////////////////////////////////////////////////////////
static const char* k_pszXmlAttr_ClassName				= "class";
static const char* k_pszXmlAttr_PlistFile				= "PlistFile";
static const char* k_pszXmlAttr_PhysicsShape			= "shape";
static const char* k_pszXmlAttr_B2D_BodyType			= "B2DBodyType";
static const char* k_pszXmlAttr_B2D_HasFixedRotation	= "HasFixedRotation";
static const char* k_pszXmlAttr_StartPos_X				= "x";
static const char* k_pszXmlAttr_StartPos_Y				= "y";

// Common Properties
static const char* k_pszXmlAttr_Common_StartFlipped		= "StartFlipped";

// Terminal Properties
static const char* k_pszXmlAttr_Terminal_IsKeyTerminal			= "IsKeyTerminal";
static const char* k_pszXmlAttr_Terminal_IsTerminator			= "IsTerminator";
static const char* k_pszXmlAttr_Terminal_IsMover					= "IsMover";
static const char* k_pszXmlAttr_Terminal_HackType				= "HackType";
static const char* k_pszXmlAttr_Terminal_ButtonHackTime			= "ButtonHackTime";
static const char* k_pszXmlAttr_Terminal_ButtonHackResetable		= "ButtonHackResetable";
static const char* k_pszXmlAttr_Terminal_QTETimePerButton		= "QTETimePerButton";
static const char* k_pszXmlAttr_Terminal_QTENumberOfEvents		= "QTENumberOfEvents";
static const char* k_pszXmlAttr_Terminal_TerminalType			= "TerminalType";

// Enemy Properties
static const char* k_pszXmlAttr_Enemy_IsMovementActivated = "ActivateMovement";
static const char* k_pszXmlAttr_Enemy_AttackRange = "AttackRange";

// Moving Platform Properties
static const char* k_pszXmlAttr_Moving_Platform_RequiredActivation = "RequiredActivation";
static const char* k_pszXmlAttr_Moving_Platform_MovingTargetX = "MovingTargetX";
static const char* k_pszXmlAttr_Moving_Platform_MovingTargetY = "MovingTargetY";

// Floor Property
static const char* k_pszXmlAttr_Floor_FloorStyle = "FloorStyle";

// Rock Property
static const char* k_pszXmlAttr_EnvRock_RockStyle = "RockStyle";

// Wall Property
static const char* k_pszXmlAttr_Wall_WallStyle = "WallStyle";

// Perspective Platform Property
static const char* k_pszXmlAttr_PerspectivePlatform_PlatformLocation = "PlatformLocation";
static const char* k_pszXmlAttr_PerspectivePlatform_PlatformStyle = "PlatformStyle";

// strings for b2BodyType enum
static const char* k_pszB2BodyType_Dynamic		= "b2_dynamicBody";
static const char* k_pszB2BodyType_Static		= "b2_staticBody";
static const char* k_pszB2BodyType_Kinematic	= "b2_kinematicBody";

// strings for hack type enum
static const char* k_pszHackType_ButtonHack		= "ButtonHack";
static const char* k_pszHackType_QTE			= "QTE";

// strings for FloorStyle enum
static const char* k_pszFloorStyle_Level_1 = "level_1";
static const char* k_pszFloorStyle_Level_2 = "level_2";
static const char* k_pszFloorStyle_Level_3 = "level_3";
static const char* k_pszFloorStyle_Level_4 = "level_4";


//////////////////////////////////////////////////////////////////////////
// Constructor
//////////////////////////////////////////////////////////////////////////
CGCLevelLoader_Ogmo::CGCLevelLoader_Ogmo( void )
: m_v2LevelDimensions( b2Vec2_zero )
{
	m_vecFactoryCreationParams.reserve( EMAX_HANDLED_UNIQUE_CREATIONPARAMS );
	m_vecFactoryCreatedObjects.reserve( EMAX_HANDLED_OBJECTS );
}


//////////////////////////////////////////////////////////////////////////
// destructor
//////////////////////////////////////////////////////////////////////////
CGCLevelLoader_Ogmo::~CGCLevelLoader_Ogmo( void )
{	
	for( u32 u = 0; u < m_vecFactoryCreationParams.size(); ++u )
	{
		delete m_vecFactoryCreationParams[ u ];
	}
}


//////////////////////////////////////////////////////////////////////////
// This function just loads the level file.
// CreateObjects() must be called to create them
//////////////////////////////////////////////////////////////////////////
bool CGCLevelLoader_Ogmo::LoadLevelFile( const char* pszOelFile )
{
	//=== 1. XML-file
	Data data = FileUtils::getInstance()->getDataFromFile( pszOelFile );
	
	if( data.getSize() > 0 )
	{
		m_xmlOgmoDocument.Parse( (const char*) data.getBytes(), data.getSize() );

		if( m_xmlOgmoDocument.Error() )
		{
			cerr << " CGCLevelLoader_Ogmo: Error during parsing of xml-file!" << endl;

			// Print details about the error / Print the "parsing status"
			cerr << " xml-file [" << pszOelFile << "] was parsed with error: " << m_xmlOgmoDocument.ErrorID() << "\n";
			cerr << " Error description: " << m_xmlOgmoDocument.GetErrorStr1() << m_xmlOgmoDocument.GetErrorStr2() << "\n";

			CCAssert( false, " CGCLevelLoader_Ogmo: Failed to load xml-file!" );
		}
		else
		{
			m_pxmlLevelNode		= m_xmlOgmoDocument.RootElement();
			std::string strName	= m_pxmlLevelNode->Name();

			CCAssert( !strcmp( m_pxmlLevelNode->Name(), "level" ), "oel file appears to be incorrect format" );

			m_v2LevelDimensions = b2Vec2( m_pxmlLevelNode->FloatAttribute( "width" ), m_pxmlLevelNode->FloatAttribute( "height" ) );
		}

	}
	else
	{
		CCAssert( false, " CGCLevelLoader_Ogmo: Failed to open oel file" );
	}

	// successful
	return true;
}


//////////////////////////////////////////////////////////////////////////
//    tiny xml * tiny xml * tiny xml * tiny xml * tiny xml * tiny xml
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
// tinyxml version of the file parser
class OgmoVisitor 
	: public tinyxml2::XMLVisitor
{
	CGCLevelLoader_Ogmo&			m_rOgmoLoader;
	CGCFactory_ObjSpritePhysics&	m_rFactoryClass;
	int								m_iNumCreated;

public:
	OgmoVisitor( CGCLevelLoader_Ogmo& rOgmoLoader, CGCFactory_ObjSpritePhysics& rClassFactory )
		: m_rOgmoLoader		( rOgmoLoader )
		, m_rFactoryClass	( rClassFactory )
		, m_iNumCreated		( 0 )
	{}

	virtual bool VisitEnter( const tinyxml2::XMLElement& rxmlElement, const tinyxml2::XMLAttribute* pxmlAttribute ) override
	{
		if( m_rOgmoLoader.AddFactoryDataForXMLElementAndCreateInstance( rxmlElement, m_rFactoryClass ) )
		{
			++m_iNumCreated;
		}
		return true;
	}

	int NumCreated()
	{
		return m_iNumCreated;
	}
};

//////////////////////////////////////////////////////////////////////////
inline bool CGCLevelLoader_Ogmo::AddFactoryDataForXMLElementAndCreateInstance( const tinyxml2::XMLElement& rxmlElement, CGCFactory_ObjSpritePhysics& rClassFactory )
{
	if( IsValidFactoryData( rxmlElement ) )
	{
		CGCFactoryCreationParams* pParams = GetCreationParamsFromCache( GetFactoryClassName( rxmlElement ) );

		if( nullptr == pParams )
		{
			pParams = AddCreationParamsToInternalStore( GetFactoryData( rxmlElement ) );
		}

		cocos2d::Vec2 v2Pos = GetObjectPosition( rxmlElement );
		CCLOG( "creating instance of %s at position [%f, %f]", pParams->strClassName.c_str(), v2Pos.x, v2Pos.y );

		CCAssert( ( m_vecFactoryCreatedObjects.size() < EMAX_HANDLED_OBJECTS ),
				  "You need to reserve more space in CGCLevelLoader_Ogmo::m_vecFactoryCreatedObjects! Make CGCLevelLoader_Ogmo::EMAX_HANDLED_OBJECTS bigger!" );



		// Handling Common Properties
		pParams->bIsStartFlipped = GetBoolFromString( rxmlElement.Attribute( k_pszXmlAttr_Common_StartFlipped ) );

		// Hotfix for the body shape mismatch issue, force to use the latest body shape name
		pParams->strPhysicsShape = rxmlElement.Attribute( k_pszXmlAttr_PhysicsShape );

		// Handling Terminal Custom Properties
		pParams->bIsKeyTerminal = GetBoolFromString( rxmlElement.Attribute( k_pszXmlAttr_Terminal_IsKeyTerminal ) );
		pParams->bIsTerminator = GetBoolFromString( rxmlElement.Attribute( k_pszXmlAttr_Terminal_IsTerminator ) );
		pParams->bIsMover = GetBoolFromString( rxmlElement.Attribute( k_pszXmlAttr_Terminal_IsMover ) );
		pParams->strHackType = GetHackType( rxmlElement );
		pParams->fHackTime = rxmlElement.FloatAttribute( k_pszXmlAttr_Terminal_ButtonHackTime );
		pParams->bIsHackResetable = GetBoolFromString( rxmlElement.Attribute( k_pszXmlAttr_Terminal_ButtonHackResetable ) );
		pParams->fHackTimePerButton = rxmlElement.FloatAttribute( k_pszXmlAttr_Terminal_QTETimePerButton );
		pParams->iHackNumberOfEvents = rxmlElement.IntAttribute( k_pszXmlAttr_Terminal_QTENumberOfEvents );
		pParams->strTerminalType = rxmlElement.Attribute( k_pszXmlAttr_Terminal_TerminalType ) ? rxmlElement.Attribute( k_pszXmlAttr_Terminal_TerminalType ) : "";

		// Handling Enemy Custom Properties
		pParams->bIsMovmentActivated = GetBoolFromString( rxmlElement.Attribute( k_pszXmlAttr_Enemy_IsMovementActivated ) );
		pParams->fEnemyAttackRange = rxmlElement.FloatAttribute( k_pszXmlAttr_Enemy_AttackRange );

		// Handling Moving Platform Properties
		pParams->bRequiredActivation = GetBoolFromString( rxmlElement.Attribute( k_pszXmlAttr_Moving_Platform_RequiredActivation ) );
		pParams->fMovingPlatformTargetX = rxmlElement.FloatAttribute( k_pszXmlAttr_Moving_Platform_MovingTargetX );
		pParams->fMovingPlatformTargetY = rxmlElement.FloatAttribute( k_pszXmlAttr_Moving_Platform_MovingTargetY );

		// Handling Floor Property
		pParams->strFloorStyle = GetFloorStyle( rxmlElement );

		// Handling Rock Property
		pParams->strRockStyle = rxmlElement.Attribute( k_pszXmlAttr_EnvRock_RockStyle ) ? rxmlElement.Attribute( k_pszXmlAttr_EnvRock_RockStyle ) : "";

		// Handling Wall Property
		pParams->strWallStyle = rxmlElement.Attribute( k_pszXmlAttr_Wall_WallStyle ) ? rxmlElement.Attribute( k_pszXmlAttr_Wall_WallStyle ) : "";

		// Handling Perspective Platform Properties
		pParams->strPerspectivePlatformLocation = rxmlElement.Attribute( k_pszXmlAttr_PerspectivePlatform_PlatformLocation ) ? rxmlElement.Attribute( k_pszXmlAttr_PerspectivePlatform_PlatformLocation ) : "";
		pParams->strPerspectivePlatformStyle = rxmlElement.Attribute( k_pszXmlAttr_PerspectivePlatform_PlatformStyle ) ? rxmlElement.Attribute( k_pszXmlAttr_PerspectivePlatform_PlatformStyle ) : "";

		m_vecFactoryCreatedObjects.push_back( rClassFactory.CreateInstance( ( *pParams ), v2Pos ) );

		return true;
	}

	return false;
}


//////////////////////////////////////////////////////////////////////////
// validates an xml node as data that can be used to create an instance
// via the class factory
//////////////////////////////////////////////////////////////////////////
// private
inline bool CGCLevelLoader_Ogmo::IsValidFactoryData( const tinyxml2::XMLElement& rxmlElement )
{
	const char*	pszName						= rxmlElement.Name();
	const tinyxml2::XMLAttribute* pName		= rxmlElement.FindAttribute( k_pszXmlAttr_ClassName );
	const tinyxml2::XMLAttribute* pPlist	= rxmlElement.FindAttribute( k_pszXmlAttr_PlistFile );
	const tinyxml2::XMLAttribute* pShape	= rxmlElement.FindAttribute( k_pszXmlAttr_PhysicsShape );
	const tinyxml2::XMLAttribute* pBodyType	= rxmlElement.FindAttribute( k_pszXmlAttr_B2D_BodyType );
	const tinyxml2::XMLAttribute* pFixedRot	= rxmlElement.FindAttribute( k_pszXmlAttr_B2D_HasFixedRotation );
	const tinyxml2::XMLAttribute* pPosX		= rxmlElement.FindAttribute( k_pszXmlAttr_StartPos_X );
	const tinyxml2::XMLAttribute* pPosY		= rxmlElement.FindAttribute( k_pszXmlAttr_StartPos_Y );

	return(		( nullptr != pName		)
			&&	( nullptr != pPlist		)
			&&	( nullptr != pShape		)
			&&	( nullptr != pBodyType	)
			&&	( nullptr != pFixedRot	)
			&&	( nullptr != pPosX		)
			&&	( nullptr != pPosY		)
			);
}

//////////////////////////////////////////////////////////////////////////
// private  
inline const char* CGCLevelLoader_Ogmo::GetFactoryClassName( const tinyxml2::XMLElement& rxmlElement )
{
	return rxmlElement.Attribute( k_pszXmlAttr_ClassName );
}

//////////////////////////////////////////////////////////////////////////
// creates the corresponding factory creation params for a valid xml node
//////////////////////////////////////////////////////////////////////////
// private
inline CGCFactoryCreationParams* CGCLevelLoader_Ogmo::GetFactoryData( const tinyxml2::XMLElement& rxmlElement )
{
	return( new CGCFactoryCreationParams(	rxmlElement.Attribute( k_pszXmlAttr_ClassName ),
			rxmlElement.Attribute( k_pszXmlAttr_PlistFile ),
			rxmlElement.Attribute( k_pszXmlAttr_PhysicsShape ),
			B2BodyTypeFromString( rxmlElement.Attribute( k_pszXmlAttr_B2D_BodyType ) ),
			rxmlElement.BoolAttribute( k_pszXmlAttr_B2D_HasFixedRotation )
			) );
}

//////////////////////////////////////////////////////////////////////////
// private  
inline cocos2d::Vec2 CGCLevelLoader_Ogmo::GetObjectPosition( const tinyxml2::XMLElement& rxmlElement )
{
	// N.B. we use the screen origin as the level position origin
	// this means that the bottom left of your level will always be visible, but the top right not guaranteed
	// resolution and sprite scale settings are all in AppDelegate.cpp - good luck!
	Point			origin = Director::getInstance()->getVisibleOrigin();
	cocos2d::Vec2	v2Origin( origin.x, origin.y );

	return( v2Origin + cocos2d::Vec2(	rxmlElement.FloatAttribute( k_pszXmlAttr_StartPos_X ),
								( m_v2LevelDimensions.y - rxmlElement.FloatAttribute( k_pszXmlAttr_StartPos_Y ) ) ) );
}


//////////////////////////////////////////////////////////////////////////
//    tiny xml * tiny xml * tiny xml * tiny xml * tiny xml * tiny xml
//////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////
// recursively iterates he xml nodes in the loaded level document
// for each one it checks if it is a valid chunk of class factory data
// and if so uses it to create an object via the class factory
//////////////////////////////////////////////////////////////////////////
u32	CGCLevelLoader_Ogmo::CreateObjects( CGCFactory_ObjSpritePhysics& rcClassFactory )
{
	OgmoVisitor cOgmoVisitor( *this, rcClassFactory );

	m_xmlOgmoDocument.Accept( &cOgmoVisitor );

	return cOgmoVisitor.NumCreated();
}

//////////////////////////////////////////////////////////////////////////
// delete all the ObjSpritePhysics derived objects we created
//////////////////////////////////////////////////////////////////////////
void CGCLevelLoader_Ogmo::DestroyObjects()
{
	for( CGCObjSpritePhysics* pCreatedObj : m_vecFactoryCreatedObjects )
	{
		delete pCreatedObj;
	}

	m_vecFactoryCreatedObjects.clear();
}


//////////////////////////////////////////////////////////////////////////
// converts the xml string version of the box 2d body type to the enum
// value expected by box 2d
//////////////////////////////////////////////////////////////////////////
// private
inline b2BodyType CGCLevelLoader_Ogmo::B2BodyTypeFromString( const char* pszBodyType )
{
	b2BodyType eReturnType = b2_dynamicBody;

	// N.B. can't use a switch because of strings 
	if( !strcmp( pszBodyType, k_pszB2BodyType_Static ) )
	{
		eReturnType = b2_staticBody;

	}
	else if( !strcmp( pszBodyType, k_pszB2BodyType_Kinematic ) )
	{
		eReturnType = b2_kinematicBody;
	}

	return eReturnType;
}


//////////////////////////////////////////////////////////////////////////
// private
 CGCFactoryCreationParams* CGCLevelLoader_Ogmo::GetCreationParamsFromCache( const char* pszClassName )
{
	CGCFactoryCreationParams* pcReturn = nullptr;

	size_t uNumElements = m_vecFactoryCreationParams.size();
	for( size_t u = 0; u < uNumElements; ++u )
	{
		if( m_vecFactoryCreationParams[ u ]->NameIs( pszClassName ) )
		{
			pcReturn = m_vecFactoryCreationParams[ u ]; 
			break;
		}
	}

	return pcReturn;
}


//////////////////////////////////////////////////////////////////////////
// this class used to store cache creation params by value, but has been 
// changed so that they're stored as pointers so that the data can be 
// extended
//////////////////////////////////////////////////////////////////////////
// private
CGCFactoryCreationParams* CGCLevelLoader_Ogmo::AddCreationParamsToInternalStore( CGCFactoryCreationParams* ptParams )
{
	CCAssert(	( m_vecFactoryCreationParams.size() < EMAX_HANDLED_UNIQUE_CREATIONPARAMS ), 
				"You need to reserve more space in CGCLevelLoader_Ogmo::m_vecFactoryCreationParams! Make CGCLevelLoader_Ogmo::EMAX_HANDLED_UNIQUE_CREATIONPARAMS bigger!" );

	m_vecFactoryCreationParams.push_back( ptParams );
	return m_vecFactoryCreationParams.back();
}

//////////////////////////////////////////////////////////////////////////
// Helpers
//////////////////////////////////////////////////////////////////////////
bool CGCLevelLoader_Ogmo::GetBoolFromString( const char* pszString )
{
	// Check the text is not null
	if( pszString == nullptr )
	{
		return false;
	}

	// Because the OgmoEditor export the boolean in True / False
	// Therefore, it won't work or accessible by using tiny2xml methods to get boolean from OgmoEditor
	// Here is a walk around for getting boolean from OgmoEditor
	if( !strcmp( pszString, "True" ) )
	{
		return true;
	}
	else if( !strcmp( pszString, "False" ) )
	{
		return false;
	}
	return false;
}

//////////////////////////////////////////////////////////////////////////
// Terminal Custom Properties
//////////////////////////////////////////////////////////////////////////
std::string CGCLevelLoader_Ogmo::GetHackType( const tinyxml2::XMLElement& rxmlElement )
{
	// Get hack type as string by the key name
	const char* pszHackType = rxmlElement.Attribute( k_pszXmlAttr_Terminal_HackType );

	// Set the default return value to button hack
	std::string strResult = k_pszHackType_ButtonHack;

	// check the hack type is not empty
	if( !pszHackType )
	{
		// Return default value
		return strResult;
	}

	// Compare the hack type and return the specified value
	if( !strcmp( pszHackType, k_pszHackType_ButtonHack ) )
	{
		strResult = k_pszHackType_ButtonHack;
	}
	else if( !strcmp( pszHackType, k_pszHackType_QTE ) )
	{
		strResult = k_pszHackType_QTE;
	}
	return strResult;
}

//////////////////////////////////////////////////////////////////////////
// Floor Custom Properties
//////////////////////////////////////////////////////////////////////////
std::string CGCLevelLoader_Ogmo::GetFloorStyle( const tinyxml2::XMLElement & rxmlElement )
{
	// Get hack type as string by the key name
	const char* pszFloorStyle = rxmlElement.Attribute( k_pszXmlAttr_Floor_FloorStyle );

	// Set the default return value
	std::string strResult = "level_1";

	// null checking
	if( !pszFloorStyle )
	{
		// If null, return the default value instead
		return strResult;
	}

	// switch to different level
	if( !strcmp( pszFloorStyle, k_pszFloorStyle_Level_1 ) )
	{
		strResult = k_pszFloorStyle_Level_1;
	}
	else if( !strcmp( pszFloorStyle, k_pszFloorStyle_Level_2 ) )
	{
		strResult = k_pszFloorStyle_Level_2;
	}
	else if( !strcmp( pszFloorStyle, k_pszFloorStyle_Level_3 ) )
	{
		strResult = k_pszFloorStyle_Level_3;
	}
	else if( !strcmp( pszFloorStyle, k_pszFloorStyle_Level_4 ) )
	{
		strResult = k_pszFloorStyle_Level_4;
	}

	return strResult;
}