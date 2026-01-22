#include "main.h"
#include "texture.h"
#include "scene.h"
#include "manager.h"
#include "input.h"

#include "pause.h"

void Pause::Init()
{
	InitSprite();
	m_TextureList.push_back(Texture::Load("asset\\texture\\fade.png"));
}

void Pause::Update()
{
	if (Input::GetKeyTrigger(VK_RETURN))
	{
		m_SceneStop = !m_SceneStop;
	}
}

void Pause::Uninit()
{
	m_VertexBuffer->Release();
	m_VertexLayout->Release();
	m_VertexShader->Release();
	m_PixelShader->Release();
}

void Pause::Draw()
{
	DrawSprite(XMFLOAT2(screenWidth * 0.5f, screenHeight * 0.5f), 0.0f, XMFLOAT2(screenWidth, screenHeight), 0, m_Alpha);
}