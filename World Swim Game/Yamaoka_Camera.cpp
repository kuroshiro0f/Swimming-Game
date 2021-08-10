#include "Yamaoka_Camera.h"
#include "Yamaoka_Actor.h"

// コンストラクタ
Yamaoka_Camera::Yamaoka_Camera(const Yamaoka_Actor& _actor)
{
	// 奥行 1.0～10000 までをカメラの描画範囲とする
	SetCameraNearFar(1.0f, 10000.0f);

	// ポジションを設定 
	m_pos = VGet(0, 60, -200);
}

// デストラクタ
Yamaoka_Camera::~Yamaoka_Camera()
{
}

// 更新
void Yamaoka_Camera::Update(const Yamaoka_Actor& _actor)
{
	// カメラに位置を反映する
	SetCameraPositionAndTargetAndUpVec(m_pos, VGet(0.0f, 20.0f, 0.0f), VGet(0, 1, 0));
}
