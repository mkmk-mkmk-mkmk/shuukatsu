#include "main.h"
#include "renderer.h"
#include "Vector2.h"
#include "texture.h"
#include "cursor.h"
#include "scene.h"
#include "manager.h"

#include "fade.h"

void Fade::Init()
{
	InitSprite();
	m_TextureList.push_back(Texture::Load("asset\\texture\\fade.png"));
}

void Fade::FadeIn()
{
	if (!m_FadeInFinished)
	{
		m_Alpha -= m_FadeSpeed;

		if (m_Alpha <= 0.0f)
		{
			m_Alpha = 0.0f;
			m_FadeInFinished = true;
		}

		Draw();
	}
}

void Fade::FadeOut()
{
	if (!m_FadeOutFinished)
	{
		m_Alpha += m_FadeSpeed;

		if (m_Alpha >= 1.0f)
		{
			m_Alpha = 1.0f;
			m_FadeOutFinished = true;
		}

		Draw();
	}
}

void Fade::Uninit()
{
	m_VertexBuffer->Release();
	m_VertexLayout->Release();
	m_VertexShader->Release();
	m_PixelShader->Release();

}

void Fade::Draw()
{
	DrawSprite(XMFLOAT2(screenWidth * 0.5f, screenHeight * 0.5f), 0.0f, XMFLOAT2(screenWidth, screenHeight), 0, m_Alpha);
}