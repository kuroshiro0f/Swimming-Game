#include "Nagatomo_Actor.h"

int Nagatomo_Actor::modelHandle = NULL;

//コンストラクタ
Nagatomo_Actor::Nagatomo_Actor()
	:mPosition(VGet(0, 0, 0))
	, mVelosity(VGet(1, 0, 0))
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
