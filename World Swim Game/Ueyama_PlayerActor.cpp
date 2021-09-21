#include "Ueyama_PlayerActor.h"

#include <random>

//-----------------------------------------------------------------------------
// 　変更なし
//-----------------------------------------------------------------------------
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

//スピード関連
const VECTOR missSpeed = VGet(5, 0, 0);		//入力ミスしたときのスピード
const float addSpeed = 3.0f;				//加算されるスピード
//入力関連
const float limitTime = 1.5f;				//入力制限時間

//コンストラクタ
Ueyama_PlayerActor::Ueyama_PlayerActor()
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
{
	startFlag = false;
	turnFlag = false;
	randomFlag = false;
	inputSpaceFlag = false;
	inputArrowFlag = false;
	mCheckKeyFlag = false;

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
	// 調整中          //

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
}

//デストラクタ
Ueyama_PlayerActor::~Ueyama_PlayerActor()
{
	// モデルのアンロード
	MV1DeleteModel(modelHandle);
	DeleteGraph(turnGraphHandle);
}

//更新処理
void Ueyama_PlayerActor::Update(float _deltaTime)
{
	UpdateActor(_deltaTime);
}

//アクターの更新処理
void Ueyama_PlayerActor::UpdateActor(float _deltaTime)
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

		dCount -= std::sqrt((mPosition.x - mPrevPosition.x) * (mPosition.x - mPrevPosition.x)) * 0.088;

		if (randomFlag == false)
		{
			randomKeyNumber = rand() % 3 + 1;				//1～4までの数字をランダムに生成
			inputStartTime = GetNowCount() / 1000;			//ランダムに矢印を生成した時間を取得
			randomFlag = true;
			mCheckKeyFlag = false;
		}

		// 息継ぎキーを押したら
		if (CheckHitKey(KEY_INPUT_C))
		{
			randomKeyNumber = 5;          // randamKeyNumberに 5 (息継ぎキーの番号)を入れる
			inputStartTime = GetNowCount() / 1000;			//ランダムに矢印を生成した時間を取得
			randomFlag = true;
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
				if (st > MinSt)
				{
					mVelosity.x += addSpeed * addStaminaSpeed;
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
				inputLimitTime -= 0.2f;
				inputArrowFlag = false;
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
				if (st > MinSt)
				{
					mVelosity.x += addSpeed * addStaminaSpeed;
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
				inputLimitTime -= 0.2f;
				inputArrowFlag = false;
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
				if (st > MinSt)
				{
					mVelosity.x += addSpeed * addStaminaSpeed;
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
				inputLimitTime -= 0.2f;
				inputArrowFlag = false;
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
				if (st > MinSt)
				{
					mVelosity.x += addSpeed * addStaminaSpeed;
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
				inputLimitTime -= 0.2f;
				inputArrowFlag = false;
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
		default:
			break;
		}

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

		//スペースが押されたとき
		if (Key & PAD_INPUT_M && turnFlag == false)
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

		PlayAnim(_deltaTime);						// アニメーション情報を取得
		MV1SetPosition(modelHandle, mPosition);		// ポジション更新
	}
}

//スタート処理
void Ueyama_PlayerActor::StartProcess(float _deltaTime)
{
	//キー入力取得
	int Key = GetJoypadInputState(DX_INPUT_KEY_PAD1);

	mNowPlayerState = STATE_SWIM;	// プレイヤーを泳ぎ状態に

	attachAnim(0);					// アニメーション000をアタッチ

	startFlag = true;               // スタートフラグを true に	
}

//描画
void Ueyama_PlayerActor::DrawActor()
{
	// 1.3f ～ 1.5f
	SetDraw3DScale(1.4f);
	// 3Dモデルの描画
	MV1DrawModel(modelHandle);

	// 3Dモデルの回転角度 (どの方向を向いているか)
	MV1SetRotationXYZ(modelHandle, mRotation);
}

//アニメーション
void Ueyama_PlayerActor::PlayAnim(float _deltaTime)
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

void Ueyama_PlayerActor::attachAnim(int _animPlay)
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
void Ueyama_PlayerActor::DrawSt(int _st, int _MaxSt, int _MinSt)
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
void Ueyama_PlayerActor::DrawToGoal(float _playerPos, float _goalPos)
{
	// デバッグ用
	//DrawFormatString(1300, 500, GetColor(0, 0, 0), "NowPos  %d", count);
	//DrawFormatString(1300, 550, GetColor(0, 0, 0), "  %d", 600 * _playerPos / _goalPos);

	// 残りの距離の表示
	/*DrawBox(1590, 895, 1850, 945, GetColor(255, 255, 0), TRUE);
	DrawFormatString(1600, 900, GetColor(0, 0, 0), "残り  %d m", (int)(_goalPos * _playerPos / _goalPos));*/

	DrawBox(1590, 95, 1850, 145, GetColor(255, 255, 0), TRUE);
	DrawFormatString(1600, 100, GetColor(0, 0, 0), "残り  %d m", (int)(_goalPos * _playerPos / _goalPos));

	// 一往復したら
	if (_playerPos <= 0)
	{
		_playerPos = 0;    // 値を固定
		SetFontSize(100);
		DrawFormatString(900, 450, GetColor(255, 0, 0), "GOAL");
	}
}

//	必殺技
void Ueyama_PlayerActor::Skill(float _playerPos, float _goalPos)
{
	SetFontSize(40);

	// 必殺技のアイコン（枠）を表示
	DrawBox(850, 100, 950, 200, GetColor(0, 0, 0), FALSE);
	DrawFormatString(850, 60, GetColor(255, 0, 0), "加速技");
	DrawFormatString(850, 200, GetColor(255, 0, 0), "PUSH A");


	// A を押すと加速技を使う
	if (CheckHitKey(KEY_INPUT_A) && st > MinSt)
	{
		skillFlag = true;

		// 必殺技のアイコン（枠）を塗りつぶす
		DrawBox(850, 100, 950, 200, GetColor(0, 255, 0), TRUE);

		mPrevKeyState = mNowKeyState;	// 今のキー状態を前回のキー状態に
		mNowKeyState = STATE_KEY_A;		// 今のキー状態をSTATE_KEY_Aに


		// スタミナが半分以上のとき
		if (st - MinSt > MaxSt - MinSt / 2)
		{
			skillTime = 180;
		}
		// スタミナが半分以下のとき
		else
		{
			skillTime = 60;
		}


		// 加速技の処理
		// 今と前回のキー状態が違うとき
		if (mNowKeyState != mPrevKeyState)
		{
			// スピードアップ
			// スタミナを最小値にする
			st = MinSt;
			// スキル効果時間が 0 以上の時
			if (skillTime >= 0)
			{
				mVelosity = VGet(30, 0, 0);
				skillTime--;
			}
		}

		// スキル効果時間が 0 以下になると
		if (skillTime <= 0)
		{
			skillFlag = false;
			skillTime = 180;
		}

	}
	else
	{
		// 現在のキー状態を変更する
		mNowKeyState = STATE_KEY_S;
	}

	// 数値表示
	//DrawFormatString(850, 700, GetColor(255, 0, 0), "skillTime   %d", skillTime);

}
