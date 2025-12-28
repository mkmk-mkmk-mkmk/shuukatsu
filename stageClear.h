#pragma once

#include "scene.h"

class StageClear : public Scene
{
private:
	class Audio* m_BGM;

	ID3D11ShaderResourceView* m_Texture;


public:
	void Init();
	void Uninit();
	void Update();
	void Draw();
};