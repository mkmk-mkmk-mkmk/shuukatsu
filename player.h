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

	Vector2 m_ClickPos;

	std::list<Vector2> m_TrailPosList;
	std::list<Vector2> m_TrailDiffList;
	bool m_HaveTrail = false;		//軌跡を持っているかどうか

	PlayerState m_PlayerState = PlayerState::Normal;	//プレイヤーの状態

public:
	void Init();
	void Uninit();
	void Update();
	void Draw();

	void SetPlayerPosition(Vector2 position) { m_Position = position; }

	Vector2 GetPlayerPosition()
	{
		return m_Position;
	}

	Vector2 GetPlayerVector()
	{
		return m_Vector;
	}

	void PlayerMove();	//プレイヤー移動処理

	bool EnemyCollision(const Vector2& enemyPosition, float enemyWidth, float enemyHeight);

protected:

	void BoxCollisionExtraStep(Vector2 boxPos, Vector2 boxScale) override
	{
		//衝突判定
		if (m_Position.x + m_Scale.x * 0.5f > boxPos.x - boxScale.x * 0.5f &&
			m_Position.x - m_Scale.x * 0.5f < boxPos.x + boxScale.x * 0.5f &&
			m_Position.y + m_Scale.x * 0.5f > boxPos.y - boxScale.y * 0.5f &&
			m_Position.y - m_Scale.x * 0.5f < boxPos.y + boxScale.y * 0.5f)
		{

			if (m_Position.y < boxPos.y	//ボックスの上に乗っている場合
				&& m_Position.x >= boxPos.x - boxScale.x * 0.5f && m_Position.x <= boxPos.x + boxScale.x * 0.5f)
			{
				m_Position.y = boxPos.y - boxScale.y * 0.5f - m_Scale.y * 0.5f; //位置をボックスの上に調整
				m_OnGround = true;
				m_Vector.y = 0.0f; //落下速度リセット
			}
			else if (m_Position.y > boxPos.y	//ボックスの下にいる場合
				&& m_Position.x >= boxPos.x - boxScale.x * 0.5f && m_Position.x <= boxPos.x + boxScale.x * 0.5f)
			{
				m_Position.y = boxPos.y + boxScale.y * 0.5f + m_Scale.y * 0.5f; //位置をボックスの下に調整
				m_Vector.y = 0.0f; //落下速度リセット
			}
			else if (m_Position.x < boxPos.x	//ボックスの左にいる場合
				&& m_Position.y >= boxPos.y - boxScale.y * 0.5f && m_Position.y <= boxPos.y + boxScale.y * 0.5f)
			{
				m_Position.x = boxPos.x - boxScale.x * 0.5f - m_Scale.x * 0.5f; //位置をボックスの左に調整
			}
			else if (m_Position.x > boxPos.x	//ボックスの右にいる場合
				&& m_Position.y >= boxPos.y - boxScale.y * 0.5f && m_Position.y <= boxPos.y + boxScale.y * 0.5f)
			{
				m_Position.x = boxPos.x + boxScale.x * 0.5f + m_Scale.x * 0.5f; //位置をボックスの右に調整
			}
		}
	}

};
