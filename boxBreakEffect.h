#pragma once

#include "Effect.h"

class BoxBreakEffect : public Effect
{
private:

	EffectData m_PieceData;

	int m_PieceCount = 10;

public:
	void Init(Vector2 position);
	void Update();
	void Draw(Vector2 drawDiff);
	void DrawPiece(int count, Vector2 drawDiff);
};