#include "Camera.h"

// コンストラクタ
Camera::Camera(const PlayerActor& playerActor)
{
	// 奥行 1.0～10000 までをカメラの描画範囲とする
	SetCameraNearFar(1.0f, 10000.0f);

	//カメラのポジション設定
	mPos = VGet(0, 60, 80);
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
	//mTempPos = playerActor.GetPos();								//プレイヤーのポジションをmTempPosにコピー
	mPos.x = playerActor.GetPosX();									//mTempPos(プレイヤー)のx座標をカメラのx座標に代入

	SetCameraPositionAndTarget_UpVecY(mPos, playerActor.GetPos());	// カメラに位置を反映.

}