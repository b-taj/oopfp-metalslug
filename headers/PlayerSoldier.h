#pragma once
#include "Soldier.h"
#include <SFML/Window/Keyboard.hpp>
#include <SFML/Window/Mouse.hpp>

/**
 * PlayerSoldier.h -- Specialized soldier that reads user input.
 */

class PlayerSoldier : public Soldier
{
public:
	virtual ~PlayerSoldier() = default;

	void	update(float dt) override;
	void	handleInput(float dt);

protected:
	PlayerSoldier();

	float	specialTimer;
	bool	specialActive;
};
