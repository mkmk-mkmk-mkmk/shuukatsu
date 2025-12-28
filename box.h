#pragma once
#include "gameObject.h"
#include "enemy.h"

class Box : public GameObject
{
private:

	ID3D11ShaderResourceView* m_Texture;

	Vector2  m_FirstPos; //ボックス初期座標

public:
	void Init();
	void Uninit();
	void Update(const std::list<Enemy*>& enemies);
	void Draw();

};