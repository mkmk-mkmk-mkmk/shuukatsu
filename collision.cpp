
#include "collision.h"

bool Collision::BoxCollisionCommon(Vector2 objectPos, Vector2 objectScale, Vector2 boxPos, Vector2 boxScale)
{
	//Õ“Ë”»’è
	if (objectPos.x + objectScale.x * 0.5f > boxPos.x - boxScale.x * 0.5f &&
		objectPos.x - objectScale.x * 0.5f < boxPos.x + boxScale.x * 0.5f &&
		objectPos.y + objectScale.y * 0.5f > boxPos.y - boxScale.y * 0.5f &&
		objectPos.y - objectScale.y * 0.5f < boxPos.y + boxScale.y * 0.5f)
	{
		return true;
	}
	return false;
}

void Collision::BoxCollision(Vector2 objectPos, Vector2 objectScale, Vector2 boxPos, Vector2 boxScale)
{
	if (BoxCollisionCommon(objectPos, objectScale, boxPos, boxScale))
	{
		BoxCollisionExtra(boxPos, boxScale);
	}
}
