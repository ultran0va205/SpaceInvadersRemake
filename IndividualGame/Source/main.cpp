#pragma once
#include "raylib.h"
#include "GameManager.h"
#include "ResourceManager.h"


int main(void)
{    
    const int screenWidth = 1200;
    const int screenHeight = 1300;

    InitWindow(screenWidth, screenHeight, "raylib [core] example - basic window");

    SetTargetFPS(60);             

    InitAudioDevice();

    auto sound = LoadSound("./hitHurt.ogg");

    GameManager manager;
    manager.game.Reset();
    manager.GS = GameStates::MAIN_MENU;
    ResourceManager::LoadResources();

    while (!WindowShouldClose())   
    {
        manager.RunGame();
        if (IsKeyPressed(KEY_SPACE))
        {     
           PlaySound(sound);   
        }

        if (IsKeyPressed(KEY_BACKSPACE))
        {
            StopSound(sound);
        }
    }
    ResourceManager::UnloadResources();

    CloseAudioDevice();
    
    CloseWindow();      

    return 0;
}