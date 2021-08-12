#include "Camera.h"

// コンストラクタ
Camera::Camera(const PlayerActor& playerActor)
{
	// 奥行 1.0～10000 までをカメラの描画範囲とする
	SetCameraNearFar(1.0f, 10000.0f);

	//カメラのポジション設定
	mPos = VGet(50, 60, 0);
	mTempPos = mPos;
}

// デストラクタ
Camera::~Camera()
{

}

// 更新
void Camera::Update(const PlayerActor& playerActor)
{
	mPos = VAdd(mTempPos, playerActor.GetPos());

	// カメラに位置を反映.
	SetCameraPositionAndTarget_UpVecY(mPos, playerActor.GetPos());

}