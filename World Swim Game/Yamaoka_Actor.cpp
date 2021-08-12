#include "Yamaoka_Actor.h"

int Yamaoka_Actor::modelHandle = NULL;

//コンストラクタ
Yamaoka_Actor::Yamaoka_Actor()
{
	// ３Dモデル読み込み
	modelHandle = MV1LoadModel("data/player/player.pmx");

	mSpeed = 0.0f;

	// ゴールまでの距離 //
	dCount = 1000;         // 進んだ距離
	maxdCount = 1000;      // ゴール  
	NowPos = 0;            // 現在の座標
	// 調整中          //

	// スタミナゲージ //
	st = 1200;      // スタミナ初期値
	MaxSt = 1200;   // スタミナ最大値
	MinSt = 700;    // スタミナ最小値

	// アニメーション生成時間初期化
	animNowTime = 0;
	animTotal = 0;
	animIndex = 0;

	mPosition = VGet(-150, 18, 0);     // 初期位置設定
	mVelocity = VGet(0, 0, 0);         // 初期速度
	mRotation = VGet(250.0f, -90.0f * DX_PI_F / 180.0f, 0.0f);   // 回転角度
}

//デストラクタ
Yamaoka_Actor::~Yamaoka_Actor()
{
	MV1DeleteModel(modelHandle);
}

// Update関数　Gameからコールされる
void Yamaoka_Actor::Update(float _deltaTime)
{
	UpdateActor(_deltaTime);
}

//アクターの更新
void Yamaoka_Actor::UpdateActor(float _deltaTime)
{
	// キー入力取得
	int Key = GetJoypadInputState(DX_INPUT_KEY_PAD1);

	// 加速処理.
	VECTOR accelVec = VGet(0, 0, 0);

	// ベロシティ加速計算
	mVelocity = VAdd(mVelocity, accelVec);

	attachAnim(0);
	if (CheckHitKey(KEY_INPUT_RIGHT))
	{
		mPosition.x += mSpeed;
		mSpeed += 0.05;

		dCount -= 1;
		st--;   // スタミナを減らす
		// 速度が最大値を超えたら
		if (mSpeed >= 1.1f)
		{
			mSpeed = 1.1f;
		}
		// 奥まで行くと
		//if (dCount <= 741)
		if(NowPos >= 271)
		{
			mSpeed = 0;
			dCount += 1;
			st++;
		}
		// スタミナが0になると
		if (st <= MinSt)
		{
			mSpeed = 0;
		}

		mPosition = VAdd(mPosition, mVelocity);

		// 奥を向く
		mRotation = VGet(250.0f, -90.0f * DX_PI_F / 180.0f, 0.0f);
	}
	if (CheckHitKey(KEY_INPUT_LEFT))
	{
		mPosition.x -= mSpeed;
		mSpeed += 0.05;

		dCount -= 1;
		st--;   // スタミナを減らす
		// 速度が最大値を超えたら
		if (mSpeed >= 1.1f)
		{
			mSpeed = 1.1f;
		}	
		// スタミナが0になると
		if (st <= MinSt)
		{
			mSpeed = 0;
		}
		// スタート位置に戻ったら
		if (NowPos <= 20)
		{
			mSpeed = 0;
		}

		mPosition = VAdd(mPosition, mVelocity);

		// 手前を向く
		mRotation = VGet(250.0f, 90.0f * DX_PI_F / 180.0f, 0.0f);
	}
	// アニメーション情報を取得
	PlayAnim();

	// 残り距離
	if (dCount <= 500)
	{
		dCount = 500;
	}

	// ポジションを更新
	mPosition = VAdd(mPosition, mVelocity);
	NowPos = (int)mPosition.x + 150;
	MV1SetPosition(modelHandle, mPosition);
}

//描画
void Yamaoka_Actor::DrawActor()
{
	// 3Dモデルの描画
	MV1DrawModel(modelHandle);

	// 3Dモデルの回転角度 (どの方向を向いているか)
	MV1SetRotationXYZ(modelHandle, mRotation);
	
	// スタミナゲージの描画
	DrawSt(st, MaxSt, MinSt);

	// ゴールまでの距離
	DrawToGoal(dCount, maxdCount);

	// デバッグ用
	//DrawFormatString(1100, 850, GetColor(0, 0, 0), "dCount  %d", dCount);
}

//アニメーション再生関数
void Yamaoka_Actor::PlayAnim()
{
	// 再生時間を進める
	animNowTime += 0.5f;
	// 再生時間がアニメーションの総再生時間を超えたら
	if (animNowTime >= animTotal)
	{
		// 再生時間を０に
		animNowTime = 0;
	}

	// アニメーションの再生時間をセットする
	MV1SetAttachAnimTime(modelHandle, animIndex, animNowTime);
}

//再生するアニメーションの番号を取得する関数
void Yamaoka_Actor::attachAnim(int _animPlay)
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

// スタミナゲージの描画
void Yamaoka_Actor::DrawSt(int _st, int _MaxSt, int _MinSt)
{
	// 色設定
	int color = GetColor(0, 255, 0);  // 中身(緑)
	int color2 = GetColor(0, 0, 0);   // 枠 (黒)

	// スタミナゲージが 0 になったら
	if (st <= MinSt)
	{
		// スタミナを最小値に
		st = MinSt;
	}

	// ゲージの枠表示
	DrawBox(MinSt, 1000, 1200, 1035, color2, FALSE);
	// ゲージの中身表示
	DrawBox(MinSt, 1000, 1200 * _st / _MaxSt, 1035, color, TRUE);
	// 数値表示 
	DrawFormatString(MinSt, 1000, GetColor(0, 0, 0), "%d / 500", 1200 * _st / _MaxSt - MinSt);
}

// ゴールまでの距離
void Yamaoka_Actor::DrawToGoal(int _playerPos, int _goalPos)
{
	// デバッグ用
	//DrawFormatString(1300, 500, GetColor(0, 0, 0), "NowPos  %d", NowPos);
	//DrawFormatString(1300, 550, GetColor(0, 0, 0), "  %d", 600 * _playerPos / _goalPos);

	// 残りの距離の表示
	DrawBox(1590, 895, 1850, 945, GetColor(0, 255, 255), TRUE);
	DrawFormatString(1600, 900, GetColor(0, 0, 0), "残り  %d m", 1000 * _playerPos/ _goalPos - 500);

	// 一往復したら
	if (_playerPos <= 500)
	{
		_playerPos = 500;    // 値を固定
		DrawFormatString(900, 950, GetColor(255, 0, 0), "GOAL");
	}

}
