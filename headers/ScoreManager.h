#pragma once

class ScoreManager
{
public:
	ScoreManager() : score(0) {}

	void	addScore(int pts) { score += pts; }
	int		getScore() { return score; }

private:
	int		score;
};
