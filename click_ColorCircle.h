#pragma once

#include "Effect.h"

class Click_ColorCircle : public Effect
{
private:
	EffectData m_ColorCircle;

	bool m_Once = false;
	bool m_EndDraw = true;

public:
	Click_ColorCircle();

	void Init(Vector2 position, int type);
	void Uninit();
	void Update(bool click);
	void Draw();

	bool GetEndDraw() { return m_EndDraw; }

};