#pragma once
#include <SFML/Graphics.hpp>
#include "Enums.h"
#include "Weapon.h"
#include "Grenade.h"

class Character;

// Collectible.h -- Abstract base for all world items the player can pick up.
// Sub-classes implement onPickup() to apply their specific effect.

class Collectible
{
public:
	virtual ~Collectible() = default;

	virtual void	onPickup(Character* c) = 0;

	void			update(float dt);
	void			draw(sf::RenderWindow& window);
	sf::FloatRect	getBounds();

protected:
	float		x;
	float		y;
	sf::Sprite	sprite;
	sf::Texture	texture;
	bool		isPickedUp;
};

// Turkey (+3 saturation) or Fruit (+2 saturation).
// Small drop chance when any enemy is killed.
class Food : public Collectible
{
public:
	void	onPickup(Character* c) override;

private:
	FoodType	foodType;
	int			saturationRestore;	// 3 = TURKEY | 2 = FRUIT
};

// Prisoner held by Morden's army. Releasing on proximity key-press drops a SupplyCrate.
class POWPrisoner : public Collectible
{
public:
	void	onPickup(Character* c) override;	// spawns a SupplyCrate at this position
	bool	checkProximity(Character* c);		// true when player is within releaseRadius

private:
	float	releaseRadius;
};

// Contains a weapon (90% HMG/Flame/Rocket | 10% Laser) + 3-5 grenades.
// Player can swap their current weapon or just top up ammo.
class SupplyCrate : public Collectible
{
public:
	void	onPickup(Character* c) override;
	void	offerWeaponSwap(Character* c);	// show prompt; apply chosen option

private:
	Weapon*				weapon;
	int				weaponAmmo;
	std::vector<Grenade*>	grenades;	// 3-5 (HandGrenade or FireBombGrenade for Eri)

	void	rollContents();		// randomise contents on spawn
};
