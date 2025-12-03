#include "main.h"
#include "manager.h"
#include "scene.h"
#include "input.h"

#include "camera.h"
#include "player.h"
#include "map.h"

void Camera::Init()
{
	//初期化の時点でも一度カメラ位置を設定しておく
	m_CameraCenterPosition = Manager::GetScene()->GetGameObject<Player>()->GetPlayerPosition();
	m_CameraTopLeftPosition = m_CameraCenterPosition - Vector2(screenWidth * 0.5f, screenHeight * 0.5f);

	m_StageBlockWidth = Manager::GetScene()->GetGameObject<Map>()->GetBlockWidth();
	m_StageBlockHeight = Manager::GetScene()->GetGameObject<Map>()->GetBlockHeight();
}

void Camera::Uninit()
{

}

void Camera::Update()
{
	//カメラの中心座標をプレイヤーの位置から取得
	m_CameraCenterPosition = Manager::GetScene()->GetGameObject<Player>()->GetPlayerPosition();

	//画面端だった場合の補正
	//x座標
	if (m_CameraCenterPosition.x < screenWidth * 0.5f + MAPCHIP_WIDTH * 1.5f)	//左端
	{
		m_CameraCenterPosition.x = screenWidth * 0.5f + MAPCHIP_WIDTH * 1.5f;
	}
	else if (m_CameraCenterPosition.x >
		float(MAPCHIP_WIDTH - 1.5f) * m_StageBlockWidth - screenWidth * 0.5f)	//右端
	{
		m_CameraCenterPosition.x =
			float(MAPCHIP_WIDTH - 1.5f) * m_StageBlockWidth - screenWidth * 0.5f;
	}

	//y座標
	if (m_CameraCenterPosition.y < screenHeight * 0.5f + MAPCHIP_HEIGHT * 1.5f)	//上端
	{
		m_CameraCenterPosition.y = screenHeight * 0.5f + MAPCHIP_HEIGHT * 1.5f;
	}
	else if (m_CameraCenterPosition.y >
		float(MAPCHIP_HEIGHT - 1.5f) * m_StageBlockHeight - screenHeight * 0.45f)//下端
	{
		m_CameraCenterPosition.y =
			float(MAPCHIP_HEIGHT - 1.5f)* m_StageBlockHeight - screenHeight * 0.45f;
	}

	//カメラ左上座標更新
	m_CameraTopLeftPosition = m_CameraCenterPosition - Vector2(screenWidth * 0.5f, screenHeight * 0.5f);
}
