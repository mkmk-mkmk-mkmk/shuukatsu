#pragma once
#include"gameObject.h"
#include <list>

enum PlayerState
{
	Normal,
	GettingTrail,
	MoveTrail,
};

class Player :public GameObject
{
private:
	ID3D11Buffer* m_VertexBuffer;

	ID3D11InputLayout* m_VertexLayout;
	ID3D11VertexShader* m_VertexShader;
	ID3D11PixelShader* m_PixelShader;

	ID3D11ShaderResourceView* m_Texture;

	//int frame = 0;

	Vector2 m_OldPosition;
	float m_Speed = 10.0f;			//プレイヤーの移動速度
	float m_JumpPower = -12.0f;		//ジャンプ力
	bool m_OnGround = false;		//地面に接地しているかどうか
	bool m_Dash = false;			//ダッシュ中かどうか

	Vector2 m_ClickPos;

	std::list<Vector2> m_TrailPosList;
	std::list<Vector2> m_TrailDiffList;
	bool m_HaveTrail = false;		//軌跡を持っているかどうか
	bool m_MoveTrail = false;		//軌跡移動中かどうか

	PlayerState m_PlayerState = PlayerState::Normal;	//プレイヤーの状態

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

	void BoxCollisionExtra(Vector2 boxPos, Vector2 boxScale) override;


};
