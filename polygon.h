#pragma once
#include"gameObject.h"

class Polygon2D:public GameObject
{

private:

	ID3D11ShaderResourceView* m_Texture;

public:
	void Init();
	void Uninit();
	void Update();
	void Draw();

	void DrawTest(float x, float y);
};