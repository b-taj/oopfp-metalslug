#pragma once
#include <SFML/Graphics.hpp>

/**
 * HUD.h -- On-screen display for player stats and game info.
 * Enhanced with graphical HP bars and dynamic scoring.
 */

class HUD
{
public:
	HUD();

	void	init();
	void	update(int hp, int maxHp, int score, const char* name, int ammo, int grenades, float dt, int bossHp = 0, int bossMaxHp = 0, bool bossActive = false);
	void	draw(sf::RenderWindow& w);
	void	triggerDamageFlash();

private:
	sf::Font	font;
	sf::Text	scoreText, nameText, ammoText, grenadeText;

	sf::RectangleShape	hpBg, hpFill;
	sf::RectangleShape	bossHpBg, bossHpFill;
	
	sf::RectangleShape	damageOverlay;
	float				damageFlashTimer;

	bool	isBossActive;
};
