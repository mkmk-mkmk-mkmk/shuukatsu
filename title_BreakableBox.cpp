
#include "main.h"
#include "renderer.h"
#include "Vector2.h"
#include "texture.h"
#include "cursor.h"
#include "input.h"
#include "scene.h"
#include "manager.h"

#include "map.h"
#include "title_BreakableBox.h"
#include "boxBreakEffect.h"
#include "spring.h"
#include "springChain.h"
#include "springBoard.h"
#include "collision.h"


void Title_BreakableBox::Init()
{
	VERTEX_3D vertex[4];

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

	//初期位置設定
	m_Position = Vector2(float(random.RandomInt(screenWidth, screenWidth * 3)) * 0.1f,
		float(random.RandomInt(screenHeight, screenHeight * 3) * -0.1f));
	m_LeftorRight = random.RandomInt(0, 1);

	switch (m_LeftorRight)
	{
		case 0: //左側
			m_Position.x = float(random.RandomInt(screenWidth, screenWidth * 3)) * 0.1f;
			break;
		case 1: //右側
			m_Position.x = float(random.RandomInt(screenWidth * 7, screenWidth * 9)) * 0.1f;
			break;
	}

	//移動ベクトル設定
	m_Vector = Vector2(float(random.RandomInt(-5, 5)), 0.0f);

	//大きさ設定
	m_Scale = Vector2(MAPCHIP_WIDTH, MAPCHIP_HEIGHT);

	D3D11_BUFFER_DESC bd{};
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(VERTEX_3D) * 4;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0;

	D3D11_SUBRESOURCE_DATA sd{};
	sd.pSysMem = vertex;

	Renderer::GetDevice()->CreateBuffer(&bd, &sd, &m_VertexBuffer);

	m_Texture = Texture::Load("asset\\texture\\tile_Breakable.png");

	Renderer::CreateVertexShader(&m_VertexShader, &m_VertexLayout, "shader\\unlitTextureVS.cso");

	Renderer::CreatePixelShader(&m_PixelShader, "shader\\unlitTexturePS.cso");

}

void Title_BreakableBox::Uninit()
{
	m_Texture->Release();

	m_VertexBuffer->Release();
	m_VertexLayout->Release();
	m_VertexShader->Release();
	m_PixelShader->Release();
}

void Title_BreakableBox::Update()
{
	if (!m_Breaking)
	{
		m_Vector.y += m_Gravity;

		m_Position += m_Vector;

		//if (m_Position.y > m_BreakHeight)
		//{
		//	m_Breaking = true;
		//	//エフェクト生成
		//	auto effect = std::make_unique<BoxBreakEffect>();
		//	effect->Init(m_Position);
		//	m_BoxBreakEffectList.push_back(std::move(effect));
		//}

		m_CursorPos = Manager::GetScene()->GetGameObject<Cursor>()->GetPosition();
		m_CursorScale = Manager::GetScene()->GetGameObject<Cursor>()->GetScale();

		if (BoxCollisionCommon(m_Position, m_Scale, m_CursorPos, m_CursorScale))
		{
			m_Breaking = true;
			//エフェクト生成
			auto effect = std::make_unique<BoxBreakEffect>();
			effect->Init(m_Position);
			m_BoxBreakEffectList.push_back(std::move(effect));
		}

	}
	else
	{
		for (auto it = m_BoxBreakEffectList.begin(); it != m_BoxBreakEffectList.end(); )
		{
			(*it)->Update();

			if ((*it)->GetDestroy())
			{
				it = m_BoxBreakEffectList.erase(it);
			}
			else
			{
				++it;
			}

			if (m_BoxBreakEffectList.empty())
			{
				this->SetDestroy();
			}
		}
	}
}

void Title_BreakableBox::Draw()
{

	if (!m_Breaking)
	{
		Renderer::GetDeviceContext()->IASetInputLayout(m_VertexLayout);

		Renderer::GetDeviceContext()->VSSetShader(m_VertexShader, NULL, 0);
		Renderer::GetDeviceContext()->PSSetShader(m_PixelShader, NULL, 0);

		//マトリクス設定
		Renderer::SetWorldViewProjection2D();

		XMMATRIX world, scale, rot, trans;
		scale = XMMatrixScaling(m_Scale.x, m_Scale.y, 1.0f);
		rot = XMMatrixRotationZ(m_Rotate);
		trans = XMMatrixTranslation(m_Position.x, m_Position.y, 0.0f);
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

		Renderer::GetDeviceContext()->PSSetShaderResources(0, 1, &m_Texture);

		Renderer::GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

		Renderer::GetDeviceContext()->Draw(4, 0);

	}
	else
	{
		for (auto& p : m_BoxBreakEffectList)
		{
			p->Draw(m_DrawDiff);
		}
	}
}
