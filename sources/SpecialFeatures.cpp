#include "../headers/ScoreManager.h"
#include "../headers/Entity.h"
#include "../headers/Soldier.h"
#include "../headers/Enemy.h"
#include <cmath>
#include <cstring>

class Soldier;
class Enemy;

// ... existing ScoreManager implementation ...

void ScoreManager::updateExpectation()
{
	if (midpointCount == 0) return;
	float sum = 0;
	for (int i = 0; i < midpointCount; i++) sum += historicMidpoints[i];
	expectedScore = sum / midpointCount;
}

void ScoreManager::recordDeath(Soldier* s, float time)
{
	if (deathCount >= 50) return;
	DeathFingerprint& df = deathHistory[deathCount++];
	df.hp = s->getHp();
	df.posX = s->getX();
	df.posY = s->getY();
	df.timeSinceCheckpoint = time;
	const char* currentWep = "CURRENT";
	for (int i = 0; i < 31 && currentWep[i] != '\0'; i++) {
		df.weapon[i] = currentWep[i];
		df.weapon[i + 1] = '\0';
	}
}

bool ScoreManager::matchesDeathPattern(Soldier* s)
{
	for (int i = 0; i < deathCount; i++) {
		float dx = s->getX() - deathHistory[i].posX;
		float dy = s->getY() - deathHistory[i].posY;
		float dhp = (float)(s->getHp() - deathHistory[i].hp);
		float dist = std::sqrt(dx*dx + dy*dy + dhp*dhp);
		
		if (dist < 100.0f) return true; // Euclidean pattern match
	}
	return false;
}
