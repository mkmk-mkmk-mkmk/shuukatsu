
#include "framework.h"
#include "main.h"
#include "renderer.h"
#include "texture.h"
#include "input.h"

#include "manager.h"
#include "scene.h"
#include "game.h"
#include "cursor.h"
#include "collision.h"
#include "UI_StartButton.h"

void UI_StartButton::Init()
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

	m_Texture[0] = Texture::Load("asset\\texture\\UI\\startButton.png");
	m_Texture[1] = Texture::Load("asset\\texture\\UI\\startButton_2.png");

	Renderer::CreateVertexShader(&m_VertexShader, &m_VertexLayout, "shader\\unlitTextureVS.cso");

	Renderer::CreatePixelShader(&m_PixelShader, "shader\\unlitTexturePS.cso");

	m_Position = Vector2((float)screenWidth * 0.5f, (float)screenHeight * 0.8);
	m_Scale = Vector2((float)screenWidth * 0.3, (float)screenHeight * 0.2f);

}

void UI_StartButton::Uninit()
{

}

void UI_StartButton::Update()
{
	m_ButtonPos = Manager::GetScene()->GetUIObject<Cursor>()->GetPosition();
	m_ButtonScale = Manager::GetScene()->GetUIObject<Cursor>()->GetScale();

	//ボタンとの当たり判定
	m_CursorOnButton = Collision::BoxCollisionCommon(m_ButtonPos, m_ButtonScale, m_Position, m_Scale);

	if (!m_CursorOnButton)
	{
		m_DrawPosition = m_Position;
	}
	else
	{
		m_DrawPosition = m_Position + Vector2(0.0f, -10.0f);
	}

	if (m_CursorOnButton && GetAsyncKeyState(VK_LBUTTON) & 0x8000)
	{
		m_ClickButton = true;
	}
	if (m_ClickButton && !m_CursorOnButton)
	{
		m_ClickButton = false;
	}

	if (m_ClickButton && !(GetAsyncKeyState(VK_LBUTTON) & 0x8000))
	{
		m_ClickButton = false;
		Manager::SetScene<Game>();
	}
}

void UI_StartButton::Draw()
{

	//テクスチャセット
	if (!m_CursorOnButton)
	{
		Renderer::GetDeviceContext()->PSSetShaderResources(0, 1, &m_Texture[0]);
	}
	else
	{
		Renderer::GetDeviceContext()->PSSetShaderResources(0, 1, &m_Texture[1]);
	}

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