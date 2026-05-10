#pragma once
#include "TransformationState.h"

/**
 * MummyState.h -- Handles the Mummy transformation.
 * Forces the soldier into knife-only combat for a fixed duration.
 */

class MummyState : public TransformationState
{
public:
	MummyState();
	void		onEnter(class Soldier* s) override;
	void		update(class Soldier* s, float dt) override;
	void		onExit(class Soldier* s) override;
	const char* getName() const override;

private:
	float		timer;
};
