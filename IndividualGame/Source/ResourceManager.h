#pragma once
#include "raylib.h"

//the structure is from an old project
struct Textures
{
	Texture2D player;
	Texture2D strongEnemy;
	Texture2D weakEnemy;
	Texture2D background;
	Texture2D foreground;
	Texture2D fireFrames_Right[3];
	Texture2D fireFrames_Left[3];

};

class ResourceManager
{
public:
	static Textures textures;
	static void LoadResources();
	static void UnloadResources(); 
};