#pragma once

/**
 * TransformationState.h -- Abstract base for the State Pattern.
 * Decouples transformation logic (Zombie, Mummy) from the main Soldier class.
 */

class TransformationState
{
public:
	virtual ~TransformationState() = default;

	virtual void		onEnter(class Soldier* s) = 0;
	virtual void		update(class Soldier* s, float dt) = 0;
	virtual void		onExit(class Soldier* s) = 0;
	virtual const char* getName() const = 0;
};
