#pragma once
#include "raylib.h"
#include "Game.h"
#include <vector>
#include <string>

struct HighScoreEntry
{
	char playerName[4];
	int score;
};

class Highscore
{
public:
	Highscore();

	void LoadHighScore();

	void AddHighScore(const std::string& playerName, int score);

	void SaveHighScore();

	std::vector<HighScoreEntry> GetTopHighScores(int count);

	void ClearHighScores();

	void SetPlayerName(const std::string& name);

	const std::string& GetPlayerName() const;

	void HandleNameInput(); //this code handles the logic for name input

private:

	std::vector<HighScoreEntry> highScores;

	std::string playerName;

	const char* highscoresFile = "highscores.dat"; // File name for storing high scores.

};