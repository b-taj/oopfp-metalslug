#include "../headers/Species.h"
#include "../headers/Genome.h"

Species::Species(int sid)
    : memberCount(0), representative(nullptr),
      bestFitness(0.f), staleness(0), id(sid)
{
    for (int i = 0; i < MAX_POPULATION; i++) members[i] = nullptr;
}

void Species::addMember(Genome* g)
{
    if (memberCount >= MAX_POPULATION) return;
    members[memberCount++] = g;
    if (g->fitness > bestFitness) bestFitness = g->fitness;
}

void Species::calculateAdjustedFitness()
{
    for (int i = 0; i < memberCount; i++)
        if (members[i])
            members[i]->adjustedFitness = members[i]->fitness / (float)memberCount;
}

void Species::cull(float keepRatio)
{
    int keep = (int)(memberCount * keepRatio);
    if (keep < 1) keep = 1;
    for (int i = 0; i < memberCount - 1; i++)
        for (int j = 0; j < memberCount - i - 1; j++)
            if (members[j] && members[j+1] &&
                members[j]->fitness < members[j+1]->fitness)
            {
                Genome* tmp   = members[j];
                members[j]   = members[j+1];
                members[j+1] = tmp;
            }
    for (int i = keep; i < memberCount; i++)
    {
        delete members[i];
        members[i] = nullptr;
    }
    memberCount = keep;
}

bool Species::isStagnant() const { return staleness > 15; }

Genome* Species::selectChampion() const
{
    Genome* best = nullptr;
    for (int i = 0; i < memberCount; i++)
        if (members[i] && (!best || members[i]->fitness > best->fitness))
            best = members[i];
    return best;
}
