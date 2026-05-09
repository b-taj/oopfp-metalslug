#pragma once
#include "Players.h"
#include "ReflexFingerprint.h"
#include <SFML/Graphics/Color.hpp>

/**
 * GhostPlayer.h -- Autopilot player using ReflexFingerprint.
 */

class GhostPlayer : public PlayerSoldier
{
public:
	GhostPlayer();

	void	update(float dt) override;
	void	draw(sf::RenderWindow& w, float camOffsetX, float camOffsetY) override;
	void	takeDamage(int dmg) override;
	
	void	loadProfile(const char* path);

private:
	ReflexFingerprint	fingerprint;
	float				alpha;
	float				damageReduction;
	float				hpMultiplier;
};
