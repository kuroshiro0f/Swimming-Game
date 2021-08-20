#include "Yamaoka_PlayerActor.h"

Yamaoka_PlayerActor::Yamaoka_PlayerActor()
	:mNowPlayerState(STATE_IDLE)
	, mNowKeyState(STATE_KEY_IDLE)
	, mPrevKeyState(STATE_KEY_IDLE)
	, startTime(0)
	, tmpTime(0)
	, countUP(0)
{
	startFlag = false;

	mPosition = VGet(150, 18, 0);								// 初期位置設定
	// 奥を向く
	mRotation = VGet(250.0f, 90.0f * DX_PI_F / 180.0f, 0.0f);	// 回転角度
	mDirection = VGet(0, 0, 1);

	//モデルのロード
	modelHandle = MV1LoadModel("data/player/player.pmx");

	animTotal = 0.0f;
	animNowTime = 0.0f;
	animIndex = 0;

	// ゴールまでの距離 //
	//dCount = 1115;         // 進んだ距離
	//maxdCount = 1115;      // ゴール  
	NowPos = 0;            // 現在の座標
	// 調整中          //

	// ゴールまでの距離　( 25m ) 
	dCount = 25.0f;         // 進んだ距離
	maxdCount = 25.0f;      // ゴール  

	// スタミナゲージ //
	st = 1250;      // スタミナ初期値
	MaxSt = 1250;   // スタミナ最大値
	MinSt = 650;    // スタミナ最小値

	count = 30;      // 次のシーンに行くまでのカウント

	countDown = 120; // カウントダウン（ 3秒 ）

}

Yamaoka_PlayerActor::~Yamaoka_PlayerActor()
{
	// モデルのアンロード
	MV1DeleteModel(modelHandle);
}

void Yamaoka_PlayerActor::Update(float _deltaTime)
{
	UpdateActor(_deltaTime);
}

void Yamaoka_PlayerActor::UpdateActor(float _deltaTime)
{
	int Key = GetJoypadInputState(DX_INPUT_KEY_PAD1);

	// スタート処理
	/*if (!startFlag)
	{
		StartProcess(_deltaTime);
		// 開始時の時間を取得
		startTime = GetNowCount() / 1000;
	}*/

	// カウントダウンが終了したら開始
	if (!startFlag && countDown <= 0)
	{
		// 開始時の時間を取得
		startTime = GetNowCount() / 1000;

		StartProcess(_deltaTime);
	}

	// 泳いでいる状態の時
	if (mNowPlayerState == STATE_SWIM)
	{
		//現在時刻を取得
		tmpTime = GetNowCount() / 1000;
		// 経過時間を計算 
		countUP = (tmpTime - startTime);

		// 泳ぎ処理
		if (Key & PAD_INPUT_RIGHT)
		{
			mPrevKeyState = mNowKeyState;					//今のキー状態を前回のキー状態に
			mNowKeyState = STATE_KEY_RIGHT;					//今のキー状態をSTATE_KEY_RIGHTに
			if (mNowKeyState != mPrevKeyState)				//今と前回のキー状態が違うとき
			{
				st--;      // スタミナを減らす
				dCount -= 0.086f;  // 残り距離を減らす
				// 奥に進むように
				mPosition = VAdd(mPosition, VGet(-1, 0, 0));
			}
		}
		if (Key & PAD_INPUT_LEFT)
		{
			mPrevKeyState = mNowKeyState;					//今のキー状態を前回のキー状態に
			mNowKeyState = STATE_KEY_LEFT;					//今のキー状態をSTATE_KEY_LEFTに
			if (mNowKeyState != mPrevKeyState)				//今と前回のキー状態が違うとき
			{
				st--;      // スタミナを減らす
				dCount -= 0.086f;  // 残り距離を減らす
				// 奥に進むように
				mPosition = VAdd(mPosition, VGet(-1, 0, 0));
			}
		}

		// 残り距離
		if (dCount <= 0.9)
		{
			// 端までついたら距離カウントを 0 にする
			dCount = 0;
			// 次のシーンまでのカウントを減らす
			count--;    
		}
	}

	// カウントダウンが 0 になると
	if (countDown <= 0)
	{
		// 値を 0 に
		countDown = 0;
	}

	PlayAnim();  // アニメーション情報を取得
	// ポジション更新
	MV1SetPosition(modelHandle, mPosition);
}

// スタート処理
void Yamaoka_PlayerActor::StartProcess(float _deltaTime)
{
	// キー入力取得
	int Key = GetJoypadInputState(DX_INPUT_KEY_PAD1);

	//if (Key & PAD_INPUT_M)				// スペースキー
	//{
	//	mNowPlayerState = STATE_SWIM;	// プレイヤーを泳ぎ状態に

	//	attachAnim(0);					// アニメーション000をアタッチ

	//	startFlag = true;               // スタートフラグを true に	
	//}

	mNowPlayerState = STATE_SWIM;	// プレイヤーを泳ぎ状態に

	attachAnim(0);					// アニメーション000をアタッチ

	startFlag = true;               // スタートフラグを true に	
}

// 描画
void Yamaoka_PlayerActor::DrawActor()
{
	// 3Dモデルの描画
	MV1DrawModel(modelHandle);

	// 3Dモデルの回転角度 (どの方向を向いているか)
	MV1SetRotationXYZ(modelHandle, mRotation);

	/*if (!startFlag)
	{
		DrawFormatString(860, 540, GetColor(255, 0, 255), "Push Space");
	}*/

	//DrawFormatString(860, 540, GetColor(255, 0, 255), "countDown   %d ", countDown);

}

// アニメーション再生時間取得関数
void Yamaoka_PlayerActor::PlayAnim()
{
	// 泳いでるいるとき
	if (mNowPlayerState == STATE_SWIM)
	{   // クロールアニメーション
		// 再生時間を進める
		animNowTime += 0.5f;
		// 再生時間がアニメーションの総再生時間を超えたら
		if (animNowTime >= animTotal)
		{   // 再生時間を0に
			animNowTime = 0;
		}
	}
	// アニメーションの再生時間をセットする
	MV1SetAttachAnimTime(modelHandle, animIndex, animNowTime);
}

// アニメーション取得関数
void Yamaoka_PlayerActor::attachAnim(int _animPlay)
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

// スタミナゲージの描画
void Yamaoka_PlayerActor::DrawSt(int _st, int _MaxSt, int _MinSt)
{
	// 色設定
	int color = GetColor(0, 255, 0);  // 中身(緑)
	int color2 = GetColor(0, 0, 0);   // 枠 (黒)

	// スタミナゲージが 0 になったら
	if (_st <= _MinSt)
	{
		// スタミナを最小値に
		_st = _MinSt;
	}

	// ゲージの枠表示
	DrawBox(_MinSt, 1000, _MaxSt, 1035, color2, FALSE);
	// ゲージの中身表示
	DrawBox(_MinSt, 1000, _MaxSt * _st / _MaxSt, 1035, color, TRUE);
	// 数値表示 
	DrawFormatString(_MinSt, 1000, GetColor(0, 0, 0), "%d / 600", _MaxSt * _st / _MaxSt - _MinSt);
}

// ゴールまでの距離
void Yamaoka_PlayerActor::DrawToGoal(float _playerPos, float _goalPos)
{
	// デバッグ用
	//DrawFormatString(1300, 500, GetColor(0, 0, 0), "NowPos  %d", count);
	//DrawFormatString(1300, 550, GetColor(0, 0, 0), "  %d", 600 * _playerPos / _goalPos);

	// 残りの距離の表示
	DrawBox(1590, 895, 1850, 945, GetColor(0, 255, 255), TRUE);
	DrawFormatString(1600, 900, GetColor(0, 0, 0), "残り  %d m", (int)(_goalPos * _playerPos / _goalPos));

	// 一往復したら
	if (_playerPos <= 0)
	{
		_playerPos = 0;    // 値を固定
		SetFontSize(100);
		DrawFormatString(900, 450, GetColor(255, 0, 0), "GOAL");
	}

}