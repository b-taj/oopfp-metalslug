#pragma once
#include "Entity.h"

/**
 * DamagableEntity.h -- Specialized entity that maintains health and mortality.
 * Forces subclasses to implement damage and death logic.
 */

class DamagableEntity : public Entity
{
public:
	virtual ~DamagableEntity() = default;

	virtual void	takeDamage(int dmg) = 0;
	virtual void	die() = 0;

	int				getHp() const;
	int				getMaxHp() const;
	bool			isDead() const;
	
	void			heal(int amount);

protected:
	int				hp;
	int				maxHp;
	int				damageLevel; // 0=fine, 1=hurt, 2=critical
};
