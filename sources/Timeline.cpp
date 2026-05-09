#include "../headers/Timeline.h"
#include <iostream>

Timeline::Timeline() : nodeCount(0), currentNode(nullptr), bufferHead(0), bufferCount(0)
{
	for (int i = 0; i < 600; ++i) nodes[i] = nullptr;
}

Timeline::~Timeline()
{
	for (int i = 0; i < nodeCount; ++i) {
		if (nodes[i]) delete nodes[i];
	}
}

void Timeline::recordFrame(const GameStateSnapshot& s)
{
	frameBuffer[bufferHead] = s;
	bufferHead = (bufferHead + 1) % 600;
	if (bufferCount < 600) bufferCount++;
}

void Timeline::rewind()
{
	std::cout << "[Timeline] Rewinding 10s of gameplay..." << std::endl;
	// Logic to apply frameBuffer[i] states in reverse would go here
	// This would trigger a "replay" visual effect.
}

void Timeline::branchAt(TimelineNode* n)
{
	if (nodeCount >= 600 || !n) return;

	TimelineNode* newNode = new TimelineNode();
	newNode->id = nodeCount;
	newNode->timestamp = n->timestamp + 0.1f; // Slight offset
	newNode->state = n->state;
	
	n->addChild(newNode);
	nodes[nodeCount++] = newNode;
	currentNode = newNode;
	
	std::cout << "[Timeline] New branch created at Node " << newNode->id << std::endl;
}

void Timeline::switchBranch(int nodeId)
{
	for (int i = 0; i < nodeCount; ++i) {
		if (nodes[i] && nodes[i]->id == nodeId) {
			currentNode = nodes[i];
			std::cout << "[Timeline] Switched to branch " << nodeId << std::endl;
			return;
		}
	}
}

void Timeline::saveToDisk(const char* path)
{
	std::ofstream ofs(path, std::ios::binary);
	if (!ofs) return;

	ofs.write((char*)&nodeCount, sizeof(int));
	for (int i = 0; i < nodeCount; ++i) {
		ofs.write((char*)nodes[i], sizeof(TimelineNode));
	}
	ofs.close();
}

void Timeline::loadFromDisk(const char* path)
{
	std::ifstream ifs(path, std::ios::binary);
	if (!ifs) return;

	// Clear existing
	for (int i = 0; i < nodeCount; ++i) delete nodes[i];
	nodeCount = 0;

	ifs.read((char*)&nodeCount, sizeof(int));
	for (int i = 0; i < nodeCount; ++i) {
		nodes[i] = new TimelineNode();
		ifs.read((char*)nodes[i], sizeof(TimelineNode));
	}
	ifs.close();
}
