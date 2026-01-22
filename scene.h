#pragma once

#include <list>
#include <vector>
#include "gameObject.h"
#include "UI.h"
#include "enemy_Ground.h"

class Scene
{
protected:
	int m_Frame = 0;

	//ヒットストップ等、瞬間的に動作を止めるフラグ
	bool m_HitStop = false;

private:
	std::list<GameObject*>m_GameObject[10];
	std::list<UI*> m_UIObject[30];

	int m_HitStopFrame = 0;
	int m_HitStopFrameMax = 10;

public:
	virtual void Init();
	virtual void Uninit();
	virtual void Update();
	virtual void Draw();

	bool HitStop();
	void SetHitStop(bool hitStop) { m_HitStop = hitStop; }

	int GetHitStopFrame() { return m_HitStopFrame; }
	int GetHitStopFrameMax() { return m_HitStopFrameMax; }

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
	T* AddUIObject(int Layer)
	{
		T* ui = new T();
		//gameObject->Init();
		m_UIObject[Layer].push_back(ui);

		return ui;
	}

	template <typename T>
	T* GetUIObject()
	{
		for (int i = 0; i < 10; i++)
		{
			for (auto ui : m_UIObject[i])
			{
				T* find = dynamic_cast<T*>(ui);
				if (find != nullptr)
					return find;
			}
		}
		return nullptr;
	}

	template <typename T>
	std::vector<T*> GetUIObjects()
	{
		std::vector<T*> finds;
		for (int i = 0; i < 10; i++)
		{
			for (auto ui : m_UIObject[i])
			{
				T* find = dynamic_cast<T*>(ui);
				if (find != nullptr)
					finds.push_back(find);
			}
		}
		return finds;
	}

	int GetUIObjectNum(int Layer)
	{
		return m_UIObject[Layer].size();
	}

};