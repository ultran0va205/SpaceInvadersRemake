#pragma once
#include "raylib.h"
#include "GameManager.h"

GameManager::GameManager() : GS(GameStates::MAIN_MENU)
{
	game = Game();
	HS = Highscore();
}

void GameManager::RunGame()
{
	switch (GS)
	{
	case GameStates::MAIN_MENU:
		UpdateMainMenu();
		break;
	case GameStates::NAME_SCREEN:
		UpdateNameScreen();
		break;
	case GameStates::GAME:
		UpdateGame();
		break;
	case GameStates::LOSE:
		UpdateLoseScreen();
		break;
	case GameStates::WIN:
		UpdateWinScreen();
		break;
	case GameStates::HIGH_SCORE:
		UpdateHighScore();
		break;
	case GameStates::COLLISION_CHECK:
		UpdateCollisionCheckScreen();
		break;
	default:
		break;
	}
}

void GameManager::UpdateMainMenu()
{
	BeginDrawing();

	ClearBackground(BLACK);

	DrawText("PLAY GAME = ENTER\nCOLLISION CHECK = C\nEXIT GAME = ESCAPE", 300, 400, 50, WHITE);

	if (IsKeyDown(KEY_ENTER))
	{
		GS = GameStates::NAME_SCREEN;
	}
	else if (IsKeyDown(KEY_C))
	{
		GS = GameStates::COLLISION_CHECK;
	}
	EndDrawing();
}

void GameManager::UpdateNameScreen()
{
	HS.HandleNameInput();

	BeginDrawing();

	ClearBackground(BLACK);

	DrawText("Enter your name (3 letters max):", 200, 10, 50, WHITE);
	DrawText(HS.GetPlayerName().c_str(), 450, 60, 40, WHITE);

	EndDrawing();

	if (HS.GetPlayerName().length() == 3)
	{
		DrawText("Press Enter", 600, 60, 40, WHITE);

		if (IsKeyDown(KEY_ENTER))
		{
			GS = GameStates::GAME;
		}
	}
	else
	{
		DrawText("Name not full!!!", 500, 800, 40, RED);
	}
	
	
}

void GameManager::UpdateGame()
{
	BeginDrawing();

	ClearBackground(BLACK);

	game.Render();

	game.Update();

	

	EndDrawing();
	
	if (game.gameLost == true)
	{
		GS = GameStates::LOSE;
	}

	else if (game.gameWon == true)
	{
		GS = GameStates::WIN;
	}
}

void GameManager::UpdateLoseScreen()
{
	BeginDrawing();

	DrawText("Game Over", 400, 40, 60, RED);

	playerName = HS.GetPlayerName();
	playerScore = game.score;

	DrawText(TextFormat("%s's Score: %d", playerName.c_str(), playerScore), 300, 250, 50, WHITE);

	DrawText("Press H to View High Scores", 300, 350, 50, WHITE);

	ClearBackground(BLACK);

	EndDrawing();

	if (IsKeyDown(KEY_H))
	{
		HS.AddHighScore(playerName, playerScore);

		GS = GameStates::HIGH_SCORE;
	}
}

void GameManager::UpdateWinScreen()
{
	BeginDrawing();

	ClearBackground(BLACK);

	DrawText("You Win!!!", 400, 40, 60, PINK);
	playerName = HS.GetPlayerName();
	playerScore = game.score;

	DrawText(TextFormat("%s's Score: %d", playerName.c_str(), playerScore), 300, 250, 50, WHITE);

	DrawText("Press H to View High Scores", 300, 350, 50, WHITE);

	EndDrawing();

	if (IsKeyDown(KEY_H))
	{
		HS.AddHighScore(playerName, playerScore);

		GS = GameStates::HIGH_SCORE;
	}
}

void GameManager::UpdateHighScore()
{
	BeginDrawing();

	ClearBackground(BLACK);

	DrawText("Press Esc to Exit", 320, 600, 40, WHITE);

	DrawText("Top 5 High Scores:", 320, 100, 40, WHITE);

	DrawText("Press C to clear High Scores", 320, 500, 40, WHITE);

	std::vector<HighScoreEntry> topScores = HS.GetTopHighScores(5);

	int yPos = 150;
	int rank = 1;

	for (const HighScoreEntry& entry : topScores)
	{
		DrawText(TextFormat("%d. %s - %d", rank, entry.playerName, entry.score), 320, yPos, 40, WHITE);
		yPos += 50; 
		rank++;
	}

	EndDrawing();

	if (IsKeyPressed(KEY_C))
	{
		HS.ClearHighScores();
	}

}

void GameManager::UpdateCollisionCheckScreen()
{
	BeginDrawing();

	ClearBackground(WHITE);

	DrawText("CIRCLE VS CIRCLE= F1\nBOX VS CIRCLE = F2\nLINE VS CIRCLE = F3\nBOX VS BOX = F4", 2, 2, 30, BLACK);
	DrawText("Backspace = Main Menu", 2, 1270, 30, BLACK);

	collisions.Render();
	collisions.Update();

	EndDrawing();

	if (IsKeyDown(KEY_BACKSPACE))
	{
		GS = GameStates::MAIN_MENU;
	}
}
