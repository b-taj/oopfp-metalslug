#pragma once
#include "TransformationState.h"

/**
 * NormalState.h -- The default state of a soldier.
 * No speed penalties or combat restrictions.
 */

class NormalState : public TransformationState
{
public:
	void		onEnter(class Soldier* s) override;
	void		update(class Soldier* s, float dt) override;
	void		onExit(class Soldier* s) override;
	const char* getName() const override;
};
