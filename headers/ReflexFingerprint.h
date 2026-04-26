#pragma once
#include <SFML/Graphics.hpp>

#include "Enums.h"

// ReflexFingerprint.h -- Records and replays a player's behavioural style.
//
// All 260 member variables match the 'parameters.csv' schema exactly (PSP_001 - PSP_260).
// Naming convention: CSV parameter_name converted to camelCase.
//   e.g.  base_walk_speed_ratio -> baseWalkSpeedRatio
//
// The file produced by saveToFile() is the canonical cross-device exchange format.
// A GhostPlayer can load it on another device and replicate the exact style.

class ReflexFingerprint
{
public:
	// Observe one frame of player input; update running statistics.
	void	record(int inputCode);

	// Write all 260 parameters to a CSV file matching parameters.csv schema.
	void	saveToFile(const std::string& path);

	// Load a previously saved fingerprint (enables cross-device ghost import).
	void	loadFromFile(const std::string& path);

	// Return the predicted next action code based on accumulated statistics.
	int		getNextAction();

	// Drive the character automatically for 'duration' seconds using this fingerprint.
	void	activateAutopilot(float duration);

	// ── All 260 parameters (PSP_001 - PSP_260) ──────────────────────────

	// ─── METADATA ──────────────────────────────────────────────────────
	std::string	profileVersion;		// [PSP_001] Schema version for cross-device compatibility validation
	std::string	playerAlias;		// [PSP_002] Display name for ghost player label
	int			sessionCount;		// [PSP_003] Number of sessions this profile was sampled over
	float		totalPlaytimeMinutes;		// [PSP_004] Total recorded playtime used to build this fingerprint
	std::string	profileChecksum;		// [PSP_005] CRC32 checksum for file integrity validation on upload
	long long	captureTimestamp;		// [PSP_006] int
	std::string	gameVersion;		// [PSP_007] Game build this profile was recorded on
	float		sampleQualityScore;		// [PSP_260] Confidence score for this profile's accuracy (higher = more data)

	// ─── MOVEMENT ──────────────────────────────────────────────────────
	float		baseWalkSpeedRatio;		// [PSP_008] Proportion of time player walks at full speed vs slowed
	float		runEngagementThreshold;		// [PSP_009] Likelihood to run toward enemies rather than hold ground
	float		crouchFrequency;		// [PSP_010] How often the player crouches during active combat
	float		crouchDurationAvgMs;		// [PSP_011] Average duration of a single crouch event
	float		jumpFrequency;		// [PSP_012] How often the player jumps per 10-second window
	float		jumpAttackRatio;		// [PSP_013] Proportion of jumps that are accompanied by a shot or melee
	float		backpedalTendency;		// [PSP_014] Tendency to move backward while firing
	float		lateralOscillationRate;		// [PSP_015] How often player side-steps repeatedly to dodge projectiles
	float		screenEdgeAvoidance;		// [PSP_016] Tendency to stay away from left and right screen edges
	float		verticalLayerPreference;		// [PSP_017] 0=prefers ground; 1=prefers elevated platforms
	float		proneUsageRatio;		// [PSP_018] How often player goes prone (down+crouch) during combat
	float		idleDurationAvgMs;		// [PSP_019] Average time player stays stationary mid-level
	float		directionChangeRate;		// [PSP_020] How frequently movement direction changes per second
	float		stairClimbSpeedRatio;		// [PSP_021] Proportion of max speed used while climbing stairs/ladders
	float		ladderCombatRatio;		// [PSP_022] How often player shoots while on a ladder
	float		forwardLeanAggression;		// [PSP_023] Tendency to advance toward the right side of the screen
	float		retreatTriggerHealthPct;		// [PSP_024] Health percentage at which player starts retreating
	float		terrainHugTendency;		// [PSP_025] How often player moves along terrain contours vs open air jumps
	float		inputHoldVsTapRatio;		// [PSP_201] Ratio of held directional inputs vs tapped inputs
	float		turnAroundFrequency;		// [PSP_202] How often player reverses facing direction per minute
	float		diagonalMovementRatio;		// [PSP_203] How often player moves diagonally vs in cardinal directions
	float		airTimePerJumpMs;		// [PSP_204] Average duration of airborne phase per jump
	float		stopBeforeShootRatio;		// [PSP_205] How often player stops moving before firing
	float		jumpCancelRatio;		// [PSP_251] How often player cancels a jump by crouching mid-air (where applicable)

	// ─── COMBAT_TIMING ──────────────────────────────────────────────────────
	float		shotBurstLengthAvg;		// [PSP_026] Average number of shots fired in a single burst
	float		burstIntervalAvgMs;		// [PSP_027] Average pause between shot bursts
	float		triggerHoldRatio;		// [PSP_028] Proportion of time trigger is held vs tapped
	float		firstShotReactionMs;		// [PSP_029] Reaction time from enemy spawn to first shot fired
	float		grenadeThrowFrequency;		// [PSP_030] Grenade throws per 10-second window
	float		grenadeAimDurationMs;		// [PSP_031] Average time spent aiming before throwing grenade
	float		meleeFrequency;		// [PSP_032] How often knife/melee is used per 10-second window
	float		meleeRangeThresholdPx;		// [PSP_033] Pixel distance at which player switches to melee
	float		shotAngleDiagonalRatio;		// [PSP_034] Proportion of shots fired at 45-degree angles
	float		shotAngleUpwardRatio;		// [PSP_035] Proportion of shots aimed upward
	float		shotAngleDownwardRatio;		// [PSP_036] Proportion of shots aimed downward
	float		continuousFireDurationAvgMs;		// [PSP_037] Average duration of continuous fire events
	float		reloadHesitationMs;		// [PSP_038] Average delay after weapon runs out before switching
	float		overkillTendency;		// [PSP_039] How often player continues firing after enemy is dead
	float		preemptiveShotRatio;		// [PSP_040] Shots fired before enemy appears on screen (anticipatory)
	float		grenadeVsGunPreference;		// [PSP_041] 0=guns only; 1=grenades only
	float		comboActionIntervalMs;		// [PSP_042] Time between chained actions in a combat sequence
	float		evasionBeforeShotRatio;		// [PSP_043] How often player evades before returning fire
	float		suppressionFireRatio;		// [PSP_044] Proportion of shots used to suppress rather than kill
	float		shotCadenceVariance;		// [PSP_045] Variance in timing between individual shots (0=metronome
	float		shotTrackingAccuracy;		// [PSP_206] Accuracy of tracking moving targets with fire
	float		leadShotTendency;		// [PSP_207] How often player leads moving targets (fires ahead of movement)
	float		areaDenialShotRatio;		// [PSP_208] How often player fires at locations before enemies reach them
	float		burstFirePreferredOverFullAuto;		// [PSP_209] Preference for burst fire over continuous fire
	float		hipFireRatio;		// [PSP_210] Ratio of fire while moving vs stationary
	float		aimSnapSpeedMs;		// [PSP_252] Speed of aim adjustment between two different vertical angles

	// ─── WEAPON_PREFERENCE ──────────────────────────────────────────────────────
	float		pistolUsageRatio;		// [PSP_046] Proportion of combat time spent using default pistol
	float		heavyMachineGunPreference;		// [PSP_047] Affinity for picking up and retaining the HMG
	float		rocketLauncherPreference;		// [PSP_048] Affinity for the rocket launcher
	float		flameShotPreference;		// [PSP_049] Affinity for the flame thrower weapon
	float		laserGunPreference;		// [PSP_050] Affinity for the laser gun
	float		ironLizardPreference;		// [PSP_051] Affinity for the iron lizard
	float		superGrenadePreference;		// [PSP_052] Affinity for the super grenade
	float		enemyChaserPreference;		// [PSP_053] Affinity for the enemy chaser homing weapon
	float		dropWeakWeaponThreshold;		// [PSP_054] Ammo % at which player drops a weapon in favor of pickups
	float		weaponSwitchFrequency;		// [PSP_055] How often player switches weapons per minute
	float		conserveAmmoTendency;		// [PSP_056] Tendency to switch to pistol to conserve special ammo
	float		weaponPickupHesitationMs;		// [PSP_057] Average delay before picking up a weapon item
	float		shortRangeWeaponBias;		// [PSP_058] Preference for short range weapons when available
	float		areaWeaponBias;		// [PSP_059] Preference for weapons with area-of-effect damage
	float		grenadeStockConservation;		// [PSP_060] How aggressively player conserves grenade count
	float		dualWeaponManagement;		// [PSP_211] How effectively player manages two-weapon situations
	float		incendiaryPreference;		// [PSP_212] Preference for fire-based weapons overall
	float		explosivePreference;		// [PSP_213] Preference for explosive weapons overall
	float		precisionWeaponPreference;		// [PSP_214] Preference for precision/single-shot weapons
	float		sprayWeaponPreference;		// [PSP_215] Preference for high-spread spray weapons
	float		weaponTypeEntropy;		// [PSP_253] Shannon entropy of weapon usage distribution (0=one weapon only

	// ─── POSITIONING ──────────────────────────────────────────────────────
	float		centerScreenAffinity;		// [PSP_061] How often player stays in horizontal center of screen
	float		spawnProximityComfortPx;		// [PSP_062] Preferred distance from enemy spawn points
	float		highGroundPreference;		// [PSP_063] How often player seeks elevated terrain
	float		coverUsageRatio;		// [PSP_064] How often player uses terrain as cover
	float		clusteredEnemyApproach;		// [PSP_065] 0=avoids groups; 1=charges groups
	float		rightSideAdvanceBias;		// [PSP_066] Tendency to advance on the right side of the screen
	float		bossDistancePreferencePx;		// [PSP_067] Preferred fighting distance from boss entities
	float		wallHuggingTendency;		// [PSP_068] How often player stays near a wall or screen boundary
	float		safeZoneRetreatRatio;		// [PSP_069] How often player retreats to a learned safe zone
	float		verticalJumpAimRatio;		// [PSP_070] Proportion of vertical movement used to acquire aim angle
	float		patrolRangePx;		// [PSP_071] Average horizontal range player moves within per room
	float		chokePointCampingRatio;		// [PSP_072] Tendency to wait at narrow passages for enemies
	float		vehicleBoardEagerness;		// [PSP_073] How quickly player boards available vehicles
	float		hostageProximityApproach;		// [PSP_074] How close player moves to hostages before rescuing
	float		aerialCombatRatio;		// [PSP_075] Proportion of combat occurring while airborne
	float		offCenterFightRatio;		// [PSP_216] How often player fights from off-center screen positions
	float		enemySandwichAvoidance;		// [PSP_217] How well player avoids being surrounded on both sides
	float		verticalSpreadCombatRatio;		// [PSP_218] How often player uses full vertical space during combat
	float		bottleneckCreationTendency;		// [PSP_219] Tendency to funnel enemies into narrow choke points
	float		spawnCampTendency;		// [PSP_220] How often player camps at or near enemy spawn points
	float		yAxisCombatCentroid;		// [PSP_254] Normalized vertical position of average combat engagement (0=ground

	// ─── ENEMY_ENGAGEMENT ──────────────────────────────────────────────────────
	float		smallSoldierPriority;		// [PSP_076] Priority assigned to regular foot soldiers
	float		heavySoldierPriority;		// [PSP_077] Priority assigned to heavy armored soldiers
	float		tankPriority;		// [PSP_078] Priority assigned to enemy tank units
	float		aircraftPriority;		// [PSP_079] Priority assigned to aerial enemies
	float		projectileSourcePriority;		// [PSP_080] Priority to eliminate enemies firing projectiles first
	float		nearestEnemyTargeting;		// [PSP_081] How often player targets the nearest enemy
	float		highestThreatTargeting;		// [PSP_082] How often player targets the highest-damage enemy
	float		groupClearTendency;		// [PSP_083] Preference for clearing all enemies before advancing
	float		skipMinorEnemyRatio;		// [PSP_084] How often player ignores minor enemies to advance
	float		ambushResponseTimeMs;		// [PSP_085] Reaction time to enemies appearing from behind
	float		flankingAttemptRatio;		// [PSP_086] How often player attempts to flank enemy positions
	float		aggressivePursuitRatio;		// [PSP_087] How often player chases retreating enemies
	float		enemyPatternAdaptationRate;		// [PSP_088] How quickly player adapts to enemy attack patterns (0=slow
	float		zombieEnemyHandling;		// [PSP_089] 0=avoids zombies; 1=engages directly
	float		underwaterEnemyComfort;		// [PSP_090] Combat effectiveness comfort in underwater sections
	float		bossPhaseRecognitionSpeed;		// [PSP_091] How quickly player identifies boss phase transitions
	float		multiTargetSpreadRatio;		// [PSP_092] Tendency to spread fire across multiple targets vs focus
	float		corpseAvoidanceRatio;		// [PSP_093] How often player avoids walking over enemy corpses
	float		enemyProjectileMemorization;		// [PSP_094] Degree to which player anticipates known projectile patterns
	float		turretEngagementPriority;		// [PSP_095] Priority assigned to destroying enemy turrets
	float		threatStackRankingVariance;		// [PSP_221] Consistency of target prioritization order
	float		paratrooperHandlingSpeedMs;		// [PSP_222] Response speed to paratrooper drops
	float		maskedSoldierCaution;		// [PSP_223] Increased caution when engaging masked/shielded soldiers
	float		sniperThreatAwareness;		// [PSP_224] Ability to identify and eliminate sniper threats quickly
	float		flamethrowerEnemyPriority;		// [PSP_225] Priority assigned to enemy flamethrower units
	float		overkillShotCountAvg;		// [PSP_255] Average extra shots fired after an enemy is confirmed dead

	// ─── RESOURCE_MANAGEMENT ──────────────────────────────────────────────────────
	float		grenadeEntryUsageRatio;		// [PSP_096] How often grenades are used at room entry
	float		grenadeBossReservationRatio;		// [PSP_097] Proportion of grenades reserved specifically for boss fights
	float		ammoPickupPriority;		// [PSP_098] How aggressively player seeks ammo pickups
	float		healthPickupTiming;		// [PSP_099] 0=picks up health immediately; 1=waits for critical health
	float		foodItemCollectionRatio;		// [PSP_100] Proportion of food/health items collected vs ignored
	float		scoreItemPriority;		// [PSP_101] How much player prioritizes score items over safety
	float		vehicleFuelConservation;		// [PSP_102] How efficiently player manages vehicle durability
	float		slugExitTimingRatio;		// [PSP_103] 0=stays in slug until destroyed; 1=exits before damage
	float		grenadeLobArcPreference;		// [PSP_104] 0=flat throw; 1=high arc throw
	float		itemBoxDetectionRatio;		// [PSP_105] How often player finds and opens hidden item boxes
	float		lifeRiskForItemsRatio;		// [PSP_106] How often player risks death to collect a pickup
	float		ammoLowThresholdRatio;		// [PSP_107] Ammo % at which behavior changes to conserve fire
	float		continueUsageThreshold;		// [PSP_108] Health ratio at which player accepts death vs fights on
	float		prisonerBonusAwareness;		// [PSP_109] How consistently player rescues prisoners for bonuses
	float		weaponCacheRouteBias;		// [PSP_110] How often player takes routes with weapon caches
	float		bonusScoreRouteRatio;		// [PSP_226] How often player takes bonus-score item routes
	float		medicalKitSaveRatio;		// [PSP_227] How often player saves medkits for boss fights
	float		powerGrenadeConservation;		// [PSP_228] How well player conserves super grenades vs regular
	float		slugAmmoEfficiency;		// [PSP_229] Efficiency of slug weapon ammo usage
	float		prisonerWeaponRetention;		// [PSP_230] How long player keeps prisoner-given weapons active

	// ─── VEHICLE_USAGE ──────────────────────────────────────────────────────
	float		metalSlugBoardingSpeedMs;		// [PSP_111] Time from vehicle appearance to boarding
	float		slugCannonFireRate;		// [PSP_112] How frequently slug cannon is fired vs machine gun
	float		vehicleMeleeUsageRatio;		// [PSP_113] How often vehicle stomp/melee attack is used
	float		vehicleRetreatTendency;		// [PSP_114] How often player retreats in a vehicle vs advances
	float		dismountUnderFireRatio;		// [PSP_115] Tendency to dismount vehicle when taking heavy fire
	float		camelSlugPreference;		// [PSP_116] Preference for using camel slug when available
	float		ostrichSlugPreference;		// [PSP_117] Preference for using ostrich slug when available
	float		submarineSlugComfort;		// [PSP_118] Combat effectiveness comfort in submarine sections
	float		vehicleRamTendency;		// [PSP_119] How often player uses vehicle to ram enemies directly
	float		vehicleUseDurationAvgMs;		// [PSP_120] Average time spent inside a vehicle per session
	float		slugCannonAimPrecision;		// [PSP_231] Accuracy of slug cannon shots
	float		vehicleUsedAsShieldRatio;		// [PSP_232] How often vehicle is intentionally used to absorb damage
	float		dismountCombatTransitionMs;		// [PSP_233] Speed of combat readiness after dismounting vehicle
	float		vehicleStompChainRatio;		// [PSP_234] How often vehicle stomp is chained multiple times

	// ─── HOSTAGE_BEHAVIOR ──────────────────────────────────────────────────────
	float		rescueSpeedMs;		// [PSP_121] Average time from hostage sighting to rescue
	float		rescueRouteDeviationRatio;		// [PSP_122] How far player deviates from main path to rescue hostages
	float		hostageClearFirstRatio;		// [PSP_123] How often player clears enemies before approaching hostages
	float		missedPrisonerRatio;		// [PSP_124] Average ratio of missed hostages per stage
	float		prisonerFollowExploitation;		// [PSP_125] How often player uses prisoner weapons effectively

	// ─── REACTION_PATTERN ──────────────────────────────────────────────────────
	float		dodgeRollReactionMs;		// [PSP_126] Reaction time from projectile detection to dodge
	float		jumpEvadeFrequency;		// [PSP_127] How often jump is used as evasion vs combat
	float		crouchDodgeFrequency;		// [PSP_128] How often crouch is used to dodge incoming fire
	float		patternMemorizationDepth;		// [PSP_129] How deeply player memorizes and exploits enemy patterns
	float		panicFireTendency;		// [PSP_130] Tendency to spray fire uncontrolled when health is critical
	float		screenScrollAnticipation;		// [PSP_131] How often player prepares for enemies before scroll reveals them
	float		audioCueResponsiveness;		// [PSP_132] Response rate to audio cues vs visual cues
	float		environmentalHazardAvoidance;		// [PSP_133] Accuracy of avoiding environmental hazards (fire
	float		multiThreatPrioritizationSpeedMs;		// [PSP_134] Speed at which player re-prioritizes when multiple threats appear
	float		deathAnimationInterruptRatio;		// [PSP_135] How often player fires during personal death animation

	// ─── RISK_TOLERANCE ──────────────────────────────────────────────────────
	float		healthAggressionCorrelation;		// [PSP_136] How much health loss changes aggression (0=no change
	float		lowHealthPlayChangeRatio;		// [PSP_137] Degree of behavior change at critical health
	float		grenadeSelfBlastTolerance;		// [PSP_138] Willingness to be caught in own grenade blast for kills
	float		aggressiveEntryRatio;		// [PSP_139] How often player charges into new rooms without scouting
	float		riskyPickupRatio;		// [PSP_140] How often player risks taking damage to collect items
	float		cliffEdgeComfort;		// [PSP_141] Comfort level fighting at screen/platform edges
	float		tankEngagementWillingness;		// [PSP_142] Willingness to engage armored vehicles without power weapon
	float		deathCountTolerance;		// [PSP_143] How many deaths occur before player changes strategy
	float		bossRushTendency;		// [PSP_144] How often player charges boss directly vs safe distance
	float		stageSpeedRunBias;		// [PSP_145] How much player optimizes for speed vs thorough clearing
	float		noHitAttemptRatio;		// [PSP_256] Proportion of stages where player actively attempts no-hit clear

	// ─── BOSS_STRATEGY ──────────────────────────────────────────────────────
	float		bossPreFightGrenadeDump;		// [PSP_146] How often player dumps grenades at boss fight start
	float		bossPhase1Aggression;		// [PSP_147] Aggression level during boss phase 1
	float		bossPhase2Aggression;		// [PSP_148] Aggression level during boss phase 2 (usually harder)
	float		bossSafeSpotExploitation;		// [PSP_149] How often player exploits known boss safe spots
	float		bossDpsOverSafetyRatio;		// [PSP_150] 0=prioritizes survival; 1=prioritizes damage output
	float		bossWeaponChoiceOptimality;		// [PSP_151] How well player selects optimal weapon for each boss
	float		bossPatternLoopExploitation;		// [PSP_152] How often player exploits pattern loops in boss AI
	float		bossStaggerFollowupSpeedMs;		// [PSP_153] Speed of follow-up attack after boss stagger
	float		finalBossGrenadeReserve;		// [PSP_154] Proportion of grenades saved specifically for final boss
	float		bossRetreatFrequency;		// [PSP_155] How often player retreats to reset during boss fights
	float		bossEntryItemDumpRatio;		// [PSP_235] How often all power items are used at boss room entry
	float		bossTrackingContinuity;		// [PSP_236] How consistently player maintains tracking fire on boss
	float		bossWeakPointFocusRatio;		// [PSP_237] Accuracy of targeting boss weak points
	float		bossInvulnerabilityRecognition;		// [PSP_238] How quickly player stops firing during boss invulnerability frames
	float		bossTimeoutAggressionShift;		// [PSP_257] Aggression increase when boss fight is taking too long

	// ─── EVASION ──────────────────────────────────────────────────────
	float		rollDirectionBias;		// [PSP_156] 0=always rolls left; 1=always rolls right
	float		jumpArcHeightPreference;		// [PSP_157] 0=prefers low jumps; 1=prefers max height jumps
	float		groundDodgePreference;		// [PSP_158] Preference for ground evasion vs aerial evasion
	float		doubleBackTendency;		// [PSP_159] How often player reverses direction to evade
	float		slideUnderProjectileRatio;		// [PSP_160] How often player crouches to slide under projectiles
	float		verticalEvasionRatio;		// [PSP_161] How often player uses vertical movement to evade
	float		projectileReadingAccuracy;		// [PSP_162] Accuracy of predicting and dodging enemy projectiles
	float		evasionLagMs;		// [PSP_163] Delay between dodge decision and movement execution
	float		multiDodgeChainRatio;		// [PSP_164] How often player chains multiple evasive moves together
	float		attackDuringEvasionRatio;		// [PSP_165] How often player fires while performing evasive maneuvers
	float		narrowGapEvasionRatio;		// [PSP_239] How often player evasion uses the exact narrow gap in enemy fire
	float		evasionPatternConsistency;		// [PSP_240] How consistent/predictable player evasion patterns are
	float		feintFrequency;		// [PSP_241] How often player makes false starts to bait enemy fire

	// ─── COMBO_STYLE ──────────────────────────────────────────────────────
	float		actionChainAvgLength;		// [PSP_166] Average number of actions in a chained combo
	float		moveAttackBlendRatio;		// [PSP_167] How often movement and attack actions are simultaneously executed
	float		grenadeGunComboRatio;		// [PSP_168] How often grenade throw is followed immediately by gunfire
	float		jumpShootLandComboRatio;		// [PSP_169] How often jump attack is followed by immediate landing shot
	float		meleeFollowupShotRatio;		// [PSP_170] How often melee attack is followed by a shot
	float		stutterStepRatio;		// [PSP_171] How often player alternates between moving and shooting
	float		comboResetTriggerRatio;		// [PSP_172] How often player resets combo chains mid-sequence
	float		environmentComboUsage;		// [PSP_173] How often environmental objects are included in combos
	float		rapidActionWindowMs;		// [PSP_174] Time window in which actions are considered part of same combo
	float		comboConsistencyRatio;		// [PSP_175] How consistently the same combo patterns are repeated
	float		inputOverlapRatio;		// [PSP_242] How often inputs overlap (button held during direction change)
	float		tacticalPauseInsertionRatio;		// [PSP_243] How often deliberate pauses are inserted between actions

	// ─── TERRAIN_USAGE ──────────────────────────────────────────────────────
	float		platformJumpPrecision;		// [PSP_176] Jump accuracy on precise platform sections
	float		destructibleTerrainUsage;		// [PSP_177] How often player destroys terrain for tactical advantage
	float		undergroundSectionComfort;		// [PSP_178] Combat effectiveness in underground sections
	float		slopeCombatUsage;		// [PSP_179] How often player fights on sloped terrain intentionally
	float		waterSectionCombatRatio;		// [PSP_180] Combat activity during water/underwater sections
	float		verticalSectionPreference;		// [PSP_181] Preference for vertical sections over horizontal
	float		shortcutUsageRatio;		// [PSP_182] How often player uses known terrain shortcuts
	float		narrowPassageTactics;		// [PSP_183] Tactical adjustment in narrow passages (0=no change
	float		rooftopCombatRatio;		// [PSP_184] Proportion of combat occurring on elevated surfaces
	float		terrainReadingSpeedMs;		// [PSP_185] Speed at which player identifies safe terrain routes
	float		ledgeGrabUsageRatio;		// [PSP_244] How often player uses ledge grabs for tactical repositioning
	float		ceilingFireRatio;		// [PSP_245] How often player fires upward at ceiling-mounted threats

	// ─── DEATH_RESPONSE ──────────────────────────────────────────────────────
	float		respawnAggressionLevel;		// [PSP_186] Aggression level immediately after respawn
	float		deathCauseAdaptationRatio;		// [PSP_187] How likely player is to change behavior after dying to same cause
	float		respawnPositionPreference;		// [PSP_188] 0=stays at spawn; 1=immediately advances
	float		deathTiltDurationMs;		// [PSP_189] Duration of impaired play after a death event
	float		consecutiveDeathRetreatRatio;		// [PSP_190] How much player retreats after two consecutive deaths
	float		lastLifeBehaviorChange;		// [PSP_191] Degree of behavior change on final life (0=none
	float		revengeAggressionSpike;		// [PSP_192] Aggression spike after dying to a specific enemy
	float		deathZoneAvoidanceRatio;		// [PSP_193] How reliably player avoids zones where they previously died
	float		rageQuitIndicator;		// [PSP_258] Normalized score for session abandonment after consecutive deaths

	// ─── SESSION_RHYTHM ──────────────────────────────────────────────────────
	float		stageOpeningAggression;		// [PSP_194] Aggression level at the start of each stage
	float		midStageEnergyDip;		// [PSP_195] Tendency for activity to drop in mid-stage sections
	float		stageFinalPushRatio;		// [PSP_196] Increase in aggression when near end of stage
	float		restIntervalAvgMs;		// [PSP_197] Average pause duration between active combat engagements
	float		consistencyAcrossStages;		// [PSP_198] How consistently player applies same strategy across stages
	float		learningRateIntraSession;		// [PSP_199] How much strategy improves within a single play session
	float		fatigueDegradationRatio;		// [PSP_200] Performance drop over time in a session (0=none
	float		attentionDecayRate;		// [PSP_246] Rate of performance decay over a long session
	float		bossApproachTensionIndex;		// [PSP_247] Change in play style as player approaches a boss zone
	float		recoveryTimeAfterHitMs;		// [PSP_248] Time to resume normal play pattern after taking damage
	float		stageCompletionSpeedVariance;		// [PSP_249] Variance in stage completion times across sessions
	float		overallAggressionIndex;		// [PSP_250] Composite weighted aggression score across all sessions
	float		stageSelectRepeatBias;		// [PSP_259] How often player replays same stages vs progresses linearly
};

// ─────────────────────────────────────────────────────────────────────────────

// Semi-transparent AI player that follows a loaded ReflexFingerprint.
// 80% less damage dealt; 300% more HP than a regular player.
// Spawns alongside the main player when a fingerprint file is imported.
class GhostPlayer
{
public:
	void	update(float dt);				// advance AI using loaded fingerprint
	void	draw(sf::RenderWindow& window);	// translucent overlay render
	void	loadProfile(const std::string& path);	// load fingerprint and configure stats

private:
	ReflexFingerprint*	fingerprint;		// owned
	float			alpha;			// transparency (0.0 - 1.0)
	float			damageReduction;	// damage dealt x 0.20
	float			hpMultiplier;		// max HP x 3.00
	sf::Sprite		sprite;
	sf::Texture		texture;
};

// ─────────────────────────────────────────────────────────────────────────────

// Tracks abstract behavioural state transitions ('Chains of Death').
// Maintains a Markov-like transition probability matrix updated across sessions.
// The enemy spawner reads predictNext() to anticipate and exploit the player's moves.
class BehaviorChain
{
public:
	BehaviorState	inferState(std::vector<float> inputs);	// classify last 4 s of input
	BehaviorState	predictNext();					// most likely next state
	void		updateTransitions(BehaviorState from, BehaviorState to);
	BehaviorState	getGapState();					// state the player avoids (exploitable)
	void		saveHistory(const std::string& file);
	void		loadHistory(const std::string& file);

private:
	std::vector<BehaviorState>		states;
	std::vector<std::vector<float>>	transitionMatrix;	// [from][to] probabilities
	BehaviorState				currentState;
	float					historyWindow;	// 4.0 s considered per inference
	float					sessionDecay;	// weight given to older sessions
};
