
#include "framework.h"
#include "main.h"
#include "texture.h"

#include "manager.h"
#include "scene.h"
#include "title.h"
#include "gameOver_Button.h"

void GameOverButton::Init()
{
	m_ButtonList.push_back(button);
	m_ButtonList[0].Init(Vector2((float)screenWidth * 0.5f, (float)screenHeight * 0.8),
		Vector2((float)screenWidth * 0.4, (float)screenHeight * 0.2f),
		Texture::Load("asset\\texture\\UI\\StartButton.png"),
		Texture::Load("asset\\texture\\UI\\StartButton_2.png"));
}

void GameOverButton::Uninit()
{

}

void GameOverButton::Update()
{
	for (int i = 0; i < m_ButtonList.size(); i++)
	{
		m_ButtonList[i].Update();
	}

	if (m_ButtonList[0].GetAction() == true)
	{
		Manager::SetScene<Title>();
	}
}

void GameOverButton::Draw()
{
	for (int i = 0; i < m_ButtonList.size(); i++)
	{
		m_ButtonList[i].Draw();
	}
}