#pragma once
#include "raylib.h"

//-------------------------PLAYER CONSTANTS----------------------
constexpr float PLAYERMOVEMENTSPEED = 8.0f;
constexpr Vector2 PLAYERSPAWNPOS = {600.0f,1200.0f};
constexpr Vector2 PLAYERSIZE =  { 50.0f, 60.0f };
constexpr float PLAYERPROJECTILERADIUS = 7.0f;
constexpr float PLAYERPROJECTILESPEED = 5.0f;
constexpr Vector2 PLAYERPROJECTILEDIRECTION = {0.0f, -1.0f};
constexpr float PLAYERSHOTCOOLDOWN = 40.0f;


//-------------------------ENEMY CONSTANTS----------------------
constexpr int ENEMYSPAWNPOS_X = 200;
constexpr int ENEMYSPAWNPOS_Y = 70;
constexpr Vector2 ENEMYSIZE = { 60.0f, 50.0f };
constexpr Vector2 ENEMYPROJECTILESIZE = { 10.0f, 15.0f };
constexpr int ENEMYPROJECTILESPEED = 10;
constexpr Vector2 ENEMYPROJECTILEDIRECTION = {0.0f, -1.0f};

//------------------------------GRID CONSTANTS-------------------
constexpr int NUMROWS = 5;
constexpr int NUMCOLUMNS = 4;
constexpr float XSPACING = 200.0f;
constexpr float YSPACING = 100.0f;



