#pragma once
#include "GameState_Snapshot.h"

/**
 * TimelineNode.h -- A node in the branching "Forks of Fate" tree.
 */

class TimelineNode
{
public:
	TimelineNode() : id(-1), timestamp(0.0f), childCount(0), parent(nullptr) {
		for (int i = 0; i < 4; ++i) children[i] = nullptr;
	}

	void addChild(TimelineNode* n) {
		if (childCount < 4 && n) {
			children[childCount++] = n;
			n->parent = this;
		}
	}

	bool isLeaf() const {
		return childCount == 0;
	}

	int					id;
	float				timestamp;
	GameStateSnapshot	state;
	TimelineNode*		children[4];
	int					childCount;
	TimelineNode*		parent;
};
