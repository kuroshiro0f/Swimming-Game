#include "Nagatomo_PlayerActor.h"

#include <random>

const float D_COUNT = 0.355f;

//スピード関連
const VECTOR missSpeed = VGet(5, 0, 0);		//入力ミスしたときのスピード
const float addSpeed = 3.0f;				//加算されるスピード
//入力関連
const float limitTime = 1.5f;				//入力制限時間

//コンストラクタ
Nagatomo_PlayerActor::Nagatomo_PlayerActor()
	:mNowPlayerState(STATE_IDLE)
	,mNowKeyState(STATE_KEY_IDLE)
	,mPrevKeyState(STATE_KEY_IDLE)
	,mEvlt(NONE)
	, startTime(0)
	, tmpTime(0)
	, countUP(0)
{

	startFlag  = false;
	turnFlag   = false;
	randomFlag = false;
	inputSpaceFlag  = false;
	inputArrowFlag  = false;
	mCheckKeyFlag   = false;

	mPosition = VGet(150, 18, 0);								// 初期位置設定
	mRotation = VGet(250.0f, 90.0f * DX_PI_F / 180.0f, 0.0f);	// 回転角度
	mDirection = VGet(0, 0, 1);									//プレイヤーの方向
	mVelosity = VGet(8, 0, 0);									//速度

	//モデルのロード
	modelHandle = MV1LoadModel("data/player/player.pmx");

	animTotal = 0.0f;
	animNowTime = 0.0f;
	animIndex = 0;

	NowPos = 0;            // 現在の座標

	// ゴールまでの距離　( 50m ) 
	dCount    = 50.0f;         // 進んだ距離
	maxdCount = 50.0f;      // ゴール  

	//スタミナ関連
	st        = 600;			// スタミナ初期値
	MaxSt     = 600;			// スタミナ最大値
	MinSt     = 300;			// スタミナ最小値
	halfSt    = 1250 / 2;		//スタミナ（50%）
	quarterSt = 1250 / 4;		//スタミナ（25%）

	//カウントダウン
	count     = 30;				// 次のシーンに行くまでのカウント
	countDown = 120;			// カウントダウン（ 3秒 ）

	//入力関連
	inputTime = 0.0f;	
	inputLimitTime = limitTime;

	//スピード関連
	addStaminaSpeed = 0.0f;
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

	// カウントダウンが終了したら開始
	if (!startFlag && countDown <= 0)
	{
		// 開始時の時間を取得
		startTime = GetNowCount() / 1000;
		MV1SetPosition(modelHandle, mPosition);				// ポジション更新
		StartProcess(_deltaTime);
	}

	//	ステートが泳ぎの時
	if (mNowPlayerState == STATE_SWIM)
	{
		//現在時刻を取得
		tmpTime = GetNowCount() / 1000;
		// 経過時間を計算 
		countUP = (tmpTime - startTime);

		mPrevPosition = mPosition;							//プレイヤーのポジションを補完

		//ターン前
		if (!turnFlag)
		{
			mPosition.x -= mVelosity.x * _deltaTime;			//プレイヤーの自動移動
		}
		//ターン後
		else
		{
			mPosition.x += mVelosity.x * _deltaTime;			//プレイヤーの自動移動
		}

		dCount -= std::sqrt((mPosition.x - mPrevPosition.x) * (mPosition.x - mPrevPosition.x)) * 0.088;		// 残り距離を減らす

		if (randomFlag == false)
		{
			randomKeyNumber = rand() % 4 + 1;				//1～4までの数字をランダムに生成
			inputStartTime = GetNowCount() / 1000;			//ランダムに矢印を生成した時間を取得
			randomFlag = true;
			mCheckKeyFlag = false;
		}

		//プレイヤーの処理//
		switch (randomKeyNumber)
		{
		case 1:	//↑キーのとき
			inputEndTime = GetNowCount() / 1000;			//現在の時間を取得
			inputTime = (inputEndTime - inputStartTime);

			//↑キーを押されたとき
			if (Key & PAD_INPUT_UP && !mCheckKeyFlag)
			{
				mVelosity.x += addSpeed * addStaminaSpeed;
				st -= 20;
				inputTime = 0;
				inputArrowFlag = true;
				mCheckKeyFlag = true;
			}
			//↓ → ← キーが押されたとき
			else if (Key & PAD_INPUT_DOWN || Key & PAD_INPUT_RIGHT || Key & PAD_INPUT_LEFT )
			{
				
				mVelosity.x = missSpeed.x * addStaminaSpeed;
				st -= 40;
				inputLimitTime = limitTime;							//入力制限時間をリセット
				inputArrowFlag = false;
				
			}
			//キー入力がされて入力制限時間を過ぎたら
			else if (inputArrowFlag && inputTime > inputLimitTime)
			{
				inputTime = 0;
				inputLimitTime -= 0.2f;	
				randomFlag = false;
			}
			//キー入力がされずに入力制限時間を過ぎたら
			else if (!inputArrowFlag && inputTime > inputLimitTime)
			{

				mVelosity.x = missSpeed.x * addStaminaSpeed;
				inputTime = 0;
				inputLimitTime = limitTime;							//入力制限時間をリセット
				randomFlag = false;
			}
			break;
		case 2:		//↓キーのとき
			inputEndTime = GetNowCount() / 1000;
			inputTime = (inputEndTime - inputStartTime);			 //入力時間をカウント

			if (Key & PAD_INPUT_DOWN && !mCheckKeyFlag)
			{
				mVelosity.x += addSpeed * addStaminaSpeed;
				st -= 20;
				inputTime = 0;
				inputArrowFlag = true;
				mCheckKeyFlag = true;
			}
			else if (Key & PAD_INPUT_UP || Key & PAD_INPUT_RIGHT || Key & PAD_INPUT_LEFT)
			{
				mVelosity.x = missSpeed.x * addStaminaSpeed;
				st -= 40;
				inputLimitTime = limitTime;							//入力制限時間をリセット
				inputArrowFlag = false;
			}
			//キー入力がされて入力制限時間を過ぎたら
			else if (inputArrowFlag && inputTime > inputLimitTime)
			{
				inputTime = 0;
				inputLimitTime -= 0.2f;
				randomFlag = false;
			}
			//キー入力がされずに入力制限時間を過ぎたら
			else if (!inputArrowFlag && inputTime > inputLimitTime)
			{

				mVelosity.x = missSpeed.x * addStaminaSpeed;
				inputTime = 0;
				inputLimitTime = limitTime;							//入力制限時間をリセット
				randomFlag = false;
			}
			break;
		case 3:		//→キーのとき
			inputEndTime = GetNowCount() / 1000;
			inputTime = (inputEndTime - inputStartTime);			//入力時間をカウント

			if (Key & PAD_INPUT_RIGHT && !mCheckKeyFlag)
			{
				mVelosity.x += addSpeed * addStaminaSpeed;
				st -= 20;
				inputTime = 0;
				inputArrowFlag = true;
				mCheckKeyFlag = true;
			}
			else if (Key & PAD_INPUT_UP || Key & PAD_INPUT_DOWN || Key & PAD_INPUT_LEFT)
			{
				mVelosity.x = missSpeed.x * addStaminaSpeed;
				st -= 40;
				inputLimitTime = limitTime;							//入力制限時間をリセット
				inputArrowFlag = false;
			}
			//キー入力がされて入力制限時間を過ぎたら
			else if (inputArrowFlag && inputTime > inputLimitTime)
			{
				inputTime = 0;
				inputLimitTime -= 0.2f;	
				randomFlag = false;
			}
			//キー入力がされずに入力制限時間を過ぎたら
			else if (!inputArrowFlag && inputTime > inputLimitTime)
			{

				mVelosity.x = missSpeed.x * addStaminaSpeed;
				inputTime = 0;
				inputLimitTime = limitTime;				//入力制限時間をリセット
				randomFlag = false;
			}
			break;
		case 4:		//←キーのとき
			inputEndTime = GetNowCount() / 1000;
			inputTime = (inputEndTime - inputStartTime);  //入力時間をカウント

			if (Key & PAD_INPUT_LEFT && !mCheckKeyFlag)
			{
				mVelosity.x += addSpeed * addStaminaSpeed;
				st -= 20;
				inputTime = 0;
				inputArrowFlag = true;
				mCheckKeyFlag = true;
			}
			else if (Key & PAD_INPUT_UP || Key & PAD_INPUT_DOWN || Key & PAD_INPUT_RIGHT)
			{
				mVelosity.x = missSpeed.x * addStaminaSpeed;
				st -= 40;
				inputLimitTime = limitTime;							//入力制限時間をリセット
				inputArrowFlag = false;
			}
			//キー入力がされて入力制限時間を過ぎたら
			else if (inputArrowFlag && inputTime > inputLimitTime)
			{
				inputTime = 0;
				inputLimitTime -= 0.2f;
				randomFlag = false;
			}
			//キー入力がされずに入力制限時間を過ぎたら
			else if (!inputArrowFlag && inputTime > inputLimitTime)
			{

				mVelosity.x = missSpeed.x * addStaminaSpeed;
				inputTime = 0;
				inputLimitTime = limitTime;							//入力制限時間をリセット
				randomFlag = false;
			}
			break;

		}

		//残りスタミナが50%以上
		if (st >= halfSt)
		{
			addStaminaSpeed = 1.0f;
		}
		//残りスタミナが50%以下
		if (st <= halfSt && st >= quarterSt)
		{
			addStaminaSpeed = 0.8f;
		}
		//残りスタミナが25%以下
		if (st <= quarterSt)
		{
			addStaminaSpeed = 0.6f;
		}

		//引き戻し処理
		if (mPosition.x >= 150)
		{
			mPosition.x = 145;
		}
		if (mPosition.x <= -138)
		{
			mPosition.x = -138;
		}

		// 残り距離
		if (dCount <= 0.9)
		{
			// 端までついたら距離カウントを 0 にする
			dCount = 0;
			// 次のシーンまでのカウントを減らす
			count--;
		}

		// カウントダウンが 0 になると
		if (countDown <= 0)
		{
			// 値を 0 に
			countDown = 0;
		}

		//スペースが押されたとき
		if (Key & PAD_INPUT_M && turnFlag == false)
		{
			inputSpaceFlag = true;
			inputLimitTime = limitTime;			//入力制限時間をリセット
			mPosX = mPosition.x;				//押された時のプレイヤーの座標を補完

			if (-90 >= mPosX && mPosX > -120)
			{
				mEvlt = BAD;					//入力が早かったらBAD評価

			}
			if (-120 >= mPosX && mPosX > -130)
			{
				mEvlt = NORMAL;					//入力が少し早かったらNORMALE評価

			}
			if (-130 >= mPosX && mPosX > -140)
			{
				mEvlt = GOOD;					//入力がちょうどだったらGOOD評価

			}
		}
		//押されないまま端まで来たとき
		else if (turnFlag == false && mPosition.x <= -136)
		{
			mEvlt = BAD;						//BAD評価に
			turnFlag = true;
			inputLimitTime = limitTime;			//入力制限時間をリセット
		}

		//プールの端まで来たら
		if (mPosition.x <= -136)
		{
			turnFlag = true;
			mRotation = VGet(250.0f, 270.0f * DX_PI_F / 180.0f, 0.0f);							//プレイヤーの向きを反転

			switch (mEvlt)
			{
			case BAD:
				mVelosity = VGet(10, 0, 0);
				st -= 15;

			case NORMAL:
				mVelosity = VGet(12, 0, 0);
				st -= 10;

			case GOOD:
				mVelosity = VGet(15, 0, 0);
				st -= 5;

			default:
				break;
			}
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

	mNowPlayerState = STATE_SWIM;	// プレイヤーを泳ぎ状態に

	attachAnim(0);					// アニメーション000をアタッチ

	startFlag = true;               // スタートフラグを true に	
}

//描画
void Nagatomo_PlayerActor::DrawActor()
{
	// 3Dモデルの描画
	MV1DrawModel(modelHandle);

	// 3Dモデルの回転角度 (どの方向を向いているか)
	MV1SetRotationXYZ(modelHandle, mRotation);

	//デバッグ用//
	if (randomFlag)
	{
		DrawFormatString(0, 0, GetColor(0, 0, 0), "%d", randomKeyNumber);
	}
	DrawFormatString(0, 50, GetColor(0, 0, 0), "入力可能時間:%f", inputTime);
	DrawFormatString(0, 100, GetColor(0, 0, 0), "入力制限時間:%f", inputLimitTime);
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

void Nagatomo_PlayerActor::attachAnim(int _animPlay)
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
	DrawFormatString(_MinSt, 1000, GetColor(0, 0, 0), "%d / 600", st);

	//--------------//
	//  デバッグ用  //
	//-------------//

	//if (mEvlt == BAD)
	//{
	//	//デバッグ
	//	DrawFormatString(1600, 900, GetColor(0, 0, 0), "BAD:%f", mPosX);
	//}
	//if (mEvlt == NORMAL)
	//{
	//	//デバッグ
	//	DrawFormatString(1600, 900, GetColor(0, 0, 0), "NORMAL:%f", mPosX);
	//}
	//if (mEvlt == GOOD)
	//{
	//	//デバッグ
	//	DrawFormatString(1600, 900, GetColor(0, 0, 0), "GOOD:%f", mPosX);
	//}
}

// ゴールまでの距離
void Nagatomo_PlayerActor::DrawToGoal(int _dCount)
{
	//-------------//
	// デバッグ用 //
	//------------//

	//DrawFormatString(1300, 500, GetColor(0, 0, 0), "NowPos  %d", count);
	//DrawFormatString(1300, 550, GetColor(0, 0, 0), "  %d", 600 * _playerPos / _goalPos);

	// 残りの距離の表示
	DrawBox(1590, 895, 1850, 945, GetColor(0, 255, 255), TRUE);
	DrawFormatString(1600, 900, GetColor(0, 0, 0), "残り  %d m", (_dCount));

	// 一往復したら
	if (_dCount <= 0)
	{
		_dCount = 0;    // 値を固定
		SetFontSize(100);
		//DrawFormatString(900, 450, GetColor(255, 0, 0), "GOAL");
	}
}
