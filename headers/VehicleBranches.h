#pragma once
#include "Vehicle.h"
#include "Constants.h"

// ==================== GROUND VEHICLE ====================
class GroundVehicle : public Vehicle {
public:
    virtual ~GroundVehicle() = default;
    void update(float dt) override {
        Vehicle::update(dt);
        if (!onGround) velocityY += GRAVITY * dt;
        // Ground resolution logic similar to Soldier
    }
protected:
    bool onGround = false;
    bool canCrouch = true;
    float tiltAngle = 0.0f;
};

// ==================== AERIAL VEHICLE ====================
class AerialVehicle : public Vehicle {
public:
    virtual ~AerialVehicle() = default;
    void update(float dt) override {
        // Free 2D movement, no gravity
        Vehicle::update(dt);
    }
};

// ==================== AQUATIC VEHICLE ====================
class AquaticVehicle : public Vehicle {
public:
    virtual ~AquaticVehicle() = default;
    void update(float dt) override {
        // Buoyancy / Water physics
        Vehicle::update(dt);
        velocityY += (GRAVITY * 0.2f) * dt; 
    }
};
