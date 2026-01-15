#pragma once

#include "title_SpringChain.h"

class Title_SpringBoard : public GameObject
{
private:
	Title_ChainPoint* m_LeftPoint;
	Title_ChainPoint* m_RightPoint;

	Vector2 m_PlayerPos;
	Vector2 m_PlayerScale;
	Vector2 m_PlayerVector;

	float m_BoardLength;

public:

	void Init(Title_ChainPoint* leftPoint, Title_ChainPoint* rightPoint, float height);
	//void Uninit();
	void Update();
	void Draw();

};
