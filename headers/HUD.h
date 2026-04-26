#pragma once
#include <SFML/Graphics.hpp>


class Character;

// HUD.h -- Heads-up display drawn on top of the game world each frame.
// Shows health bar, ammo count, grenade count, score, and damage flash overlay.

class HUD
{
public:
	void	draw(sf::RenderWindow& window);	// draw all elements in screen space
	void	update(Character* character);	// refresh values from active character

	// Trigger a translucent red hue overlay for 1 second (damage feedback).
	void	showDamageFlash();

	// Display a centred level message (e.g. "LEVEL CLEAR") briefly.
	void	showLevelMessage(const std::string& msg);

private:
	sf::Font	font;
	sf::Sprite	healthBar;
	sf::Text	ammoText;
	sf::Text	grenadeText;
	sf::Text	scoreText;
	sf::Sprite	damageOverlay;	// full-screen red hue sprite
	sf::Texture	hudTexture;
};
