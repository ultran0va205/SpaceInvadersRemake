#include "raylib.h"
#include "HighscoreManager.h"
#include <iostream>
#include <fstream>
#include <algorithm>
#include <string>

Highscore::Highscore()
{
	LoadHighScore(); // Load high scores from the file when an instance is created.
}

void Highscore::LoadHighScore()
{
	std::ifstream inputFile("highscores.dat", std::ios::binary); //Open the binary file for reading

	if (inputFile.is_open())
	{
		// Read highscores from the binary file.
		while (!inputFile.eof())
		{
			HighScoreEntry entry;
			inputFile.read(reinterpret_cast<char*>(&entry), sizeof(entry));

			if (!inputFile.eof())
			{
				highScores.push_back(entry);
			}
		}

		inputFile.close();
	}
}

void Highscore::AddHighScore(const std::string& playerName, int score)
{
	//Create new highscore entry
	HighScoreEntry entry;
	entry.score = score;

	// Generate a random player name.
	strncpy_s(entry.playerName, playerName.c_str(), sizeof(entry.playerName));

	//Add entry to the highscore
	highScores.push_back(entry);

	//Sort the highscores by score (highest to lowest)
	std::sort(highScores.begin(), highScores.end(), [](const HighScoreEntry& a, const HighScoreEntry& b)
	{
			return a.score > b.score;
	});

	// Limit the highscores to the top 5 entries.
	if (highScores.size() > 5)
	{
		highScores.resize(5);
	}

	//Save the updated highscores
	SaveHighScore();
}

void Highscore::SaveHighScore()
{
	// Open a binary output file for writing highscore entries.
	std::ofstream outputFile("highscores.dat", std::ios::binary);

	if (outputFile.is_open())
	{
		// Write all the highscore entries to the file.
		for (const HighScoreEntry& entry : highScores)
		{
			outputFile.write(reinterpret_cast<const char*>(&entry), sizeof(entry));
		}

		outputFile.close();
	} 
}

void Highscore::ClearHighScores()
{
	highScores.clear();
	SaveHighScore(); 
}

std::vector<HighScoreEntry> Highscore::GetTopHighScores(int count)
{
	// Ensure that we don't request more highscores than are available.	
	count = std::min(count, static_cast<int>(highScores.size()));

	// Return a vector containing the top 'count' highscore entries.
	return std::vector<HighScoreEntry>(highScores.begin(), highScores.begin() + count);
}


void Highscore::SetPlayerName(const std::string& name)
{
	playerName = name;
}

const std::string& Highscore::GetPlayerName() const
{
	return playerName;
}

void Highscore::HandleNameInput()
{
	if (IsKeyPressed(KEY_BACKSPACE) && !playerName.empty())
	{
		playerName.pop_back(); 
	}

	for (int key = KEY_A; key <= KEY_Z; ++key)
	{
		char character = static_cast<char>('A' + (key - KEY_A));

		if (IsKeyReleased(key) && playerName.length() < 3)
		{
			playerName += character;
		}
	}
}
