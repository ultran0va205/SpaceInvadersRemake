#include "ResourceManager.h"

Textures ResourceManager::textures;

void ResourceManager::LoadResources()
{
	textures.player = LoadTexture("./Assets/spaceship.png");
	textures.strongEnemy = LoadTexture("./Assets/strongEnemy.png");
	textures.weakEnemy = LoadTexture("./Assets/weakEnemy.png");

	textures.background = LoadTexture("./Assets/backgroundSpace.png");
	textures.foreground = LoadTexture("./Assets/foregroundSpace.png");

	textures.fireFrames_Right[0] = LoadTexture("./Assets/FireFrameRight1.png");
	textures.fireFrames_Right[1] = LoadTexture("./Assets/FireFrameRight3.png");
	textures.fireFrames_Right[2] = LoadTexture("./Assets/FireFrameRight2.png");

	textures.fireFrames_Left[0] = LoadTexture("./Assets/FireFrameLeft1.png");
	textures.fireFrames_Left[1] = LoadTexture("./Assets/FireFrameLeft3.png");
	textures.fireFrames_Left[2] = LoadTexture("./Assets/FireFrameLeft2.png");


}

void ResourceManager::UnloadResources()
{
	UnloadTexture(textures.player);
	UnloadTexture(textures.strongEnemy);
	UnloadTexture(textures.weakEnemy);
	UnloadTexture(textures.background);
	UnloadTexture(textures.foreground);

	UnloadTexture(textures.fireFrames_Right[0]);
	UnloadTexture(textures.fireFrames_Right[1]);
	UnloadTexture(textures.fireFrames_Right[2]);

	UnloadTexture(textures.fireFrames_Left[0]);
	UnloadTexture(textures.fireFrames_Left[1]);
	UnloadTexture(textures.fireFrames_Left[2]);

}