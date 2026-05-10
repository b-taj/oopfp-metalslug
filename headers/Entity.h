#pragma once
#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/RenderWindow.hpp>

/**
 * Entity.h -- Absolute root of the project hierarchy.
 * Manages spatial properties and lifecycle status without visual data.
 */

class Entity
{
public:
	virtual ~Entity() = default;

	virtual void	update(float dt) = 0;
	virtual void	draw(sf::RenderWindow& window, float camOffsetX, float camOffsetY) = 0;

	sf::FloatRect	getBounds() const;
	
	float			getX() const;
	float			getY() const;
	float			getWidth() const;
	float			getHeight() const;
	bool			isActive() const;

	void			setPosition(float nx, float ny);
	void			setActive(bool a);

protected:
	float			x;
	float			y;
	float			width;
	float			height;
	bool			active;
};
