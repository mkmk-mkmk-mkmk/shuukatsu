#pragma once

class Stage1 : public Scene
{
private:
	class Audio* m_BGM;

public:
	void Init();
	void Uninit();
	void Update();
	void Draw();
};