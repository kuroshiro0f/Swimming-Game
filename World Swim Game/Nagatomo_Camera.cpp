#include "Nagatomo_Camera.h"

// コンストラクタ
Camera::Camera(const Nagatomo_PlayerActor& playerActor)
{
	// 奥行 1.0～10000 までをカメラの描画範囲とする
	SetCameraNearFar(1.0f, 10000.0f);

	//カメラの初期ポジション設定
	mPos = VGet(0, 60, 80);
}

// デストラクタ
Camera::~Camera()
{

}

// 更新
void Camera::Update(const Nagatomo_PlayerActor& playerActor)
{							
	//プレイヤーのポジションをmTempPosにコピー
	mPos.x = playerActor.GetPosX();									//mTempPos(プレイヤー)のx座標をカメラのx座標に代入

	/*VECTOR aimPos = VGet(playerActor.GetPos().x, 60, 80);		//b
	VECTOR posToAim = VSub(aimPos, mPos);	//(b - a)
	VECTOR scaledPosToAim = VScale(posToAim, 0.5f);	//((b-a)*t)
	mPos = VAdd(mPos, scaledPosToAim);*/

	SetCameraPositionAndTarget_UpVecY(mPos, playerActor.GetPos());	// カメラに位置を反映.

}
