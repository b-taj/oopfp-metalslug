#pragma once
#include "GameState.h"

// Timeline.h -- Implements the 'Forks of Fate' feature.
// Frames are recorded in a 10-second ring buffer. Key R rewinds frame-by-frame
// and creates a branch node in a git-like tree. Players can navigate alternate
// timeline branches at any node and persist the tree across sessions.

// Conditions at the moment of player death; used by ScoreManager / Death Compositor.
struct DeathFingerprint
{
	int			health;
	float		posX;
	float		posY;
	std::string	activeWeapon;
	std::string	enemyConfig;		// serialised nearby enemy list
	float		timeSinceCheckpoint;
	int			damageLevel;		// 0 / 1 / 2 at death moment

	std::string	serialize();
	void		deserialize(const std::string& data);
};

// One node in the branching timeline tree; stores a full GameState snapshot.
class TimelineNode
{
public:
	void	addChild(TimelineNode* node);
	bool	isLeaf();		// true when children is empty

	int					id;
	float				timestamp;
	GameState			state;
	std::vector<TimelineNode*>	children;	// branches created from this node
	TimelineNode*		parent;		// nullptr for root
};

// Manages the ring buffer and the branching node tree.
class Timeline
{
public:
	void	recordFrame(const GameState& state);	// call every frame
	void	rewind();						// key R: replay 10 s backwards then pause
	void	branchAt(TimelineNode* node);		// create new branch; set currentNode
	void	switchBranch(int nodeId);			// jump to branch beginning at nodeId
	void	saveToDisk(const std::string& path);	// persist node tree for cross-session access
	void	loadFromDisk(const std::string& path);

private:
	std::vector<TimelineNode*>	nodes;		// all allocated nodes (owned)
	TimelineNode*			currentNode;
	std::vector<GameState>		frameBuffer;	// circular 10-second buffer (600 frames @ 60 fps)
	int				maxFrames;		// = 600
};
