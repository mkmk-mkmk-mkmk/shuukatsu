#pragma once
#include"gameObject.h"
#include "renderer.h"
#include <list>

enum PlayerState
{
	Normal,
	GettingTrail,
	HaveTrail,
	MoveTrail,
};

class Player :public GameObject
{
private:
	ID3D11Buffer* m_VertexBuffer;

	ID3D11InputLayout* m_VertexLayout;
	ID3D11VertexShader* m_VertexShader;
	ID3D11PixelShader* m_PixelShader;

	ID3D11ShaderResourceView* m_Texture[4];

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

	PlayerState m_PlayerState = PlayerState::Normal;	//プレイヤーの状態

	VERTEX_3D vertex[4];

public:
	void Init();
	void Uninit();
	void Update();
	void Draw();

	void PlayerMove();	//プレイヤー移動処理

	bool GetMoveTrail()
	{
		return m_MoveTrail;
	}

	void BoxCollisionExtra(Vector2 objectPos, Vector2 objectScale, Vector2 boxPos, Vector2 boxScale) override;

	PlayerState GetPlayerState()
	{
		return m_PlayerState;
	}

};
