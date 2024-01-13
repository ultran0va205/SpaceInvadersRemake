#pragma once
#include "raylib.h"
#include "Constants.h"
#include "CollisionManager.h"
#include <vector>
#include <iostream>

struct AnimationData //took help from my groupmate and Ulf for animations
{
	int ticks = 0;
	int currentFrame = 0;
	float ticksPerFrame = 10; //FPS or tick per second is 60
	static const auto FRAME_COUNT = 3; //length of animation
};

enum class Entities
{
	Player, Enemy, Projectile
};

enum class EnemyType
{
	Weak,Strong
};

enum class ProjectileType
{
	Player, Enemy
};


struct Entity
{
	Vector2 position{};
	Vector2 size{};
	float radius = 0;
	bool isDead = false;
	Color color = WHITE;
};

struct Player : public Entity
{
	Player()
	{
		color = GREEN;
	}

	bool canShoot = true;
	float shotTimer = 0;

	bool isMovingLeft = false;
	bool isMovingRight = false;

	AnimationData animPlayer;
};

struct Enemy :public Entity
{
	EnemyType eType{};
	Enemy() {};

	Enemy(EnemyType type) :eType(type)
	{
		if (type == EnemyType::Weak)
		{
			color = DARKGREEN;
		}

		else if (type == EnemyType::Strong)
		{
			color = RED;
		}
	}
};


struct ProjectileManager :public Entity
{
	ProjectileManager(Vector2 pos, Vector2 dir, ProjectileType type);
	ProjectileType pType{};
	Vector2 position = { 0,0 };
	Vector2 size = { 0,0 };
	Vector2 direction = { 0,0 };
	float radius = 0;
	bool hasCollided = false;
};


class Game
{
public:
	Game();

	int score = 0;
	bool gameLost = false;
	bool gameWon = false;

	Player& GetPlayer();

	void MovePlayer();
	void limitPlayerTravel();
	void MoveEnemies();
	void CreateEnemyGrid();

	void SpawnPlayerProjectile(const Player& player);
	void MovePlayerProjectile();

	void SpawnEnemyProjectile(const Enemy& enemy);
	void MoveEnemyProjectile();

	void PlayerShoot();
	void EnemyShoot();

	void ProjectileEnemyCollision();
	void ProjectilePlayerCollision();

	void WinCondition();
	void LoseCondition();

	void DestroyEntity();

	void HandleParallax();
	void HandleAnimation();

	void DrawParallax();
	void UpdateParallax();

	void Reset();
	void Update();
	void Render();

private:
	Player player{};
	Enemy weakEnemy{EnemyType::Weak};
	Enemy strongEnemy{ EnemyType::Strong };


	Enemy enemyGrid[NUMROWS][NUMCOLUMNS];

	std::vector<Entities> AllEntities;
	std::vector<ProjectileManager> projectiles;
	std::string playerName;

	Collisions collisions;

	float gridMovementSpeed = 3.0f;
	int direction = 1;
	float enemyProjectileTimer = 0.0f;
	float enemyFireRate = 1.0f;
	bool allEnemiesDead = false;


	Vector2 backgroundPosition{};
	Vector2 foregroundPosition{};
	float BGscrollSpeed{};
	float FGscrollSpeed{};

};

