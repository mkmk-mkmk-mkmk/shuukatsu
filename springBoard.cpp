#include "main.h"
#include "renderer.h"
#include "Vector2.h"
#include "texture.h"
#include "cursor.h"
#include "input.h"
#include "scene.h"
#include "manager.h"

#include "springBoard.h"
#include "map.h"
#include "player.h"
#include "camera.h"
#include "enemy.h"

void SpringBoard::Init(Vector2 boardLeftPos, Vector2 boardRightPos, float height)
{
	// 左端
	leftPoint.pos = boardLeftPos;
	leftPoint.oldPos = boardLeftPos;
	leftPoint.acceleration = Vector2(0, 0);
	leftPoint.lock = false;

	// 右端
	rightPoint.pos = boardRightPos;
	rightPoint.oldPos = boardRightPos;
	rightPoint.acceleration = Vector2(0, 0);
	rightPoint.lock = false;

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

	m_Texture = Texture::Load("asset\\texture\\tile.png");

	Renderer::CreateVertexShader(&m_VertexShader, &m_VertexLayout, "shader\\unlitTextureVS.cso");

	Renderer::CreatePixelShader(&m_PixelShader, "shader\\unlitTexturePS.cso");

}

void SpringBoard::Uninit()
{

}

void SpringBoard::Update()
{
	//重力適用
	if (!leftPoint.lock)
	{
		leftPoint.acceleration.y += m_Gravity;
	}

	if (!rightPoint.lock)
	{
		rightPoint.acceleration.y += m_Gravity;
	}

	// 左端
	if (!leftPoint.lock)
	{
		Vector2 temp = leftPoint.pos;
		leftPoint.pos += (leftPoint.pos - leftPoint.oldPos) + leftPoint.acceleration;
		leftPoint.oldPos = temp;
		leftPoint.acceleration = Vector2(0, 0);
	}

	// 右端
	if (!rightPoint.lock)
	{
		Vector2 temp = rightPoint.pos;
		rightPoint.pos += (rightPoint.pos - rightPoint.oldPos) + rightPoint.acceleration;
		rightPoint.oldPos = temp;
		rightPoint.acceleration = Vector2(0, 0);
	}

	//距離制約
	Vector2 delta = rightPoint.pos - leftPoint.pos;
	float pointDistance = length(delta);
	if (pointDistance == 0.0f)
	{
		return;
	}

	// 板の元の長さを保ちたい
	static float boardLength = pointDistance; // 初回のみ記録

	float diff = (pointDistance - boardLength) / pointDistance;

	if (!leftPoint.lock)
	{
		leftPoint.pos += delta * 0.5f * diff;
	}

	if (!rightPoint.lock)
	{
		rightPoint.pos -= delta * 0.5f * diff;
	}
}

void SpringBoard::Draw()
{

}