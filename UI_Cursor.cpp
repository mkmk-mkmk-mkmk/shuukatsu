#include "UI_Cursor.h"  
#include "texture.h"
#include "renderer.h"
#include "input.h"
#include "manager.h"
#include "scene.h"
#include "fade.h"
#include "title_BreakableBox.h"


void UI_Cursor::Init()
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

	InitSprite();

	m_TextureList.push_back(Texture::Load("asset\\texture\\UI\\cursor.png"));
	m_TextureList.push_back(Texture::Load("asset\\texture\\UI\\cursor_2.png"));
	m_TextureList.push_back(Texture::Load("asset\\texture\\UI\\cursor_GettingTrail.png"));
	m_TextureList.push_back(Texture::Load("asset\\texture\\UI\\cursor_GettingReverse.png"));

	//マウス初期位置変更後再移動
	m_CursorLockPos = { screenWidth * 0.5f, screenHeight * 0.5f };
	SetCursorPos(m_CursorLockPos.x, m_CursorLockPos.y);

	//マウスカーソル非表示
	ShowCursor(FALSE);


	////カーソル用テクスチャの分割数を設定
	//cursorInstance.m_TextureSplit = { 1, 1 };
}

void UI_Cursor::Uninit()
{
	////カーソル情報解放
	//if (cursorInstance.m_VertexBuffer) 
	//{
	//	cursorInstance.m_VertexBuffer->Release();
	//	cursorInstance.m_VertexBuffer = nullptr;
	//}

	m_VertexBuffer->Release();
	m_VertexLayout->Release();
	m_VertexShader->Release();
	m_PixelShader->Release();

}

void UI_Cursor::Update()
{
	if (GetCursorPos(&pt))
	{
		SetPosition(Vector2{ static_cast<float>(pt.x), static_cast<float>(pt.y) });
	}

	//クリック検出
	m_LeftClick = (GetAsyncKeyState(VK_LBUTTON) & 0x8000);
	m_RightClick = (GetAsyncKeyState(VK_RBUTTON) & 0x8000);

	//パーティクル生成
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

void UI_Cursor::Draw()
{
	//m_HitBox = Manager::GetScene()->GetGameObject<Title_BreakableBox>()->GetHitCursor();

	if (m_ButtonUse || m_HitBox)
	{
		m_TextureType = 1;
	}
	else
	{
		m_TextureType = 0;
	}

	DrawSprite(XMFLOAT2(m_Position.x, m_Position.y), m_Rotate, XMFLOAT2(m_Scale.x, m_Scale.y), m_TextureType, 1.0f);

	for (auto& p : m_ParticleTouchList)
	{
		p->Draw();
	}
}
