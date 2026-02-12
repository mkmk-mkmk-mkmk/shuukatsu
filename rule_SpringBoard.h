#pragma once

#include "rule_SpringChain.h"

class Rule_SpringBoard : public GameObject
{
private:
	Rule_ChainPoint* m_LeftPoint;
	Rule_ChainPoint* m_RightPoint;

	Vector2 m_PlayerPos;
	Vector2 m_PlayerScale;
	Vector2 m_PlayerVector;

	float m_BoardLength;

public:

	void Init(Rule_ChainPoint* leftPoint, Rule_ChainPoint* rightPoint, float height);
	void Update();
	void Draw();

};
