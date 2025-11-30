#include "main.h"
#include "manager.h"
#include "renderer.h"
#include "scene.h"

#include "map.h"
//#include "camera.h"
//#include "fade.h"

void Scene::Init()
{

}


void Scene::Uninit()
{
	for (int i = 0; i < 10; i++)
	{
		for (auto gameObject : m_GameObject[i])
		{
			gameObject->Uninit();
			delete gameObject;

		}
		m_GameObject[i].clear();
	}
}

void Scene::Update()
{
	for (int i = 0; i < 10; i++)
	{
		for (auto gameObject : m_GameObject[i])
		{
			gameObject->Update();
		}
	}

	for (int i = 0; i < 10; i++)
	{
		//ƒ‰ƒ€ƒ_Ž®
		m_GameObject[i].remove_if([](GameObject* object)
			{
				return object->Destroy();
			});
	}

}

void Scene::Draw()
{
	for (int i = 0; i < 10; i++)
	{
		for (auto gameObject : m_GameObject[i])
		{
			gameObject->Draw();

		}
	}
}
