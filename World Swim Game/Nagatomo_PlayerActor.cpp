#include "Nagatomo_PlayerActor.h"

//コンストラクタ
Nagatomo_PlayerActor::Nagatomo_PlayerActor()
	:mNowPlayerState(STATE_IDLE)
	,mNowKeyState(STATE_KEY_IDLE)
	,mPrevKeyState(STATE_KEY_IDLE)
{

	startFlag = true;

	mPosition = VGet(150, 18, 0);								// 初期位置設定
	mRotation = VGet(250.0f, 90.0f * DX_PI_F / 180.0f, 0.0f);	// 回転角度
	mDirection = VGet(0, 0, 1);
	mVelosity = VGet(250, 0, 0);

	//モデルのロード
	modelHandle = MV1LoadModel("data/player/player.pmx");

	animTotal = 0.0f;
	animNowTime = 0.0f;
	animIndex = 0;

	// ゴールまでの距離 //
	dCount = 1000;         // 進んだ距離
	maxdCount = 1000;      // ゴール  
	NowPos = 0;            // 現在の座標
	// 調整中          //

	// スタミナゲージ //
	st = 1250;      // スタミナ初期値
	MaxSt = 1250;   // スタミナ最大値
	MinSt = 650;    // スタミナ最小値
}

//デストラクタ
Nagatomo_PlayerActor::~Nagatomo_PlayerActor()
{
	// モデルのアンロード
	MV1DeleteModel(modelHandle);
}

//更新処理
void Nagatomo_PlayerActor::Update(float _deltaTime)
{
	UpdateActor(_deltaTime);
}

//アクターの更新処理
void Nagatomo_PlayerActor::UpdateActor(float _deltaTime)
{
	int Key = GetJoypadInputState(DX_INPUT_KEY_PAD1);

	
	//スタート処理
	//startFlagがtrueのとき
	if (startFlag)
	{
		PlayAnim(_deltaTime);								// アニメーション情報を取得
		MV1SetPosition(modelHandle, mPosition);				// ポジション更新
		StartProcess(_deltaTime);
	}

	//startFlagがfalseのとき
	if (!startFlag)
	{
		//泳ぎ処理
		if (Key & PAD_INPUT_RIGHT && mPosition.x <=150 && mPosition.x >= -138)
		{
			mPrevKeyState = mNowKeyState;					//今のキー状態を前回のキー状態に
			mNowKeyState = STATE_KEY_RIGHT;					//今のキー状態をSTATE_KEY_RIGHTに

			if (mNowKeyState != mPrevKeyState)				//今と前回のキー状態が違うとき
			{
				mPosition.x -= mVelosity.x * _deltaTime;	//mPositionにmVelosityを加算
				st--;										 // スタミナを減らす
				dCount--;									 // 残り距離を減らす
			}
		}

		if (Key & PAD_INPUT_LEFT && mPosition.x <=150 && mPosition.x >= -138)
		{
			mPrevKeyState = mNowKeyState;					//今のキー状態を前回のキー状態に
			mNowKeyState = STATE_KEY_LEFT;					//今のキー状態をSTATE_KEY_LEFTに

			if (mNowKeyState != mPrevKeyState)				//今と前回のキー状態が違うとき
			{
				mPosition.x -= mVelosity.x * _deltaTime;	//mPositionにmVelosityを加算
				st--;										// スタミナを減らす
				dCount--;									// 残り距離を減らす
			}
		}
		//引き戻し処理
		if (mPosition.x >= 150)
		{
			mPosition.x = 145;
		}

		//ターン処理
		/*if (mPosition.x <= -132 && mPosition.x >= -140 && Key & PAD_INPUT_M)					//端まで行くと
		{
			mRotation = VGet(250.0f, 270.0f * DX_PI_F / 180.0f, 0.0f);							//プレイヤーを反転
			mVelosity = VGet(-100, 0, 0);														//速度を反転
		}*/

		// 残り距離
		if (dCount <= 500)
		{
			dCount = 500;
		}

		PlayAnim(_deltaTime);						// アニメーション情報を取得
		MV1SetPosition(modelHandle, mPosition);		// ポジション更新
	}
}

//スタート処理
void Nagatomo_PlayerActor::StartProcess(float _deltaTime)
{

	//キー入力取得
	int Key = GetJoypadInputState(DX_INPUT_KEY_PAD1);

	if (Key & PAD_INPUT_M)				//スペースキー
	{
		mNowPlayerState = STATE_SWIM;	//プレイヤーを泳ぎ状態に

		startFlag = false;
		attachAnim(0);					//アニメーション000をアタッチ
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
void Nagatomo_PlayerActor::PlayAnim(float _deltaTime)
{
	// 泳いでるいるとき
	if (mNowPlayerState == STATE_SWIM)
	{   // クロールアニメーション
		// 再生時間を進める
		animNowTime += 15.0f * _deltaTime;
		// 再生時間がアニメーションの総再生時間を超えたら
		if (animNowTime >= animTotal)
		{   // 再生時間を0に
			animNowTime = 0;
		}
	}
		
	// アニメーションの再生時間をセットする
	MV1SetAttachAnimTime(modelHandle, animIndex, animNowTime);
}

//再生するアニメーションをアタッチする関数
void Nagatomo_PlayerActor::attachAnim(int _animPlay)
{
	if (animIndex != -1)
	{   // アニメーションを削除する
		MV1DetachAnim(modelHandle, animIndex);
	}
	// アニメーションをアタッチする
	animIndex = MV1AttachAnim(modelHandle, _animPlay,-1,FALSE);
	// アニメーションの総再生時間を取得する
	animTotal = MV1GetAnimTotalTime(modelHandle, animIndex);
}

// スタミナゲージの描画
void Nagatomo_PlayerActor::DrawSt(int _st, int _MaxSt, int _MinSt)
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
void Nagatomo_PlayerActor::DrawToGoal(int _playerPos, int _goalPos)
{
	// デバッグ用
	//DrawFormatString(1300, 500, GetColor(0, 0, 0), "NowPos  %d", NowPos);
	//DrawFormatString(1300, 550, GetColor(0, 0, 0), "  %d", 600 * _playerPos / _goalPos);

	// 残りの距離の表示
	DrawBox(1590, 895, 1850, 945, GetColor(0, 255, 255), TRUE);
	DrawFormatString(1600, 900, GetColor(0, 0, 0), "残り  %d m", 1000 * _playerPos / _goalPos - 500);

	// 一往復したら
	if (_playerPos <= 500)
	{
		_playerPos = 500;    // 値を固定
		DrawFormatString(900, 950, GetColor(255, 0, 0), "GOAL");
	}

}