
#include "framework.h"
#include "main.h"
#include "texture.h"

#include "manager.h"
#include "scene.h"
#include "rule.h"
#include "stageSelect.h"
#include "collision.h"
#include "title_Button.h"

void TitleButton::Init()
{
	m_ButtonList.push_back(button);
	m_ButtonList[0].Init(Vector2((float)screenWidth * 0.5f, (float)screenHeight * 0.65),
		Vector2((float)screenWidth * 0.3, (float)screenHeight * 0.2f),
		Texture::Load("asset\\texture\\UI\\StartButton.png"),
		Texture::Load("asset\\texture\\UI\\StartButton_2.png"));

	m_ButtonList.push_back(button);
	m_ButtonList[1].Init(Vector2((float)screenWidth * 0.5f, (float)screenHeight * 0.85),
		Vector2((float)screenWidth * 0.25, (float)screenHeight * 0.15f),
		Texture::Load("asset\\texture\\UI\\StartButton.png"),
		Texture::Load("asset\\texture\\UI\\StartButton_2.png"));

}

void TitleButton::Uninit()
{

}

void TitleButton::Update()
{
	for (int i = 0; i < m_ButtonList.size(); i++)
	{
		m_ButtonList[i].Update();
	}

	if (m_ButtonList[0].GetAction() == true)
	{
		Manager::SetScene<StageSelect>();
	}
	else if (m_ButtonList[1].GetAction() == true)
	{
		Manager::SetScene<Rule>();
	}

}

void TitleButton::Draw()
{
	for (int i = 0; i < m_ButtonList.size(); i++)
	{
		m_ButtonList[i].Draw();
	}
}