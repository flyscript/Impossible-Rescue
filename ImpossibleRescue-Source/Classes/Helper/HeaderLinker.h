//////////////////////////////////////////////////////////////////////////
// Levels
//////////////////////////////////////////////////////////////////////////

// In-box level

// Act 1
#include "../GamerCamp/GameSpecific/Levels/Act_1/CLevel1.h"
#include "../GamerCamp/GameSpecific/Levels/Act_1/CLevel2.h"
#include "../GamerCamp/GameSpecific/Levels/Act_1/CLevel3.h"
#include "../GamerCamp/GameSpecific/Levels/Act_1/CLevel4.h"

// Act 2
//#include "../GamerCamp/GameSpecific/Levels/Act_2/GCLevel5.h"
//#include "../GamerCamp/GameSpecific/Levels/Act_2/GCLevel6.h"
//#include "../GamerCamp/GameSpecific/Levels/Act_2/GCLevel7.h"
//#include "../GamerCamp/GameSpecific/Levels/Act_2/GCLevel8.h"

// Act 3
//#include "../GamerCamp/GameSpecific/Levels/Act_3/GCLevel9.h"
//#include "../GamerCamp/GameSpecific/Levels/Act_3/GCLevel10.h"
//#include "../GamerCamp/GameSpecific/Levels/Act_3/GCLevel11.h"
//#include "../GamerCamp/GameSpecific/Levels/Act_3/GCLevel12.h"



//////////////////////////////////////////////////////////////////////////
// Items
//////////////////////////////////////////////////////////////////////////

// In-box collectables
#include "../GamerCamp/GameSpecific/Items/GCObjItem.h"

// In-box collectables group
#include "GamerCamp/GameSpecific/Items/GCObjGroupItem.h"

// Data Persistence
#include "GamerCamp/GameController/DataPersistence/CDataManager.h"


//////////////////////////////////////////////////////////////////////////
// Nodes
//////////////////////////////////////////////////////////////////////////

// Jump Node
#include "../GamerCamp/GameSpecific/Nodes/CJumpNode.h"

// Turn Node
#include "../GamerCamp/GameSpecific/Nodes/CTurnNode.h"

// Exit node
#include "GamerCamp/GameSpecific/Nodes/CExitNode.h"



//////////////////////////////////////////////////////////////////////////
// Platforms
//////////////////////////////////////////////////////////////////////////

// In-box platform
#include "../GamerCamp/GameSpecific/Platforms/GCObjPlatform.h"

// Moving Platforms
#include "GamerCamp/GameSpecific/Platforms/CObjMovingPlatform.h"

// Crumbling Platforms
#include "GamerCamp/GameSpecific/Platforms/CObjCrumblingPlatform.h"

// In-box platform group
#include "GamerCamp/GameSpecific/Platforms/GCObjGroupPlatform.h"

// Floor
#include "GamerCamp/GameSpecific/Platforms/CObjFloor.h"



//////////////////////////////////////////////////////////////////////////
// Player
//////////////////////////////////////////////////////////////////////////

// Player
#include "../GamerCamp/GameSpecific/Player/GCObjPlayer.h"

// Shadow
#include "../GamerCamp/GameSpecific/Player/CObjShadow.h"



//////////////////////////////////////////////////////////////////////////
// Projectiles
//////////////////////////////////////////////////////////////////////////

// Group
#include "GamerCamp/GameSpecific/Projectiles/GCObjGroupProjectile.h"

// Projectile
#include "GamerCamp/GameSpecific/Projectiles/GCObjProjectile.h"



//////////////////////////////////////////////////////////////////////////
// Enemies
//////////////////////////////////////////////////////////////////////////

// Enemy R1
#include "GamerCamp/GameSpecific/Enemies/R1/CEnemyR1.h"

// Enemy R2
#include "GamerCamp/GameSpecific/Enemies/R2/CEnemyR2.h"

// Enemy R3
#include "GamerCamp/GameSpecific/Enemies/R3/CEnemyR3.h"

//////////////////////////////////////////////////////////////////////////
// Envirnoments Objects
//////////////////////////////////////////////////////////////////////////

// Rock A
#include "GamerCamp/GameSpecific/Environments/CEnvRock.h"

//////////////////////////////////////////////////////////////////////////
// Custom UIs
//////////////////////////////////////////////////////////////////////////

// Animated Text
#include "GamerCamp/GameSpecific/UI/UIAnimatedText.h"

// Animated Bar
#include "GamerCamp/GameSpecific/UI/UIAnimatedBar.h"



//////////////////////////////////////////////////////////////////////////
// Environment and others
//////////////////////////////////////////////////////////////////////////

// Terminal
#include "GamerCamp/GameSpecific/Terminals/CObjTerminal.h"

//Hackables
#include "GamerCamp/GameSpecific/Hackables/CHackable.h"
#include "GamerCamp/GameSpecific/Hackables/CButtonHack.h"
#include "GamerCamp/GameSpecific/Hackables/CQTEHack.h"

// Door 
#include "GamerCamp/GameSpecific/Door/CObjDoor.h"
#include "GamerCamp/GameSpecific/Door/CObjDoorLeft.h"
#include "GamerCamp/GameSpecific/Door/CObjDoorRight.h"

// Audio Manager
#include "GamerCamp/GameController/AudioManager/CAudioManager.h"


//////////////////////////////////////////////////////////////////////////
// GamerCamp Core
//////////////////////////////////////////////////////////////////////////

// Object Manager
#include "GamerCamp/GCObject/GCObjectManager.h"

// Object Sprite
#include "GamerCamp/GCCocosInterface/GCObjSprite.h"

// Object Factory
#include "GamerCamp/GCCocosInterface/GCFactory_ObjSpritePhysics.h"

// Event management
#include "GamerCamp/EventManager/CEventManager.h"
#include "GamerCamp/EventManager/CEventListener.h"