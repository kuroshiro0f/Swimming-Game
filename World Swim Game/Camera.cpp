#include "Camera.h"

const int CAMERA_ADD_Y = 7;

// コンストラクタ
Camera::Camera(const PlayerActor& playerActor)
{
	// 奥行 1.0〜10000 までをカメラの描画範囲とする
	SetCameraNearFar(1.0f, 10000.0f);

	//カメラのポジション設定
	mPos = VGet(0, 40, 50);
	//プレイヤーのポジションをコピーする用の変数を初期化
	mTempPos = VGet(0, 0, 0);
}

// デストラクタ
Camera::~Camera()
{

}

// 更新
void Camera::Update(const PlayerActor& playerActor)
{
	mPos.x = playerActor.GetPosX();
	mPlayerPos = playerActor.GetPos();
	mPlayerPos.y = playerActor.GetPos().y + CAMERA_ADD_Y;

	if (!playerActor.turnFlag)
	{
		mPlayerPos.x -= mCorrection;
		mPos.x -= mCorrection;
	}
	if (playerActor.turnFlag)
	{
		mPlayerPos.x += mCorrection;
		mPos.x += mCorrection;
	}

	if (playerActor.dCount <= 15)
	{
		mPos.x = playerActor.GetPosX() - 25 + mCorrection;

	}

	SetCameraPositionAndTarget_UpVecY(mPos, mPlayerPos);	// カメラに位置を反映.
}