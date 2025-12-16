#include "main.h"
#include "renderer.h"
#include "Vector2.h"
#include "texture.h"
#include "cursor.h"
#include "input.h"
#include "scene.h"
#include "manager.h"

#include "springChain.h"
#include "map.h"
#include "player.h"
#include "camera.h"
#include "enemy.h"

void SpringChain::Init(Vector2 topPos, Vector2 bottomPos, float chainSplit, float chainWidth)
{
	//分割数+1が点の数
	m_ChainPointCount = (int)chainSplit + 1;

	//一つ一つのチェーンの大きさ取得（絶対値で）
	Vector2 chainPieceScale = (bottomPos - topPos) / chainSplit;
	m_ChainLength = length(chainPieceScale);

	//リストをクリアしておく
	m_ChainPointList.clear();

	//点のリスト初期化
	for (int i = 0; i < m_ChainPointCount; i++)
	{
		ChainPoint pointList;
		pointList.pos = topPos + (chainPieceScale * (float)i);
		pointList.oldPos = pointList.pos;	//初期速度は0
		pointList.acceleration = Vector2(0.0f, 0.0f);
		pointList.lock = false;			//後に一番上だけ固定する

		m_ChainPointList.push_back(pointList);
	}

	m_Scale = Vector2(chainWidth, chainPieceScale.y);


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

	m_Texture = Texture::Load("asset\\texture\\springChain.png");

	Renderer::CreateVertexShader(&m_VertexShader, &m_VertexLayout, "shader\\unlitTextureVS.cso");

	Renderer::CreatePixelShader(&m_PixelShader, "shader\\unlitTexturePS.cso");

}

void SpringChain::Uninit()
{

}

void SpringChain::Update()
{
	//重力適用
	for (auto &point : m_ChainPointList)
	{
		if (!point.lock)
		{
			point.acceleration.y += m_Gravity;
		}
	}

	//位置更新
	for (auto &point : m_ChainPointList)
	{
		if (point.lock)
		{
			continue;
		}

		Vector2 oldPointPos = point.pos;

		// 位置更新
		point.pos += (point.pos - point.oldPos) + point.acceleration;
		point.oldPos = oldPointPos;
		point.acceleration = Vector2(0, 0);
	}

	//距離制約
	const int constraintLoop = 5;	//距離制約をフレームごとに何回解くか
	//なんかブランコとかだとこのくらいの数値がいいらしい

	for (int n = 0; n < constraintLoop; n++)
	{
		for (int i = 0; i < m_ChainPointCount - 1; i++)
		{
			ChainPoint& point1 = m_ChainPointList[i];
			ChainPoint& point2 = m_ChainPointList[i + 1];

			Vector2 delta = point2.pos - point1.pos;
			float pointDistance = length(delta);
			if (pointDistance == 0.0f)
			{
				continue;
			}

			float stiffness = 0.5f;
			float diff = (pointDistance - m_ChainLength) / pointDistance;

			if (!point1.lock)
			{
				point1.pos += delta * 0.5f * diff * stiffness;
			}

			if (!point2.lock)
			{
				point2.pos -= delta * 0.5f * diff * stiffness;
			}
		}
	}

	if (m_ChainPointList.front().pos.y > -screenHeight / 4)
	{
		//一番上の点を固定
		m_ChainPointList.front().lock = true;
	}
}

void SpringChain::Draw()
{
	for (int i = 0; i < m_ChainPointCount - 1; i++)
	{
		DrawPiece(i);
	}
}

void SpringChain::DrawPiece(int count)
{
	ChainPoint point1 = m_ChainPointList[count];
	ChainPoint point2 = m_ChainPointList[count + 1];

	m_Position = point1.pos + ((point2.pos - point1.pos) * 0.5f);
	m_Rotate = PI * 0.5f - atan2(point2.pos.y - point1.pos.y,
									point2.pos.x - point1.pos.x);

	Renderer::GetDeviceContext()->IASetInputLayout(m_VertexLayout);

	Renderer::GetDeviceContext()->VSSetShader(m_VertexShader, NULL, 0);
	Renderer::GetDeviceContext()->PSSetShader(m_PixelShader, NULL, 0);

	//マトリクス設定
	Renderer::SetWorldViewProjection2D();

	XMMATRIX world, scale, rot, trans;
	scale = XMMatrixScaling(m_Scale.x, m_Scale.y + 50.0f, 1.0f);	//少し長めに描画
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