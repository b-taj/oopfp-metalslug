#include "../headers/HUD.h"
#include "../headers/Constants.h"
#include <cstdio>

HUD::HUD() : damageFlashTimer(0.0f)
{
	damageOverlay.setSize(sf::Vector2f((float)SCREEN_W, (float)SCREEN_H));
	damageOverlay.setFillColor(sf::Color(255, 0, 0, 0));
}

void HUD::init()
{
	if (font.loadFromFile("Sprites/font.ttf")) {
		hpText.setFont(font);
		scoreText.setFont(font);
		nameText.setFont(font);
		ammoText.setFont(font);
		grenadeText.setFont(font);

		hpText.setCharacterSize(24);
		scoreText.setCharacterSize(24);
		nameText.setCharacterSize(30);
		ammoText.setCharacterSize(24);
		grenadeText.setCharacterSize(24);

		nameText.setPosition(20, 20);
		hpText.setPosition(20, 60);
		scoreText.setPosition(SCREEN_W - 200, 20);
		ammoText.setPosition(20, SCREEN_H - 100);
		grenadeText.setPosition(20, SCREEN_H - 60);

		healthBarBG.setSize(sf::Vector2f(200, 20));
		healthBarBG.setFillColor(sf::Color(50, 50, 50));
		healthBarBG.setPosition(100, 65);

		healthBar.setSize(sf::Vector2f(200, 20));
		healthBar.setFillColor(sf::Color::Red);
		healthBar.setPosition(100, 65);
	}
}

void HUD::update(int hp, int maxHp, int score, const char* name, int ammo, int grenades, float dt)
{
	char buffer[64];
	
	std::snprintf(buffer, sizeof(buffer), "NAME: %s", name);
	nameText.setString(buffer);

	std::snprintf(buffer, sizeof(buffer), "HP: %d/%d", hp, maxHp);
	hpText.setString(buffer);

	std::snprintf(buffer, sizeof(buffer), "SCORE: %06d", score);
	scoreText.setString(buffer);

	std::snprintf(buffer, sizeof(buffer), "AMMO: %d", ammo);
	ammoText.setString(buffer);

	std::snprintf(buffer, sizeof(buffer), "GRENADES: %d", grenades);
	grenadeText.setString(buffer);

	float hpRatio = (float)hp / maxHp;
	if (hpRatio < 0) hpRatio = 0;
	healthBar.setSize(sf::Vector2f(200.0f * hpRatio, 20.0f));

	if (damageFlashTimer > 0) {
		damageFlashTimer -= dt;
		if (damageFlashTimer < 0) damageFlashTimer = 0;
		int alpha = (int)((damageFlashTimer / 1.0f) * 128);
		damageOverlay.setFillColor(sf::Color(255, 0, 0, alpha));
	}
}

void HUD::draw(sf::RenderWindow& w)
{
	w.draw(nameText);
	w.draw(scoreText);
	w.draw(hpText);
	w.draw(healthBarBG);
	w.draw(healthBar);
	w.draw(ammoText);
	w.draw(grenadeText);

	if (damageFlashTimer > 0) {
		w.draw(damageOverlay);
	}
}

void HUD::triggerDamageFlash()
{
	damageFlashTimer = 1.0f;
}
