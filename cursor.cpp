#include "cursor.h"  
#include "texture.h"
#include "renderer.h"
#include "input.h"
#include "manager.h"
#include "scene.h"
#include "player.h"


void Cursor::Init()
{
	//マウス初期位置
	m_CursorLockPos = { 0.0f, 0.0f };
	SetCursorPos(m_CursorLockPos.x, m_CursorLockPos.y);

	if (GetCursorPos(&pt))
	{
		SetPosition(Vector2{ static_cast<float>(pt.x), static_cast<float>(pt.y) });
	}

	//マウスを取得できない場合
	if (!GetCursorPos(&pt))
	{
		//エラーメッセージ
		MessageBox(nullptr, "Cursor position could not be retrieved.", "Error", MB_OK | MB_ICONERROR);
	}

	m_Scale = { 80.0f, 80.0f }; //カーソルのスケールを初期化

	VERTEX_3D vertex[4];

	vertex[0].Position = { -0.5f, -0.5f, 0.0f };
	vertex[0].Normal = XMFLOAT3(0.0f, 0.0f, 0.0f);
	vertex[0].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[0].TexCoord = { 0.0f, 0.0f };

	vertex[1].Position = { 0.5f, -0.5f, 0.0f };
	vertex[1].Normal = XMFLOAT3(0.0f, 0.0f, 0.0f);
	vertex[1].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[1].TexCoord = { 1.0f, 0.0f };

	vertex[2].Position = { -0.5f, 0.5f, 0.0f };
	vertex[2].Normal = XMFLOAT3(0.0f, 0.0f, 0.0f);
	vertex[2].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[2].TexCoord = { 0.0f, 1.0f };

	vertex[3].Position = { 0.5f, 0.5f, 0.0f };
	vertex[3].Normal = XMFLOAT3(0.0f, 0.0f, 0.0f);
	vertex[3].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[3].TexCoord = { 1.0f, 1.0f };


	D3D11_BUFFER_DESC bd{};
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(VERTEX_3D) * 4;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0;

	D3D11_SUBRESOURCE_DATA sd{};
	sd.pSysMem = vertex;

	Renderer::GetDevice()->CreateBuffer(&bd, &sd, &m_VertexBuffer);

	m_Texture[0] = Texture::Load("asset\\texture\\UI\\cursor.png");
	m_Texture[1] = Texture::Load("asset\\texture\\UI\\cursor_GettingReverse.png");
	m_Texture[2] = Texture::Load("asset\\texture\\UI\\cursor_GettingTrail.png");

	Renderer::CreateVertexShader(&m_VertexShader, &m_VertexLayout, "shader\\unlitTextureVS.cso");

	Renderer::CreatePixelShader(&m_PixelShader, "shader\\unlitTexturePS.cso");

	//マウス初期位置変更後再移動
	m_CursorLockPos = { screenWidth * 0.5f, screenHeight * 0.5f };
	SetCursorPos(m_CursorLockPos.x, m_CursorLockPos.y);

	//マウスカーソル非表示
	ShowCursor(FALSE);


	////カーソル用テクスチャの分割数を設定
	//cursorInstance.m_TextureSplit = { 1, 1 };
}

void Cursor::Uninit()
{
	////カーソル情報解放
	//if (cursorInstance.m_VertexBuffer) 
	//{
	//	cursorInstance.m_VertexBuffer->Release();
	//	cursorInstance.m_VertexBuffer = nullptr;
	//}

	m_Texture[0]->Release();
	m_Texture[1]->Release();

	m_VertexBuffer->Release();
	m_VertexLayout->Release();
	m_VertexShader->Release();
	m_PixelShader->Release();


}

void Cursor::Update()
{
	if (GetCursorPos(&pt))
	{
		SetPosition(Vector2{ static_cast<float>(pt.x), static_cast<float>(pt.y) });
	}

	//前回の状態を保存
	m_OldCursorState = m_CursorState;

	//プレイヤーが軌跡を持っているかどうか
	if (Manager::GetScene()->GetGameObject<Player>() != nullptr)
	{
		m_PlayerHaveTrail = Manager::GetScene()->GetGameObject<Player>()->GetHaveTrail();
	}

	//クリック検出
	if (!m_PlayerHaveTrail)
	{
		if (GetAsyncKeyState(VK_LBUTTON) & 0x8000)
		{
			m_CursorState = CursorState::LeftClick;
		}
		else if (GetAsyncKeyState(VK_RBUTTON) & 0x8000)
		{
			m_CursorState = CursorState::RightClick;
		}
		else
		{
			m_CursorState = CursorState::Cursor_Normal;
		}
	}
	else
	{
		m_CursorState = CursorState::Cursor_Normal;
	}


	if (m_OldCursorState != CursorState::Cursor_Normal && m_CursorState == CursorState::Cursor_Normal)
	{
		m_ReleaseClick = true;
	}

	if (!m_DrawCurcle && m_CursorState != CursorState::Cursor_Normal)
	{
		m_ClickColorCircle.Init(GetPosition(), (m_CursorState == CursorState::LeftClick) ? 0 : 1);
		m_DrawCurcle = true;
	}

	if (m_DrawCurcle)
	{
		m_ClickColorCircle.Update(m_ReleaseClick);
		if (m_ClickColorCircle.GetEndDraw())
		{
			m_DrawCurcle = false;
			m_ReleaseClick = false;
			m_ClickColorCircle.Uninit();
		}
	}

}

void Cursor::Draw()
{
	if (m_DrawCurcle)
	{
		m_ClickColorCircle.Draw();
	}

	switch (m_CursorState)
	{
	case CursorState::Cursor_Normal:
		Renderer::GetDeviceContext()->PSSetShaderResources(0, 1, &m_Texture[0]);
		break;

	case CursorState::LeftClick:
		Renderer::GetDeviceContext()->PSSetShaderResources(0, 1, &m_Texture[1]);
		break;

	case CursorState::RightClick:
		Renderer::GetDeviceContext()->PSSetShaderResources(0, 1, &m_Texture[2]);
		break;
	}

	Renderer::GetDeviceContext()->IASetInputLayout(m_VertexLayout);

	Renderer::GetDeviceContext()->VSSetShader(m_VertexShader, NULL, 0);
	Renderer::GetDeviceContext()->PSSetShader(m_PixelShader, NULL, 0);

	//マトリクス設定
	Renderer::SetWorldViewProjection2D();

	XMMATRIX world, scale, rot, trans;
	scale = XMMatrixScaling(m_Scale.x, m_Scale.y, 1.0f);
	rot = XMMatrixRotationZ(m_Rotate);
	trans = XMMatrixTranslation(GetPosition().x, GetPosition().y, 0.0f);
	//trans = XMMatrixTranslation(0.0f, 0.0f, 0.0f);

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

	//カーソル描画
	Renderer::GetDeviceContext()->Draw(4, 0);

}
