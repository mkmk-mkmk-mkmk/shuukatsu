
#include "framework.h"
#include "main.h"
#include "texture.h"
#include "input.h"

#include "manager.h"
#include "scene.h"

#include "click_ColorCircle.h"

Click_ColorCircle::Click_ColorCircle()
{

}

void Click_ColorCircle::Init(Vector2 position, int type)
{
	m_ColorCircle.pos = position;
	m_ColorCircle.scale = screenWidth * 0.1f;
	m_ColorCircle.effectType = type;
	m_EndDraw = false;

	InitSprite();

	m_TextureList.push_back(Texture::Load("asset\\texture\\effect\\blueCircle.png"));
	m_TextureList.push_back(Texture::Load("asset\\texture\\effect\\greenCircle.png"));

}

void Click_ColorCircle::Uninit()
{
	UnInitSprite();
}

void Click_ColorCircle::Update(bool click)
{
	if (!click)
	{
		m_ColorCircle.scale *= 1.2f;
		if (m_ColorCircle.scale > screenWidth * 2.0f)
		{
			m_ColorCircle.scale = screenWidth * 2.0f;
		}
	}
	else
	{
		m_ColorCircle.scale *= 0.5f;
	}

	if (m_ColorCircle.scale <= 10.0f)
	{
		m_EndDraw = true;
	}
}

void Click_ColorCircle::Draw()
{
	DrawSprite(XMFLOAT2(m_ColorCircle.pos.x, m_ColorCircle.pos.y), 0.0f,
		XMFLOAT2(m_ColorCircle.scale, m_ColorCircle.scale), m_ColorCircle.effectType, 0.1f);

}
