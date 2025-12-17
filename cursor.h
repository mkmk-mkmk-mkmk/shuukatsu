#pragma once
#include "main.h"
#include <iostream>
#include <memory>

#include "framework.h"
#include "renderer.h"
#include "gameObject.h"

#include "particle_touch.h"

class Cursor : public GameObject
{
private:
	ID3D11Buffer* m_VertexBuffer;

	ID3D11InputLayout* m_VertexLayout;
	ID3D11VertexShader* m_VertexShader;
	ID3D11PixelShader* m_PixelShader;

	ID3D11ShaderResourceView* m_Texture;

	bool	m_ButtonUse = false;	//ボタン使用中

	Vector2  m_CursorLockPos;		//カーソル位置固定座標

	POINT pt;

	bool m_MakeParticle = true; //パーティクルを生成するかどうか
	std::vector<std::unique_ptr<ParticleTouch>> m_ParticleTouchList;

public:
	void Init();
	void Update();
	void Uninit();
	void Draw();

	bool GetMakeParticle() { return m_MakeParticle; }
	void SetMakeParticle(bool make)
	{
		m_MakeParticle = make;
	}
};

