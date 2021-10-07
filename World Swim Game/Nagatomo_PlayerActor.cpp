#include "Nagatomo_PlayerActor.h"

#include <random>

//コンストラクタ
Nagatomo_PlayerActor::Nagatomo_PlayerActor()
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

	mPosition = VGet(150, 18, 0);								// 初期位置設定
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
	st = 600;			// スタミナ初期値
	MaxSt = 600;			// スタミナ最大値
	MinSt = 0;				// スタミナ最小値
	halfSt = 600 / 2;		//スタミナ（50%）
	quarterSt = 600 / 4;		//スタミナ（25%）

	count = 30;      // 次のシーンに行くまでのカウント

	countDown = 170; // カウントダウン（ 3秒 ）
	inputTime = 0;

	skillCount = 0;  // スキルの使用カウント
	skillTime = 180;   // スキルの効果時間

	//入力関連
	inputTime = 0.0f;
	inputLimitTime = limitTime;

	//スピード関連
	addStaminaSpeed = 0.0f;

	// 停止時間初期化
	inputCount = 0;
}

//デストラクタ
Nagatomo_PlayerActor::~Nagatomo_PlayerActor()
{
	// モデルのアンロード
	MV1DeleteModel(modelHandle);
	DeleteGraph(turnGraphHandle);
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
		countDownFinishFlag = true;
	}

	//	ステートが泳ぎの時
	if (mNowPlayerState == STATE_SWIM)
	{
		//現在時刻を取得
		tmpTime = GetNowCount() / 1000;
		// 経過時間を計算 
		countUP = (tmpTime - startTime);

		mPrevPosition = mPosition;							//プレイヤーのポジションを補完

		AutoMove(turnFlag, _deltaTime);

		//	技が終わるまで位置を固定
		if (skillFlag)
		{
			mPosition.x = mPosition.x;
		}

		dCount -= std::sqrt((mPosition.x - mPrevPosition.x) * (mPosition.x - mPrevPosition.x)) * 0.088;		//残りの距離

		GenRandomKey(randomFlag, ultFlag);

		// 息継ぎキーを押したら
		if (CheckHitKey(KEY_INPUT_C))
		{
			randomKeyNumber = 5;          // randamKeyNumberに 5 (息継ぎキーの番号)を入れる
			inputStartTime = GetNowCount() / 1000;			//ランダムに矢印を生成した時間を取得
			randomFlag = true;
		}

		if (dCount <= dCountUlt)
		{
			ultLimitFlag = true;
		}

		ProcessInput(ultFlag, skillFlag);

		// 息継ぎ処理
		if (randomKeyNumber == STATE_KEY_C)
		{
			inputEndTime = GetNowCount() / 1000;			//現在の時間を取得
			inputTime = (inputEndTime - inputStartTime);	//現在時間とランダムに矢印を生成した時間の差

			// Cキーを押されたとき
			if (Key & PAD_INPUT_C)
			{
				st += 5;			// スタミナを減らす
				inputTime = 0;		// 入力可能時間を初期化
				// スタミナが最大値を超えたら
				if (st >= MaxSt)
				{
					st = MaxSt;
				}

				mVelosity = VGet(2.0, 0, 0);
			}
			// 現在時間とランダムに矢印を生成した時間の差が1秒たったら
			else if (inputTime > 1)
			{
				randomFlag = false;
				inputTime = 0;			// 入力可能時間を初期化
				inputLimitTime = limitTime;							//入力制限時間をリセット
				inputArrowFlag = false;
			}
		}

		StaminaCoefficient(st, halfSt, quarterSt);

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
				mVelosity = VGet(2.0, 0, 0);
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

		TurnProcess(turnFlag, mPosition);

		LastSpurt();	// ラストスパート

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
	// 1.3f ～ 1.5f
	SetDraw3DScale(1.4f);
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
void Nagatomo_PlayerActor::DrawToGoal(float _playerPos, float _goalPos)
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

void Nagatomo_PlayerActor::AutoMove(bool _turnFlag,float _deltaTime)
{
	//ターン前
	if (!_turnFlag)
	{
		mPosition.x -= mVelosity.x * _deltaTime;			//プレイヤーの自動移動
	}
	//ターン後
	else
	{
		mPosition.x += mVelosity.x * _deltaTime;			//プレイヤーの自動移動
	}
}

void Nagatomo_PlayerActor::GenRandomKey(bool _randomFlag, bool _ultFlag)
{
	if (_randomFlag == false && !_ultFlag)
	{
		randomKeyNumber = rand() % 3 + 1;				//1～4までの数字をランダムに生成
		inputStartTime = GetNowCount() / 1000;			//ランダムに矢印を生成した時間を取得
		randomFlag = true;
		mCheckKeyFlag = false;
	}
}

void Nagatomo_PlayerActor::ProcessInput(bool _ultFlag, bool _skillFlag)
{
	int Key = GetJoypadInputState(DX_INPUT_KEY_PAD1);

	if (!_ultFlag && !_skillFlag)
	{
		switch (randomKeyNumber)
		{
		case 1:	//↑キーのとき
			inputEndTime = GetNowCount() / 1000;			//現在の時間を取得
			inputTime = (inputEndTime - inputStartTime);	//入力時間をカウント

			//↑キーを押されたとき
			if (Key & PAD_INPUT_UP && !mCheckKeyFlag)
			{
				SuccessInput(st, MinSt);
			}
			//↓ → ← キーが押されたとき
			else if (!mCheckKeyFlag && Key & PAD_INPUT_DOWN || !mCheckKeyFlag && Key & PAD_INPUT_RIGHT || !mCheckKeyFlag && Key & PAD_INPUT_LEFT)
			{
				FailureInput(st, MinSt);
			}
			InputOverTime(inputArrowFlag, inputTime, inputLimitTime);
			break;

		case 2:		//↓キーのとき
			inputEndTime = GetNowCount() / 1000;					 //現在の時間を取得
			inputTime = (inputEndTime - inputStartTime);			 //入力時間をカウント

			if (Key & PAD_INPUT_DOWN && !mCheckKeyFlag)
			{
				SuccessInput(st, MinSt);
			}
			else if (!mCheckKeyFlag && Key & PAD_INPUT_UP || !mCheckKeyFlag && Key & PAD_INPUT_RIGHT || !mCheckKeyFlag && Key & PAD_INPUT_LEFT)
			{
				FailureInput(st, MinSt);
			}
			InputOverTime(inputArrowFlag, inputTime, inputLimitTime);
			break;

		case 3:		//→キーのとき
			inputEndTime = GetNowCount() / 1000;					//現在の時間を取得
			inputTime = (inputEndTime - inputStartTime);			//入力時間をカウント

			if (Key & PAD_INPUT_RIGHT && !mCheckKeyFlag)
			{
				SuccessInput(st, MinSt);
			}
			else if (!mCheckKeyFlag && Key & PAD_INPUT_UP || !mCheckKeyFlag && Key & PAD_INPUT_DOWN || !mCheckKeyFlag && Key & PAD_INPUT_LEFT)
			{
				FailureInput(st, MinSt);
			}
			InputOverTime(inputArrowFlag, inputTime, inputLimitTime);
			break;

		case 4:		//←キーのとき
			inputEndTime = GetNowCount() / 1000;					//現在の時間を取得
			inputTime = (inputEndTime - inputStartTime);			//入力時間をカウント

			if (Key & PAD_INPUT_LEFT && !mCheckKeyFlag)
			{
				SuccessInput(st, MinSt);
			}
			else if (!mCheckKeyFlag && Key & PAD_INPUT_UP || !mCheckKeyFlag && Key & PAD_INPUT_DOWN || !mCheckKeyFlag && Key & PAD_INPUT_RIGHT)
			{
				FailureInput(st, MinSt);
			}
			InputOverTime(inputArrowFlag,inputTime,inputLimitTime);

			break;
		default:
			break;
		}
	}
}

void Nagatomo_PlayerActor::SuccessInput(int _st, int _MinSt)
{
	if (_st > _MinSt)
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

void Nagatomo_PlayerActor::FailureInput(int _st, int _MinSt)
{
	if (st > MinSt)
	{
		mVelosity.x = missSpeed.x * addStaminaSpeed;
		st -= ST_FAI_DEC;
		inputLimitTime = limitTime;
		inputArrowFlag = false;
		mCheckKeyFlag = true;
	}
}

void Nagatomo_PlayerActor::InputOverTime(bool _inputArrowFlag, float _inputTime, float _inputLimitTime)
{
	//キー入力がされて入力制限時間を過ぎたら
	if (_inputArrowFlag && _inputTime > _inputLimitTime)
	{
		_inputTime = 0;
		_inputArrowFlag = false;
		randomFlag = false;
		if (_inputLimitTime < maxTime)
		{
			_inputLimitTime = maxTime;
		}
		else
		{
			_inputLimitTime -= 0.2f;
		}
	}
	//キー入力がされずに入力制限時間を過ぎたら
	if (!_inputArrowFlag && _inputTime > _inputLimitTime)
	{

		mVelosity.x = missSpeed.x * addStaminaSpeed;
		_inputTime = 0;
		_inputLimitTime = limitTime;
		randomFlag = false;
	}
}

void Nagatomo_PlayerActor::TurnProcess(bool _turnFlag, VECTOR _mPosition)
{
	int Key = GetJoypadInputState(DX_INPUT_KEY_PAD1);

	//スペースが押されたとき
	if (Key & PAD_INPUT_M && _turnFlag == false)
	{
		inputSpaceFlag = true;
		inputLimitTime = limitTime;			//入力制限時間をリセット
		mPosX = _mPosition.x;				//押された時のプレイヤーの座標を補完

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
	else if (_turnFlag == false && _mPosition.x <= -140)
	{
		mEvlt = BAD;						//BAD評価に
		turnGraphHandle = LoadGraph("data/img/Game/Turn1.png");
		turnFlag = true;
		inputLimitTime = limitTime;			//入力制限時間をリセット
	}


	//プールの端まで来たら
	if (_mPosition.x <= -136)
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
}

// 必殺技
void Nagatomo_PlayerActor::LastSpurt()
{
	// 残り15m以下になったら
	if (dCount <= dCountUlt)
	{
		DrawFormatString(850, 700, GetColor(255, 0, 0), "Last Spurt");

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

		// 矢印の入力終了まで停止
		if (m_ultFinishFlag == 4)
		{
			skillCount = 1;
		}
		// 停止時間が終わったら
		if (skillCount == 1 && !finishFlag)
		{
			skillFlag = false;
			stopTime = 0;

			// 再開時のズレた分を引く（足す）
			countUP -= 3;
		}
		// ゴールしたら
		if (GetPosX() >= 130 && GetTurnFlag() == true)
		{
			finishFlag = true;
		}

	}

	// 数値表示
	//DrawFormatString(850, 700, GetColor(255, 0, 0), "skillTime   %d", skillTime);
	//DrawFormatString(0, 30, GetColor(255, 0, 255), "mVelosity.x        %f", mVelosity.x);
	//DrawFormatString(0, 100, GetColor(255, 0, 255), "stopTime    %d", stopTime);
}

void Nagatomo_PlayerActor::UltNumber(bool _randomFlag)
{
	if (_randomFlag)
	{
		for (int i = 0; i < 4; i++)
		{
			randomKeyNumber = rand() % 3 + 1;				//1～4までの数字をランダムに生成
			arrow[i] = randomKeyNumber;
		}
	}
}

void Nagatomo_PlayerActor::UltProcessInput(int _arrow[], int _size)
{
	int Key = GetJoypadInputState(DX_INPUT_KEY_PAD1);
	count = 0;

	for (int i = 0; i < _size; i++)
	{
		switch (_arrow[i])
		{
		case STATE_KEY_UP:
			if (CheckHitKey(KEY_INPUT_UP) && mCheckKeyFlag)
			{
				inputCount += 1;
				mCheckKeyFlag = false;
			}
			else
			{
				break;
			}
			break;
		case STATE_KEY_DOWN:
			if (CheckHitKey(KEY_INPUT_DOWN) && mCheckKeyFlag)
			{
				inputCount += 1;
				mCheckKeyFlag = false;
			}
			else
			{
				break;
			}
			break;
		case STATE_KEY_RIGHT:
			if (CheckHitKey(KEY_INPUT_RIGHT) && mCheckKeyFlag)
			{
				inputCount += 1;
				mCheckKeyFlag = false;
			}
			else
			{
				break;
			}
			break;
		case STATE_KEY_LEFT:
			if (CheckHitKey(KEY_INPUT_LEFT) && mCheckKeyFlag)
			{
				inputCount += 1;
				mCheckKeyFlag = false;
			}
			else
			{
				break;
			}
			break;
		}
		mVelosity = VGet(inputCount * 2, 0, 0);
		mCheckKeyFlag = true;
	}
}

void Nagatomo_PlayerActor::StaminaCoefficient(int _st, int _halfSt, int _quarterSt)
{
	//残りスタミナが50%以上
	if (_st >= _halfSt)
	{
		addStaminaSpeed = 1.0f;
	}
	//残りスタミナが50%以下
	if (_st <= _halfSt && st >= _quarterSt)
	{
		addStaminaSpeed = 0.8f;
	}
	//残りスタミナが25%以下
	if (_st <= _quarterSt)
	{
		addStaminaSpeed = 0.6f;
	}
}
