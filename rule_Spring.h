
#pragma once

#include "gameObject.h"
#include "rule_SpringChain.h"
#include "rule_SpringBoard.h"

class Rule_Spring : public GameObject
{
private:

	Rule_SpringChain leftChain;
	Rule_SpringChain rightChain;

	Rule_SpringBoard board;

public:

	void Init(Vector2 topLeftPos, Vector2 bottomLeftPos, Vector2 topRightPos, Vector2 bottomRightPos,
		float chainSplit, float chainSplitRight, float chainWidth, float boardHeight, int gameObjectNumber);
	void Uninit();
	void Update();
	void Draw();

};
