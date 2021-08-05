#include "Nagatomo_PlayerActor.h"

//コンストラクタ
Nagatomo_PlayerActor::Nagatomo_PlayerActor()
	:mNowPlayerState(STATE_IDLE)
	,mNowKeyState(STATE_KEY_IDLE)
	,mPrevKeyState(STATE_KEY_IDLE)
{
	startFlag = true;

	//モデルのロード
	modelHandle = MV1LoadModel("data/model/miku.pmx");

	animTotal = 0.0f;
	animNowTime = 0.0f;
	animIndex = 0;
}

//デストラクタ
Nagatomo_PlayerActor::~Nagatomo_PlayerActor()
{
	// モデルのアンロード
	MV1DeleteModel(modelHandle);
}

//アクターの更新処理
void Nagatomo_PlayerActor::UpdateActor(float _deltaTime)
{
	int Key = GetJoypadInputState(DX_INPUT_KEY_PAD1);

	if (Key & PAD_INPUT_RIGHT)
	{
		mPrevKeyState = mNowKeyState;
		mNowKeyState = STATE_KEY_RIGHT;

		mPosition = VAdd(mPosition, mVelosity);

	}
	if (Key & PAD_INPUT_LEFT)
	{
		mPrevKeyState = mNowKeyState;
		mNowKeyState = STATE_KEY_LEFT;

		mPosition = VAdd(mPosition, mVelosity);
	}
}

//スタート処理
void Nagatomo_PlayerActor::StartProcess(float _deltaTime)
{
	//キー入力取得
	int Key = GetJoypadInputState(DX_INPUT_KEY_PAD1);

	if (mNowPlayerState == STATE_IDLE && timer == 0)
	{
		startFlag = false;
	}

	if (Key & PAD_INPUT_M && startFlag == false)	//スペースキー
	{
		mNowPlayerState = STATE_SWIM;
	}
}

//描画
void Nagatomo_PlayerActor::DrawActor()
{
	// 3Dモデルの描画
	MV1DrawModel(modelHandle);

	// 3Dモデルの回転角度 (どの方向を向いているか)
	MV1SetRotationXYZ(modelHandle, mRotation);
}

//アニメーション
void Nagatomo_PlayerActor::PlayAnim()
{
	// 泳いでるいるとき
	if (mNowPlayerState == STATE_SWIM)
	{   // クロールアニメーション
		// 再生時間を進める
		animNowTime += 0.25f;
		// 再生時間がアニメーションの総再生時間を超えたら
		if (animNowTime >= animTotal)
		{   // 再生時間を0に
			animNowTime = 0;
		}
	}

	// アニメーションの再生時間をセットする
	MV1SetAttachAnimTime(modelHandle, animIndex, animNowTime);
}

//アニメーションのアタッチ
void Nagatomo_PlayerActor::attachAnim(int _animPlay)
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
