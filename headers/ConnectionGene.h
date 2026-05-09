#pragma once

/**
 * ConnectionGene.h -- Represents a single connection between two nodes in NEAT.
 */

struct ConnectionGene
{
	int		inNode;
	int		outNode;
	float	weight;
	bool	enabled;
	int		innovation;
};
