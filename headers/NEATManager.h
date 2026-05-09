#pragma once
#include "Genome.h"
#include "Species.h"
#include "Constants.h"

/**
 * NEATManager.h -- Controls the evolutionary process.
 */

class NEATManager
{
public:
	NEATManager();
	~NEATManager();

	void	initPopulation(int inputs, int outputs);
	void	evolve();
	void	speciate();
	void	assignFitness(Genome* g, float fitness);
	
	Genome*	getNextGenome();
	Genome*	getBestGenome();
	
	void	saveGlobalBest(const char* file);
	void	loadGlobalBest(const char* file);

private:
	Genome*	crossover(Genome* p1, Genome* p2);
	int		getInnovation(int inNode, int outNode);
	float	compatDistance(Genome* a, Genome* b);

	Genome*	population[MAX_POPULATION];
	int		populationSize;
	
	Species	species[MAX_SPECIES];
	int		speciesCount;
	
	int		generation;
	int		innovationCounter;
	
	float	c1, c2, c3, deltaT;
	
	Genome*	globalBest;
	
	int		innoMapIn[MAX_GENOME_CONNS];
	int		innoMapOut[MAX_GENOME_CONNS];
	int		innoMapVal[MAX_GENOME_CONNS];
	int		innoMapCount;

	int		currentEvalIndex; 
	};

	int		currentEvalIndex; // For tracking who is playing
};
