#pragma once

/**
 * Enums.h -- Project-wide enumerations for state management and type identification.
 * Follows a library-free approach.
 */

enum class CharacterState 
{ 
    NORMAL, 
    INJURED, 
    CRITICAL, 
    UNDEAD, 
    MUMMY, 
    DEAD 
};

enum class EnemyAIMode 
{ 
    PATROL, 
    ATTACK, 
    GRUDGE, 
    DEAD 
};

enum class ProjectileType 
{ 
    STRAIGHT, 
    BALLISTIC 
};

enum class WeaponType 
{ 
    PISTOL, 
    HMG, 
    ROCKET_LAUNCHER, 
    FLAME_SHOT, 
    LASER_GUN, 
    KNIFE 
};

enum class VehicleForm 
{ 
    GROUND, 
    AERIAL, 
    AQUATIC 
};

enum class BossPhase 
{ 
    GROUND, 
    AERIAL, 
    AQUATIC, 
    ULTIMATE 
};

enum class GameStateID 
{ 
    MENU, 
    PLAY, 
    PAUSE, 
    GAME_OVER, 
    SELF_PLAY 
};

enum class BiomeType 
{ 
    AERIAL, 
    PLAINS, 
    AQUATIC 
};

enum class CollectibleType 
{ 
    FOOD_TURKEY, 
    FOOD_FRUIT, 
    POW_PRISONER, 
    SUPPLY_CRATE 
};

enum class BlockType 
{ 
    AIR, 
    GRASS, 
    STONE, 
    WATER, 
    INDESTRUCTIBLE 
};

enum class NodeType
{
    INPUT,
    HIDDEN,
    OUTPUT,
    BIAS
};
