#pragma once
#include "Enemy.h"
#include "ConcreteWeapons.h"
#include <iostream>

// ==================== REBEL SOLDIER ====================
class RebelSoldier : public Enemy {
public:
    RebelSoldier() {
        hp = 2; maxHp = 2; scoreValue = 50;
        weapon = new Pistol();
    }
    void onDeath() override { std::cout << "RebelSoldier down. Score +50\n"; }
    void shootPlayer(class Soldier* player) override { /* Firing logic... */ }
};

// ==================== SHIELDED SOLDIER ====================
class ShieldedSoldier : public Enemy {
public:
    ShieldedSoldier() : shieldIntact(true) {
        hp = 5; maxHp = 5; scoreValue = 75;
    }
    void takeDamage(int dmg) override {
        if (shieldIntact && facingRight) { /* Block from front logic */ return; }
        Enemy::takeDamage(dmg);
    }
    void takeDamageFromAbove(int dmg) { shieldIntact = false; Enemy::takeDamage(dmg); }
    void onDeath() override { std::cout << "ShieldedSoldier down.\n"; }
    void shootPlayer(class Soldier* player) override {}
private:
    bool shieldIntact;
};

// ==================== BAZOOKA SOLDIER ====================
class BazookaSoldier : public Enemy {
public:
    BazookaSoldier() {
        hp = 2; maxHp = 2; scoreValue = 100;
        weapon = new RocketLauncher();
    }
    void onDeath() override { std::cout << "Bazooka down.\n"; }
    void shootPlayer(class Soldier* player) override {}
};

// ==================== GRENADE SOLDIER ====================
class GrenadeSoldier : public Enemy {
public:
    GrenadeSoldier() {
        hp = 2; maxHp = 2; scoreValue = 100;
    }
    void onDeath() override { std::cout << "Grenadier down.\n"; }
    void shootPlayer(class Soldier* player) override { /* Fire BallisticProjectile... */ }
};

// ==================== ZOMBIE ====================
#include "UndeadState.h"
class Zombie : public Enemy {
public:
    Zombie() {
        hp = 5; maxHp = 5; scoreValue = 100;
        weapon = new Pistol();
    }
    void update(float dt, class Soldier* player) override {
        Enemy::update(dt, player);
        // Contact infection logic handled by EntityManager usually, 
        // but hooks are here.
    }
    void onDeath() override { std::cout << "Zombie neutralized.\n"; }
    void shootPlayer(class Soldier* player) override {}
};

// ==================== MUMMY WARRIOR ====================
#include "MummyState.h"
class MummyWarrior : public Enemy {
public:
    MummyWarrior() : isCrumbled(false), resurrectTimer(3.0f) {
        hp = 5; maxHp = 5; scoreValue = 150;
    }
    void takeDamage(int dmg) override {
        // Only die from fire damage (hypothetically dmg > 50 for now)
        if (dmg > 50) Enemy::takeDamage(dmg);
        else crumble();
    }
    void crumble() { isCrumbled = true; }
    void onDeath() override { std::cout << "Mummy incinerated.\n"; }
    void shootPlayer(class Soldier* player) override {}
private:
    bool isCrumbled;
    float resurrectTimer;
};
