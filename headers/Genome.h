#pragma once
#include "NodeGene.h"
#include "ConnectionGene.h"
#include "NeuralNetwork.h"
#include "Constants.h"

/**
 * Genome.h -- The genotype representing a network topology.
 */

class Genome
{
public:
	Genome();
	
	void			mutateWeights();
	void			mutateAddConnection(int innovation);
	void			mutateAddNode(int& innovCounter);
	
	NeuralNetwork	buildNetwork() const;
	Genome*			clone() const;

	NodeGene		nodeGenes[MAX_GENOME_NODES];
	int				nodeCount;
	
	ConnectionGene	connGenes[MAX_GENOME_CONNS];
	int				connCount;
	
	float			fitness;
	float			adjustedFitness;
	int				speciesId;
};
