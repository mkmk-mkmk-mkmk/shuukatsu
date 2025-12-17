#include "main.h"
#include "manager.h"
#include "renderer.h"
#include "sprite.h"
#include "input.h"
#include "game.h"
#include "title.h"

#include "cursor.h"
#include "polygon.h"
#include "player.h"
//#include "map.h"

Cursor cursor;
Polygon2D polygon;
//MapManager mapmanager;

Scene* Manager::m_Scene = nullptr;
Scene* Manager::m_NextScene = nullptr;
Scene* Manager::m_FadeScene = nullptr;

Fade* Manager::m_Fade = new Fade();

void Manager::Init()
{
	Renderer::Init();
	Input::Init();
	m_Fade->Init();
	//InitSprite();

	m_Scene = new Title();
	m_Scene->Init();

 }


void Manager::Uninit()
{
	m_Scene->Uninit();

	Input::Uninit();
	Renderer::Uninit();
}

void Manager::Update()
{
	Input::Update();

	if (m_NextScene != nullptr && m_Fade->GetFadeOutFinished())
	{
		m_Scene->Uninit();

		m_Scene = m_NextScene;
		m_NextScene = nullptr;

		m_Scene->Init();
		m_Scene->Update();
	}
	else if (m_NextScene != nullptr && !m_Fade->GetFade())
	{
		m_Fade->SetFade(true);
		m_Scene->Update();
	}
	else
	{
		m_Scene->Update();
	}
}

void Manager::Draw()
{
	Renderer::Begin();

	m_Scene->Draw();

	if (!m_Fade->GetFade())
	{

	}
	else if (m_Fade->GetFade() && !m_Fade->GetFadeOutFinished())
	{
		m_Fade->FadeOut();
	}
	else if (m_Fade->GetFade() && !m_Fade->GetFadeInFinished())
	{
		m_Fade->FadeIn();
	}
	else if (m_Fade->GetFade() && m_Fade->GetFadeInFinished() && m_Fade->GetFadeOutFinished())
	{
		m_Fade->SetFade(false);
		m_Fade->SetFadeInFinished(false);
		m_Fade->SetFadeOutFinished(false);
	}

	Renderer::End();
}
