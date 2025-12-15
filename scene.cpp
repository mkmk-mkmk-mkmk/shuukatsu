#include "main.h"
#include "manager.h"
#include "renderer.h"
#include "scene.h"

#include "map.h"
#include "box.h"
#include "breakableBox.h"
#include "enemy.h"
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
		auto& list = m_GameObject[i];
		for (auto it = list.begin(); it != list.end(); )
		{
			GameObject* obj = *it;
			obj->Update();

			//BoxのUpdate
			if (Box* box = dynamic_cast<Box*>(obj))
			{
				//enemyのリストが必要なので作る
				std::list<Enemy*> enemies;
				for (int j = 0; j < 10; j++)
				{
					for (auto enemyObj : m_GameObject[j])
					{
						if (Enemy* enemy = dynamic_cast<Enemy*>(enemyObj))
						{
							enemies.push_back(enemy);
						}
					}
				}

				box->Update(enemies);
			}

			//BoxのUpdate
			if (BreakableBox* bBox = dynamic_cast<BreakableBox*>(obj))
			{
				//enemyのリストが必要なので作る
				std::list<Enemy*> enemies;
				for (int j = 0; j < 10; j++)
				{
					for (auto enemyObj : m_GameObject[j])
					{
						if (Enemy* enemy = dynamic_cast<Enemy*>(enemyObj))
						{
							enemies.push_back(enemy);
						}
					}
				}

				bBox->Update(enemies);
			}

			if (obj->Destroy())
			{
				it = list.erase(it);	//ここでeraseした次のイテレータを返してる
			}
			else
			{
				it++;
			}
		}
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
