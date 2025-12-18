#include "cursor.h"  
#include "texture.h"
#include "renderer.h"
#include "input.h"
#include "manager.h"
#include "scene.h"
//#include "player.h"
#include "title_BreakableBox.h"


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
	m_Texture[1] = Texture::Load("asset\\texture\\UI\\cursor_2.png");

	m_Texture[2] = Texture::Load("asset\\texture\\UI\\cursor_GettingTrail.png");
	m_Texture[3] = Texture::Load("asset\\texture\\UI\\cursor_GettingReverse.png");

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

	if (m_MakeParticle)
	{
		bool left = (GetAsyncKeyState(VK_LBUTTON) & 0x8000);

		if (left && !m_ButtonUse)
		{
			auto particle = std::make_unique<ParticleTouch>();
			particle->Init(m_Position);
			m_ParticleTouchList.push_back(std::move(particle));
		}

		m_ButtonUse = left;
	}

	//カーソル情報更新
	if (GetCursorPos(&pt))
	{
		SetPosition(Vector2{ static_cast<float>(pt.x), static_cast<float>(pt.y) });
	}

	for (auto it = m_ParticleTouchList.begin(); it != m_ParticleTouchList.end(); )
	{
		(*it)->Update();

		if ((*it)->GetDestroy())
		{
			it = m_ParticleTouchList.erase(it);
		}
		else
		{
			++it;
		}
	}

}

void Cursor::Draw()
{  
	//m_HitBox = Manager::GetScene()->GetGameObject<Title_BreakableBox>()->GetHitCursor();

	if (m_ButtonUse || m_HitBox)
	{
		Renderer::GetDeviceContext()->PSSetShaderResources(0, 1, &m_Texture[1]);
	}
	else
	{
		Renderer::GetDeviceContext()->PSSetShaderResources(0, 1, &m_Texture[0]);
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


	for (auto& p : m_ParticleTouchList)
	{
		p->Draw();
	}
}
