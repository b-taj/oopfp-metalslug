#include "../headers/NEAT.h"
#include <cmath>

// ConnectionGene
void ConnectionGene::disable() { enabled = false; }
void ConnectionGene::perturbWeight(float amount) { weight += amount; }

// NodeGene
float NodeGene::activate(float x) { return 1.0f / (1.0f + exp(-x)); } // Sigmoid

// NeuralNetwork
void NeuralNetwork::feedforward(float* inputs, int inputCount, float* outputs, int outputCount) {
    (void)inputs; (void)inputCount; (void)outputs; (void)outputCount;
}
float NeuralNetwork::activate(int nodeId) { (void)nodeId; return 0.0f; }
float NeuralNetwork::getOutput(int idx) { (void)idx; return 0.0f; }

// Genome
void Genome::mutateWeights() {}
void Genome::mutateAddConnection(int innovation) { (void)innovation; }
void Genome::mutateAddNode(int innovation) { (void)innovation; }
void Genome::buildNetwork(NeuralNetwork& net) { (void)net; }

// Species
void Species::addMember(Genome* genome) { (void)genome; }
void Species::removeMember(Genome* genome) { (void)genome; }
void Species::calculateAdjustedFitness() {}
void Species::cull(float keepRatio) { (void)keepRatio; }
bool Species::isStagnant() { return false; }
Genome* Species::selectChampion() { return nullptr; }

// NEATManager
void NEATManager::initPopulation(int inputs, int outputs) { (void)inputs; (void)outputs; }
void NEATManager::evolve() {}
void NEATManager::speciate() {}
