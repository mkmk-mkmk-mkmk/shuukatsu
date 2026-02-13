#pragma once
#include "gameObject.h"
#include "enemy.h"

class DamageBox : public GameObject
{
private:

	Vector2  m_FirstPos; //ボックス初期座標

	int m_Damage = 1;

public:
	void Init();
	void Uninit();
	void Update();
	void Draw();

};