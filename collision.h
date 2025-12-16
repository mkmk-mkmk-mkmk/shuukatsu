#pragma once

#include "Vector2.h"

struct AABB
{
	Vector2 pos;
	Vector2 scale;
};

class Collision
{
public:

	bool BoxCollisionCommon(Vector2 objectPos, Vector2 objectScale, Vector2 boxPos, Vector2 boxScale);

	virtual void BoxCollisionExtra(Vector2 objectPos, Vector2 objectScale, Vector2 boxPos, Vector2 boxScale) {}


	void BoxCollision(Vector2 objectPos, Vector2 objectScale, Vector2 boxPos, Vector2 boxScale);
};

