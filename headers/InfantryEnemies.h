#pragma once
#include "Enemy.h"
#include "ConcreteWeapons.h"
#include <iostream>

class RebelSoldier : public Enemy {
public:
    RebelSoldier() {
        hp = 2; maxHp = 2; scoreValue = 50;
        weapon = new Pistol();
        loadTexture("Sprites/4x upscaled/Rebel Soldier_4x.png");
        animator.setScale(0.25f, 0.25f);

        int fw = 200; int fh = 200; // Smaller soldier frames

        SpriteAnimator::Frame walkFrames[] = {
            {{0, 0, fw, fh}, 0.12f}, {{fw, 0, fw, fh}, 0.12f}, 
            {{fw*2, 0, fw, fh}, 0.12f}, {{fw*3, 0, fw, fh}, 0.12f}
        };
        animator.addAnimation("walk", walkFrames, 4, true);

        SpriteAnimator::Frame atkFrames[] = {
            {{0, fh, fw, fh}, 0.08f}, {{fw, fh, fw, fh}, 0.08f}, {{fw*2, fh, fw, fh}, 0.08f}
        };
        animator.addAnimation("attack", atkFrames, 3, false);

        SpriteAnimator::Frame dieFrames[] = {
            {{0, fh*2, fw, fh}, 0.1f}, {{fw, fh*2, fw, fh}, 0.1f}, {{fw*2, fh*2, fw, fh}, 0.1f},
            {{fw*3, fh*2, fw, fh}, 0.1f}, {{fw*4, fh*2, fw, fh}, 0.1f}
        };
        animator.addAnimation("die", dieFrames, 5, false);
    }
    void onDeath() override { std::cout << "RebelSoldier down. Score +50\n"; }
    void shootPlayer(class Soldier* player) override { (void)player; }
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
