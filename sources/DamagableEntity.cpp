#include "../headers/DamagableEntity.h"

int DamagableEntity::getHp() const { return hp; }
int DamagableEntity::getMaxHp() const { return maxHp; }
bool DamagableEntity::isDead() const { return hp <= 0; }

void DamagableEntity::heal(int amount)
{
	hp += amount;
	if (hp > maxHp) hp = maxHp;
	
	// Recalculate damage level
	float ratio = (float)hp / maxHp;
	if (ratio > 0.6f) damageLevel = 0;
	else if (ratio > 0.3f) damageLevel = 1;
}
