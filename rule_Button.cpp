
#include "framework.h"
#include "main.h"
#include "texture.h"

#include "manager.h"
#include "scene.h"
#include "title.h"
#include "collision.h"
#include "rule_Button.h"

void RuleButton::Init()
{
	m_ButtonList.push_back(button);
	m_ButtonList[0].Init(Vector2((float)screenWidth * 0.5f, (float)screenHeight * 0.88),
		Vector2((float)screenWidth * 0.8, (float)screenHeight * 0.15f),
		Texture::Load("asset\\texture\\UI\\backToTitleButton.png"),
		Texture::Load("asset\\texture\\UI\\backToTitleButton_2.png"));

}

void RuleButton::Uninit()
{

}

void RuleButton::Update()
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

void RuleButton::Draw()
{
	for (int i = 0; i < m_ButtonList.size(); i++)
	{
		m_ButtonList[i].Draw();
	}
}