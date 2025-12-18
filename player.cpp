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

	vertex[0].Position = XMFLOAT3(-0.5f, -0.5f, 0.0f);
	vertex[0].Normal = XMFLOAT3(0.0f, 0.0f, 0.0f);
	vertex[0].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[0].TexCoord = XMFLOAT2(0.0f, 0.0f);

	vertex[1].Position = XMFLOAT3(0.5f, -0.5f, 0.0f);
	vertex[1].Normal = XMFLOAT3(0.0f, 0.0f, 0.0f);
	vertex[1].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[1].TexCoord = XMFLOAT2(1.0f, 0.0f);

	vertex[2].Position = XMFLOAT3(-0.5f, 0.5f, 0.0f);
	vertex[2].Normal = XMFLOAT3(0.0f, 0.0f, 0.0f);
	vertex[2].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[2].TexCoord = XMFLOAT2(0.0f, 1.0f);

	vertex[3].Position = XMFLOAT3(0.5f, 0.5f, 0.0f);
	vertex[3].Normal = XMFLOAT3(0.0f, 0.0f, 0.0f);
	vertex[3].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[3].TexCoord = XMFLOAT2(1.0f, 1.0f);


	D3D11_BUFFER_DESC bd{};
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(VERTEX_3D) * 4;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0;

	D3D11_SUBRESOURCE_DATA sd{};
	sd.pSysMem = vertex;

	Renderer::GetDevice()->CreateBuffer(&bd, &sd, &m_VertexBuffer);

	m_Texture[0] = Texture::Load("asset\\texture\\roboR1_green.png");
	m_Texture[1] = Texture::Load("asset\\texture\\roboR1_red.png");
	m_Texture[2] = Texture::Load("asset\\texture\\roboR1_blue.png");
	m_Texture[3] = Texture::Load("asset\\texture\\roboR1_green.png");

	Renderer::CreateVertexShader(&m_VertexShader, &m_VertexLayout, "shader\\unlitTextureVS.cso");

	Renderer::CreatePixelShader(&m_PixelShader, "shader\\unlitTexturePS.cso");

}

void Player::Uninit()
{
	m_Texture[0]->Release();
	m_Texture[1]->Release();
	m_Texture[2]->Release();
	m_Texture[3]->Release();

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
	case PlayerState::Normal:		//通常状態
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

		}
		else if (GetAsyncKeyState(VK_LBUTTON) & 0x8000 && !m_HaveTrail) //左クリックで逆走の取得
		{
			m_GettingTrail = true;

			m_TrailDiffList.push_front(-m_Vector);
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
			m_PlayerState = PlayerState::Normal;
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
		Renderer::GetDeviceContext()->PSSetShaderResources(0, 1, &m_Texture[0]);
	}
	else if (m_GettingTrail)
	{
		Renderer::GetDeviceContext()->PSSetShaderResources(0, 1, &m_Texture[1]);
	}
	else if (m_HaveTrail)
	{
		Renderer::GetDeviceContext()->PSSetShaderResources(0, 1, &m_Texture[2]);
	}
	else
	{
		Renderer::GetDeviceContext()->PSSetShaderResources(0, 1, &m_Texture[0]);
	}

	//描画位置更新
	m_DrawPosition =
		m_Position - Manager::GetScene()->GetGameObject<Camera>()->GetCameraTopLeftPosition();

	Renderer::GetDeviceContext()->IASetInputLayout(m_VertexLayout);

	Renderer::GetDeviceContext()->VSSetShader(m_VertexShader, NULL, 0);
	Renderer::GetDeviceContext()->PSSetShader(m_PixelShader, NULL, 0);

	//マトリクス設定
	Renderer::SetWorldViewProjection2D();

	XMMATRIX world, scale, rot, trans;
	scale = XMMatrixScaling(m_Scale.x, m_Scale.y, 1.0f);
	rot = XMMatrixRotationZ(m_Rotate);
	trans = XMMatrixTranslation(m_DrawPosition.x, m_DrawPosition.y, 0.0f);
	world = scale * rot * trans;

	Renderer::SetWorldMatrix(world);

	//マテリアル設定
	MATERIAL material{};
	material.Diffuse = { 1.0f, 1.0f, 1.0f, 1.0f };
	material.TextureEnable = true;
	Renderer::SetMaterial(material);

	UINT stride = sizeof(VERTEX_3D);
	UINT offset = 0;

	Renderer::GetDeviceContext()->IASetVertexBuffers(0, 1, &m_VertexBuffer, &stride, &offset);

	Renderer::GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	Renderer::GetDeviceContext()->Draw(4, 0);

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

