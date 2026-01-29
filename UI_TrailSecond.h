#pragma once

#include "UI.h"

class UI_TrailSecond : public UI
{
private:

	float m_TrailTime;		//æ“¾‚µ‚Ä‚¢‚é‹OÕ‚Ì‘±ŠÔ
	float m_TrailSecond;	//æ“¾‚µ‚Ä‚¢‚é‹OÕ‚Ì•b”

	int m_TrailSecond_001;	//æ“¾‚µ‚Ä‚¢‚é‹OÕ‚Ì•b”i0.01•b’PˆÊj
	int m_TrailSecond_01;	//æ“¾‚µ‚Ä‚¢‚é‹OÕ‚Ì•b”i0.1•b’PˆÊj
	int m_TrailSecond_1;	//æ“¾‚µ‚Ä‚¢‚é‹OÕ‚Ì•b”i1•b’PˆÊj
	int m_TrailSecond_10;	//æ“¾‚µ‚Ä‚¢‚é‹OÕ‚Ì•b”i10•b’PˆÊj

	bool m_TrailType;		//‹OÕ‚Ìí—ŞBtrueFÄ¶AfalseF‹tÄ¶

public:

	void Init();
	void Uninit();
	void Update();
	void Draw();

};
