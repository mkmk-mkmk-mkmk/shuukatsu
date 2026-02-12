#pragma once

#include "UI.h"
#include "enemy.h"

class UI_EnemyState : public UI
{
private:

	Vector2 m_DrawEnemyPosition;
	Vector2 m_DrawScale;
	AnimationState m_EnemyState;

public:
	void Init();
	void Uninit();
	void Update();
	void Draw();

};