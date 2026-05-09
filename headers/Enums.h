#pragma once
// Enums.h -- All project-wide enumerations shared across the class hierarchy.

// Injury / transformation state of a playable character.
enum class CharacterState
{
	NORMAL,
	INJURED,
	CRITICAL,
	UNDEAD,       // -50% walk speed for 10 s
	MUMMY,        // knife-only for 10 s
	DEAD
};

// Tile types used in the 64x64 block grid.
enum class BlockType
{
	GRASS,
	STONE,
	WATER,
	DIRT,
	INDESTRUCTIBLE,  // permanent bottom row
	AIR
};

// Distinguishes projectile behaviour on impact.
enum class ProjectileType
{
	BULLET,
	ROCKET,
	FLAME_STREAM,
	LASER_BEAM,
	ENERGY_BEAM,
	CANNONBALL
};

// Active phase for UltimateBoss polymorphic dispatch.
enum class BossPhase
{
	GROUND,
	AERIAL,
	AQUATIC
};

// Active form of the AmphibiousSlug vehicle.
enum class VehicleForm
{
	GROUND_SLUG,
	FLYER,
	MARINER
};

// Role of a neuron node inside a NEAT genome.
enum class NodeType
{
	INPUT,
	HIDDEN,
	OUTPUT,
	BIAS
};

// Abstract behavioural states used by BehaviorChain (Chains of Death).
enum class BehaviorState
{
	PRESSING,
	CONSOLIDATING,
	RETREATING,
	BURNING_RESOURCES,
	STALLING
};

// Direction of a SlugMariner missile salvo.
enum class MissileType
{
	HORIZONTAL,
	VERTICAL,
	REVERSE
};

// Food collectible sub-type controlling saturation restored.
enum class FoodType
{
	TURKEY,   // restores 3 saturation units
	FRUIT     // restores 2 saturation units
};
