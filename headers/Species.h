#pragma once
#include "Genome.h"
#include "Constants.h"

/**
 * Species.h -- Groups similar genomes to protect innovation.
 */

class Species
{
public:
	Species(int sid = 0);
	
	void	addMember(Genome* g);
	void	calculateAdjustedFitness();
	void	cull(float keepRatio);
	bool	isStagnant() const;
	Genome*	selectChampion() const;

	Genome*	members[MAX_POPULATION];
	int		memberCount;
	
	Genome*	representative;
	float	bestFitness;
	int		staleness;
	int		id;
};
