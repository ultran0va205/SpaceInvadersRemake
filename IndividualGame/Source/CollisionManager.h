#pragma once
#include "raylib.h"

enum class Shape
{
	NONE, CIRCLE, LINE, RECTANGLE 
};

struct Circle
{
	Vector2 pos{};
	float radius;
	Color color;
};

struct Line
{
	Vector2 startPos{};
	Vector2 endPos{};
	Color color;
};

struct AABB
{
	Vector2 pos{};
	Vector2 size{};
	Color color;
};

class Collisions
{
public:

	Shape staticShape{};
	Shape dynamicShape{};

	//Static Shapes
	Circle staticCircle = { {(float)GetScreenWidth()/2, 500}, 80, PINK};
	AABB staticRect = { {500, 400}, {200,350}, PINK };
	Line staticLine = { {150, 400},{1050, 600}, DARKBROWN };

	//Dynamic Shapes
	Circle dynamicCircle = { {GetMousePosition()}, 50, SKYBLUE};
	AABB dynamicRect = { {GetMousePosition()}, {150, 250}, SKYBLUE };


	bool CircleCircleCollision(Vector2 circle1Pos, float circle1Radius, Vector2 circle2Pos, float circle2Radius);
	bool CircleLineCollision(Circle circle, Line line);
	bool CircleAABBCollision(Circle circle, AABB rect);
	bool AABBAABBCollision(AABB rect1, AABB rect2);

	void CollisionCheck();
	void ShapeUpdate();

	void Update();
	void Render();
};



