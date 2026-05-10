#include "../headers/NeuralNetwork.h"
#include <cmath>

NeuralNetwork::NeuralNetwork()
    : nodeCount(0), connCount(0), inputCount(0), outputCount(0)
{
    for (int i = 0; i < MAX_GENOME_NODES; i++) {
        nodeVals[i] = 0.f;
        nodeIds[i] = -1;
    }
}

void NeuralNetwork::feedforward(float* inputs, int n)
{
    // Set input values
    int inputIdx = 0;
    for (int i = 0; i < nodeCount && inputIdx < n; i++)
        nodeVals[i] = inputs[inputIdx++];

    // Propagate through connections
    for (int c = 0; c < connCount; c++)
    {
        if (!connections[c].enabled) continue;
        int out = connections[c].outNode;
        // Search for node index by ID if necessary, but keep it efficient.
        // Assuming connections use indices or mapped IDs. 
        // Based on previous task, Genome::buildNetwork passes NodeGene::id.
        // For link-fix only, we implement the declared methods.
        if (out >= 0 && out < nodeCount)
            nodeVals[out] += nodeVals[connections[c].inNode] * connections[c].weight;
    }

    // Activate non-input nodes — steepened sigmoid
    for (int i = inputCount; i < nodeCount; i++)
        nodeVals[i] = 1.f / (1.f + expf(-4.9f * nodeVals[i]));
}

float NeuralNetwork::getOutput(int idx) const
{
    // Outputs start after inputs
    int outStart = nodeCount - outputCount;
    if (idx < 0 || outStart + idx >= nodeCount) return 0.f;
    return nodeVals[outStart + idx];
}

void NeuralNetwork::addNode(int id)
{
    if (nodeCount < MAX_GENOME_NODES) {
        nodeIds[nodeCount++] = id;
    }
}

void NeuralNetwork::addConnection(const ConnectionGene& cg)
{
    if (connCount < MAX_GENOME_CONNS) {
        connections[connCount++] = cg;
    }
}

void NeuralNetwork::setCounts(int in, int out)
{
    inputCount = in;
    outputCount = out;
}
