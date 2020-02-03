#ifndef _IR_GAMECONFIG_H_
#define _IR_GAMECONFIG_H_


namespace GameConfig
{

	//////////////////////////////////////////////////////////////////////////
	// UI
	//////////////////////////////////////////////////////////////////////////

	// Introduction text animation speed given as 1 / characters per second
	const static float	k_fIntroTextSpeed =			1 / 11.1f;

	// Cut scene text animation speed given as 1 / characters per second
	const static float	k_fCutSceneTextSpeed =		1 / 17.0f;





	//////////////////////////////////////////////////////////////////////////
	// CONTROLLER
	//////////////////////////////////////////////////////////////////////////
	
	// Deadzones
	const static float	k_fLookDeadzone				= 0000.10f;
	const static float	k_fMovementDeadzone			= 0000.50f;

	// Clamping
	const static float	k_f45DegreeClampingFactor	= 0000.45f;





	//////////////////////////////////////////////////////////////////////////
	// PLAYER
	//////////////////////////////////////////////////////////////////////////

	// Player movement values
	const static float	k_fPlayerWalkForce			= 3500.00f;
	const static float	k_fPlayerWalkDragForce		= 0250.00f;
	const static float	k_fPlayerJumpForce			= 5500.00f;
	const static float	k_fPlayerJumpDragForce		= 0150.00f;
	const static float	k_fPlayerMinJumpHeight		= 0160.00f;
	const static float	k_fPlayerMaxJumpHeight		= 0220.00f;
	const static int	k_iPlayerJumps				= 0001;
	const static int	k_iMaxNumberOfJumps			= 0001;

	// Player health values
	const static int	k_iPlayerLives				= 0003;
	const static float	k_iPlayerMaxHealth			= 0003;

	// Player weapon damage values
	const static float	k_fStunShotDamage			= 0000.00f;
	const static float	k_fOverchargeDamage			= 0100.00f;

	// Player weapon fire rate values (time between shots)
	const static float	k_fStunFireRate				= 0005.00f;
	const static float	k_fOverchargeFireRate		= 0001.00f;

	// Player weapon muzzle velocity values
	const static float	k_fStunMuzzleVelocity		= 00020.00f;
	const static float	k_fOverchargeMuzzleVelocity	= 00020.00f;

	// Player weapon shot lifetime
	const static float	k_fStunShotLifetime			= 0000.90f;
	const static float	k_fChargeShotLifetime		= 0003.00f;

	// How long weapon pickups last
	const static float	k_fOverchargePickupLifeTime	= 0005.00f;
	




	//////////////////////////////////////////////////////////////////////////
	// ENEMY
	//////////////////////////////////////////////////////////////////////////
	
	// Enemy movement values
	const static float	k_fR1WalkForce				= 0360.00f;
	const static float	k_fR1WalkDrag				= 0050.00f;
	const static float	k_fR2WalkForce				= 0360.00f;
	const static float	k_fR2WalkDrag				= 0050.00f;
	const static float	k_fR3WalkForce				= 0360.00f;
	const static float	k_fR3WalkDrag				= 0050.00f;

	// Enemy health values
	const static float	k_fR1Health					= 0100.00f;
	const static float	k_fR2Health					= 0100.00f;
	const static float	k_fR3Health					= 0100.00f;
	const static float	k_fBionicHealth				= 0100.00f;

	// Enemy immobilizations time when hit with a taser
	const static float	k_fR1TaseImmobiliseTime		= 0003.00f;
	const static float	k_fR2TaseImmobiliseTime		= 0003.00f;
	const static float	k_fR3TaseImmobiliseTime		= 0003.00f;
	const static float	k_fBionicTaseImmobiliseTime = 0003.00f;

	// Enemy contact damage values
	const static int	k_iR1ContactDamage			= 0001;
	const static int	k_iR2ContactDamage			= 0001;
	const static int	k_iR3ContactDamage			= 0001;
	const static int	k_iBionicContactDamage		= 0001;

	// Enemy weapon damage values
	const static int	k_iR1ShotDamage				= 0000;
	const static int	k_iR2ShotDamage				= 0001;
	const static int	k_iR3ShotDamage				= 0002;
	const static int	k_iBionicShotDamage			= 0002;
	
	// Enemy weapon speed values
	const static float	k_fR1ShotMuzzleVelocity		= 0000.00f;
	const static float	k_fR2ShotMuzzleVelocity		= 0020.00f;
	const static float	k_fR3ShotMuzzleVelocity		= 0005.00f;
	const static float	k_fBionicShotMuzzleVelocity	= 0010.00f;

	// Enemy weapon shot lifetime
	const static float	k_fR1ShotLifetime			= 0000.00f;
	const static float	k_fR2ShotLifetime			= 0001.50f;
	const static float	k_fR3ShotLifetime			= 0010.00f;
	const static float	k_fBionicLifetime			= 0010.00f;


	// Enemy weapon fire rate values (time between shots)
	const static float	k_fR1FireRate				= 0000.00f;
	const static float	k_fR2FireRate				= 0001.00f;
	const static float	k_fR3FireRate				= 0001.00f;
	const static float	k_fBionicFireRate			= 0005.00f;

	// The power of the knockback force applied when the player hits an enemy
	const static float	k_fEnemyKnockbackForce		= 0025.00f;





	//////////////////////////////////////////////////////////////////////////
	// ENUMS
	//////////////////////////////////////////////////////////////////////////

	// Z Index Layer order
	enum class EZIndexLayer
	{
		EZIL_TOP = 0, // Do not re-order this
		EZIL_Tutorial,
		EZIL_HUD_Top,
		EZIL_Player,
		EZIL_Shadow,
		EZIL_HUD_Bottom,
		EZIL_Interface_Top,
		EZIL_Interface_Effect,
		EZIL_Interface_Button,
		EZIL_Interface_Page,
		EZIL_Interface_Menu,
		EZIL_Interface_Bottom,
		EZIL_Enemy,
		EZIL_Terminal,
		EZIL_Platform,
		EZIL_Background
	};

	// Player actions
	enum class EPlayerActions
	{
		EPA_NULL = 0,
		EPA_Up,
		EPA_Down,
		EPA_Left,
		EPA_Right,
		EPA_Jump,
		EPA_Fire,
		EPA_Hack,
		EPA_Options,
		EPA_Fire_Axis,
		EPA_AxisMove_X,
		EPA_AxisMove_Y
	};

	//Controller input to bring out pause menu
	//
	enum class ELevelInput
	{
		ELI_Option = 0,
		ELI_ShadowSkip
	};
	// Controller Inputs for Menu Scene	
	enum class EInputs
	{
		EI_Up ,
		EI_Down ,
		EI_AxisY,
		EI_Select,
		EI_Back,
		EI_AxisX,
		EI_Left,
		EI_Right
	};
	
	// Ammo types
	enum EWeaponType
	{
		EWT_Stun = 0,
		EWT_Overcharge,
		EWT_R2,
		EWT_R3
	};
	
	enum class EBackgroundSounds
	{
		EBS_Level1 = 0 ,
		EBS_Level2,
		EBS_Level3,
		EBS_Level4,
		EBS_Level5,
		EBS_Level6,
		EBS_Level7,
		EBS_Level8,
		EBS_Level9,
		EBS_Level10,
		EBS_Level11,
		EBS_Level12,
		EBS_Menu,
		EBS_Credits,
		EBS_End
	};

	//enum class ESoundEffects
	//{
	//	ESE_DanielleMove = 0,
	//	ESE_DanielleJump,
	//	ESE_DanielleJumpLand,
	//	ESE_ShadowShoot,
	//	ESE_R1Move,
	//	ESE_R2Move,
	//	ESE_R3Move,
	//	ESE_B1Move,
	//	ESE_RobotDeath,
	//	ESE_DanielleHack,
	//	ESE_DanielleHackPass,
	//	ESE_DanielleHackFail,
	//	ESE_End
	//	
	//};
	//
	enum class EMusic
	{
		EM_Act1 = 0 ,
		EM_Act1Timer,
		EM_Act2,
		EM_Act2Timer,
		EM_Act3,
		EM_Act3Timer,
		EM_Act4,
		EM_Act4Timer,
		EM_DanielleMove,
		EM_DanielleJump,
		EM_DanielleJumpLand,
		EM_ShadowShoot,
		EM_R1Move,
		EM_R2Move,
		EM_R3Move,
		EM_B1Move,
		EM_RobotDeath,
		EM_DanielleHack,
		EM_DanielleHackPass,
		EM_DanielleHackFail,
		EM_DanielleHurt,
		EM_DanielleDie,
		EM_MenuBack,
		EM_MenuDown,
		EM_MenuUp,
		EM_MenuOK,
		EM_Geiger,
		EM_Fade,
		EM_DoorOpen,
		EM_DoorClosed,
		EM_Spaceship,
		EM_ShadowCharge,
		EM_CrumblingPlatformBreak,
		EM_End
	};






	//////////////////////////////////////////////////////////////////////////
	// Event Names
	//////////////////////////////////////////////////////////////////////////

	//Add any events you want, just ensure iterator_end remains the last one.
	enum EEventType
	{
		EET_HackSucceed = 0,
		EET_HackFail,
		EET_GainedObjective,
		EET_DeactivateEnemies,
		EET_ReturnToMenu,
		EET_ActivateMovingPlatform,
		EET_ControlDisable,
		EET_ControlEnable,
		EET_LevelCompleted,
		EET_TimerFinished,
		EET_LivesOver,
		iterator_end
	};

	// Audio ID
	static int g_MenuMusicID = 0;
}

#endif