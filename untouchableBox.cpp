#include "main.h"
#include "texture.h"
#include "input.h"
#include "scene.h"
#include "manager.h"

#include "untouchableBox.h"
#include "map.h"
#include "camera.h"

void UntouchableBox::Init()
{
	//初期位置設定
	m_Position = Manager::GetScene()->GetGameObject<Map>()->m_UntouchableBoxPosList.front();

	//大きさ設定
	m_Scale = Vector2(MAPCHIP_WIDTH, MAPCHIP_HEIGHT);

	InitSprite();
	m_TextureList.push_back(Texture::Load("asset\\texture\\tile.png"));
}

void UntouchableBox::Uninit()
{
	UnInitSprite();
}

void UntouchableBox::Update()
{

}

void UntouchableBox::Draw()
{
	//描画位置更新
	m_DrawPosition =
		m_Position - Manager::GetScene()->GetGameObject<Camera>()->GetCameraTopLeftPosition();

	DrawSprite(XMFLOAT2(m_DrawPosition.x, m_DrawPosition.y), m_Rotate, XMFLOAT2(m_Scale.x, m_Scale.y), 0, 1.0f);
}
