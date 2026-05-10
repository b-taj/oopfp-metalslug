#pragma once
#include "TransformationState.h"

/**
 * UndeadState.h -- Handles the Zombie transformation.
 * Restricts speed by 50% for a fixed duration.
 */

class UndeadState : public TransformationState
{
public:
	UndeadState();
	void		onEnter(class Soldier* s) override;
	void		update(class Soldier* s, float dt) override;
	void		onExit(class Soldier* s) override;
	const char* getName() const override;

private:
	float		timer;
};
