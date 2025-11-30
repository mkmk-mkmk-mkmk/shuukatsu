#include "main.h"
#include "renderer.h"
#include "Vector2.h"
#include "texture.h"
#include "cursor.h"
#include "input.h"
#include "scene.h"
#include "manager.h"

#include "transparentBox.h"
#include "map.h"
#include "player.h"
#include "camera.h"

void TransparentBox::Init()
{
	VERTEX_3D vertex[4];

	vertex[0].Position = XMFLOAT3(-MAPCHIP_WIDTH * 0.5f, -MAPCHIP_HEIGHT * 0.5f, 0.0f);
	vertex[0].Normal = XMFLOAT3(0.0f, 0.0f, 0.0f);
	vertex[0].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[0].TexCoord = XMFLOAT2(0.0f, 0.0f);

	vertex[1].Position = XMFLOAT3(MAPCHIP_WIDTH * 0.5f, -MAPCHIP_HEIGHT * 0.5f, 0.0f);
	vertex[1].Normal = XMFLOAT3(0.0f, 0.0f, 0.0f);
	vertex[1].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[1].TexCoord = XMFLOAT2(1.0f, 0.0f);

	vertex[2].Position = XMFLOAT3(-MAPCHIP_WIDTH * 0.5f, MAPCHIP_HEIGHT * 0.5f, 0.0f);
	vertex[2].Normal = XMFLOAT3(0.0f, 0.0f, 0.0f);
	vertex[2].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[2].TexCoord = XMFLOAT2(0.0f, 1.0f);

	vertex[3].Position = XMFLOAT3(MAPCHIP_WIDTH * 0.5f, MAPCHIP_HEIGHT * 0.5f, 0.0f);
	vertex[3].Normal = XMFLOAT3(0.0f, 0.0f, 0.0f);
	vertex[3].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	vertex[3].TexCoord = XMFLOAT2(1.0f, 1.0f);

	//初期位置設定
	m_FirstPos = Manager::GetScene()->GetGameObject<Map>()->m_TransparentBoxPosList.front();

	//大きさ設定
	m_Scale = Vector2(MAPCHIP_WIDTH, MAPCHIP_HEIGHT);

	//最初にも一度座標更新
	m_Position = m_FirstPos + Manager::GetScene()->GetGameObject<Camera>()->GetPosition();

	D3D11_BUFFER_DESC bd{};
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(VERTEX_3D) * 4;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0;

	D3D11_SUBRESOURCE_DATA sd{};
	sd.pSysMem = vertex;

	Renderer::GetDevice()->CreateBuffer(&bd, &sd, &m_VertexBuffer);

}

void TransparentBox::Uninit()
{

}

void TransparentBox::Update()
{
	//座標更新
	m_Position = m_FirstPos - Manager::GetScene()->GetGameObject<Camera>()->GetPosition();

	Manager::GetScene()->GetGameObject<Player>()->BoxCollision(m_Position, m_Scale);
}

void TransparentBox::Draw()
{
	//透明箱のため描画無し
}
