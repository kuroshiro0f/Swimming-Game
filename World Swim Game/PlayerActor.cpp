#include "PlayerActor.h"

PlayerActor::PlayerActor()
	:mNowPlayerState(STATE_IDLE)
	, mNowKeyState(STATE_KEY_IDLE)
	, mPrevKeyState(STATE_KEY_IDLE)
{
	startFlag = true;

	mPosition = VGet(0, 18, -150);								// 初期位置設定
	mRotation = VGet(250.0f, 180.0f * DX_PI_F / 180.0f, 0.0f);	// 回転角度
	mDirection = VGet(0, 0, 1);
	//モデルのロード
	modelHandle = MV1LoadModel("data/player/player.pmx");

	animTotal = 0.0f;
	animNowTime = 0.0f;
	animIndex = 0;
}

PlayerActor::~PlayerActor()
{
	// モデルのアンロード
	MV1DeleteModel(modelHandle);
}

void PlayerActor::Update(float _deltaTime)
{
	UpdateActor(_deltaTime);
}

void PlayerActor::UpdateActor(float _deltaTime)
{
	int Key = GetJoypadInputState(DX_INPUT_KEY_PAD1);

	//スタート処理
	if (startFlag)
	{
		StartProcess(_deltaTime);
	}

	//泳ぎ処理
	if (Key & PAD_INPUT_RIGHT)
	{
		mPrevKeyState = mNowKeyState;					//今のキー状態を前回のキー状態に
		mNowKeyState = STATE_KEY_RIGHT;					//今のキー状態をSTATE_KEY_RIGHTに

		if (mNowKeyState != mPrevKeyState)				//今と前回のキー状態が違うとき
		{
			mPosition = VAdd(mPosition, mVelosity);
		}

	}
	if (Key & PAD_INPUT_LEFT)
	{
		mPrevKeyState = mNowKeyState;					//今のキー状態を前回のキー状態に
		mNowKeyState = STATE_KEY_LEFT;					//今のキー状態をSTATE_KEY_LEFTに

		if (mNowKeyState != mPrevKeyState)				//今と前回のキー状態が違うとき
		{
			mPosition = VAdd(mPosition, mVelosity);
		}
	}

	PlayAnim();
	MV1SetPosition(modelHandle, mPosition);
}

void PlayerActor::StartProcess(float _deltaTime)
{
	startFlag = false;

	//キー入力取得
	int Key = GetJoypadInputState(DX_INPUT_KEY_PAD1);

	if (Key & PAD_INPUT_M)				//スペースキー
	{
		mNowPlayerState = STATE_SWIM;	//プレイヤーを泳ぎ状態に

		attachAnim(0);					//アニメーション000をアタッチ
	}
}

void PlayerActor::DrawActor()
{
	// 3Dモデルの描画
	MV1DrawModel(modelHandle);

	// 3Dモデルの回転角度 (どの方向を向いているか)
	MV1SetRotationXYZ(modelHandle, mRotation);
}

void PlayerActor::PlayAnim()
{
	// 泳いでるいるとき
	if (mNowPlayerState == STATE_SWIM)
	{   // クロールアニメーション
		// 再生時間を進める
		animNowTime += 0.5f;
		// 再生時間がアニメーションの総再生時間を超えたら
	}
	if (animNowTime >= animTotal)
	{   // 再生時間を0に
		animNowTime = 0;
	}

	// アニメーションの再生時間をセットする
	MV1SetAttachAnimTime(modelHandle, animIndex, animNowTime);
}

void PlayerActor::attachAnim(int _animPlay)
{
	if (animIndex != -1)
	{   // アニメーションを削除する
		MV1DetachAnim(modelHandle, animIndex);
	}
	// アニメーションをアタッチする
	animIndex = MV1AttachAnim(modelHandle, _animPlay, -1, FALSE);
	// アニメーションの総再生時間を取得する
	animTotal = MV1GetAnimTotalTime(modelHandle, animIndex);
}
