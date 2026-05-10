#define _CRT_SECURE_NO_WARNINGS
#include "../headers/ScoreManager.h"
#include <fstream>
#include <cstring>

ScoreManager::ScoreManager() : score(0), highScoreCount(0)
{
	for (int i = 0; i < 10; ++i) {
		highScores[i] = 0;
		std::memset(highScoreNames[i], 0, 32);
	}
}

void ScoreManager::addScore(int pts) { score += pts; }
int ScoreManager::getScore() const { return score; }

void ScoreManager::recordKill(Enemy* e, bool isMelee, bool isAerial, int multiKillCount)
{
	if (!e) return;
	
	int pts = e->getScoreValue();
	
	if (isMelee) pts += 50;
	if (isAerial) pts += 100;
	
	if (multiKillCount == 2) {
		pts += 200;
	} else if (multiKillCount >= 3) {
		pts += 300 + (multiKillCount - 3) * 50;
	}
	
	addScore(pts);
}

void ScoreManager::recordLevelClear(bool isSurvival, bool isFlawless)
{
	if (isSurvival) addScore(1000);
	else addScore(3000); // Campaign
	
	if (isFlawless) addScore(5000);
}

void ScoreManager::addToLeaderboard(const char* name, int newScore)
{
	// Find insertion point
	int pos = -1;
	for (int i = 0; i < 10; ++i) {
		if (newScore > highScores[i]) {
			pos = i;
			break;
		}
	}

	if (pos != -1) {
		// Shift down
		for (int i = 9; i > pos; --i) {
			highScores[i] = highScores[i - 1];
			for (int j = 0; j < 31 && highScoreNames[i - 1][j] != '\0'; j++) {
				highScoreNames[i][j] = highScoreNames[i - 1][j];
				highScoreNames[i][j + 1] = '\0';
			}
		}
		// Insert
		highScores[pos] = newScore;
		for (int j = 0; j < 31 && name[j] != '\0'; j++) {
			highScoreNames[pos][j] = name[j];
			highScoreNames[pos][j + 1] = '\0';
		}
		if (highScoreCount < 10) highScoreCount++;
	}
}

void ScoreManager::saveLeaderboard(const char* path)
{
	std::ofstream ofs(path, std::ios::binary);
	if (ofs.is_open()) {
		ofs.write((char*)&highScoreCount, sizeof(int));
		for (int i = 0; i < 10; ++i) {
			ofs.write((char*)&highScores[i], sizeof(int));
			ofs.write(highScoreNames[i], 32);
		}
		ofs.close();
	}
}

void ScoreManager::loadLeaderboard(const char* path)
{
	std::ifstream ifs(path, std::ios::binary);
	if (ifs.is_open()) {
		ifs.read((char*)&highScoreCount, sizeof(int));
		for (int i = 0; i < 10; ++i) {
			ifs.read((char*)&highScores[i], sizeof(int));
			ifs.read(highScoreNames[i], 32);
		}
		ifs.close();
	}
}

void ScoreManager::drawLeaderboard(sf::RenderWindow& w, sf::Font& font)
{
	sf::Text text;
	text.setFont(font);
	text.setCharacterSize(24);
	text.setFillColor(sf::Color::White);

	for (int i = 0; i < highScoreCount; ++i) {
		char line[64];
		std::snprintf(line, sizeof(line), "%d. %s - %d", i + 1, highScoreNames[i], highScores[i]);
		text.setString(line);
		text.setPosition(600.0f, 200.0f + i * 30.0f);
		w.draw(text);
	}
}
