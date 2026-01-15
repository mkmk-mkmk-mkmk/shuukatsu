
#include "framework.h"
#include "main.h"
#include "renderer.h"
#include "texture.h"
#include "input.h"

#include "manager.h"
#include "scene.h"
#include "rule.h"
#include "game.h"
#include "title_Cursor.h"
#include "collision.h"
#include "UI_StartButton.h"

void UI_StartButton::Init()
{
	m_ButtonList.push_back(button);
	m_ButtonList[0].Init(Vector2((float)screenWidth * 0.5f, (float)screenHeight * 0.65),
		Vector2((float)screenWidth * 0.3, (float)screenHeight * 0.2f),
		Texture::Load("asset\\texture\\UI\\StartButton.png"),
		Texture::Load("asset\\texture\\UI\\StartButton_2.png"));

	m_ButtonList.push_back(button);
	m_ButtonList[1].Init(Vector2((float)screenWidth * 0.5f, (float)screenHeight * 0.9),
		Vector2((float)screenWidth * 0.25, (float)screenHeight * 0.15f),
		Texture::Load("asset\\texture\\UI\\StartButton.png"),
		Texture::Load("asset\\texture\\UI\\StartButton_2.png"));

}

void UI_StartButton::Uninit()
{

}

void UI_StartButton::Update()
{
	for (int i = 0; i < m_ButtonList.size(); i++)
	{
		m_ButtonList[i].Update();
	}

	if (m_ButtonList[0].GetAction() == true)
	{
		Manager::SetScene<Game>();
	}
	else if (m_ButtonList[1].GetAction() == true)
	{
		Manager::SetScene<Rule>();
	}

}

void UI_StartButton::Draw()
{
	for (int i = 0; i < m_ButtonList.size(); i++)
	{
		m_ButtonList[i].Draw();
	}
}