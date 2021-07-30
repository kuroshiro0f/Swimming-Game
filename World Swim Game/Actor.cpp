//#include "Actor.h"
//
////コンストラクタ
//Actor::Actor()
//{
//}
//
////デストラクタ
//Actor::~Actor()
//{
//}
//
//// Update関数　Gameからコールされる
//void Actor::Update(float _deltaTime)
//{
//	UpdateActor(_deltaTime);
//}
//
////アクターの更新
//void Actor::UpdateActor(float _deltaTime)
//{
//}
//
////描画
//void Actor::DrawActor()
//{
//	// 3Dモデルの描画
//	MV1DrawModel(modelHandle);
//
//	// 3Dモデルの回転角度 (どの方向を向いているか)
//	MV1SetRotationXYZ(modelHandle, mRotation);
//}
//
////アニメーション再生関数
//void Actor::PlayAnim()
//{
//}
//
////再生するアニメーションの番号を取得する関数
//void Actor::attachAnim(int _animPlay)
//{
//	if (animIndex != -1)
//	{   // アニメーションを削除する
//		MV1DetachAnim(modelHandle, animIndex);
//	}
//	// アニメーションをアタッチする
//	animIndex = MV1AttachAnim(modelHandle, _animPlay);
//	// アニメーションの総再生時間を取得する
//	animTotal = MV1GetAnimTotalTime(modelHandle, animIndex);
//}
