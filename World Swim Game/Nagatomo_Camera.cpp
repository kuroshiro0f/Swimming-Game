#include "Nagatomo_Camera.h"

// コンストラクタ
Nagatomo_Camera::Nagatomo_Camera(const Nagatomo_PlayerActor& playerActor)
{
	// 奥行 1.0～10000 までをカメラの描画範囲とする
	SetCameraNearFar(1.0f, 10000.0f);

	//カメラのポジション設定
	mPos = VGet(50, 60, 0);
	mTempPos = mPos;
}

// デストラクタ
Nagatomo_Camera::~Nagatomo_Camera()
{

}

// 更新
void Nagatomo_Camera::Update(const Nagatomo_PlayerActor& playerActor)
{
	mPos = VAdd(mTempPos,playerActor.GetPos());

	// カメラに位置を反映.
	SetCameraPositionAndTarget_UpVecY(mPos, playerActor.GetPos());

}