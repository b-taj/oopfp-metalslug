#include "../headers/Genome.h"
#include <cmath>
#include <cstdlib>

Genome::Genome()
    : nodeCount(0), connCount(0),
      fitness(0.f), adjustedFitness(0.f), speciesId(0)
{}

void Genome::mutateWeights()
{
    for (int i = 0; i < connCount; i++)
    {
        if ((rand() % 10) < 8)  // 80% perturb
            connGenes[i].weight += ((float)(rand() % 400) - 200.f) / 1000.f;
        else                    // 10% new random
            connGenes[i].weight = ((float)(rand() % 2000) - 1000.f) / 1000.f;

        if (connGenes[i].weight >  4.f) connGenes[i].weight =  4.f;
        if (connGenes[i].weight < -4.f) connGenes[i].weight = -4.f;
    }
}

void Genome::mutateAddConnection(int innovation)
{
    if (connCount >= MAX_GENOME_CONNS) return;
    ConnectionGene cg;
    cg.inNode     = nodeGenes[rand() % nodeCount].id;
    cg.outNode    = nodeGenes[rand() % nodeCount].id;
    cg.weight     = ((float)(rand() % 2000) - 1000.f) / 1000.f;
    cg.enabled    = true;
    cg.innovation = innovation;
    connGenes[connCount++] = cg;
}

void Genome::mutateAddNode(int& innovCounter)
{
    if (connCount == 0 || nodeCount >= MAX_GENOME_NODES) return;
    int idx = rand() % connCount;
    if (!connGenes[idx].enabled) return;

    connGenes[idx].enabled = false;

    NodeGene ng;
    ng.id   = ++innovCounter;
    ng.type = NodeType::HIDDEN;
    nodeGenes[nodeCount++] = ng;

    ConnectionGene c1, c2;
    c1.inNode = connGenes[idx].inNode; c1.outNode = ng.id;
    c1.weight = 1.f; c1.enabled = true; c1.innovation = ++innovCounter;

    c2.inNode = ng.id; c2.outNode = connGenes[idx].outNode;
    c2.weight = connGenes[idx].weight; c2.enabled = true;
    c2.innovation = ++innovCounter;

    if (connCount + 2 < MAX_GENOME_CONNS)
    {
        connGenes[connCount++] = c1;
        connGenes[connCount++] = c2;
    }
}

NeuralNetwork Genome::buildNetwork() const
{
    NeuralNetwork net;
    int inCount = 0, outCount = 0;

    for (int i = 0; i < nodeCount; i++)
    {
        net.addNode(nodeGenes[i].id);
        if (nodeGenes[i].type == NodeType::INPUT)  inCount++;
        if (nodeGenes[i].type == NodeType::OUTPUT) outCount++;
    }
    
    net.setCounts(inCount, outCount);
    
    for (int i = 0; i < connCount; i++)
    {
        if (connGenes[i].enabled)
        {
            net.addConnection(connGenes[i]);
        }
    }

    return net;
}

Genome* Genome::clone() const
{
    Genome* g     = new Genome();
    g->nodeCount  = nodeCount;
    g->connCount  = connCount;
    g->fitness    = fitness;
    g->adjustedFitness = adjustedFitness;
    g->speciesId  = speciesId;
    for (int i = 0; i < nodeCount; i++) g->nodeGenes[i] = nodeGenes[i];
    for (int i = 0; i < connCount; i++) g->connGenes[i] = connGenes[i];
    return g;
}
