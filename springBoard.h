#pragma once

#include "springChain.h"

class SpringBoard : public GameObject
{
private:
	ChainPoint* m_LeftPoint;
	ChainPoint* m_RightPoint;

	Vector2 m_PlayerPos;
	Vector2 m_PlayerScale;
	Vector2 m_PlayerVector;

	float m_BoardLength;

public:

	void Init(ChainPoint* leftPoint, ChainPoint* rightPoint, float height);
	//void Uninit();
	void Update();
	void Draw();

};
