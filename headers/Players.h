#pragma once
#include "PlayerSoldier.h"

class PlayerMarco : public PlayerSoldier {
public:
    PlayerMarco() {
        hp = 100; speedX = 200.0f; grenadeCount = 10;
        loadTexture("Sprites/4x upscaled/Marco Rossi_4x.png");
        animator.setScale(0.25f, 0.25f);

        int fw = 300; int fh = 300;

        SpriteAnimator::Frame idleFrames[] = { {{0, 0, fw, fh}, 0.1f} };
        animator.addAnimation("idle", idleFrames, 1, true);

        SpriteAnimator::Frame walkFrames[] = {
            {{0, fh, fw, fh}, 0.1f}, {{fw, fh, fw, fh}, 0.1f}, {{fw*2, fh, fw, fh}, 0.1f},
            {{fw*3, fh, fw, fh}, 0.1f}, {{fw*4, fh, fw, fh}, 0.1f}, {{fw*5, fh, fw, fh}, 0.1f}
        };
        animator.addAnimation("walk", walkFrames, 6, true);

        SpriteAnimator::Frame jumpFrames[] = { {{0, fh*2, fw, fh}, 0.1f} };
        animator.addAnimation("jump", jumpFrames, 1, true);

        SpriteAnimator::Frame shootFrames[] = { {{0, fh*3, fw, fh}, 0.05f}, {{fw, fh*3, fw, fh}, 0.05f} };
        animator.addAnimation("shoot", shootFrames, 2, false);

        SpriteAnimator::Frame dieFrames[] = {
            {{0, fh*4, fw, fh}, 0.12f}, {{fw, fh*4, fw, fh}, 0.12f}, 
            {{fw*2, fh*4, fw, fh}, 0.12f}, {{fw*3, fh*4, fw, fh}, 0.12f}
        };
        animator.addAnimation("die", dieFrames, 4, false);
    }
    void activateSpecialPower() override {}
    float getFireRate() const override { return 1.25f; }
    const char* getName() const override { return "MARCO"; }
};

class PlayerTarma : public PlayerSoldier {
public:
    PlayerTarma() {
        hp = 80; speedX = 160.0f;
        loadTexture("Sprites/4x upscaled/Tarma Roving_4x.png");
        animator.setScale(0.25f, 0.25f);
        
        // Use Marco's mapping as baseline for now
        int fw = 300; int fh = 300;
        SpriteAnimator::Frame f = { {0, 0, fw, fh}, 0.1f };
        animator.addAnimation("idle", &f, 1, true);
        animator.addAnimation("walk", &f, 1, true);
        animator.addAnimation("jump", &f, 1, true);
        animator.addAnimation("die", &f, 1, false);
    }
    void activateSpecialPower() override {}
    float getFireRate() const override { return 1.0f; }
    const char* getName() const override { return "TARMA"; }
};

class PlayerEri : public PlayerSoldier {
public:
    PlayerEri() {
        grenadeCount = 20;
        loadTexture("Sprites/4x upscaled/Eri Kasamoto_4x.png");
        animator.setScale(0.25f, 0.25f);

        int fw = 300; int fh = 300;
        SpriteAnimator::Frame f = { {0, 0, fw, fh}, 0.1f };
        animator.addAnimation("idle", &f, 1, true);
        animator.addAnimation("walk", &f, 1, true);
        animator.addAnimation("jump", &f, 1, true);
        animator.addAnimation("die", &f, 1, false);
    }
    void activateSpecialPower() override {}
    float getFireRate() const override { return 0.8f; }
    const char* getName() const override { return "ERI"; }
};

class PlayerFio : public PlayerSoldier {
public:
    PlayerFio() {
        grenadeCount = 8;
        loadTexture("Sprites/4x upscaled/Fiolina Germi_4x.png");
        animator.setScale(0.25f, 0.25f);

        int fw = 300; int fh = 300;
        SpriteAnimator::Frame f = { {0, 0, fw, fh}, 0.1f };
        animator.addAnimation("idle", &f, 1, true);
        animator.addAnimation("walk", &f, 1, true);
        animator.addAnimation("jump", &f, 1, true);
        animator.addAnimation("die", &f, 1, false);
    }
    void activateSpecialPower() override {}
    float getFireRate() const override { return 1.1f; }
    const char* getName() const override { return "FIO"; }
};
