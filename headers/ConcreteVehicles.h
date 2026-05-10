#pragma once
#include "VehicleBranches.h"
#include "StraightProjectile.h"
#include "BallisticProjectile.h"

// ==================== METAL SLUG ====================
class MetalSlug : public GroundVehicle {
public:
    MetalSlug() { 
        bombAmmo = 10; 
        loadTexture("Sprites/4x upscaled/Metal Slug_4x.png");
        animator.setScale(0.25f, 0.25f);

        int fw = 400; int fh = 400;

        SpriteAnimator::Frame moveFrames[] = {
            {{0, 0, fw, fh}, 0.1f}, {{fw, 0, fw, fh}, 0.1f}, {{fw*2, 0, fw, fh}, 0.1f}
        };
        animator.addAnimation("move", moveFrames, 3, true);

        SpriteAnimator::Frame shootFrames[] = {
            {{0, fh, fw, fh}, 0.05f}, {{fw, fh, fw, fh}, 0.05f}
        };
        animator.addAnimation("shoot", shootFrames, 2, false);

        SpriteAnimator::Frame deathFrames[] = {
            {{0, fh*2, fw, fh}, 0.15f}, {{fw, fh*2, fw, fh}, 0.15f}, {{fw*2, fh*2, fw, fh}, 0.15f}
        };
        animator.addAnimation("explode", deathFrames, 3, false);
    }
    Projectile* fire() override {
        animator.play("shoot");
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
