#pragma once
#include "ProjectileWeapon.h"
#include <cmath>

/**
 * Concrete Weapons for Metal Slug VS.
 */

#include "StraightProjectile.h"
#include "BallisticProjectile.h"
#include "Constants.h"

// Note: To support BallisticProjectile::explode(), we assume EntityManager is accessible.
// For now, these create the objects. EntityManager injection will be handled in the Game loop.

class Pistol : public ProjectileWeapon {
public:
    Pistol() { infiniteAmmo = true; damage = 3; fireRate = 4.0f; }
    Projectile* fire(float x, float y, float angle) override {
        fireTimer = 0.0f;
        float rad = angle * 3.14159f / 180.0f;
        return new StraightProjectile(x, y, std::cos(rad)*1200.0f, std::sin(rad)*1200.0f, damage, true);
    }
    WeaponType getType() const override { return WeaponType::PISTOL; }
};

class HeavyMachineGun : public ProjectileWeapon {
public:
    HeavyMachineGun() { ammo = 200; damage = 3; fireRate = 8.0f; }
    Projectile* fire(float x, float y, float angle) override {
        fireTimer = 0.0f; ammo--;
        float rad = angle * 3.14159f / 180.0f;
        return new StraightProjectile(x, y, std::cos(rad)*1500.0f, std::sin(rad)*1500.0f, damage, true);
    }
    WeaponType getType() const override { return WeaponType::HMG; }
};

class RocketLauncher : public ProjectileWeapon {
public:
    RocketLauncher() { ammo = 30; damage = 5; fireRate = 0.5f; }
    Projectile* fire(float x, float y, float angle) override {
        fireTimer = 0.0f; ammo--;
        float rad = angle * 3.14159f / 180.0f;
        // EntityManager* em = ... // Passed via context in actual usage
        BallisticProjectile* p = new BallisticProjectile(x, y, std::cos(rad)*800.0f, std::sin(rad)*800.0f, damage, true, nullptr);
        p->setExplosive(true, 3.0f * CELL_SIZE);
        return p;
    }
    WeaponType getType() const override { return WeaponType::ROCKET_LAUNCHER; }
};

// ==================== FLAME SHOT ====================
class FlameShot : public ProjectileWeapon {
public:
    FlameShot() {
        ammo = 100;
        damage = 2;
        fireRate = 10.0f;
    }
    Projectile* fire(float x, float y, float angle) override {
        fireTimer = 0.0f; ammo--;
        Projectile* p = new Projectile();
        p->setPosition(x, y);
        float rad = angle * 3.14159f / 180.0f;
        p->velX = std::cos(rad) * 600.0f;
        p->velY = std::sin(rad) * 600.0f;
        p->damage = damage;
        p->type = ProjectileType::STRAIGHT;
        return p;
    }
    WeaponType getType() const override { return WeaponType::FLAME_SHOT; }
};

// ==================== LASER GUN ====================
class LaserGun : public ProjectileWeapon {
public:
    LaserGun() {
        ammo = 30;
        damage = 100;
        fireRate = 2.0f;
    }
    Projectile* fire(float x, float y, float angle) override {
        fireTimer = 0.0f; ammo--;
        Projectile* p = new Projectile();
        p->setPosition(x, y);
        float rad = angle * 3.14159f / 180.0f;
        p->velX = std::cos(rad) * 5000.0f;
        p->velY = std::sin(rad) * 5000.0f;
        p->damage = damage;
        p->type = ProjectileType::STRAIGHT;
        return p;
    }
    WeaponType getType() const override { return WeaponType::LASER_GUN; }
};

// ==================== KNIFE (Melee) ====================
#include "Weapon.h"
class Knife : public Weapon {
public:
    Knife() {
        infiniteAmmo = true;
        damage = 2;
        fireRate = 2.0f;
    }
    Projectile* fire(float x, float y, float angle) override {
        (void)x; (void)y; (void)angle;
        fireTimer = 0.0f;
        return nullptr; // Melee logic handled in update
    }
    WeaponType getType() const override { return WeaponType::KNIFE; }
};
