#pragma once
#include "main.h"
#include <iostream>
#include <memory>

#include "framework.h"
#include "renderer.h"
#include "UI.h"

#include "particle_touch.h"
#include "title_BreakableBox.h"

class UI_Cursor : public UI
{
private:

	bool	m_UsableButton = false; //フェードが明けてから入力を受け付ける

	bool	m_ButtonUse = false;	//ボタン使用中（パーティクル用）

	bool	m_LeftClick = false;	//左ボタンが押されているかどうか
	bool	m_RightClick = false;	//右ボタンが押されているかどうか

	bool	m_HitBox = false;		//ボックスに当たったかどうか

	Vector2  m_CursorLockPos;		//カーソル位置固定座標

	POINT pt;

	bool m_MakeParticle = true; //パーティクルを生成するかどうか
	std::vector<std::unique_ptr<ParticleTouch>> m_ParticleTouchList;

	std::vector<std::unique_ptr<Title_BreakableBox>> m_BreakableBoxList;

public:
	void Init();
	void Update();
	void Uninit();
	void Draw();

	bool GetMakeParticle() { return m_MakeParticle; }
	void SetMakeParticle(bool make)
	{
		m_MakeParticle = make;
	}
};

