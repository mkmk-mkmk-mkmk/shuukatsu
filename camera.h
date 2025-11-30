#pragma once
#include "gameObject.h"
#include <d3d11.h>

class Camera : public GameObject
{
private:
	int m_StageBlockWidth;  //ステージの横幅（ブロック数）
	int m_StageBlockHeight; //ステージの縦幅（ブロック数）

	Vector2  m_CameraCenterPosition; //カメラの中心座標
	Vector2  m_CameraTopLeftPosition; //カメラの左上座標
	Vector2  m_PlayerPosition; //画面上のプレイヤー座標保存用

	Vector2  m_Scale;	 //カメラサイズ（ズームイン、アウト用に一応）
	float	 m_Rotation; //カメラ回転角

public:
	void Init();
	void Uninit();
	void Update();

	Vector2 GetCameraCenterPosition() { return m_CameraCenterPosition; }
	void SetCameraCenterPositionX(float posX) { m_CameraCenterPosition.x = posX; }
	void SetCameraCenterPositionY(float posY) { m_CameraCenterPosition.y = posY; }

	void AddCameraCenterPositionX(float posX) { m_CameraCenterPosition.x += posX; }
	void AddCameraCenterPositionY(float posY) { m_CameraCenterPosition.y += posY; }

	const Vector2 GetCameraTopLeftPosition() { return m_CameraTopLeftPosition; }

};