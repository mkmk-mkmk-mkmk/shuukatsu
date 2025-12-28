#pragma once

#include "gameObject.h"

class BackGround : public GameObject
{
private:

	ID3D11ShaderResourceView* m_Texture;


public:
	void Init(int sceneType);
	void Uninit();
	void Update();
	void Draw();

};