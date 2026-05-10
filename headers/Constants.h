#pragma once

/**
 * Constants.h -- Global compile-time constants for engine configuration.
 * No STL dependencies.
 */

constexpr int SCREEN_W = 1600;
constexpr int SCREEN_H = 900;
constexpr int CELL_SIZE = 64;

constexpr int MAX_ENEMIES = 64;
constexpr int MAX_PROJECTILES = 512;
constexpr int MAX_COLLECTIBLES = 64;
constexpr int MAX_ENTITIES = 256;

constexpr int MAX_SPECIES = 50;
constexpr int MAX_GENOME_NODES = 100;
constexpr int MAX_GENOME_CONNS = 500;
constexpr int MAX_POPULATION = 200;

constexpr float GRAVITY = 900.0f;
constexpr float JUMP_FORCE = -750.0f;

constexpr int LEVEL_WIDTH = 200;
constexpr int LEVEL_HEIGHT = 20;
