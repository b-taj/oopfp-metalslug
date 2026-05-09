#pragma once

#include "Enums.h"

// NEAT.h -- NeuroEvolution of Augmenting Topologies (Stanley & Miikkulainen 2002).
// Used by SelfPlayingMode to evolve agents that play the game automatically.
// The global best genome persists across sessions; key B shows it live for comparison.

// One weighted directed connection between two nodes.
// The innovation number is a historical marker used to align genomes for crossover.
class ConnectionGene
{
public:
	void	disable();			// mark enabled=false (still stored; may re-enable in offspring)
	void	perturbWeight(float amount);	// Gaussian perturbation or full randomisation

	int		inNode;
	int		outNode;
	float	weight;
	bool	enabled;		// false = not expressed in phenotype
	int		innovation;		// global chronological innovation number
};

// One neuron in the network; type determines its role in forward propagation.
class NodeGene
{
public:
	// Steepened sigmoid: phi(x) = 1 / (1 + e^(-4.9x))  [NEAT paper, sec. 4.1].
	float	activate(float x);

	int		id;
	NodeType	type;
};

// Runtime phenotype built from a Genome. Supports recurrent connections.
class NeuralNetwork
{
public:
	std::vector<float>	feedforward(std::vector<float> inputs);
	float	activate(int nodeId);		// compute activation value for one node
	float	getOutput(int idx);		// retrieve output at index (mapped to game action)

private:
	std::map<int, float>			nodes;		// nodeId -> activation value
	std::vector<ConnectionGene>		connections;	// expressed (enabled) connections only
	int						inputCount;
	int						outputCount;
};

// Genotype: ordered lists of NodeGene and ConnectionGene.
class Genome
{
public:
	void						mutateWeights();		// perturb or randomise connection weights
	void						mutateAddConnection(int innovation);	// add new connection gene
	void						mutateAddNode(int innovation);		// split connection; insert node

	// Construct and return the runtime NeuralNetwork for evaluation.
	NeuralNetwork				buildNetwork();

	// Return pairs of matching ConnectionGenes (same innovation) for crossover alignment.
	std::vector<std::pair<ConnectionGene, ConnectionGene>>	getMatchingGenes(Genome* other);

	// Deep-copy (used when copying champion unchanged into next generation).
	Genome*						clone();

	std::vector<NodeGene>		nodeGenes;
	std::vector<ConnectionGene>	connGenes;
	float					fitness;
	float					adjustedFitness;	// fitness / species size
	int					speciesId;
};

// Cluster of topologically similar genomes. Protects innovation by letting
// organisms compete within their own niche before facing the full population.
class Species
{
public:
	void	addMember(Genome* genome);
	void	removeMember(Genome* genome);
	void	calculateAdjustedFitness();	// divide raw fitness by species size
	void	cull(float keepRatio);			// remove the lowest (1-keepRatio) fraction
	bool	isStagnant();				// true when bestFitness unchanged for 15 generations
	Genome*	selectChampion();			// highest-fitness member (copied to next gen)

	int					id;
	std::vector<Genome*>	members;
	Genome*				representative;	// random member from previous generation
	float				bestFitness;
	int					staleness;		// generations without fitness improvement
	int					offspringCount;	// allocated offspring for next generation
};

// Coordinates the full NEAT loop: speciation -> evaluation -> reproduction.
class NEATManager
{
public:
	void	initPopulation(int inputs, int outputs);	// seed with zero-hidden-node networks
	void	evolve();			// one full generation: speciate -> reproduce -> mutate
	void	speciate();			// assign each genome to a species via compatibility distance
	void	assignFitness(Genome* genome, float fitness);	// record score; update globalBest
	Genome*	getNextGenome();	// next genome awaiting evaluation
	Genome*	getBestGenome();	// highest-fitness genome across all sessions
	void	saveGlobalBest(const std::string& file);
	void	loadGlobalBest(const std::string& file);

private:
	std::vector<Genome*>				population;
	std::vector<Species>				species;
	int							generation;
	int							populationSize;		// 150 default
	int							innovationCounter;
	float							c1;				// excess gene coefficient
	float							c2;				// disjoint gene coefficient
	float							c3;				// average weight difference coefficient
	float							deltaT;			// compatibility threshold (3.0 default)
	Genome*							globalBest;
	std::map<std::pair<int,int>, int>	innoMap;		// (inNode,outNode) -> innovation

	Genome*	crossover(Genome* parent1, Genome* parent2);
	int		getInnovation(int inNode, int outNode);	// mint or reuse innovation number
	float	compatDistance(Genome* a, Genome* b);	// eq. 1 in NEAT paper
	float	adjustedFitnessOf(Genome* genome, Species& sp);	// eq. 2 in NEAT paper
};
