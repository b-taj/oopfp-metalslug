#pragma once
#include "TimelineNode.h"
#include <fstream>

/**
 * Timeline.h -- Manages a circular buffer of snapshots and a tree of timeline nodes.
 * Allows rewinding time and branching into alternate realities.
 */

class Timeline
{
public:
	Timeline();
	~Timeline();

	void	recordFrame(const GameStateSnapshot& s);
	void	rewind();
	
	void	branchAt(TimelineNode* n);
	void	switchBranch(int nodeId);

	void	saveToDisk(const char* path);
	void	loadFromDisk(const char* path);

private:
	TimelineNode*		nodes[600];		// OWNED array pool
	int					nodeCount;
	TimelineNode*		currentNode;

	GameStateSnapshot	frameBuffer[600]; // 10s at 60fps
	int					bufferHead;
	int					bufferCount;
};
