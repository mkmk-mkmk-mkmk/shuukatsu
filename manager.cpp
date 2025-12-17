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


void Manager::Init()
{
	Renderer::Init();
	Input::Init();
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

	if (m_NextScene == nullptr)
	{
		m_Scene->Update();
	}
	else
	{
		m_Scene->Uninit();

		m_Scene = m_NextScene;
		m_NextScene = nullptr;

		m_Scene->Init();
		m_Scene->Update();
	}
}

void Manager::Draw()
{
	Renderer::Begin();

	m_Scene->Draw();

	Renderer::End();
}
