#pragma once
#include "raylib.h"
#include "Game.h"
#include "CollisionManager.h"
#include "HighscoreManager.h"
#include <iostream>


enum class GameStates
{
	MAIN_MENU, NAME_SCREEN, GAME, COLLISION_CHECK, LOSE, WIN , HIGH_SCORE
};

class GameManager
{
public:
    GameManager();

    GameStates GS;
	Collisions collisions;
    Game game;
    Highscore HS;

    std::string playerName;
    int playerScore;

    void RunGame(); 
    void UpdateMainMenu();
    void UpdateNameScreen();
    void UpdateGame();
    void UpdateLoseScreen();
    void UpdateWinScreen();
    void UpdateHighScore();
    void UpdateCollisionCheckScreen();
}; 



