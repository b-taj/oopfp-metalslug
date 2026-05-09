#include "../headers/Boss.h"
#include "../headers/IronNokana.h"
#include "../headers/HairbusterRiberts.h"
#include "../headers/SeaSatan.h"
#include "../headers/UltimateBoss.h"
#include "../headers/EntityManager.h"
#include "../headers/InfantryEnemies.h"

// ==================== BOSS BASE ====================
Boss::Boss(EntityManager* em) : entityMgr(em), minionTimer(0.0f)
{
	maxHp = 30;
	hp = 30;
}

bool Boss::isAtHalfHp() const { return hp <= maxHp / 2; }

// ==================== IRON NOKANA ====================
IronNokana::IronNokana(EntityManager* em) : Boss(em)
{
	missileTurret = new RocketLauncher();
	flameShot = new FlameShot();
	// Set initial minionType for reference if needed
}

IronNokana::~IronNokana() { delete missileTurret; delete flameShot; }

void IronNokana::spawnMinions() {
	if (!entityMgr) return;
	for (int i = 0; i < 3; ++i) { // 3 Rebel Soldiers
		RebelSoldier* rs = new RebelSoldier();
		rs->setPosition(x + i * 50.0f, y);
		entityMgr->addEnemy(rs);
	}
}

void IronNokana::retreat() { velocityX = -100.0f; }
void IronNokana::onDeath() { /* Trigger Phase 2 transition logic in Level/Mode */ }
void IronNokana::shootPlayer(Soldier* player) { (void)player; }

void IronNokana::update(float dt, Soldier* player) {
	Enemy::update(dt, player);
	if (isAtHalfHp()) retreat();
}

// ==================== HAIRBUSTER RIBERTS ====================
HairbusterRiberts::HairbusterRiberts(EntityManager* em) : Boss(em)
{
	for (int i = 0; i < 4; ++i) rocketBays[i] = new RocketLauncher();
}

HairbusterRiberts::~HairbusterRiberts() {
	for (int i = 0; i < 4; ++i) delete rocketBays[i];
}

void HairbusterRiberts::spawnMinions() {
	// e.g. Spawn Paratroopers
}

void HairbusterRiberts::retreat() { velocityY = -150.0f; }
void HairbusterRiberts::onDeath() { }
void HairbusterRiberts::shootPlayer(Soldier* player) { (void)player; }

void HairbusterRiberts::update(float dt, Soldier* player) {
	// Aerial movement (ignore standard gravity)
	x += velocityX * dt;
	y += velocityY * dt;
	if (currentAI) currentAI->update(this, dt, player);
	if (isAtHalfHp()) retreat();
}

// ==================== SEA SATAN ====================
SeaSatan::SeaSatan(EntityManager* em) : Boss(em)
{
	leftGun = new HeavyMachineGun();
	rightGun = new HeavyMachineGun();
	rocketLauncher = new RocketLauncher();
}

SeaSatan::~SeaSatan() { delete leftGun; delete rightGun; delete rocketLauncher; }

void SeaSatan::spawnMinions() {}
void SeaSatan::retreat() { velocityY = 100.0f; } // Submerge
void SeaSatan::onDeath() {}
void SeaSatan::shootPlayer(Soldier* player) { (void)player; }

void SeaSatan::update(float dt, Soldier* player) {
	x += velocityX * dt;
	y += velocityY * dt;
	if (currentAI) currentAI->update(this, dt, player);
	if (isAtHalfHp()) retreat();
}

// ==================== ULTIMATE BOSS ====================
UltimateBoss::UltimateBoss(EntityManager* em) : Boss(em), currentPhase(BossPhase::GROUND), phaseTimer(10.0f)
{
	groundBoss = new IronNokana(em);
	aerialBoss = new HairbusterRiberts(em);
	aquaticBoss = new SeaSatan(em);
	groundHp = 30; aerialHp = 30; aquaticHp = 30;
	hp = 90; maxHp = 90;
}

UltimateBoss::~UltimateBoss() {
	delete groundBoss; delete aerialBoss; delete aquaticBoss;
}

Boss* UltimateBoss::getCurrentBoss() {
	switch (currentPhase) {
		case BossPhase::GROUND: return groundBoss;
		case BossPhase::AERIAL: return aerialBoss;
		case BossPhase::AQUATIC: return aquaticBoss;
		default: return groundBoss;
	}
}

void UltimateBoss::spawnMinions() { getCurrentBoss()->spawnMinions(); }
void UltimateBoss::retreat() { getCurrentBoss()->retreat(); }
void UltimateBoss::onDeath() {}
void UltimateBoss::shootPlayer(Soldier* player) { getCurrentBoss()->shootPlayer(player); }

void UltimateBoss::takeDamage(int dmg) {
	Boss::takeDamage(dmg);
	switch (currentPhase) {
		case BossPhase::GROUND: groundHp -= dmg; break;
		case BossPhase::AERIAL: aerialHp -= dmg; break;
		case BossPhase::AQUATIC: aquaticHp -= dmg; break;
		default: break;
	}
}

void UltimateBoss::transitionPhase() {
	if (currentPhase == BossPhase::GROUND) currentPhase = BossPhase::AERIAL;
	else if (currentPhase == BossPhase::AERIAL) currentPhase = BossPhase::AQUATIC;
	else currentPhase = BossPhase::GROUND;
}

void UltimateBoss::update(float dt, Soldier* player) {
	phaseTimer -= dt;
	if (phaseTimer <= 0.0f) {
		phaseTimer = 10.0f; // 10s cycle
		transitionPhase();
	}
	getCurrentBoss()->update(dt, player);
}
