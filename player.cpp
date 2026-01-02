#include "main.h"
#include "renderer.h"
#include "texture.h"
#include "input.h"

#include "cursor.h"
#include "scene.h"
#include "manager.h"
#include "camera.h"
#include "map.h"
#include "sprite.h"
#include "player.h"

#include "gameOver.h"

void Player::Init()
{
	//プレイヤーのライフ初期設定
	m_Life = 100;

	//プレイヤースタート位置
	float playerFirstPosX = MAPCHIP_WIDTH * 5.0f + m_Scale.x * 0.5f;
	float playerFirstPosY = Manager::GetScene()->GetGameObject<Map>()->GetBlockHeight() * MAPCHIP_HEIGHT
		- (MAPCHIP_HEIGHT + m_Scale.y * 0.5f);

	SetPosition(Vector2(playerFirstPosX, playerFirstPosY));

	//スケール設定
	m_Scale = { 100.0f,100.0f };

	InitSprite();

	m_TextureList.push_back(Texture::Load("asset\\texture\\roboR1_green.png"));
	m_TextureList.push_back(Texture::Load("asset\\texture\\roboR1_red.png"));
	m_TextureList.push_back(Texture::Load("asset\\texture\\roboR1_blue.png"));
	m_TextureList.push_back(Texture::Load("asset\\texture\\roboR1_green.png"));
}

void Player::Uninit()
{
	m_VertexBuffer->Release();
	m_VertexLayout->Release();
	m_VertexShader->Release();
	m_PixelShader->Release();
}

void Player::Update()
{
	//体力0以下でタイトルへ
	if (m_Life <= 0)
	{
		Manager::SetScene<GameOver>();
	}

	if (Input::GetKeyPress('g') || Input::GetKeyPress('G'))
	{
		m_Life = 0;
	}

	m_OldPosition = m_Position;

	m_Vector.x *= 0.5f; // 横移動リセット

	//軌跡移動中
	switch (m_PlayerState)
	{
	case PlayerState::Player_Normal:		//通常状態
	{
		//WASD移動
		PlayerMove();

		if ((Input::GetKeyPress(VK_RBUTTON) || Input::GetKeyPress(VK_LBUTTON)) && !m_HaveTrail) //クリックで軌跡取得開始
		{
			m_PlayerState = PlayerState::GettingTrail;
		}


		// 重力を適用
		if (!m_OnGround)
		{
			m_Vector.y += m_Gravity;
		}

		break;
	}
	case PlayerState::HaveTrail:	//軌跡取得済み
	{
		//WASD移動
		PlayerMove();


		if (m_HaveTrail)
		{
			if (Input::GetKeyPress(VK_SPACE)) //スペースキーで移動開始
			{
				m_PlayerState = PlayerState::MoveTrail;

			}
			else if (Input::GetKeyPress('C') || Input::GetKeyPress('c')) //Cキーで軌跡クリア
			{
				m_TrailDiffList.clear();
				m_HaveTrail = false;
			}
			else
			{

			}
		}

		// 重力を適用
		if (!m_OnGround)
		{
			m_Vector.y += m_Gravity;
		}

		break;
	}
	case PlayerState::GettingTrail:	//軌跡取得中
	{
		//WASD移動
		PlayerMove();

		m_ClickPos = Manager::GetScene()->GetUIObject<Cursor>()->GetPosition();
		if (GetAsyncKeyState(VK_RBUTTON) & 0x8000 && !m_HaveTrail) //右クリックで軌跡取得
		{
			m_GettingTrail = true;

			m_TrailDiffList.push_back(m_Vector);

			m_TrailType = true;
		}
		else if (GetAsyncKeyState(VK_LBUTTON) & 0x8000 && !m_HaveTrail) //左クリックで逆走の取得
		{
			m_GettingTrail = true;

			m_TrailDiffList.push_front(-m_Vector);

			m_TrailType = false;
		}
		else
		{
			m_HaveTrail = true;
			m_GettingTrail = false;
			m_PlayerState = PlayerState::HaveTrail;
		}

		// 重力を適用
		if (!m_OnGround)
		{
			m_Vector.y += m_Gravity;
		}
		break;
	}
	case PlayerState::MoveTrail:	//軌跡移動中
	{
		//先頭から順番に要素を取得して移動
		Vector2 diff = m_TrailDiffList.front();
		m_Vector = diff;
		m_TrailDiffList.pop_front();
		m_MoveTrail = true;

		//全て移動し終わったら終了
		if (m_TrailDiffList.empty())
		{
			m_TrailDiffList.clear();

			m_OnGround = false;
			m_HaveTrail = false;
			m_MoveTrail = false;
			m_PlayerState = PlayerState::Player_Normal;
		}
		break;
	}}
	

	//ワールド座標更新
	m_Position += m_Vector;
	
}

void Player::Draw()
{
	//テクスチャセット
	if (m_MoveTrail)
	{
		m_TextureType = 0;
	}
	else if (m_GettingTrail)
	{
		m_TextureType = 1;
	}
	else if (m_HaveTrail)
	{
		m_TextureType = 2;
	}
	else
	{
		m_TextureType = 0;
	}

	//描画位置更新
	m_DrawPosition =
		m_Position - Manager::GetScene()->GetGameObject<Camera>()->GetCameraTopLeftPosition();

	DrawSprite(XMFLOAT2(m_DrawPosition.x, m_DrawPosition.y), m_Rotate, XMFLOAT2(m_Scale.x, m_Scale.y), m_TextureType, 1.0f);

}

void Player::PlayerMove()
{

	if (Input::GetKeyPress('W') || Input::GetKeyPress('w'))
	{
		//ジャンプ
		if (m_OnGround)
		{
			m_Vector.y = m_JumpPower; //上に移動
			m_OnGround = false;
		}
	}

	if (Input::GetKeyPress('A') || Input::GetKeyPress('a'))
	{
		m_Vector.x = -m_Speed; //左に移動
		m_OnGround = false;
	}

	if (Input::GetKeyPress('S') || Input::GetKeyPress('s'))
	{
		//m_PlayerVector.y = m_Speed; //下に移動
		//m_OnGround = false;
	}

	if (Input::GetKeyPress('D') || Input::GetKeyPress('d'))
	{
		m_Vector.x = m_Speed; //右に移動
		m_OnGround = false;
	}

	if (Input::GetKeyPress(VK_SHIFT))
	{
		m_Dash = true;
		m_Vector.x *= 2.0f;
	}
	else
	{
		m_Dash = false;
	}

	if (m_Vector.x > 0)
	{
		m_Direction = true; //右向き
	}
	else if (m_Vector.x < 0)
	{
		m_Direction = false; //左向き
	}

}

void Player::BoxCollisionExtra(Vector2 objectPos, Vector2 objectScale, Vector2 boxPos, Vector2 boxScale)
{
	if (m_Position.y < boxPos.y	//ボックスの上に乗っている場合
		&& m_Position.x + m_Scale.x * 0.25f >= boxPos.x - boxScale.x * 0.5f 
		&& m_Position.x - m_Scale.x * 0.25f <= boxPos.x + boxScale.x * 0.5f)
	{
		m_Position.y = boxPos.y - boxScale.y * 0.5f - m_Scale.y * 0.5f; //位置をボックスの上に調整
		m_OnGround = true;
		m_Vector.y = 0.0f; //落下速度リセット
	}
	else if (m_Position.y > boxPos.y	//ボックスの下にいる場合
		&& m_Position.x + m_Scale.x * 0.25f >= boxPos.x - boxScale.x * 0.5f
		&& m_Position.x - m_Scale.x * 0.25f <= boxPos.x + boxScale.x * 0.5f)
	{
		m_Position.y = boxPos.y + boxScale.y * 0.5f + m_Scale.y * 0.5f; //位置をボックスの下に調整
		
		if (m_Vector.y < 0)
		{
			m_Vector.y = 0.0f; //落下速度リセット
		}
	}
	else if (m_Position.x < boxPos.x	//ボックスの左にいる場合
		&& m_Position.y + m_Scale.y * 0.25f >= boxPos.y - boxScale.y * 0.5f 
		&& m_Position.y - m_Scale.y * 0.25f <= boxPos.y + boxScale.y * 0.5f)
	{
		m_Position.x = boxPos.x - boxScale.x * 0.5f - m_Scale.x * 0.5f; //位置をボックスの左に調整
	}
	else if (m_Position.x > boxPos.x	//ボックスの右にいる場合
		&& m_Position.y + m_Scale.y * 0.25f >= boxPos.y - boxScale.y * 0.5f
		&& m_Position.y - m_Scale.y * 0.25f <= boxPos.y + boxScale.y * 0.5f)
	{
		m_Position.x = boxPos.x + boxScale.x * 0.5f + m_Scale.x * 0.5f; //位置をボックスの右に調整
	}
}

