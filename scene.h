#pragma once

#include<list>
#include<vector>
#include"gameObject.h"

class Scene
{
private:
	std::list<GameObject*>m_GameObject[10];

public:
	virtual void Init();
	virtual void Uninit();
	virtual void Update();
	virtual void Draw();

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
	std::vector<T*>GetGameObjects()
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

};