#include "Yamaoka_PlayerActor.h"

const float D_COUNT = 0.355f;
//const float D_COUNT = 0.27f;

//	スタミナゲージの色が変わる残りゲージ量
const int GREEN = 950;
const int ORANGE = 800;
//const int RED = 3;


//コンストラクタ
Yamaoka_PlayerActor::Yamaoka_PlayerActor()
	: mNowPlayerState(STATE_IDLE)
	, mNowKeyState(STATE_KEY_IDLE)
	, mPrevKeyState(STATE_KEY_IDLE)
	, mEvlt(NONE)
	, startTime(0)
	, tmpTime(0)
	, countUP(0)
	, mPosX(0)
{
	// フラグをfalseに
	startFlag = false;
	turnFlag = false;

	mPosition = VGet(150, 18, 0);								// 初期位置設定
	mRotation = VGet(250.0f, 90.0f * DX_PI_F / 180.0f, 0.0f);	// 回転角度
	mDirection = VGet(0, 0, 1);
	mVelosity = VGet(250, 0, 0);

	// モデルのロード
	// テストモデル
	//modelHandle = MV1LoadModel("data/player/player.pmx");

	// 自作モデル
	modelHandle = MV1LoadModel("data/swimmer/player.pmx");

	// アニメーション時間、再生番号の初期化
	animTotal = 0.0f;
	animNowTime = 0.0f;
	animIndex = 0;

	// ゴールまでの距離 
	//dCount = 25.0f;         // 進むことのできる距離
	//maxdCount = 25.0f;      // ゴール

	dCount = 50.0f;         // 進むことのできる距離
	maxdCount = 50.0f;      // ゴール

	// スタミナゲージ //
	st = 1250;      // スタミナ初期値
	MaxSt = 1250;   // スタミナ最大値
	MinSt = 650;    // スタミナ最小値

	count = 30;      // 次のシーンに行くまでのカウント

	countDown = 120; // カウントダウン（ 3秒 ）

	skillCount = 0;  // スキルの使用カウント

	framcount = 30;  // 加速スキルのカウント
}

//デストラクタ
Yamaoka_PlayerActor::~Yamaoka_PlayerActor()
{
	// モデルのアンロード
	MV1DeleteModel(modelHandle);
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

		//泳ぎ処理
		if (Key & PAD_INPUT_RIGHT && mPosition.x <= 150 && mPosition.x >= -138)
		{
			mPrevKeyState = mNowKeyState;					//今のキー状態を前回のキー状態に
			mNowKeyState = STATE_KEY_RIGHT;					//今のキー状態をSTATE_KEY_RIGHTに

			if (mNowKeyState != mPrevKeyState)				//今と前回のキー状態が違うとき
			{
				mPosition.x -= mVelosity.x * _deltaTime;	//mPositionにmVelosityを加算
				st -= 2.25;									// スタミナを減らす
				dCount -= D_COUNT;							// 残り距離を減らす
			}
		}

		if (Key & PAD_INPUT_LEFT && mPosition.x <= 150 && mPosition.x >= -138)
		{
			mPrevKeyState = mNowKeyState;					//今のキー状態を前回のキー状態に
			mNowKeyState = STATE_KEY_LEFT;					//今のキー状態をSTATE_KEY_LEFTに

			if (mNowKeyState != mPrevKeyState)				//今と前回のキー状態が違うとき
			{
				mPosition.x -= mVelosity.x * _deltaTime;	//mPositionにmVelosityを加算
				st -= 2.25;									// スタミナを減らす
				dCount -= D_COUNT;							// 残り距離を減らす
			}

		}
		//引き戻し処理
		if (mPosition.x >= 150)
		{
			mPosition.x = 145;
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

		// スペースが押されたとき
		if (Key & PAD_INPUT_M)
		{

			mPosX = mPosition.x;				// 押された時のプレイヤーの座標を補完

			if (-90 >= mPosX && mPosX > -120)
			{
				mEvlt = BAD;					// 入力が早かったらBAD評価

			}
			if (-120 >= mPosX && mPosX > -130)
			{
				mEvlt = NORMAL;					// 入力が少し早かったらNORMALE評価

			}
			if (-130 >= mPosX && mPosX > -140)
			{
				mEvlt = GOOD;					// 入力がちょうどだったらGOOD評価

			}
		}

		// プールの端まで来たら
		if (mPosition.x <= -136)
		{
			// プレイヤーの向きを反転
			mRotation = VGet(250.0f, 270.0f * DX_PI_F / 180.0f, 0.0f);
			turnFlag = true;

			// スタミナの調整用
			if (st <= 0)
			{
				st = 0;
			}

			switch (mEvlt)
			{
			case BAD:
				mVelosity = VGet(-50, 0, 0);
				//st -= 15;

			case NORMAL:
				mVelosity = VGet(-80, 0, 0);
				//st -= 10;

			case GOOD:
				mVelosity = VGet(-100, 0, 0);
				//st -= 5;
			default:
				break;
			}			//速度を反転
		}

		PlayAnim(_deltaTime);						// アニメーション情報を取得
		MV1SetPosition(modelHandle, mPosition);		// ポジション更新
	}
}

// スタート処理
void Yamaoka_PlayerActor::StartProcess(float _deltaTime)
{
	//キー入力取得
	int Key = GetJoypadInputState(DX_INPUT_KEY_PAD1);

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

}

// アニメーション再生時間の設定
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

// 再生するアニメーション番号を取得
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
	DrawBox(_MinSt, 1000, _MaxSt, 1035, frame_color, FALSE);
	// ゲージの中身表示
	DrawBox(_MinSt, 1000, _st, 1035, color, TRUE);
	// 数値表示 
	DrawFormatString(_MinSt, 1000, GetColor(0, 0, 0), "%d / %d",
		                                            _st - _MinSt, _MaxSt - _MinSt);
}

// ゴールまでの距離
void Yamaoka_PlayerActor::DrawToGoal(float _playerPos, float _goalPos)
{
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

	// デバッグ用
	//DrawFormatString(1300, 500, GetColor(0, 0, 0), "NowPos  %d", count);
	//DrawFormatString(1300, 550, GetColor(0, 0, 0), "  %d", 600 * _playerPos / _goalPos);
}

// 必殺技
// 回復技
void Yamaoka_PlayerActor::healSkill(float _playerPos, float _goalPos)
{
	SetFontSize(40);
	// 必殺技のアイコン（枠）を表示
	DrawBox(850, 100, 950, 200, GetColor(0, 0, 0), FALSE);

	// turnFlag = true でスキルカウントが 0 のとき
	if (turnFlag && skillCount == 0)
	{
		DrawFormatString(680, 60, GetColor(255, 0, 0), "ひっさつわざ つかえる");
		// 必殺技のアイコン（枠）を塗りつぶす
		DrawBox(850, 100, 950, 200, GetColor(255, 0, 0), TRUE);
		DrawFormatString(800, 200, GetColor(0, 255, 255), "PUSH S KEY");
	}
	else
	{
		DrawFormatString(680, 60, GetColor(255, 0, 0), "ひっさつわざ つかえない");
	}

	// プレイヤーがターンしたら
	if (turnFlag && (int)(_goalPos * _playerPos / _goalPos) <= 25)
	{
		// Sキー を押すと必殺技を使う(スタミナを回復)
		if (CheckHitKey(KEY_INPUT_S))
		{
			mPrevKeyState = mNowKeyState;		// 今のキー状態を前回のキー状態に
			mNowKeyState = STATE_KEY_SPACE;		// 今のキー状態をSTATE_KEY_SPACEに

			// 必殺技の処理
			// 今と前回のキー状態が違うとき
			if (mNowKeyState != mPrevKeyState && skillCount <= 0)
			{
				// スタミナを回復する
				st += 30;
				// スキルカウントを 1 に
				skillCount = 1;
				// turnFlag を false に
				turnFlag = false;
			}
		}
	}
	// 数値表示
	//DrawFormatString(850, 800, GetColor(255, 0, 255), "st   %d", st);
	//DrawFormatString(850, 850, GetColor(255, 0, 255), "skill  %d", skillCount);
}

// 加速技
void Yamaoka_PlayerActor::speedUpSkill(float _deltaTime)
{
	// Aキー　を押したら発動
	if (CheckHitKey(KEY_INPUT_A) && /*dCount <= 25 &&*/ framcount >= 0)
	{
		// 技の発動時間のカウント
		framcount--;
		// プレイヤーの速度を倍にする
		mPosition.x -= mVelosity.x * _deltaTime * 2.0f;
		// 残り距離を減らす
		//dCount -= D_COUNT * 2;

		// カウントが０以下になったら
		if (framcount <= 0)
		{
			// 値を -1 で固定する
			framcount = -1;
		}
	}
	//DrawFormatString(850, 850, GetColor(255, 0, 255), "mvelosity.x  %d", playerSpeed.x);
}

