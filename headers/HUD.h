#pragma once
#include <SFML/Graphics.hpp>

class Character;

// HUD.h -- Heads-up display drawn on top of the game world each frame.
// Shows health bar, ammo count, grenade count, score, and character name.

class HUD
{
public:
	// Initialize fonts and default positions
	void	init();

	void	draw(sf::RenderWindow& window);	// draw all elements in screen space
	void	update(int hp, int score, const char* name);	// refresh values

	// Trigger a translucent red hue overlay for 1 second (damage feedback).
	// Note: Will use a Sprite with a red texture since RectangleShape is banned.
	void	showDamageFlash();

	// Display a centred level message (e.g. "LEVEL CLEAR") briefly.
	// CHANGED: Uses const char* because std::string is NOT allowed.
	void	showLevelMessage(const char* msg);

private:
	sf::Font	font;
	sf::Sprite	healthBar;
	sf::Text	ammoText;
	sf::Text	grenadeText;
	sf::Text	scoreText;
	sf::Text	hpText;
	sf::Text	nameText;
	sf::Sprite	damageOverlay;	// full-screen red hue sprite
	sf::Texture	hudTexture;
};
