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
}

void Camera::Uninit()
{

}

void Camera::Update()
{
	//カメラの中心座標はプレイヤーの更新で取得している
	m_CameraTopLeftPosition = m_CameraCenterPosition - Vector2(screenWidth * 0.5f, screenHeight * 0.25f);
}
