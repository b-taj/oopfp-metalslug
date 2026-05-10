#pragma once
#include "Collectible.h"
#include "InteractableObject.h"
#include "Soldier.h"
#include "ConcreteWeapons.h"

// ==================== FOOD ====================
class Food : public Collectible {
public:
    Food(CollectibleType type) {
        ctype = type;
        saturationRestore = (type == CollectibleType::FOOD_TURKEY) ? 3 : 2;
    }
    void onPickup(Soldier* s) override { s->heal(saturationRestore); }
private:
    int saturationRestore;
};

// ==================== SUPPLY CRATE ====================
class SupplyCrate : public InteractableObject {
public:
    SupplyCrate() {
        rollContents();
    }
    void onInteract(Soldier* s) override {
        // Simple logic for now: give weapon if better or reload
        s->pickupWeapon(containedWeapon); // Simplification of swap logic
        active = false;
    }
    void rollContents() {
        int r = rand() % 100;
        if (r < 10) containedWeapon = new LaserGun();
        else if (r < 40) containedWeapon = new HeavyMachineGun();
        else if (r < 70) containedWeapon = new RocketLauncher();
        else containedWeapon = new FlameShot();
    }
private:
    Weapon* containedWeapon;
};

// ==================== POW PRISONER ====================
class POWPrisoner : public InteractableObject {
public:
    void onInteract(Soldier* s) override {
        (void)s;
        // Logic to spawn SupplyCrate would go here, requiring EntityManager access
        active = false;
    }
};
