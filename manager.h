#pragma once
#include "scene.h"

class Manager
{
private:
	static class Scene* m_Scene;
	static class Scene* m_NextScene;
	static class Scene* m_FadeScene;

public:
	static void Init();
	static void Uninit();
	static void Update();
	static void Draw();

	static Scene* GetScene() { return m_Scene; }

	template <typename T>
	static void SetScene()
	{
		m_NextScene = new T();
	}

};