#include "../headers/HUD.h"
#include "../headers/Constants.h"
#include <cstdio>

HUD::HUD() : damageFlashTimer(0.0f), isBossActive(false)
{
	damageOverlay.setSize(sf::Vector2f((float)SCREEN_W, (float)SCREEN_H));
	damageOverlay.setFillColor(sf::Color(255, 0, 0, 0));
}

void HUD::init()
{
	if (font.loadFromFile("Sprites/font.ttf")) {
		scoreText.setFont(font);
		nameText.setFont(font);
		ammoText.setFont(font);
		grenadeText.setFont(font);

		scoreText.setCharacterSize(24);
		nameText.setCharacterSize(30);
		nameText.setOutlineThickness(2.0f);
		nameText.setOutlineColor(sf::Color::Black);
		nameText.setFillColor(sf::Color::Yellow);

		ammoText.setCharacterSize(24);
		grenadeText.setCharacterSize(24);

		hpBg.setSize(sf::Vector2f(200.f, 18.f));
		hpBg.setPosition(10.f, 10.f);
		hpBg.setFillColor(sf::Color(60, 60, 60));

		hpFill.setSize(sf::Vector2f(200.f, 18.f));
		hpFill.setPosition(10.f, 10.f);

		bossHpBg.setSize(sf::Vector2f(600.f, 20.f));
		bossHpBg.setPosition(500.f, 10.f);
		bossHpBg.setFillColor(sf::Color(80, 0, 0));

		bossHpFill.setSize(sf::Vector2f(600.f, 20.f));
		bossHpFill.setPosition(500.f, 10.f);
		bossHpFill.setFillColor(sf::Color(220, 20, 20));
	}
}

void HUD::update(int hp, int maxHp, int score, const char* name, int ammo, int grenades, float dt, int bossHp, int bossMaxHp, bool bossActive)
{
	char buffer[64];
	
	nameText.setString(name);
	nameText.setPosition(10.f, SCREEN_H - 45.f);

	std::snprintf(buffer, sizeof(buffer), "SCORE: %06d", score);
	scoreText.setString(buffer);
	scoreText.setPosition(SCREEN_W - scoreText.getLocalBounds().width - 10.f, 10.f);

	std::snprintf(buffer, sizeof(buffer), "AMMO: %d", ammo);
	ammoText.setString(buffer);
	ammoText.setPosition(10.f, SCREEN_H - 100.f);

	std::snprintf(buffer, sizeof(buffer), "GRENADES: %d", grenades);
	grenadeText.setString(buffer);
	grenadeText.setPosition(10.f, SCREEN_H - 130.f);

	// Player HP Bar Logic
	float ratio = (float)hp / (float)maxHp;
	if (ratio < 0) ratio = 0;
	hpFill.setSize(sf::Vector2f(200.f * ratio, 18.f));
	sf::Uint8 r = (sf::Uint8)(255.f * (1.0f - ratio));
	sf::Uint8 g = (sf::Uint8)(255.f * ratio);
	hpFill.setFillColor(sf::Color(r, g, 0));

	// Boss HP Bar Logic
	isBossActive = bossActive;
	if (isBossActive && bossMaxHp > 0) {
		float bRatio = (float)bossHp / (float)bossMaxHp;
		if (bRatio < 0) bRatio = 0;
		bossHpFill.setSize(sf::Vector2f(600.f * bRatio, 20.f));
	}

	if (damageFlashTimer > 0) {
		damageFlashTimer -= dt;
		if (damageFlashTimer < 0) damageFlashTimer = 0;
		sf::Uint8 alpha = (sf::Uint8)((damageFlashTimer / 1.0f) * 120.f);
		damageOverlay.setFillColor(sf::Color(255, 0, 0, alpha));
	}
}

void HUD::draw(sf::RenderWindow& w)
{
	w.draw(hpBg);
	w.draw(hpFill);
	w.draw(nameText);
	w.draw(scoreText);
	w.draw(ammoText);
	w.draw(grenadeText);

	if (isBossActive) {
		w.draw(bossHpBg);
		w.draw(bossHpFill);
	}

	if (damageFlashTimer > 0) {
		w.draw(damageOverlay);
	}
}

void HUD::triggerDamageFlash() { damageFlashTimer = 1.0f; }
