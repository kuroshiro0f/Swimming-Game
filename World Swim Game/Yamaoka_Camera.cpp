#include "Yamaoka_Camera.h"

// コンストラクタ
Camera::Camera(const Yamaoka_PlayerActor& playerActor)
{
	// 奥行 1.0～10000 までをカメラの描画範囲とする
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
void Camera::Update(const Yamaoka_PlayerActor& playerActor)
{
	//mTempPos = playerActor.GetPos();			//プレイヤーのポジションをmTempPosにコピー
	mPos.x = playerActor.GetPosX();				//mTempPos(プレイヤー)のx座標をカメラのx座標に代入

	if (playerActor.dCount <= 15)
	{
		mPos.x = playerActor.GetPosX() - 25;				//mTempPos(プレイヤー)のx座標をカメラのx座標に代入

	}

	SetCameraPositionAndTarget_UpVecY(mPos, playerActor.GetPos());	// カメラに位置を反映.

}

void Camera::Draw()
{
	DrawFormatString(20,  0, GetColor(255, 0, 0), "Pos.x   %f", mPos.x);
	DrawFormatString(20, 40, GetColor(255, 0, 0), "Pos.y   %f", mPos.y);
	DrawFormatString(20, 80, GetColor(255, 0, 0), "Pos.z   %f", mPos.z);
}
