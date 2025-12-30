#pragma once

#include "UI.h"

class PlayerTrailBar : public UI
{
private:

	float m_TrailTime;		//æ“¾‚µ‚Ä‚¢‚éÄ¶‚Ì‘±ŠÔ
	float m_ReverseTime;	//æ“¾‚µ‚Ä‚¢‚é‹tÄ¶‚Ì‘±ŠÔ

	bool m_GettingTrail;	//‹OÕ‚ğæ“¾’†‚©‚Ç‚¤‚©
	bool m_GettingReverse;	//‹tÄ¶‚ğæ“¾’†‚©‚Ç‚¤‚©

public:

	void Init();
	void Uninit();
	void Update();
	void Draw();

};
