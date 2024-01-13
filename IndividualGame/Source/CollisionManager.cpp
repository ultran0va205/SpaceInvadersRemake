#pragma once
#include "raylib.h"
#include "CollisionManager.h"

//looked at https://www.jeffreythompson.org/collision-detection/circle-circle.php to understand the logic
bool Collisions::CircleCircleCollision(Vector2 staticPos, float staticRadius, Vector2 dynamicPos, float dynamicRadius)
{
	float dx = staticPos.x - dynamicPos.x; 
	float dy = staticPos.y - dynamicPos.y;

	float distanceSqrt = ((dx * dx) + (dy * dy));
	float radiiSum = dynamicRadius + staticRadius;

	if (distanceSqrt <= (radiiSum * radiiSum))
	{
		return true;
	}

	return false;
}

//looked at https://www.jeffreythompson.org/collision-detection/circle-rect.php
bool Collisions::CircleAABBCollision(Circle circle, AABB rect)
{
	float closestX;
	float closestY;

	if (circle.pos.x < rect.pos.x) 
	{
		closestX = rect.pos.x;
	}
	else if (circle.pos.x > rect.pos.x + rect.size.x) 
	{
		closestX = rect.pos.x + rect.size.x;
	}
	else 
	{
		closestX = circle.pos.x;
	}

	if (circle.pos.y < rect.pos.y) 
	{
		closestY = rect.pos.y;
	}
	else if (circle.pos.y > rect.pos.y + rect.size.y)
	{
		closestY = rect.pos.y + rect.size.y;
	}
	else 
	{
		closestY = circle.pos.y;
	}

	float distSquared = ((circle.pos.x - closestX) * (circle.pos.x - closestX)) + 
		((circle.pos.y - closestY) * (circle.pos.y - closestY));

	if (distSquared <= circle.radius * circle.radius)
	{
		return true;
	}

	return false;
}

//looked at https://www.jeffreythompson.org/collision-detection/line-circle.php for understanding
bool Collisions::CircleLineCollision(Circle circle, Line line)
{
	float dx = circle.pos.x - line.startPos.x;
	float dy = circle.pos.y - line.startPos.y;

	float lineVectorX = line.endPos.x - line.startPos.x;
	float lineVectorY = line.endPos.y - line.startPos.y;

	float lineLengthSquared = ((lineVectorX * lineVectorX) + (lineVectorY * lineVectorY));

	float dotProduct = ((dx * lineVectorX) + (dy * lineVectorY));

	float t = dotProduct / lineLengthSquared;

	if (t < 0) 
	{
		t = 0;
	}
	else if (t > 1) 
	{
		t = 1;
	}

	float closestX = line.startPos.x + (t * lineVectorX);
	float closestY = line.startPos.y + (t * lineVectorY);

	float a = ((circle.pos.x - closestX) * (circle.pos.x - closestX));
	float b = ((circle.pos.y - closestY) * (circle.pos.y - closestY));

	float distSquared = a + b;
		
	if (distSquared <= circle.radius * circle.radius)
	{
		return true;
	} 
	return false;

}


bool Collisions::AABBAABBCollision(AABB rect1, AABB rect2)
{
	bool CollisionX = (rect1.pos.x + rect1.size.x  >= rect2.pos.x) && (rect2.pos.x + rect2.size.x >= rect1.pos.x);
	bool CollisionY = (rect1.pos.y + rect1.size.y >= rect2.pos.y) && (rect2.pos.y + rect2.size.y >= rect1.pos.y);

	if (CollisionX && CollisionY)
	{
		return true;
	}

	return false;

}
 

void Collisions::CollisionCheck()
{
	//CircleCircle
	if (staticShape == Shape::CIRCLE && dynamicShape == Shape::CIRCLE)
	{
		if (CircleCircleCollision(staticCircle.pos,staticCircle.radius, dynamicCircle.pos,dynamicCircle.radius))
		{
			true;
			staticCircle.color = RED;
			dynamicCircle.color = RED;
			DrawText("Collided", 550, 800, 40, BLACK);
		}
		else
		{
			false;
			staticCircle.color = PINK;
			dynamicCircle.color = SKYBLUE;
		}
	}
	//CircleBox
	if (staticShape == Shape::RECTANGLE && dynamicShape == Shape::CIRCLE)
	{
		if (CircleAABBCollision(dynamicCircle, staticRect))
		{
			true;
			staticRect.color = RED;
			dynamicCircle.color = RED;
			DrawText("Collided", 550, 800, 40, BLACK);
		}
		else
		{
			false;
			staticRect.color = PINK;
			dynamicCircle.color = SKYBLUE;
		}
	}
	//CircleLine
	if (staticShape == Shape::LINE && dynamicShape == Shape::CIRCLE)
	{
		if (CircleLineCollision(dynamicCircle,staticLine))
		{
			true;
			staticLine.color = RED;
			dynamicCircle.color = RED;
			DrawText("Collided", 550, 800, 40, BLACK);
		}
		else
		{
			false;
			staticLine.color = DARKBROWN;
			dynamicCircle.color = SKYBLUE;
		}
	}
	//AABBvsAABB
	if (staticShape == Shape::RECTANGLE && dynamicShape == Shape::RECTANGLE)
	{
		if (AABBAABBCollision(staticRect,dynamicRect))
		{
			 true;
			staticRect.color = RED;
			dynamicRect.color = RED;
			DrawText("Collided", 550, 800, 40, BLACK);
		}
		else
		{
			false;
			staticRect.color = PINK;
			dynamicRect.color = SKYBLUE;
		}
	}
}

void Collisions::ShapeUpdate()
{
	if (IsKeyPressed(KEY_F1))
	{
		staticShape = Shape::CIRCLE;
		dynamicShape = Shape::CIRCLE;
	}
	if (IsKeyPressed(KEY_F2))
	{
		staticShape = Shape::RECTANGLE;
		dynamicShape = Shape::CIRCLE;
	}
	if (IsKeyPressed(KEY_F3))
	{
		staticShape = Shape::LINE;
		dynamicShape = Shape::CIRCLE;
	}
	if (IsKeyPressed(KEY_F4))
	{
		staticShape = Shape::RECTANGLE;
		dynamicShape = Shape::RECTANGLE;
	}
	
}

void Collisions::Render()
{
	//CircleCircle
	if (staticShape == Shape::CIRCLE)
	{
		DrawCircle(staticCircle.pos.x, staticCircle.pos.y, staticCircle.radius, staticCircle.color);
	}
	if (dynamicShape == Shape::CIRCLE)
	{
		DrawCircleV({ dynamicCircle.pos }, dynamicCircle.radius, dynamicCircle.color);
	}

	//CircleBox
	if (staticShape == Shape::RECTANGLE)
	{
		DrawRectangleV(staticRect.pos, staticRect.size, staticRect.color);
	}
	if (dynamicShape == Shape::CIRCLE)
	{
		DrawCircleV({ dynamicCircle.pos }, dynamicCircle.radius, dynamicCircle.color);
	}

	//CircleLine
	if (staticShape == Shape::LINE)
	{
		DrawLineV(staticLine.startPos, staticLine.endPos, staticLine.color);
	}
	if (dynamicShape == Shape::CIRCLE)
	{
		DrawCircleV({ dynamicCircle.pos }, dynamicCircle.radius, dynamicCircle.color);
	}

	//AABBvsAABB
	if (staticShape == Shape::RECTANGLE)
	{
		DrawRectangleV(staticRect.pos, staticRect.size, staticRect.color);
	}
	if (dynamicShape == Shape::RECTANGLE)
	{
		DrawRectangleV(dynamicRect.pos, dynamicRect.size, dynamicRect.color);
	}
}

void Collisions::Update()
{
	CollisionCheck();
	ShapeUpdate();

	dynamicCircle.pos = GetMousePosition();
	dynamicRect.pos = GetMousePosition();
}





