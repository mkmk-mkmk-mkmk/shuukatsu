#pragma once
#include "scene.h"
#include "fade.h"
#include "soundManager.h"
#include "BGM.h"
#include "SE.h"

#include <vector>

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

	std::unique_ptr<SE> m_SEPtr;

public:
	static void Init();
	static void Uninit();
	static void Update();
	static void Draw();

	static Scene* GetScene() { return m_Scene; }

	static BGM* GetBGM() { return m_BGM; }
	static SE* GetSE() { return m_SE; }

	//static void SetSE(SEType type)
	//{
	//	m_SE->Init(type);
	//}

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