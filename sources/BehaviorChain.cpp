#include "../headers/BehaviorChain.h"
#include <cmath>

BehaviorChain::BehaviorChain()
    : currentState(BehaviorState::CONSOLIDATING),
      historyWindow(4.0f),
      sessionDecay(0.9f)
{
    for (int i = 0; i < 5; i++)
        for (int j = 0; j < 5; j++)
            transitionMatrix[i][j] = 0.2f;  // uniform initialisation
}

BehaviorState BehaviorChain::inferState(float* inputs, int n)
{
    if (!inputs || n < 3) return currentState;
    // Heuristic: use velocity and HP to infer state
    float hp       = inputs[0];  // normalised 0-1
    float velX     = inputs[1];  // normalised
    float nearEnemy= n > 2 ? inputs[2] : 0.5f;

    if (hp < 0.25f)                         return BehaviorState::RETREATING;
    if (nearEnemy < 0.2f && fabsf(velX) > 0.5f) return BehaviorState::PRESSING;
    if (fabsf(velX) < 0.1f)                return BehaviorState::CONSOLIDATING;
    return BehaviorState::STALLING;
}

BehaviorState BehaviorChain::predictNext()
{
    int cur = (int)currentState;
    // Find highest probability transition from current state
    float best = -1.f;
    int   next = cur;
    for (int j = 0; j < 5; j++)
        if (transitionMatrix[cur][j] > best)
            { best = transitionMatrix[cur][j]; next = j; }
    return (BehaviorState)next;
}

void BehaviorChain::updateTransitions(BehaviorState from, BehaviorState to)
{
    int f = (int)from, t = (int)to;
    transitionMatrix[f][t] += 1.0f;
    // Normalise row
    float sum = 0.f;
    for (int j = 0; j < 5; j++) sum += transitionMatrix[f][j];
    if (sum > 0.f)
        for (int j = 0; j < 5; j++) transitionMatrix[f][j] /= sum;
    // Apply session decay
    for (int i = 0; i < 5; i++)
        for (int j = 0; j < 5; j++)
            transitionMatrix[i][j] *= sessionDecay;
    currentState = to;
}

BehaviorState BehaviorChain::getGapState()
{
    // Find the state the player transitions TO least often
    float minFreq = 1e9f;
    int   gap     = 0;
    for (int j = 0; j < 5; j++)
    {
        float colSum = 0.f;
        for (int i = 0; i < 5; i++) colSum += transitionMatrix[i][j];
        if (colSum < minFreq) { minFreq = colSum; gap = j; }
    }
    return (BehaviorState)gap;
}

void BehaviorChain::saveHistory(const char* file)
{
    // Stub — implement with fstream binary write if needed
}

void BehaviorChain::loadHistory(const char* file)
{
    // Stub — implement with fstream binary read if needed
}
