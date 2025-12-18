
#include "main.h"
#include "renderer.h"
#include "texture.h"
#include "manager.h"

#include "random.h"
#include "boxBreakEffect.h"

void BoxBreakEffect::Init(Vector2 position)
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


	D3D11_BUFFER_DESC bd{};
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(VERTEX_3D) * 4;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0;

	D3D11_SUBRESOURCE_DATA sd{};
	sd.pSysMem = vertex;

	Renderer::GetDevice()->CreateBuffer(&bd, &sd, &m_VertexBuffer);

	m_Texture = Texture::Load("asset\\texture\\breakableBox_piece.png");

	Renderer::CreateVertexShader(&m_VertexShader, &m_VertexLayout, "shader\\unlitTextureVS.cso");

	Renderer::CreatePixelShader(&m_PixelShader, "shader\\unlitTexturePS.cso");

	for (int i = 0; i < m_PieceCount; i++)
	{
		m_PieceData.pos = position;
		m_PieceData.drawPos = position;
		m_PieceData.scale = float(random.RandomInt(15, 25));
		m_PieceData.rotate = float(random.RandomInt(0, 6));
		m_PieceData.vec = Vector2(float(random.RandomInt(-2, 2)), float(random.RandomInt(-2, 2)));

		m_EffectDataList.push_back(m_PieceData);
	}

}

void BoxBreakEffect::Update()
{
	for (int i = 0; i < m_EffectDataList.size(); i++)
	{
		//重力も適用する
		m_EffectDataList[i].vec.y += m_Gravity;

		m_EffectDataList[i].pos += m_EffectDataList[i].vec;
		m_EffectDataList[i].scale -= 0.2f;
		if (m_EffectDataList[i].scale < 0.0f)
		{
			m_EffectDataList.erase(m_EffectDataList.begin() + i);
			i--;
		}
	}

	if (m_EffectDataList.empty())
	{
		m_Destroy = true;
	}
}

void BoxBreakEffect::Draw(Vector2 drawDiff)
{
	for (int i = 0; i < m_EffectDataList.size(); i++)
	{
		DrawPiece(i, drawDiff);
	}
}

void BoxBreakEffect::DrawPiece(int count, Vector2 drawDiff)
{
	//ワールド座標と描画座標がずれることがあるので補正
	m_EffectDataList[count].drawPos = m_EffectDataList[count].pos - drawDiff;

	Renderer::GetDeviceContext()->IASetInputLayout(m_VertexLayout);

	Renderer::GetDeviceContext()->VSSetShader(m_VertexShader, NULL, 0);
	Renderer::GetDeviceContext()->PSSetShader(m_PixelShader, NULL, 0);

	//マトリクス設定
	Renderer::SetWorldViewProjection2D();

	XMMATRIX world, scale, rot, trans;
	scale = XMMatrixScaling(m_EffectDataList[count].scale, m_EffectDataList[count].scale, 1.0f);
	rot = XMMatrixRotationZ(m_EffectDataList[count].rotate);
	trans = XMMatrixTranslation(m_EffectDataList[count].drawPos.x, m_EffectDataList[count].drawPos.y, 0.0f);
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