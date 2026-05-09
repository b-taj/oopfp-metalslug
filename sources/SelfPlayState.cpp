#include "../headers/SelfPlayState.h"
#include <cmath>

SelfPlayState::SelfPlayState(Level* lvl, Camera* cam, HUD* h, ScoreManager* s)
	: level(lvl), camera(cam), hud(h), scoreManager(s), agent(nullptr), bestAgent(nullptr), 
	  currentGenome(nullptr), showGlobalBest(false), timeSurvived(0.0f), 
	  enemiesKilledThisRun(0), distanceTravelled(0.0f), deathCount(0)
{
	agent = new PlayerMarco();
	agent->setSoundManager(nullptr); // Silent evaluation
}

SelfPlayState::~SelfPlayState() { if (agent) delete agent; }

void SelfPlayState::onEnter() { neatManager.initPopulation(10, 5); }

void SelfPlayState::update(float dt)
{
	if (!agent || !agent->isActive()) {
		// Run Ended - Assign Fitness
		float fitness = (float)scoreManager->getScore() * 1.0f + timeSurvived * 0.5f + (float)enemiesKilledThisRun * 10.0f;
		fitness -= (float)deathCount * 50.0f;
		fitness += distanceTravelled * 0.05f;
		if (fitness < 0.0f) fitness = 0.0f;

		neatManager.assignFitness(currentGenome, fitness);
		
		// Reset for next
		currentGenome = neatManager.getNextGenome();
		agent->setActive(true);
		agent->heal(100);
		timeSurvived = 0.0f;
		enemiesKilledThisRun = 0;
		distanceTravelled = 0.0f;
		deathCount++;
	}

	timeSurvived += dt;
	distanceTravelled += std::abs(agent->getVelocityX() * dt);

	// NEAT Logic
	float worldW = (float)(LEVEL_WIDTH * CELL_SIZE);
	float worldH = (float)(LEVEL_HEIGHT * CELL_SIZE);
	float inputs[10];
	inputs[0] = agent->getX() / worldW;
	inputs[1] = agent->getY() / worldH;
	inputs[2] = (float)agent->getHp() / (float)agent->getMaxHp();
	inputs[3] = agent->isOnGround() ? 1.0f : 0.0f;
	inputs[4] = agent->getVelocityX() / 300.0f;
	inputs[5] = agent->getVelocityY() / 900.0f;

	Enemy* nearest = level->getEntities().getNearestEnemy(agent->getX(), agent->getY());
	inputs[6] = nearest ? (nearest->getX() / worldW) : 0.5f;
	inputs[7] = nearest ? (nearest->getY() / worldH) : 0.5f;
	inputs[8] = nearest ? ((nearest->getX() - agent->getX()) / worldW) : 0.0f;
	inputs[9] = nearest ? ((nearest->getY() - agent->getY()) / worldH) : 0.0f;

	NeuralNetwork nn = currentGenome->buildNetwork();
	nn.feedforward(inputs, 10);

	if (nn.getOutput(0) > 0.5f) agent->moveLeft(dt);
	if (nn.getOutput(1) > 0.5f) agent->moveRight(dt);
	if (nn.getOutput(2) > 0.5f) agent->jump();
	if (nn.getOutput(3) > 0.5f) {
		Projectile* p = agent->shoot(0.0f);
		if (p) level->addProjectile(p);
	}
	if (nn.getOutput(4) > 0.5f) {
		Projectile* g = agent->throwGrenade(0.0f);
		if (g) level->addProjectile(g);
	}

	agent->update(dt);
	level->update(dt, *scoreManager);
	camera->update(agent->getX(), agent->getY());
}

void SelfPlayState::render(sf::RenderWindow& w)
{
	level->draw(w, *camera);
	agent->draw(w, camera->getOffsetX(), camera->getOffsetY());
	hud->update(agent->getHp(), 100, scoreManager->getScore(), "AI_AGENT", 0, agent->getGrenadeCount(), 0.016f);
	hud->draw(w);
}

void SelfPlayState::handleEvents(sf::RenderWindow& w, sf::Event& e) { (void)w; (void)e; }
void SelfPlayState::onExit() {}
