#include "main.h"
#include "manager.h"
#include "renderer.h"
#include"input.h"
#include "game.h"
#include "cursor.h"
#include "polygon.h"
#include "player.h"
//#include "map.h"

Cursor cursor;
Polygon2D polygon;
Player player;
//MapManager mapmanager;

Scene* Manager::m_Scene = nullptr;
Scene* Manager::m_NextScene = nullptr;
Scene* Manager::m_FadeScene = nullptr;


void Manager::Init()
{
	Renderer::Init();
	Input::Init();	

	m_Scene = new Game();
	m_Scene->Init();

	//cursor.Init();
	//polygon.Init();
	//player.Init();
}


void Manager::Uninit()
{
	//player.Uninit();
	//polygon.Uninit();
	//cursor.Uninit();
	m_Scene->Uninit();

	Input::Uninit();
	Renderer::Uninit();
}

void Manager::Update()
{
	Input::Update();

	m_Scene->Update();

	//cursor.Update();
	//polygon.Update();
	//player.Update();
}

void Manager::Draw()
{
	Renderer::Begin();

	m_Scene->Draw();

	//cursor.Draw();
	//polygon.Draw();
	//player.Draw();
	//mapmanager.Draw();

	Renderer::End();
}
