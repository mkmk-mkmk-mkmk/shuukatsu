#pragma once
#include "gameObject.h"
#include "enemy_Ground.h"

class Box : public GameObject
{
private:

	Vector2  m_FirstPos; //ボックス初期座標

public:
	void Init();
	void Uninit();
	void Update(const std::list<Enemy_Ground*>& enemies);
	void Draw();

};