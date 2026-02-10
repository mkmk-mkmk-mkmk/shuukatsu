#pragma once
#include "scene.h"
#include "fade.h"
#include "soundManager.h"

class Manager
{
private:
	static class Scene* m_Scene;
	static class Scene* m_NextScene;
	static class Scene* m_FadeScene;

	static class Fade* m_Fade;
	static class SoundManager* m_SoundManager;

public:
	static void Init();
	static void Uninit();
	static void Update();
	static void Draw();

	static Scene* GetScene() { return m_Scene; }

	template <typename T>
	static void SetScene()
	{
		if (m_Fade->GetFade())
		{
			return;
		}
		m_NextScene = new T();
	}
};