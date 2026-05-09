#pragma once
#include "VehicleBranches.h"
#include "StraightProjectile.h"
#include "BallisticProjectile.h"

// ==================== METAL SLUG ====================
class MetalSlug : public GroundVehicle {
public:
    MetalSlug() { bombAmmo = 10; }
    Projectile* fire() override {
        return new StraightProjectile(x, y, (facingRight?1:-1)*1500.0f, 0, 3, true);
    }
    Projectile* fireCannon() {
        if (bombAmmo > 0) {
            bombAmmo--;
            return new BallisticProjectile(x, y, (facingRight?1:-1)*600.0f, -400.0f, 20, true, nullptr);
        }
        return nullptr;
    }
    VehicleForm getForm() const override { return VehicleForm::GROUND; }
};

// ==================== SLUG FLYER ====================
class SlugFlyer : public AerialVehicle {
public:
    Projectile* fire() override {
        return new StraightProjectile(x, y, (facingRight?1:-1)*750.0f, 0, 3, true); // Half speed
    }
    VehicleForm getForm() const override { return VehicleForm::AERIAL; }
};

// ==================== AMPHIBIOUS SLUG ====================
class AmphibiousSlug : public Vehicle {
public:
    AmphibiousSlug() {
        groundSlug = new MetalSlug();
        flyer = new SlugFlyer();
        currentForm = VehicleForm::GROUND;
    }
    ~AmphibiousSlug() {
        delete groundSlug;
        delete flyer;
    }
    Projectile* fire() override {
        if (currentForm == VehicleForm::GROUND) return groundSlug->fire();
        return flyer->fire();
    }
    VehicleForm getForm() const override { return currentForm; }

private:
    MetalSlug* groundSlug;
    SlugFlyer* flyer;
    VehicleForm currentForm;
};
