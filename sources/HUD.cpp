#include "../headers/HUD.h"
#include <iostream>

// Helper to convert integer to string without using STL <string>
void intToString(int n, char* str) {
    int i = 0;
    if (n == 0) {
        str[i++] = '0';
        str[i] = '\0';
        return;
    }
    int temp = n;
    int len = 0;
    while (temp > 0) {
        temp /= 10;
        len++;
    }
    for (i = len - 1; i >= 0; i--) {
        str[i] = (n % 10) + '0';
        n /= 10;
    }
    str[len] = '\0';
}

void HUD::init()
{
    if (font.loadFromFile("Sprites/font.ttf")) {
        hpText.setFont(font);
        hpText.setCharacterSize(30);
        hpText.setFillColor(sf::Color::Red);
        hpText.setPosition(20, 20);

        nameText.setFont(font);
        nameText.setCharacterSize(30);
        nameText.setFillColor(sf::Color::White);
        nameText.setPosition(20, 60);

        scoreText.setFont(font);
        scoreText.setCharacterSize(30);
        scoreText.setFillColor(sf::Color::Yellow);
        scoreText.setPosition(1350, 20);
    }
}

void HUD::update(int hp, int score, const char* name)
{
    char hpBuf[32] = "HP: ";
    char hpNum[16];
    intToString(hp, hpNum);
    
    // Manual cat
    int idx = 4;
    for(int i=0; hpNum[i] != '\0'; i++) hpBuf[idx++] = hpNum[i];
    hpBuf[idx] = '\0';
    hpText.setString(hpBuf);

    char scoreBuf[64] = "SCORE: ";
    char scoreNum[16];
    intToString(score, scoreNum);
    
    idx = 7;
    for(int i=0; scoreNum[i] != '\0'; i++) scoreBuf[idx++] = scoreNum[i];
    scoreBuf[idx] = '\0';
    scoreText.setString(scoreBuf);

    nameText.setString(name);
}

void HUD::draw(sf::RenderWindow& window)
{
    window.draw(hpText);
    window.draw(nameText);
    window.draw(scoreText);
}

void HUD::showDamageFlash() {}
void HUD::showLevelMessage(const char* msg) { (void)msg; }
