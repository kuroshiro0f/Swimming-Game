#include "Yamaoka_PlayerActor.h"

#include <random>

const float D_COUNT = 0.355f;

//	スタミナゲージの色が変わる残りゲージ量
const int GREEN = 300;
const int ORANGE = 150;
//const int RED = 3;

//	スタミナゲージの表示位置
const int ST_FIRST_X = 650;
const int ST_FIRST_Y = 1000;
const int ST_END_X = 1250;
const int ST_END_Y = 1035;

//	スタミナの減少量
const int ST_SUC_DEC = 60;
const int ST_FAI_DEC = 100;
//const int ST_SUC_DEC = 1;
//const int ST_FAI_DEC = 2;

//スピード関連
const VECTOR missSpeed = VGet(5, 0, 0);		//入力ミスしたときのスピード
const float addSpeed = 3.0f;				//加算されるスピード
const float maxSpeed = 15.0f;				//最大スピード

//入力関連
const float limitTime = 1.5f;				//入力制限時間
const float maxTime = 0.5f;					//最大時間

const int dCountUlt = 15;					//ウルトが使えるようになる残り距離


//コンストラクタ
Yamaoka_PlayerActor::Yamaoka_PlayerActor()
	:mNowPlayerState(STATE_IDLE)
	, mNowKeyState(STATE_KEY_IDLE)
	, mPrevKeyState(STATE_KEY_IDLE)
	, mEvlt(NONE)
	, startTime(0)
	, tmpTime(0)
	, countUP(0)
	, countDownFinishFlag(false)
	, skillFlag(false)
	, turnGraphFlag(false)
	, finishFlag(false)
	, m_ultCheckFlag(false)
	, m_ultFinishFlag(0)
{
	startFlag = false;
	turnFlag = false;
	randomFlag = false;
	inputSpaceFlag = false;
	inputArrowFlag = false;
	mCheckKeyFlag = false;
	ultLimitFlag = false;
	ultFlag = false;

	mPosition = VGet(150, 17, 0);								// 初期位置設定
	mRotation = VGet(250.0f, 90.0f * DX_PI_F / 180.0f, 0.0f);	// 回転角度
	mDirection = VGet(0, 0, 1);									//プレイヤーの方向
	mVelosity = VGet(8, 0, 0);									//速度

	// モデルのロード
	modelHandle = MV1LoadModel("data/swimmer/player.pmx");

	animTotal = 0.0f;
	animNowTime = 0.0f;
	animIndex = 0;

	NowPos = 0;            // 現在の座標

	// ゴールまでの距離　( 50m )
	dCount = 50.0f;         // 進んだ距離
	maxdCount = 50.0f;      // ゴール

	//スタミナ関連
	st = 600;				// スタミナ初期値
	MaxSt = 600;			// スタミナ最大値
	MinSt = 0;				// スタミナ最小値
	halfSt = 600 / 2;		//スタミナ（50%）
	quarterSt = 600 / 4;	//スタミナ（25%）

	count = 30;      // 次のシーンに行くまでのカウント

	countDown = 170; // カウントダウン（ 3秒 ）
	inputTime = 0;

	skillCount = 0;    // スキルの使用カウント
	skillTime = 180;   // スキルの効果時間

	//入力関連
	inputTime = 0.0f;
	inputLimitTime = limitTime;

	//スピード関連
	addStaminaSpeed = 0.0f;

	inputCount = 0;

	// 停止時間初期化
	stopTime = 0;

	countSpeed = 0;

}

//デストラクタ
Yamaoka_PlayerActor::~Yamaoka_PlayerActor()
{
	// メモリの解放
	MV1DeleteModel(modelHandle);
	DeleteGraph(turnGraphHandle);
}

//更新処理
void Yamaoka_PlayerActor::Update(float _deltaTime)
{
	UpdateActor(_deltaTime);
}

//アクターの更新処理
void Yamaoka_PlayerActor::UpdateActor(float _deltaTime)
{
	int Key = GetJoypadInputState(DX_INPUT_KEY_PAD1);

	//------------------------YAMAOKA--------------------------↓
	// カウントダウンが終了したら開始
	if (!startFlag && countDown <= 0)
	{
		// 開始時の時間を取得
		startTime = GetNowCount() / 1000;
		MV1SetPosition(modelHandle, mPosition);				// ポジション更新
		StartProcess(_deltaTime);
		countDownFinishFlag = true;
	}
	//------------------------YAMAOKA--------------------------↑

	//	ステートが泳ぎの時
	if (mNowPlayerState == STATE_SWIM)
	{
		//------------------------YAMAOKA--------------------------↓
		if (!skillFlag && !finishFlag)
		{
			//現在時刻を取得
			tmpTime = GetNowCount() / 1000;
			// 経過時間を計算 
			countUP = (tmpTime - startTime);
		}

		// 4秒おきにスタミナ回復
		if (countUP % 4 == 0 && !skillFlag)
		{
			st += 1;
		}
		// 最大値より多くなると
		if (st >= MaxSt)
		{
			st = MaxSt;
		}
		//------------------------YAMAOKA--------------------------↑

		mPrevPosition = mPosition;							//プレイヤーのポジションを補完

		//ターン前
		if (!turnFlag)
		{
			mPosition.x -= mVelosity.x * _deltaTime;			//プレイヤーの自動移動
		}
		//ターン後
		else if (!skillFlag)
		{
			mPosition.x += mVelosity.x * _deltaTime;			//プレイヤーの自動移動
		}

		dCount -= std::sqrt((mPosition.x - mPrevPosition.x) * (mPosition.x - mPrevPosition.x)) * 0.088;

		if (randomFlag == false && skillFlag == false)
		{
			randomKeyNumber = rand() % 4 + 1;				//1〜4までの数字をランダムに生成
			inputStartTime = GetNowCount() / 1000;			//ランダムに矢印を生成した時間を取得
			randomFlag = true;
			mCheckKeyFlag = false;
		}

		//------------------------YAMAOKA--------------------------↓
		// 息継ぎキーを押したら
		if (CheckHitKey(KEY_INPUT_C) && skillFlag == false)
		{
			randomKeyNumber = 5;          // randamKeyNumberに 5 (息継ぎキーの番号)を入れる
			inputStartTime = GetNowCount() / 1000;			//ランダムに矢印を生成した時間を取得
			randomFlag = true;
		}
		//------------------------YAMAOKA--------------------------↑

		if (dCount <= dCountUlt)
		{
			ultLimitFlag = true;
		}
		//必殺技
		if (/*CheckHitKey(KEY_INPUT_A) &&*/ ultLimitFlag)
		{
			ultFlag = true;
			randomFlag = false;
		}

		/*if (ultFlag)
		{
			if (!m_ultCheckFlag)
			{
				UltNumber(randomFlag);
			}
			UltProcessInput(arrow, 4);
			m_ultCheckFlag = true;
			randomFlag = true;
		}*/

		if (!ultFlag && !skillFlag)
		{
			//プレイヤーの処理//
			switch (randomKeyNumber)
			{
			case 1:	//↑キーのとき
				inputEndTime = GetNowCount() / 1000;			//現在の時間を取得
				inputTime = (inputEndTime - inputStartTime);

				//↑キーを押されたとき
				if (Key & PAD_INPUT_UP && !mCheckKeyFlag)
				{
					if (st > MinSt)
					{
						if (mVelosity.x > maxSpeed)
						{
							mVelosity.x = maxSpeed;
						}
						else
						{
							mVelosity.x += addSpeed * addStaminaSpeed;
						}
						st -= ST_SUC_DEC;
						inputTime = 0;
						inputArrowFlag = true;
						mCheckKeyFlag = true;
					}

				}
				//↓ → ← キーが押されたとき
				else if (!mCheckKeyFlag && Key & PAD_INPUT_DOWN || !mCheckKeyFlag && Key & PAD_INPUT_RIGHT || !mCheckKeyFlag && Key & PAD_INPUT_LEFT)
				{
					if (st > MinSt)
					{
						mVelosity.x = missSpeed.x * addStaminaSpeed;
						st -= ST_FAI_DEC;
						inputLimitTime = limitTime;							//入力制限時間をリセット
						inputArrowFlag = false;
						mCheckKeyFlag = true;
					}

				}
				//キー入力がされて入力制限時間を過ぎたら
				else if (inputArrowFlag && inputTime > inputLimitTime)
				{
					inputTime = 0;
					inputArrowFlag = false;
					randomFlag = false;
					if (inputLimitTime < maxTime)
					{
						inputLimitTime = maxTime;
					}
					else
					{
						inputLimitTime -= 0.2f;
					}
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
					if (st > MinSt)
					{
						if (mVelosity.x > maxSpeed)
						{
							mVelosity.x = maxSpeed;
						}
						else
						{
							mVelosity.x += addSpeed * addStaminaSpeed;
						}
						st -= ST_SUC_DEC;
						inputTime = 0;
						inputArrowFlag = true;
						mCheckKeyFlag = true;
					}
				}
				else if (!mCheckKeyFlag && Key & PAD_INPUT_UP || !mCheckKeyFlag && Key & PAD_INPUT_RIGHT || !mCheckKeyFlag && Key & PAD_INPUT_LEFT)
				{
					if (st > MinSt)
					{
						mVelosity.x = missSpeed.x * addStaminaSpeed;
						st -= ST_FAI_DEC;
						inputLimitTime = limitTime;							//入力制限時間をリセット
						inputArrowFlag = false;
						mCheckKeyFlag = true;
					}
				}
				//キー入力がされて入力制限時間を過ぎたら
				else if (inputArrowFlag && inputTime > inputLimitTime)
				{
					inputTime = 0;
					inputArrowFlag = false;
					randomFlag = false;
					if (inputLimitTime < maxTime)
					{
						inputLimitTime = maxTime;
					}
					else
					{
						inputLimitTime -= 0.2f;
					}
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
					if (st > MinSt)
					{
						if (mVelosity.x > maxSpeed)
						{
							mVelosity.x = maxSpeed;
						}
						else
						{
							mVelosity.x += addSpeed * addStaminaSpeed;
						}
						st -= ST_SUC_DEC;
						inputTime = 0;
						inputArrowFlag = true;
						mCheckKeyFlag = true;
					}
				}
				else if (!mCheckKeyFlag && Key & PAD_INPUT_UP || !mCheckKeyFlag && Key & PAD_INPUT_DOWN || !mCheckKeyFlag && Key & PAD_INPUT_LEFT)
				{
					if (st > MinSt)
					{
						mVelosity.x = missSpeed.x * addStaminaSpeed;
						st -= ST_FAI_DEC;
						inputLimitTime = limitTime;							//入力制限時間をリセット
						inputArrowFlag = false;
						mCheckKeyFlag = true;
					}
				}
				//キー入力がされて入力制限時間を過ぎたら
				else if (inputArrowFlag && inputTime > inputLimitTime)
				{
					inputTime = 0;
					inputArrowFlag = false;
					randomFlag = false;
					if (inputLimitTime < maxTime)
					{
						inputLimitTime = maxTime;
					}
					else
					{
						inputLimitTime -= 0.2f;
					}
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
					if (st > MinSt)
					{
						if (mVelosity.x > maxSpeed)
						{
							mVelosity.x = maxSpeed;
						}
						else
						{
							mVelosity.x += addSpeed * addStaminaSpeed;
						}
						st -= ST_SUC_DEC;
						inputTime = 0;
						inputArrowFlag = true;
						mCheckKeyFlag = true;
					}
				}
				else if (!mCheckKeyFlag && Key & PAD_INPUT_UP || !mCheckKeyFlag && Key & PAD_INPUT_DOWN || !mCheckKeyFlag && Key & PAD_INPUT_RIGHT)
				{
					if (st > MinSt)
					{
						mVelosity.x = missSpeed.x * addStaminaSpeed;
						st -= ST_FAI_DEC;
						inputLimitTime = limitTime;							//入力制限時間をリセット
						inputArrowFlag = false;
						mCheckKeyFlag = true;
					}
				}
				//キー入力がされて入力制限時間を過ぎたら
				else if (inputArrowFlag && inputTime > inputLimitTime)
				{
					inputTime = 0;
					inputArrowFlag = false;
					randomFlag = false;
					if (inputLimitTime < maxTime)
					{
						inputLimitTime = maxTime;
					}
					else
					{
						inputLimitTime -= 0.2f;
					}
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
			default:
				break;
			}
		}

		//------------------------YAMAOKA--------------------------↓
		// 息継ぎ処理
		if (randomKeyNumber == STATE_KEY_C)
		{
			inputEndTime = GetNowCount() / 1000;			//現在の時間を取得
			inputTime = (inputEndTime - inputStartTime);	//現在時間とランダムに矢印を生成した時間の差

			// Cキーを押されたとき
			if (Key & PAD_INPUT_C)
			{
				st += 5;			// スタミナを増やす
				inputTime = 0;		// 入力可能時間を初期化
				// スタミナが最大値を超えたら
				if (st >= MaxSt)
				{
					st = MaxSt;
				}

				mVelosity.x = 7;  // プレイヤーの速度を設定

			}
			// 現在時間とランダムに矢印を生成した時間の差が1秒たったら
			else if (inputTime > 1)
			{
				randomFlag = false;
				inputTime = 0;			// 入力可能時間を初期化
				inputLimitTime = limitTime;				//入力制限時間をリセット
				inputArrowFlag = false;
			}
		}
		//------------------------YAMAOKA--------------------------↑

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

		//------------------------YAMAOKA--------------------------↓
		// スタミナが切れたら
		if (!skillFlag)
		{
			if (st <= MinSt)
			{
				// スタミナを最小値で固定
				st = MinSt;
			}
			// ターンする前
			if (st <= MinSt)
			{
				// 速度を通常の半分程度に
				mVelosity = VGet(2, 0, 0);
			}
		}

		//引き戻し処理
		if (mPosition.x >= 150)
		{
			mPosition.x = 145;
		}
		if (mPosition.x <= -138)
		{
			mPosition.x = 138;
		}
		// 残り距離
		if (dCount <= 0)
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
		//------------------------YAMAOKA--------------------------↑


		//スペースが押されたとき
		if (Key & PAD_INPUT_M && turnFlag == false /*&& -90 >= mPosX && mPosX > -140*/)
		{
			inputSpaceFlag = true;
			inputLimitTime = limitTime;			//入力制限時間をリセット
			mPosX = mPosition.x;				//押された時のプレイヤーの座標を補完

			if (-90 >= mPosX && mPosX > -120)
			{
				mEvlt = BAD;					//入力が早かったらBAD評価
				turnGraphHandle = LoadGraph("data/img/Game/Turn1.png");

			}
			if (-120 >= mPosX && mPosX > -130)
			{
				mEvlt = NORMAL;					//入力が少し早かったらNORMALE評価
				turnGraphHandle = LoadGraph("data/img/Game/Turn2.png");

			}
			if (-130 >= mPosX && mPosX > -140)
			{
				mEvlt = GOOD;					//入力がちょうどだったらGOOD評価
				turnGraphHandle = LoadGraph("data/img/Game/Turn3.png");

			}
		}
		//押されないまま端まで来たとき
		else if (turnFlag == false && mPosition.x <= -140)
		{
			mEvlt = BAD;						//BAD評価に
			turnGraphHandle = LoadGraph("data/img/Game/Turn1.png");
			turnFlag = true;
			inputLimitTime = limitTime;			//入力制限時間をリセット
		}


		//プールの端まで来たら
		if (mPosition.x <= -136)
		{
			turnFlag = true;
			mRotation = VGet(250.0f, 270.0f * DX_PI_F / 180.0f, 0.0f);							//プレイヤーの向きを反転
			turnGraphFlag = true;
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

		//------------------------YAMAOKA--------------------------↓
		LastSpurt();	// ラストスパート

		PlayAnim(_deltaTime);						// アニメーション情報を取得
		MV1SetPosition(modelHandle, mPosition);		// ポジション更新
		//------------------------YAMAOKA--------------------------↑
	}
}

//スタート処理
void Yamaoka_PlayerActor::StartProcess(float _deltaTime)
{
	//キー入力取得
	int Key = GetJoypadInputState(DX_INPUT_KEY_PAD1);

	mNowPlayerState = STATE_SWIM;	// プレイヤーを泳ぎ状態に

	attachAnim(0);					// アニメーション000をアタッチ

	startFlag = true;               // スタートフラグを true に	
}

//描画
void Yamaoka_PlayerActor::DrawActor()
{
	// 1.3f 〜 1.5f
	SetDraw3DScale(1.4f);
	// 3Dモデルの描画
	MV1DrawModel(modelHandle);

	// 3Dモデルの回転角度 (どの方向を向いているか)
	MV1SetRotationXYZ(modelHandle, mRotation);
}

//------------------------YAMAOKA--------------------------↓
//アニメーション
void Yamaoka_PlayerActor::PlayAnim(float _deltaTime)
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
	int color;							//	スタミナゲージの中身の色
	// 色設定
	if (_st >= GREEN)
	{
		color = GetColor(0, 255, 0);		//	中身(緑)
	}
	if (_st < GREEN && _st >= ORANGE)
	{
		color = GetColor(255, 177, 27);	//	中身（オレンジ）
	}
	if (_st < ORANGE)
	{
		color = GetColor(255, 0, 0);		//	中身（赤）
	}
	int frame_color = GetColor(0, 0, 0);	//	枠 (黒)

	// スタミナゲージが 0 になったら
	if (_st <= _MinSt)
	{
		// スタミナを最小値に
		_st = _MinSt;
	}

	// ゲージの枠表示
	DrawBox(ST_FIRST_X, ST_FIRST_Y, ST_END_X, ST_END_Y, frame_color, FALSE);
	// ゲージの中身表示
	DrawBox(ST_FIRST_X, ST_FIRST_Y, _st + ST_FIRST_X, ST_END_Y, color, TRUE);
	// 数値表示 
	DrawFormatString(ST_FIRST_X, ST_FIRST_Y, GetColor(0, 0, 0), "%d / 600", _st);
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

// ラストスパート
void Yamaoka_PlayerActor::LastSpurt()
{
	// 残り15m以下になったら
	if (dCount <= dCountUlt)
	{
		//DrawFormatString(850, 700, GetColor(255, 0, 0), "Last Spurt");

		if (!m_ultCheckFlag)
		{
			UltNumber(randomFlag);
		}
		UltProcessInput(arrow, 4);
		m_ultCheckFlag = true;
		ultFlag = true;
		randomFlag = false;
		skillFlag = true;
		//mVelosity.x = 60.0f;

		stopTime++;

		// 3秒間停止
		if (stopTime % 180 == 0)
		{		
			skillCount = 1;
		}
		// 停止時間が終わったら
		if (skillCount == 1 && !finishFlag)
		{
			skillFlag = false;
			stopTime = 0;
			mVelosity = VGet(inputCount * 2, 0, 0);

			// 再開時のズレた分を引く（足す）
			countUP -= 2;
		}
		// ゴールしたら
		if (GetPosX() >= 130 && GetTurnFlag() == true)
		{
			finishFlag = true;
			skillCount = 0;
		}

	}

	// 数値表示
	//DrawFormatString(850, 700, GetColor(255, 0, 0), "skillTime   %d", skillTime);
	//DrawFormatString(0, 30, GetColor(255, 0, 255), "skillCount   %d", skillCount);
	//DrawFormatString(0, 100, GetColor(255, 0, 255), "stopTime    %d", stopTime);
}
//------------------------YAMAOKA--------------------------↑

void Yamaoka_PlayerActor::UltNumber(bool _randomFlag)
{
	if (!_randomFlag)
	{
		for (int i = 0; i < 4; i++)
		{
			randomKeyNumber = rand() % 3 + 1;				//1〜4までの数字をランダムに生成
			arrow[i] = randomKeyNumber;
		}
	}
}

void Yamaoka_PlayerActor::UltProcessInput(int _arrow[], int _size)
{
	int Key = GetJoypadInputState(DX_INPUT_KEY_PAD1);
	count = 0;

	// 技を使用していないとき
	if (skillCount == 0)
	{
		for (int i = 0; i < _size; i++)
		{
			switch (_arrow[i])
			{
			case STATE_KEY_UP:
				if (CheckHitKey(KEY_INPUT_UP) && mCheckKeyFlag)
				{
					inputCount += 1;
					m_ultFinishFlag++;
					mCheckKeyFlag = false;
				}
				else if (CheckHitKey(KEY_INPUT_DOWN) && mCheckKeyFlag || CheckHitKey(KEY_INPUT_RIGHT) && mCheckKeyFlag
					|| CheckHitKey(KEY_INPUT_LEFT) && mCheckKeyFlag)
				{
					m_ultCheckFlag++;
					mCheckKeyFlag = false;
				}
				break;
			case STATE_KEY_DOWN:
				if (CheckHitKey(KEY_INPUT_DOWN) && mCheckKeyFlag)
				{
					inputCount += 1;
					m_ultFinishFlag++;
					mCheckKeyFlag = false;
				}
				else if (CheckHitKey(KEY_INPUT_UP) && mCheckKeyFlag || CheckHitKey(KEY_INPUT_RIGHT) && mCheckKeyFlag
					|| CheckHitKey(KEY_INPUT_LEFT) && mCheckKeyFlag)
				{
					m_ultCheckFlag++;
					mCheckKeyFlag = false;
				}
				break;
			case STATE_KEY_RIGHT:
				if (CheckHitKey(KEY_INPUT_RIGHT) && mCheckKeyFlag)
				{
					inputCount += 1;
					m_ultFinishFlag++;
					mCheckKeyFlag = false;
				}
				else if (CheckHitKey(KEY_INPUT_UP) && mCheckKeyFlag || CheckHitKey(KEY_INPUT_DOWN) && mCheckKeyFlag
					|| CheckHitKey(KEY_INPUT_LEFT) && mCheckKeyFlag)
				{
					m_ultCheckFlag++;
					mCheckKeyFlag = false;
				}
				break;
			case STATE_KEY_LEFT:
				if (CheckHitKey(KEY_INPUT_LEFT) && mCheckKeyFlag)
				{
					inputCount += 1;
					m_ultFinishFlag++;
					mCheckKeyFlag = false;
				}
				else if (CheckHitKey(KEY_INPUT_UP) && mCheckKeyFlag || CheckHitKey(KEY_INPUT_DOWN) && mCheckKeyFlag
					|| CheckHitKey(KEY_INPUT_RIGHT) && mCheckKeyFlag)
				{
					m_ultCheckFlag++;
					mCheckKeyFlag = false;
				}
				break;
			}

			//mVelosity = VGet(inputCount * 2, 0, 0);
			mCheckKeyFlag = true;
		}
	}
}
