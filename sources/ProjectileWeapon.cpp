#include "../headers/ProjectileWeapon.h"

void ProjectileWeapon::loadBulletTexture(const char* path)
{
	bulletTex.loadFromFile(path);
}
