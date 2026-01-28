
#include "framework.h"
#include "main.h"
#include "texture.h"

#include "manager.h"
#include "scene.h"
#include "stage1.h"
#include "stageSelect_Button.h"

void StageSelectButton::Init()
{
	m_ButtonList.push_back(button);
	m_ButtonList[0].Init(Vector2((float)screenWidth * 0.3f, (float)screenHeight * 0.25),
		Vector2((float)screenWidth * 0.2, (float)screenHeight * 0.4f),
		Texture::Load("asset\\texture\\UI\\StartButton.png"),
		Texture::Load("asset\\texture\\UI\\StartButton_2.png"));

	m_ButtonList.push_back(button);
	m_ButtonList[1].Init(Vector2((float)screenWidth * 0.6f, (float)screenHeight * 0.75),
		Vector2((float)screenWidth * 0.2, (float)screenHeight * 0.4f),
		Texture::Load("asset\\texture\\UI\\StartButton.png"),
		Texture::Load("asset\\texture\\UI\\StartButton_2.png"));

}

void StageSelectButton::Uninit()
{

}

void StageSelectButton::Update()
{
	for (int i = 0; i < m_ButtonList.size(); i++)
	{
		m_ButtonList[i].Update();
	}

	if (m_ButtonList[0].GetAction() == true)
	{
		Manager::SetScene<Stage1>();
	}
	else if (m_ButtonList[1].GetAction() == true)
	{
		Manager::SetScene<Stage1>();
	}

}

void StageSelectButton::Draw()
{
	for (int i = 0; i < m_ButtonList.size(); i++)
	{
		m_ButtonList[i].Draw();
	}
}