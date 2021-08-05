#include "Nagatomo_Camera.h"

// コンストラクタ
Nagatomo_Camera::Nagatomo_Camera(const Nagatomo_PlayerActor& player)
{
	// 奥行 1.0～10000 までをカメラの描画範囲とする
	SetCameraNearFar(1.0f, 10000.0f);

	pos = VGet(0, 25, -55);
}

// デストラクタ
Nagatomo_Camera::~Nagatomo_Camera()
{

}

// 更新
void Nagatomo_Camera::Update(const Nagatomo_PlayerActor& player)
{
	// カメラに位置を反映する
	SetCameraPositionAndTargetAndUpVec(pos, VGet(0.0f, 20.0f, 0.0f), VGet(0, 1, 0));

}