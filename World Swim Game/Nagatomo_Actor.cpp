#include "Nagatomo_Actor.h"

//コンストラクタ
Nagatomo_Actor::Nagatomo_Actor()
{
}

//デストラクタ
Nagatomo_Actor::~Nagatomo_Actor()
{
}

// Update関数　Gameからコールされる
void Nagatomo_Actor::Update(float _deltaTime)
{
	UpdateActor(_deltaTime);
}

//アクターの更新
void Nagatomo_Actor::UpdateActor(float _deltaTime)
{
}

//描画
void Nagatomo_Actor::DrawActor()
{
	// 3Dモデルの描画
	MV1DrawModel(modelHandle);

	// 3Dモデルの回転角度 (どの方向を向いているか)
	MV1SetRotationXYZ(modelHandle, mRotation);
}

//アニメーション再生関数
void Nagatomo_Actor::PlayAnim()
{
}

//再生するアニメーションの番号を取得する関数
void Nagatomo_Actor::attachAnim(int _animPlay)
{
	if (animIndex != -1)
	{   // アニメーションを削除する
		MV1DetachAnim(modelHandle, animIndex);
	}
	// アニメーションをアタッチする
	animIndex = MV1AttachAnim(modelHandle, _animPlay);
	// アニメーションの総再生時間を取得する
	animTotal = MV1GetAnimTotalTime(modelHandle, animIndex);
}
