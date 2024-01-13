#include "raylib.h"
#include "Game.h"
#include "ResourceManager.h"
#include "raymath.h"
#include <iostream>

Game::Game()
{
	playerName = "";
	score = 0;
}

//---------------------------------------------------------------GET FUNCTIONS
Player& Game::GetPlayer()
{
	return player;
}

//---------------------------------------------------------------PLAYER FUNCTIONS
void Game::MovePlayer()
{
	player.isMovingLeft = IsKeyDown(KEY_LEFT) || IsKeyDown(KEY_A);
	player.isMovingRight = IsKeyDown(KEY_RIGHT) || IsKeyDown(KEY_D);	
	if (player.isMovingLeft)	{
		player.position.x -= PLAYERMOVEMENTSPEED;		
	}
	if (player.isMovingRight){
		player.position.x += PLAYERMOVEMENTSPEED;		
	}
	limitPlayerTravel();
}

float clamp(float in, int min, int max) noexcept {
	if (in < static_cast<float>(min)) return  static_cast<float>(min);
	if (in > static_cast<float>(max)) return  static_cast<float>(max);
	return in;
}

void Game::limitPlayerTravel(){
	player.position.x = clamp(player.position.x, 0, GetScreenWidth());
}

void Game::PlayerShoot() //this is just the input of player shooting
{
	if (IsKeyPressed(KEY_SPACE) && player.canShoot == true)
	{
		SpawnPlayerProjectile(player);
		player.shotTimer = 0;
		player.canShoot = false;
	}

	player.shotTimer++;

	if (player.shotTimer >= PLAYERSHOTCOOLDOWN)
	{
		player.canShoot = true;
	}
}

void Game::HandleAnimation() 
{
	player.animPlayer.ticks++;

	if (player.animPlayer.ticks > player.animPlayer.ticksPerFrame)
	{
		player.animPlayer.currentFrame++;
		player.animPlayer.ticks = 0;

		if (player.animPlayer.currentFrame == AnimationData::FRAME_COUNT)
		{
			player.animPlayer.currentFrame = 0;
		}
	}

	if (player.isMovingLeft)
	{
		Vector2 flameOffset = { player.size.x, player.size.y - ResourceManager::textures.fireFrames_Left->height };

		DrawTextureV(ResourceManager::textures.fireFrames_Left[player.animPlayer.currentFrame], Vector2Add(player.position, flameOffset), WHITE);
	}

	if (player.isMovingRight)
	{
		Vector2 flameOffset = {
			 static_cast<float>(-ResourceManager::textures.fireFrames_Right->width),
			player.size.y - ResourceManager::textures.fireFrames_Right->height
		};

		DrawTextureV(ResourceManager::textures.fireFrames_Right[player.animPlayer.currentFrame], Vector2Add(player.position, flameOffset), WHITE);
	}
}



//----------------------------------------------------------------ENEMY FUNCTIONS
void Game::CreateEnemyGrid()
{
	for (int row = 0; row < NUMROWS; row++)
	{
		for (int column = 0; column < NUMCOLUMNS; column++)
		{
			EnemyType type;

			if (row< NUMROWS/2)
			{
				type = EnemyType::Strong;
			}

			else
			{
				type = EnemyType::Weak;
			}

			enemyGrid[row][column] = Enemy(type); 
			enemyGrid[row][column].position.x = ENEMYSPAWNPOS_X + column * XSPACING;
			enemyGrid[row][column].position.y = ENEMYSPAWNPOS_Y + row * YSPACING;
			enemyGrid[row][column].size = ENEMYSIZE;
		}
	}
}

void Game::MoveEnemies()
{
	for (int row = 0; row < NUMROWS; row++)
	{
		for (int column = 0; column < NUMCOLUMNS; column++)
		{
			enemyGrid[row][column].position.x += gridMovementSpeed;

		}
	}

	if (enemyGrid[0][0].position.x  < enemyGrid[0][0].size.x||enemyGrid[0][NUMCOLUMNS - 1].position.x + enemyGrid[0][0].size.x *2 > GetScreenWidth())
	{
		gridMovementSpeed *= -1;

		for (int row = 0; row < NUMROWS; row++)
		{
			for (int column = 0; column < NUMCOLUMNS; column++)
			{
				enemyGrid[row][column].position.y += 20;
			}
		}
	}
}

void Game::EnemyShoot() //this function handles the randomization and time rate of enemy projectile
{
	enemyProjectileTimer += GetFrameTime();

	if (enemyProjectileTimer >= enemyFireRate)
	{
		enemyProjectileTimer = 0;

		int randRow = GetRandomValue(0, NUMROWS - 1);
		int randCol = GetRandomValue(0, NUMCOLUMNS - 1);

		if (!enemyGrid[randRow][randCol].isDead)
		{
			SpawnEnemyProjectile(enemyGrid[randRow][randCol]);
		}
	}
}

//--------------------------------------------------------------Projectiles
ProjectileManager::ProjectileManager(Vector2 pos, Vector2 dir,  ProjectileType type)
{
	pType = type;
	position = pos;
	direction = dir;
	radius = PLAYERPROJECTILERADIUS;
	size = ENEMYPROJECTILESIZE;
}

void Game::SpawnPlayerProjectile(const Player& player) //this spawns the player projectile
{
	ProjectileManager projectile{ player.position,  PLAYERPROJECTILEDIRECTION  ,ProjectileType::Player };

	projectiles.push_back(projectile);
}

void Game::MovePlayerProjectile() //this is for the player projectile movement
{
	for (auto& p : projectiles)
	{
		p.position.y -= PLAYERPROJECTILESPEED * direction;

		if (p.position.y <= 0 && !p.isDead)
		{
			p.isDead = true;
		}
	}
}

void Game::SpawnEnemyProjectile(const Enemy& enemy) //this spawns the enemy projectile
{
	ProjectileManager projectile{ enemy.position,ENEMYPROJECTILEDIRECTION, ProjectileType::Enemy};

	projectiles.push_back(projectile);
}

void Game::MoveEnemyProjectile() //this is for the enemy projectile movement
{
	for (auto& p : projectiles)
	{
		if (p.pType == ProjectileType::Enemy)
		{
			p.position.y +=  ENEMYPROJECTILESPEED;

			if (p.position.y >= GetScreenHeight() && !p.isDead)
			{
				p.isDead = true;
			}
		}
	}
}

void Game::ProjectileEnemyCollision() //collision of projectile with the enemy
{
	for (auto& playerProjectile : projectiles)
	{
		if (playerProjectile.pType == ProjectileType::Player)
		{
			for (int row = 0; row < NUMROWS; row++)
			{
				for (int column = 0; column < NUMCOLUMNS; column++)
				{
					if (!enemyGrid[row][column].isDead)
					{
						Circle pProjectile = { playerProjectile.position, playerProjectile.radius, WHITE };

						AABB enemy = { enemyGrid[row][column].position, enemyGrid[row][column].size, WHITE };

						if (collisions.CircleAABBCollision(pProjectile, enemy))
						{
							enemyGrid[row][column].isDead = true;
							score += 100;
							playerProjectile.isDead = true;
						}
					}
				}
			}
		}
	}
}

void Game::ProjectilePlayerCollision() //collision between player and projectile
{
	for (auto& enemyProjectile : projectiles)
	{
		if (enemyProjectile.pType == ProjectileType::Enemy)
		{
			if (!player.isDead)
			{
				AABB playerRect = { player.position, player.size, WHITE };

				AABB eProjectile = { enemyProjectile.position, enemyProjectile.size, WHITE };

				if (collisions.AABBAABBCollision(playerRect, eProjectile))
				{
					player.isDead = true;
					enemyProjectile.isDead = true;
					gameLost = true;
				}
			}
		}
	}
}
//---------------------------------------------------------------END CONDTIONS
void Game::WinCondition()
{
	allEnemiesDead = true;

	for (int row = 0; row < NUMROWS; row++)
	{
		for (int column = 0; column < NUMCOLUMNS; column++)
		{
			if (!enemyGrid[row][column].isDead)
			{
				allEnemiesDead = false;
				break;
			}
		}

		if (!allEnemiesDead) {
			break;
		}
	}

	if (allEnemiesDead)
	{
		gameWon = true;
	}
}

void Game::LoseCondition()
{
	int lastAliveRow = -1;
	for (int row = 0; row < NUMROWS; row++)
	{
		for (int column = 0; column < NUMCOLUMNS; column++)
		{
			if (!enemyGrid[row][column].isDead)
			{
				lastAliveRow = row;
				break;
			}
		}
	}

	// Get the position of the last alive row of enemies
	Vector2 lastAliveRowPosition = enemyGrid[lastAliveRow][0].position;

	if (lastAliveRowPosition.y + enemyGrid[lastAliveRow][0].size.y >= player.position.y)
	{
		gameLost = true;
	}
}


//--------------------------------------------------------------RESET,RENDER,UPDATE

void Game::Reset() //called in main.cpp to initiate the game
{
	player.position = PLAYERSPAWNPOS;

	CreateEnemyGrid();
}

void Game::Render()
{
	DrawParallax();

	player.size = PLAYERSIZE;

	//Player Render
	if (!player.isDead)
	{
		DrawTextureV(ResourceManager::textures.player, player.position, WHITE);

	}

	//Enemy grid render
	for (int row = 0; row < NUMROWS; row++)
	{
		for (int column = 0; column < NUMCOLUMNS; column++)
		{
			const auto& tile = enemyGrid[row][column];
			if (tile.isDead) {
				continue;
			}
			if (tile.eType == EnemyType::Strong)
			{
				DrawTextureV(ResourceManager::textures.strongEnemy, tile.position, WHITE);
			}
			else if (tile.eType == EnemyType::Weak)
			{
				DrawTextureV(ResourceManager::textures.weakEnemy, tile.position, WHITE);
			}
		}
	}

	//Projectile Render
	for (auto& p : projectiles) {
		if (p.isDead) {
			continue;
		}
		if (p.pType == ProjectileType::Player)
		{
			DrawCircleV(p.position, p.radius, WHITE);
		}
		else
		{
			DrawRectangleV(p.position, p.size, PINK);
		}

	}

	//Score render
	char scoreText[20];
	sprintf_s(scoreText, "Score: %d", score);
	DrawText(scoreText, 450, 10, 50, WHITE);

}

void Game::HandleParallax() //follows similar logic as to https://www.raylib.com/examples/textures/loader.html?name=textures_background_scrolling
{
	float BGwidth = (float) ResourceManager::textures.background.width;
	float FGwidth = (float) ResourceManager::textures.foreground.width;

	BGscrollSpeed -= 0.1f;
	FGscrollSpeed -= 0.8f;

	DrawTextureEx(ResourceManager::textures.background, { BGscrollSpeed, 0 }, 0.0f, 1.0f, WHITE);
	DrawTextureEx(ResourceManager::textures.background, { BGwidth + BGscrollSpeed, 0 }, 0.0f, 1.0f, WHITE);

	DrawTextureEx(ResourceManager::textures.foreground, { FGscrollSpeed , 0 }, 0.0f, 1.0f, WHITE);
	DrawTextureEx(ResourceManager::textures.foreground, { FGwidth + FGscrollSpeed, 0 }, 0.0f, 1.0f, WHITE);

	if (BGscrollSpeed <= -BGwidth) //wrapping
	{
		BGscrollSpeed = 0;
	}
	if (FGscrollSpeed <= -FGwidth) //wrapping
	{
		FGscrollSpeed = 0;
	}
}

void Game::DrawParallax()
{
	float BGwidth = (float)ResourceManager::textures.background.width;
	float FGwidth = (float)ResourceManager::textures.foreground.width;

	DrawTextureEx(ResourceManager::textures.background, { BGscrollSpeed, 0 }, 0.0f, 1.0f, WHITE);
	DrawTextureEx(ResourceManager::textures.background, { BGwidth + BGscrollSpeed, 0 }, 0.0f, 1.0f, WHITE);

	DrawTextureEx(ResourceManager::textures.foreground, { FGscrollSpeed, 0 }, 0.0f, 1.0f, WHITE);
	DrawTextureEx(ResourceManager::textures.foreground, { FGwidth + FGscrollSpeed, 0 }, 0.0f, 1.0f, WHITE);
}

void Game::UpdateParallax()
{
	float BGwidth = (float)ResourceManager::textures.background.width;
	float FGwidth = (float)ResourceManager::textures.foreground.width;

	BGscrollSpeed -= 0.1f;
	FGscrollSpeed -= 0.8f;

	if (BGscrollSpeed <= -BGwidth) // Wrapping
	{
		BGscrollSpeed = 0;
	}

	if (FGscrollSpeed <= -FGwidth) // Wrapping
	{
		FGscrollSpeed = 0;
	}
}

void Game::DestroyEntity() //took this from an old piece of code and had to modify it a bit with the help of my groupmate
{
	for (int row = 0; row < NUMROWS; row++)
	{
		for (int column = 0; column < NUMCOLUMNS; column++)
		{
			if (enemyGrid[row][column].isDead)
			{
				AllEntities.erase(std::remove(AllEntities.begin(), AllEntities.end(), Entities::Enemy), AllEntities.end()); //removes enemy from vector
			}
		}
	}

	projectiles.erase(std::remove_if(projectiles.begin(), projectiles.end(),
		[](const ProjectileManager& projectile) { return projectile.isDead; }), projectiles.end()); //removes projectile from vector

	if (player.isDead)
	{
		AllEntities.erase(std::remove(AllEntities.begin(), AllEntities.end(), Entities::Player), AllEntities.end()); //removes player from vector
	}

}


void Game::Update()
{
	MovePlayer();
	MoveEnemies();
	HandleAnimation();
	//HandleParallax();
	PlayerShoot();
	EnemyShoot();
	MovePlayerProjectile();
	MoveEnemyProjectile();
	ProjectileEnemyCollision();
	ProjectilePlayerCollision();
	DestroyEntity();
	WinCondition();
	LoseCondition();
	UpdateParallax();
}

