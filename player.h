#pragma once
#include"gameObject.h"
#include "renderer.h"
#include <list>

enum PlayerState
{
	Player_Normal,
	GettingTrail,
	HaveTrail,
	MoveTrail,
};

class Player :public GameObject
{
private:
	//int frame = 0;

	float m_Speed = 10.0f;			//プレイヤーの移動速度
	float m_JumpPower = -12.0f;		//ジャンプ力
	bool m_Dash = false;			//ダッシュ中かどうか

	Vector2 m_ClickPos;

	std::list<Vector2> m_TrailPosList;
	std::list<Vector2> m_TrailDiffList;

	bool m_HaveTrail = false;		//軌跡を持っているかどうか
	bool m_GettingTrail = false;	//軌跡取得中
	bool m_MoveTrail = false;		//軌跡移動中かどうか
	bool m_TrailType = false;		//軌跡の種類。true：再生、false：逆再生

	PlayerState m_PlayerState = PlayerState::Player_Normal;	//プレイヤーの状態

	VERTEX_3D vertex[4];

public:
	void Init();
	void Uninit();
	void Update();
	void Draw();

	//軌跡を少しでも持っているかどうか取得
	bool GetPlayerHaveTrail()
	{
		if (m_GettingTrail || m_HaveTrail)
		{
			return true;
		}
		
		return false;
	}

	//軌跡を取得し終えて、持っている状態かどうか
	bool GetHaveTrail()
	{
		return m_HaveTrail;
	}

	void PlayerMove();	//プレイヤー移動処理
	//移動中かどうか取得
	bool GetMoveTrail()
	{
		return m_MoveTrail;
	}

	void BoxCollisionExtra(Vector2 objectPos, Vector2 objectScale, Vector2 boxPos, Vector2 boxScale) override;

	PlayerState GetPlayerState()
	{
		return m_PlayerState;
	}

	int GetTrailTime()
	{
		return m_TrailDiffList.size();
	}

	bool GetTrailType()
	{
		return m_TrailType;
	}

};
