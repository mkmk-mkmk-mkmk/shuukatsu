#include "main.h"
#include "renderer.h"
#include "Vector2.h"
#include "texture.h"
#include "cursor.h"
#include "scene.h"
#include "manager.h"

#include "backGround.h"
#include "camera.h"

void BackGround::Init(int sceneType)
{
	m_Position = Vector2(screenWidth * 0.5f, screenHeight * 0.5f);
	m_Scale = Vector2(screenWidth, screenHeight);

	InitSprite();

	m_TextureType = sceneType;

	m_TextureList.push_back(Texture::Load("asset\\texture\\AI_BackGround.jpg"));
	m_TextureList.push_back(Texture::Load("asset\\texture\\AI_BackGround.jpg"));
	m_TextureList.push_back(Texture::Load("asset\\texture\\stageclear.png"));
	m_TextureList.push_back(Texture::Load("asset\\texture\\gameover.png"));

}

void BackGround::Uninit()
{
	m_VertexBuffer->Release();
	m_VertexLayout->Release();
	m_VertexShader->Release();
	m_PixelShader->Release();
}

void BackGround::Update()
{

}

void BackGround::Draw()
{
	DrawSprite(XMFLOAT2(m_Position.x, m_Position.y), m_Rotate, XMFLOAT2(m_Scale.x, m_Scale.y), m_TextureType, 1.0f);
}
