#include "main.h"
#include "renderer.h"
#include "texture.h"
#include "input.h"

#include "scene.h"
#include "manager.h"
#include "sprite.h"
#include "player.h"

#include "UI_TrailSecond.h"

void UI_TrailSecond::Init()
{
	InitSprite();

	m_TextureList.push_back(Texture::Load("asset\\texture\\UI\\copyTime.png"));
	m_TextureList.push_back(Texture::Load("asset\\texture\\UI\\numbers.png"));
	m_TextureList.push_back(Texture::Load("asset\\texture\\UI\\point.png"));
	m_TextureList.push_back(Texture::Load("asset\\texture\\UI\\sec.png"));

}

void UI_TrailSecond::Uninit()
{
	m_VertexBuffer->Release();
	m_VertexLayout->Release();
	m_VertexShader->Release();
	m_PixelShader->Release();
}

void UI_TrailSecond::Update()
{
	m_TrailTime = Manager::GetScene()->GetGameObject<Player>()->GetTrailTime();
	m_TrailType = Manager::GetScene()->GetGameObject<Player>()->GetTrailType();

	m_TrailSecond = m_TrailTime / 60.0f;

	m_TrailSecond_001 = (fmodf(m_TrailSecond * 10.0f, 1.0f)) * 10.0f;
	m_TrailSecond_01 = (fmodf(m_TrailSecond, 1.0f)) * 10.0f;
	m_TrailSecond_1 = (int)m_TrailSecond % 10;
	m_TrailSecond_10 = (int)m_TrailSecond / 10;
}

void UI_TrailSecond::Draw()
{
	//テキスト
	DrawSpriteAnim(XMFLOAT2(screenWidth * 0.65f, screenHeight * 0.05f), 0.0f,
		XMFLOAT2(screenWidth * 0.25f, screenHeight * 0.1f), 1, 1, 1, 0, 1.0f, false);

	//10の位
	DrawSpritePiece(XMFLOAT2(screenWidth * 0.8f, screenHeight * 0.05f), m_Rotate,
		XMFLOAT2(screenWidth * 0.05f, screenHeight * 0.07f), m_TrailSecond_10, 10, 1, 1, 1.0f, false);

	//1の位
	DrawSpritePiece(XMFLOAT2(screenWidth * 0.83f, screenHeight * 0.05f), m_Rotate,
		XMFLOAT2(screenWidth * 0.05f, screenHeight * 0.07f), m_TrailSecond_1, 10, 1, 1, 1.0f, false);

	//小数点
	DrawSpriteAnim(XMFLOAT2(screenWidth * 0.85f, screenHeight * 0.07f), m_Rotate,
		XMFLOAT2(screenWidth * 0.05f, screenHeight * 0.05), 1, 1, 1, 2, 1.0f, false);

	//0.1の位
	DrawSpritePiece(XMFLOAT2(screenWidth * 0.87f, screenHeight * 0.05f), m_Rotate,
		XMFLOAT2(screenWidth * 0.05f, screenHeight * 0.07f), m_TrailSecond_01, 10, 1, 1, 1.0f, false);

	//0.01の位
	DrawSpritePiece(XMFLOAT2(screenWidth * 0.9f, screenHeight * 0.05f), m_Rotate,
		XMFLOAT2(screenWidth * 0.05f, screenHeight * 0.07f), m_TrailSecond_001, 10, 1, 1, 1.0f, false);

	//秒
	DrawSpriteAnim(XMFLOAT2(screenWidth * 0.95f, screenHeight * 0.05f), 0.0f,
		XMFLOAT2(screenWidth * 0.06f, screenHeight * 0.03f), 1, 1, 1, 3, 1.0f, false);

}

