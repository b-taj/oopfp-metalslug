#pragma once
#include "Enums.h"
#include <fstream>

/**
 * BehaviorChain.h -- Markov chain modeling of BehaviorState.
 */

class BehaviorChain
{
public:
	BehaviorChain();
	
	BehaviorState	inferState(float* inputs, int n);
	BehaviorState	predictNext();
	void			updateTransitions(BehaviorState from, BehaviorState to);
	BehaviorState	getGapState();
	
	void			saveHistory(const char* file);
	void			loadHistory(const char* file);

private:
	float			transitionMatrix[5][5];
	BehaviorState	currentState;
	float			historyWindow;
	float			sessionDecay;
};
