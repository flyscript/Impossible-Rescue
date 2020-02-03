#ifndef _LEVEL_BASE_H
#define _LEVEL_BASE_H

#include "GamerCamp/GCCocosInterface/IGCGameLayer.h"
#include "GamerCamp/GameController/AudioManager/CAudioManager.h"
#include "GamerCamp/GameController/GCController.h"
#include "GamerCamp/EventManager/CEventManager.h"
#include "GamerCamp/GameSpecific/ShadowTutorial/CShadowTutorial.h"

#ifndef _GCLEVELLOADER_OGMO_H_
#include "GamerCamp/GCCocosInterface/LevelLoader/GCLevelLoader_Ogmo.h"
#endif

//fwd declaration
class CGCObjSprite;
class CGCObjGroupProjectile;
class UIAnimatedBar;
class CObjBarrier;
class CGCEvent;
class CDataPersistence;

class CLevelBase
	: public IGCGameLayer
	, public b2ContactListener
{
private:

	// Level
	std::string m_sOgmoLevel;

	// Background
	char m_psBackground[ 60 ];

	int m_iLevel;

	// Prevent construction other than by derivation
	CLevelBase();

	// Remove copy assign
	CLevelBase( const CLevelBase& );
	CLevelBase& operator=( const CLevelBase& );

	//For controller mapping
	TGCActionToKeyMap<GameConfig::ELevelInput >* m_pcControllerActionToKeyMap;

	// Game state - Player Died
	bool m_bHasPlayerDied;

	// Corners
	cocos2d::Sprite* m_pTopLeftCorner;
	cocos2d::Sprite* m_pTopRightCorner;
	cocos2d::Sprite* m_pBottomLeftCorner;
	cocos2d::Sprite* m_pBottomRightCorner;



protected:
	// Shadow tutorial skipped flag
	bool m_bTutorialIsSkipped;

	// Main constructor
	CLevelBase( GCTypeID idIGCGameLayerDerived, std::string sOgmoLevel, char* psBackground, int iLevelGoals, int iLevel );
	CLevelBase( GCTypeID idIGCGameLayerDerived, std::string sOgmoLevel, char* psBackground, int iLevelGoals, int iLevel, f32 fLevelTime );

	// Audio Manager
	CAudioManager*				m_pcAudioManager;

	// Data Persistence
	CDataPersistence*			m_pcDataManager;

	// Projectile group
	CGCObjGroupProjectile*		m_pcGCGroupProjectile;

	// Backgrounds
	cocos2d::Sprite*			m_pcSprBackground;

	// Timer percentage
	cocos2d::Label*				m_pTimerPercentage;

	// Objective Label
	cocos2d::Label*				m_pObjectiveLabel;

	// Level Label
	cocos2d::Label*				m_pLevelLabel;

	// Weapon icon
	cocos2d::Sprite*			m_pWeaponIcon;

	// Level loader
	CGCLevelLoader_Ogmo			m_cLevelLoader;

	// Event Manager reference
	CEventManager&	m_cEventManager = CEventManager::Manager();

	// Level Goals
	int							m_iLevelGoals;
	int							m_iGoalsAcheived;
	bool						m_bLevelIsTimed;
	f32							m_fLevelTimeElapsed;
	f32							m_fLevelTime;
	bool						m_bCanGoToNextLevel;
	CShadowTutorial*			m_pcShadowTutorial;

public:

	// Virtual destructor
	virtual ~CLevelBase();

	// Callbacks for menu buttons
	void Callback_OnQuitButton( Ref* pSender );
	void Callback_OnResetButton( Ref* pSender );

	// Populate level
	void PopulateMenu();
	void PopulateGround();

	// CCNode interface
	virtual void onEnter() override;

	// IGCGameLayer interface
	virtual	void VOnCreate() override;
	virtual void VOnUpdate( f32 fTimeStep ) override;
	virtual	void VOnDestroy( void ) override;
	virtual void VOnReset() override;

	// Update the level timer if there is one
	void UpdateTimer( f32 fTimeStep );

	// Needs to be overridden to go to next level
	virtual void NextLevel() = 0;

	// Sets m_bCanGoToNextLevel flag to true
	virtual void AdvanceToNextLevel();

	// B2ContactListener interface
	virtual void BeginContact( b2Contact* pB2Contact ) override;
	virtual void EndContact( b2Contact* pB2Contact ) override;
	virtual void PreSolve( b2Contact* pB2Contact, const b2Manifold* pOldManifold ) override;
	virtual void PostSolve( b2Contact* pB2Contact, const b2ContactImpulse* pImpulse ) override;

	// Collision handling
	void HandleTurnNodeAndEnemyR1Collision( CGCObjSpritePhysics* pGcSprPhysA, CGCObjSpritePhysics* pGcSprPhysB );
	void HandleTurnNodeAndEnemyR3Collision( CGCObjSpritePhysics* pGcSprPhysA, CGCObjSpritePhysics* pGcSprPhysB );
	void HandlePlayerAndCoinCollision( CGCObjSpritePhysics* pGcSprPhysA, CGCObjSpritePhysics* pGcSprPhysB );
	void HandlePlayerAndExitNode( CGCObjSpritePhysics* pGcSprPhysA, CGCObjSpritePhysics* pGcSprPhysB );
	void HandlePlayerAndTerminalCollision( CGCObjSpritePhysics* pGcSprPhysA, CGCObjSpritePhysics* pGcSprPhysB );
	void HandlePlayerLeaveTerminalCollision( CGCObjSpritePhysics* pGcSprPhysA, CGCObjSpritePhysics* pGcSprPhysB );

	void IncrementLevelGoals();
	void ResetLevelGoals();

	//Controls
	void KeyboardControls();
	void ControllerControls( TGCController< GameConfig::ELevelInput > cController );

	//Method to play the background music when shadow tutorial is skipped, It should be implemented in the derived classes  
	virtual void PlayBackgroundMusic();

private:

	// Lets us know to quit or reset during update
	bool m_bResetWasRequested;
	bool m_bQuitWasRequested;

	void RequestReset()
	{
		m_bResetWasRequested = true;
	}

	void ResetRequestWasHandled()
	{
		m_bResetWasRequested = false;
	}

	bool ResetWasRequested()
	{
		return m_bResetWasRequested;
	}

	void RequestQuit()
	{
		m_bQuitWasRequested = true;
	}

	void QuitRequestWasHandled()
	{
		m_bQuitWasRequested = false;
	}

	bool QuitWasRequested()
	{
		return m_bQuitWasRequested;
	}
};

#endif
