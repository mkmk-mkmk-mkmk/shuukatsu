#include "main.h"
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
#include "stageClear.h"

void Player::Init()
{
	//プレイヤーのライフ初期設定
	m_Life = 5;

	//プレイヤースタート位置
	float playerFirstPosX = MAPCHIP_WIDTH * 5.0f + m_Scale.x * 0.5f;
	float playerFirstPosY = Manager::GetScene()->GetGameObject<Map>()->GetBlockHeight() * MAPCHIP_HEIGHT
		- (MAPCHIP_HEIGHT + m_Scale.y * 0.5f);

	SetPosition(Vector2(playerFirstPosX, playerFirstPosY));

	//スケール設定
	m_Scale = { 100.0f,100.0f };

	InitSprite();

	//通常状態
	m_TextureList.push_back(Texture::Load("asset\\texture\\player\\player_NormalAnimation.png"));
	m_TextureList.push_back(Texture::Load("asset\\texture\\player\\player_Normal_MoveAnimation.png"));
	m_TextureList.push_back(Texture::Load("asset\\texture\\player\\player_Normal_FlyingAnimation.png"));
	m_TextureList.push_back(Texture::Load("asset\\texture\\player\\player_Normal_Flying2.png"));

	//再生取得状態
	m_TextureList.push_back(Texture::Load("asset\\texture\\player\\player_PlaybackAnimation.png"));
	m_TextureList.push_back(Texture::Load("asset\\texture\\player\\player_Playback_MoveAnimation.png"));
	m_TextureList.push_back(Texture::Load("asset\\texture\\player\\player_Playback_FlyingAnimation.png"));
	m_TextureList.push_back(Texture::Load("asset\\texture\\player\\player_Playback_Flying2.png"));

	//逆再生取得状態
	m_TextureList.push_back(Texture::Load("asset\\texture\\player\\player_ReverseAnimation.png"));
	m_TextureList.push_back(Texture::Load("asset\\texture\\player\\player_Reverse_MoveAnimation.png"));
	m_TextureList.push_back(Texture::Load("asset\\texture\\player\\player_Reverse_FlyingAnimation.png"));
	m_TextureList.push_back(Texture::Load("asset\\texture\\player\\player_Reverse_Flying2.png"));

	//再生状態
	m_TextureList.push_back(Texture::Load("asset\\texture\\player\\player_TrailMoveAnimation.png"));
}

void Player::Uninit()
{
	UnInitSprite();
}

void Player::Update()
{
	//体力0以下でタイトルへ
	if (m_Life <= 0)
	{
		Manager::SetScene<GameOver>();
	}

	//デバッグ用体力減少
	if (Input::GetKeyPress('l') || Input::GetKeyPress('L'))
	{
		m_Life = 0;
	}

	//デバッグ用ステージクリア
	if (Input::GetKeyPress('k') || Input::GetKeyPress('K'))
	{
		Manager::SetScene<StageClear>();
	}

	NoDamage(); //無敵時間処理

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
				m_PlayerState = PlayerState::Player_Normal;
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
		if (m_TrailDiffList.size() == 1)
		{
			m_TrailDiffList.clear();

			m_OnGround = false;
			m_HaveTrail = false;
			m_MoveTrail = false;
			m_PlayerState = PlayerState::Player_Normal;
		}
		break;
	}
	}


	//ワールド座標更新
	m_Position += m_Vector;

}

void Player::Draw()
{
	//テクスチャセット
	if (m_MoveTrail)
	{
		m_TextureType = 12;
	}
	else if ((m_GettingTrail || m_HaveTrail) && m_TrailType)
	{
		if (!m_OnGround)
		{
			//ジャンプ、落下中のアニメーション
			if (!m_JumpAnimationFirst)
			{
				m_TextureType = 6;
				m_JumpAnimationFirst = true;
			}
			else if (AnimFinish())
			{
				m_TextureType = 7;
			}
			else
			{
				m_TextureType = 7;
			}
		}
		else if (m_Vector.x != 0.0f)
		{
			//移動中のアニメーション
			m_TextureType = 5;
		}
		else
		{
			//待機中のアニメーション
			m_TextureType = 4;
		}

		if (m_OnGround)
		{
			m_JumpAnimationFirst = false;
		}

	}
	else if ((m_GettingTrail || m_HaveTrail) && !m_TrailType)
	{
		if (!m_OnGround)
		{
			//ジャンプ、落下中のアニメーション
			if (!m_JumpAnimationFirst)
			{
				m_TextureType = 10;
				m_JumpAnimationFirst = true;
			}
			else if (AnimFinish())
			{
				m_TextureType = 11;
			}
			else
			{
				m_TextureType = 11;
			}
		}
		else if (m_Vector.x != 0.0f)
		{
			//移動中のアニメーション
			m_TextureType = 9;
		}
		else
		{
			//待機中のアニメーション
			m_TextureType = 8;
		}

		if (m_OnGround)
		{
			m_JumpAnimationFirst = false;
		}
	}
	else
	{
		if (!m_OnGround)
		{
			//ジャンプ、落下中のアニメーション
			if (!m_JumpAnimationFirst)
			{
				m_TextureType = 2;
				m_JumpAnimationFirst = true;
			}
			else if (AnimFinish())
			{
				m_TextureType = 3;
			}
			else
			{
				m_TextureType = 3;
			}
		}
		else if (m_Vector.x != 0.0f)
		{
			//移動中のアニメーション
			m_TextureType = 1;
		}
		else
		{
			//待機中のアニメーション
			m_TextureType = 0;
		}

		if (m_OnGround)
		{
			m_JumpAnimationFirst = false;
		}
	}

	//描画位置更新
	m_DrawPosition =
		m_Position - Manager::GetScene()->GetGameObject<Camera>()->GetCameraTopLeftPosition();

	switch (m_TextureType)
	{
	case 0:
		m_AnimationPattern = 8;
		m_Animationcols = 8;
		m_Animationrows = 1;
		m_DrawDirection = m_Direction;
		break;
	case 1:
		m_AnimationPattern = 8;
		m_Animationcols = 8;
		m_Animationrows = 1;
		m_DrawDirection = m_Direction;
		break;
	case 2:
		m_AnimationPattern = 8;
		m_Animationcols = 8;
		m_Animationrows = 1;
		m_DrawDirection = m_Direction;
		break;
	case 3:
		m_AnimationPattern = 1;
		m_Animationcols = 1;
		m_Animationrows = 1;
		m_DrawDirection = m_Direction;
		break;
	case 4:
		m_AnimationPattern = 8;
		m_Animationcols = 8;
		m_Animationrows = 1;
		m_DrawDirection = m_Direction;
		break;
	case 5:
		m_AnimationPattern = 8;
		m_Animationcols = 8;
		m_Animationrows = 1;
		m_DrawDirection = m_Direction;
		break;
	case 6:
		m_AnimationPattern = 8;
		m_Animationcols = 8;
		m_Animationrows = 1;
		m_DrawDirection = m_Direction;
		break;
	case 7:
		m_AnimationPattern = 1;
		m_Animationcols = 1;
		m_Animationrows = 1;
		m_DrawDirection = m_Direction;
		break;
	case 8:
		m_AnimationPattern = 8;
		m_Animationcols = 8;
		m_Animationrows = 1;
		m_DrawDirection = m_Direction;
		break;
	case 9:
		m_AnimationPattern = 8;
		m_Animationcols = 8;
		m_Animationrows = 1;
		m_DrawDirection = m_Direction;
		break;
	case 10:
		m_AnimationPattern = 8;
		m_Animationcols = 8;
		m_Animationrows = 1;
		m_DrawDirection = m_Direction;
		break;
	case 11:
		m_AnimationPattern = 1;
		m_Animationcols = 1;
		m_Animationrows = 1;
		m_DrawDirection = m_Direction;
		break;
	case 12:
		m_AnimationPattern = 8;
		m_Animationcols = 8;
		m_Animationrows = 1;
		m_DrawDirection = m_Direction;
		break;
	default:
		break;
	}

	DrawSpriteAnim(XMFLOAT2(m_DrawPosition.x, m_DrawPosition.y),
		m_Rotate, XMFLOAT2(m_Scale.x, m_Scale.y),
		m_AnimationPattern, m_Animationcols, m_Animationrows,
		m_TextureType, alpha, m_DrawDirection);

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
	else if (Input::GetKeyPress('D') || Input::GetKeyPress('d'))
	{
		m_Vector.x = m_Speed; //右に移動
		m_OnGround = false;
	}
	else
	{
		m_Vector.x = 0.0f; //移動しない
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

void Player::NoDamage()
{
	if (m_NoDamage)
	{
		m_DamageFrame++;
		if (m_DamageFrame % 10 == 0)
		{
			alpha = (alpha == 1.0f) ? 0.5f : 1.0f; //点滅効果
		}

		if (m_DamageFrame >= m_NoDamageFrame)
		{
			m_NoDamage = false;
			m_DamageFrame = 0;
		}
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

