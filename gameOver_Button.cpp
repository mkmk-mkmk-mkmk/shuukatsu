
#include "framework.h"
#include "main.h"
#include "texture.h"

#include "manager.h"
#include "scene.h"
#include "title.h"
#include "stageSelect.h"
#include "gameOver_Button.h"

void GameOverButton::Init()
{
	m_ButtonList.push_back(button);
	m_ButtonList[0].Init(Vector2((float)screenWidth * 0.25f, (float)screenHeight * 0.8),
		Vector2((float)screenWidth * 0.3, (float)screenHeight * 0.15f),
		Texture::Load("asset\\texture\\UI\\titleButton.png"),
		Texture::Load("asset\\texture\\UI\\titleButton_2.png"));

	m_ButtonList.push_back(button);
	m_ButtonList[1].Init(Vector2((float)screenWidth * 0.75f, (float)screenHeight * 0.8),
		Vector2((float)screenWidth * 0.3, (float)screenHeight * 0.15f),
		Texture::Load("asset\\texture\\UI\\stageSelectButton.png"),
		Texture::Load("asset\\texture\\UI\\stageSelectButton_2.png"));

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

	if (m_ButtonList[1].GetAction() == true)
	{
		Manager::SetScene<StageSelect>();
	}

}

void GameOverButton::Draw()
{
	for (int i = 0; i < m_ButtonList.size(); i++)
	{
		m_ButtonList[i].Draw();
	}
}