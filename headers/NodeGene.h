#pragma once
#include "Enums.h"
#include <cmath>

/**
 * NodeGene.h -- Represents a neuron in the NEAT network.
 */

struct NodeGene
{
	int			id;
	NodeType	type;

	// Steepened sigmoid from Stanley & Miikkulainen 2002
	float activate(float x) const {
		return 1.0f / (1.0f + std::exp(-4.9f * x));
	}
};
