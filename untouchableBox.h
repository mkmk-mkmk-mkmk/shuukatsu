#pragma once

#include "gameObject.h"

class UntouchableBox : public GameObject
{
private:
	Vector2  m_FirstPos; //ボックス初期座標

public:
	void Init();
	void Uninit();
	void Update();
	void Draw();

};