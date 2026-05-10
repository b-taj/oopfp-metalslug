#include "../headers/NEATManager.h"
#include "../headers/Genome.h"
#include "../headers/Species.h"
#include <cmath>

NEATManager::NEATManager()
    : populationSize(150), speciesCount(0), generation(0),
      innovationCounter(0), globalBest(nullptr),
      c1(1.0f), c2(1.0f), c3(0.4f), deltaT(3.0f),
      innoMapCount(0)
{
    for (int i = 0; i < MAX_POPULATION; i++) population[i] = nullptr;
}

NEATManager::~NEATManager()
{
    for (int i = 0; i < populationSize; i++)
        if (population[i]) { delete population[i]; population[i] = nullptr; }
    if (globalBest) { delete globalBest; globalBest = nullptr; }
}

void NEATManager::initPopulation(int inputs, int outputs)
{
    for (int i = 0; i < populationSize; i++)
    {
        population[i] = new Genome();
        // Add input nodes
        for (int n = 0; n < inputs; n++)
        {
            NodeGene ng;
            ng.id   = n;
            ng.type = NodeType::INPUT;
            population[i]->nodeGenes[population[i]->nodeCount++] = ng;
        }
        // Add output nodes
        for (int n = 0; n < outputs; n++)
        {
            NodeGene ng;
            ng.id   = inputs + n;
            ng.type = NodeType::OUTPUT;
            population[i]->nodeGenes[population[i]->nodeCount++] = ng;
        }
        // Connect every input to every output with random weight
        for (int in = 0; in < inputs; in++)
        {
            for (int out = 0; out < outputs; out++)
            {
                ConnectionGene cg;
                cg.inNode     = in;
                cg.outNode    = inputs + out;
                cg.weight     = ((float)(rand() % 2000) - 1000.f) / 1000.f;
                cg.enabled    = true;
                cg.innovation = getInnovation(in, inputs + out);
                population[i]->connGenes[population[i]->connCount++] = cg;
            }
        }
    }
}

void NEATManager::assignFitness(Genome* g, float fitness)
{
    if (!g) return;
    g->fitness = fitness;
    if (!globalBest || fitness > globalBest->fitness)
    {
        if (globalBest) delete globalBest;
        globalBest = g->clone();
    }
}

Genome* NEATManager::getNextGenome()
{
    // Return first genome with fitness == 0 (not yet evaluated)
    for (int i = 0; i < populationSize; i++)
        if (population[i] && population[i]->fitness == 0.f)
            return population[i];
    return population[0];
}

Genome* NEATManager::getBestGenome()
{
    return globalBest;
}

void NEATManager::evolve()
{
    speciate();
    // Simple evolution: mutate all genomes
    for (int i = 0; i < populationSize; i++)
    {
        if (population[i])
        {
            population[i]->mutateWeights();
            population[i]->fitness = 0.f;  // reset for next evaluation
        }
    }
    generation++;
}

void NEATManager::speciate()
{
    // Stub — full speciation can be added later
    speciesCount = 1;
}

int NEATManager::getInnovation(int inNode, int outNode)
{
    for (int i = 0; i < innoMapCount; i++)
        if (innoMapIn[i] == inNode && innoMapOut[i] == outNode)
            return innoMapVal[i];

    innoMapIn[innoMapCount]  = inNode;
    innoMapOut[innoMapCount] = outNode;
    innoMapVal[innoMapCount] = ++innovationCounter;
    innoMapCount++;
    return innovationCounter;
}

float NEATManager::compatDistance(Genome* a, Genome* b)
{
    if (!a || !b) return 0.f;
    // Simplified: just use weight difference
    float wDiff = 0.f;
    int   matching = 0;
    for (int i = 0; i < a->connCount; i++)
        for (int j = 0; j < b->connCount; j++)
            if (a->connGenes[i].innovation == b->connGenes[j].innovation)
            {
                wDiff += fabsf(a->connGenes[i].weight - b->connGenes[j].weight);
                matching++;
            }
    return matching > 0 ? (c3 * wDiff / matching) : 0.f;
}

void NEATManager::saveGlobalBest(const char* file)
{
    if (!globalBest) return;
    // Binary write — full implementation in SpecialFeatures.cpp
}

void NEATManager::loadGlobalBest(const char* file)
{
    // Binary read — full implementation in SpecialFeatures.cpp
}
