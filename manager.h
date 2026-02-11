#pragma once
#include "scene.h"
#include "fade.h"
#include "soundManager.h"
#include "BGM.h"
#include "SE.h"

class Manager
{
private:
	static class Scene* m_Scene;
	static class Scene* m_NextScene;
	static class Scene* m_FadeScene;

	static class Fade* m_Fade;

	static class SoundManager* m_SoundManager;
	static class BGM* m_BGM;
	static class SE* m_SE;

public:
	static void Init();
	static void Uninit();
	static void Update();
	static void Draw();

	static Scene* GetScene() { return m_Scene; }

	static BGM* GetBGM() { return m_BGM; }
	static SE* GetSE() { return m_SE; }

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