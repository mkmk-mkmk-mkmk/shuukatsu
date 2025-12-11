#pragma once

#include <list>
#include <vector>
#include "gameObject.h"
#include "UI.h"
#include "enemy.h"

class Scene
{
private:
	std::list<GameObject*>m_GameObject[10];
	std::list<UI*>m_UI[50];

public:
	virtual void Init();
	virtual void Uninit();
	virtual void Update();
	virtual void Draw();

	//GameObject
	template <typename T>
	T* AddGameObject(int Layer)
	{
		T* gameObject = new T();
		//gameObject->Init();
		m_GameObject[Layer].push_back(gameObject);

		return gameObject;
	}

	template <typename T>
	T* GetGameObject()
	{
		for (int i = 0; i < 10; i++)
		{
			for (auto gameObject : m_GameObject[i])
			{
				T* find = dynamic_cast<T*>(gameObject);
				if (find != nullptr)
					return find;
			}
		}
		return nullptr;
	}

	template <typename T>
	std::vector<T*> GetGameObjects()
	{
		std::vector<T*> finds;
		for (int i = 0; i < 10; i++)
		{
			for (auto gameObject : m_GameObject[i])
			{
				T* find = dynamic_cast<T*>(gameObject);
				if (find != nullptr)
					finds.push_back(find);
			}
		}
		return finds;
	}

	int GetGameObjectNum(int Layer)
	{
		return m_GameObject[Layer].size();
	}

	//UI
	template <typename T>
	T* AddUI(int Layer)
	{
		T* ui = new T();
		//ui->Init();
		m_UI[Layer].push_back(ui);

		return ui;
	}

	template <typename T>
	T* GetUI()
	{
		for (int i = 0; i < 10; i++)
		{
			for (auto ui : m_UI[i])
			{
				T* find = dynamic_cast<T*>(ui);
				if (find != nullptr)
					return find;
			}
		}
		return nullptr;
	}

	template <typename T>
	std::vector<T*> GetUIs()
	{
		std::vector<T*> finds;
		for (int i = 0; i < 10; i++)
		{
			for (auto ui : m_UI[i])
			{
				T* find = dynamic_cast<T*>(ui);
				if (find != nullptr)
					finds.push_back(find);
			}
		}
		return finds;
	}

	int GetUINum(int Layer)
	{
		return m_UI[Layer].size();
	}

};