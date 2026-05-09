#pragma once
#include "ProjectileWeapon.h"
#include "StraightProjectile.h"
#include "BallisticProjectile.h"
#include "Constants.h"
#include "SoundManager.h"
#include <cmath>

/**
 * Concrete Weapons for Metal Slug VS.
 */

// ==================== PISTOL ====================
class Pistol : public ProjectileWeapon {
public:
    Pistol() { infiniteAmmo = true; damage = 3; fireRate = 4.0f; }
    Projectile* fire(float x, float y, float angle, SoundManager* sm) override {
        if (sm) sm->play("shoot_pistol");
        fireTimer = 0.0f;
        float rad = angle * 3.14159f / 180.0f;
        return new StraightProjectile(x, y, std::cos(rad)*1200.0f, std::sin(rad)*1200.0f, damage, true);
    }
    WeaponType getType() const override { return WeaponType::PISTOL; }
};

// ==================== HEAVY MACHINE GUN ====================
class HeavyMachineGun : public ProjectileWeapon {
public:
    HeavyMachineGun() { ammo = 200; damage = 3; fireRate = 8.0f; }
    Projectile* fire(float x, float y, float angle, SoundManager* sm) override {
        if (sm) sm->play("shoot_hmg");
        fireTimer = 0.0f; ammo--;
        float rad = angle * 3.14159f / 180.0f;
        return new StraightProjectile(x, y, std::cos(rad)*1500.0f, std::sin(rad)*1500.0f, damage, true);
    }
    WeaponType getType() const override { return WeaponType::HMG; }
};

// ==================== ROCKET LAUNCHER ====================
class RocketLauncher : public ProjectileWeapon {
public:
    RocketLauncher() { ammo = 30; damage = 5; fireRate = 0.5f; }
    Projectile* fire(float x, float y, float angle, SoundManager* sm) override {
        if (sm) sm->play("shoot_rocket");
        fireTimer = 0.0f; ammo--;
        float rad = angle * 3.14159f / 180.0f;
        BallisticProjectile* p = new BallisticProjectile(x, y, std::cos(rad)*800.0f, std::sin(rad)*800.0f, damage, true, nullptr);
        p->setExplosive(true, 3.0f * CELL_SIZE);
        p->setSoundManager(sm);
        return p;
    }
    WeaponType getType() const override { return WeaponType::ROCKET_LAUNCHER; }
};

// ==================== FLAME SHOT ====================
class FlameShot : public ProjectileWeapon {
public:
    FlameShot() { ammo = 100; damage = 2; fireRate = 10.0f; }
    Projectile* fire(float x, float y, float angle, SoundManager* sm) override {
        if (sm) sm->play("shoot_flame");
        fireTimer = 0.0f; ammo--;
        float rad = angle * 3.14159f / 180.0f;
        return new StraightProjectile(x, y, std::cos(rad)*600.0f, std::sin(rad)*600.0f, damage, true);
    }
    WeaponType getType() const override { return WeaponType::FLAME_SHOT; }
};

// ==================== LASER GUN ====================
class LaserGun : public ProjectileWeapon {
public:
    LaserGun() { ammo = 30; damage = 100; fireRate = 2.0f; }
    Projectile* fire(float x, float y, float angle, SoundManager* sm) override {
        if (sm) sm->play("shoot_laser");
        fireTimer = 0.0f; ammo--;
        float rad = angle * 3.14159f / 180.0f;
        return new StraightProjectile(x, y, std::cos(rad)*5000.0f, std::sin(rad)*5000.0f, damage, true);
    }
    WeaponType getType() const override { return WeaponType::LASER_GUN; }
};

// ==================== KNIFE (Melee) ====================
class Knife : public Weapon {
public:
    Knife() { infiniteAmmo = true; damage = 2; fireRate = 2.0f; }
    Projectile* fire(float x, float y, float angle, SoundManager* sm) override {
        (void)x; (void)y; (void)angle;
        if (sm) sm->play("shoot_knife");
        fireTimer = 0.0f;
        return nullptr; 
    }
    WeaponType getType() const override { return WeaponType::KNIFE; }
};
