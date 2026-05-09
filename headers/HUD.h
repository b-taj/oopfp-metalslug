#pragma once
#include <SFML/Graphics.hpp>

/**
 * HUD.h -- On-screen display for player stats and game info.
 * Includes a full-screen damage flash overlay.
 */

class HUD
{
public:
	HUD();

	void	init();
	void	update(int hp, int maxHp, int score, const char* name, int ammo, int grenades, float dt);
	void	draw(sf::RenderWindow& w);
	void	triggerDamageFlash();

private:
	sf::Font	font;
	sf::Text	hpText, scoreText, nameText, ammoText, grenadeText;

	sf::RectangleShape	healthBar;
	sf::RectangleShape	healthBarBG;
	
	sf::RectangleShape	damageOverlay;
	float				damageFlashTimer;
};
