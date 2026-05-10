#pragma once
#include "ConnectionGene.h"
#include "Constants.h"

/**
 * NeuralNetwork.h -- The phenotype network evaluated during gameplay.
 */

class NeuralNetwork
{
public:
	NeuralNetwork();
	
	void	feedforward(float* inputs, int n);
	float	getOutput(int idx) const;

	void	addNode(int id);
	void	addConnection(const ConnectionGene& cg);
	void	setCounts(int in, int out);

private:
	float			nodeVals[MAX_GENOME_NODES];
	int				nodeIds[MAX_GENOME_NODES];
	int				nodeCount;
	
	ConnectionGene	connections[MAX_GENOME_CONNS];
	int				connCount;
	
	int				inputCount;
	int				outputCount;
};
