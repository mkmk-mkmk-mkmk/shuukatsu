#pragma once

#include "UI.h"

class PlayerTrailBar : public UI
{
private:

	int m_TrailTime;		//取得している再生の持続時間
	int m_ReverseTime;		//取得している逆再生の持続時間

	bool m_HaveTrail;		//軌跡を取得中、また輪取得済みかどうか
	bool m_TrailType;		//軌跡の種類。true：再生、false：逆再生

public:

	void Init();
	void Uninit();
	void Update();
	void Draw();

};
