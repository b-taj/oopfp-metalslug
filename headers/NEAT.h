#pragma once

#include "Enums.h"

// NEAT.h -- NeuroEvolution of Augmenting Topologies
// Removed restricted libraries (vector, map, string).

class ConnectionGene
{
public:
	void	disable();			
	void	perturbWeight(float amount);	

	int		inNode;
	int		outNode;
	float	weight;
	bool	enabled;		
	int		innovation;		
};

class NodeGene
{
public:
	float	activate(float x);

	int		id;
	NodeType	type;
};

class NeuralNetwork
{
public:
	// Use arrays instead of std::vector
	void	feedforward(float* inputs, int inputCount, float* outputs, int outputCount);
	float	activate(int nodeId);		
	float	getOutput(int idx);		

private:
	float	nodeValues[128]; // Fixed size
	int		nodeIds[128];
	int		totalNodes;
	
	ConnectionGene connections[256];
	int		connectionCount;
};

class Genome
{
public:
	void	mutateWeights();		
	void	mutateAddConnection(int innovation);	
	void	mutateAddNode(int innovation);		

	void	buildNetwork(NeuralNetwork& net);

	NodeGene		nodeGenes[128];
	int				nodeGeneCount;
	ConnectionGene	connGenes[256];
	int				connGeneCount;
	
	float					fitness;
	float					adjustedFitness;	
	int					speciesId;
};

class Species
{
public:
	void	addMember(Genome* genome);
	void	removeMember(Genome* genome);
	void	calculateAdjustedFitness();	
	void	cull(float keepRatio);			
	bool	isStagnant();				
	Genome*	selectChampion();			

	int					id;
	Genome*				members[64];
	int					memberCount;
	Genome*				representative;	
	float				bestFitness;
	int					staleness;		
	int					offspringCount;	
};

class NEATManager
{
public:
	void	initPopulation(int inputs, int outputs);	
	void	evolve();			
	void	speciate();			

private:
	Genome*		population[100];
	int			popSize;
	Species*	species[20];
	int			speciesCount;
};
