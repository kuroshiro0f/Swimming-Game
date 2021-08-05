#include "Yamaoka_Actor.h"

int Yamaoka_Actor::modelHandle = NULL;

//コンストラクタ
Yamaoka_Actor::Yamaoka_Actor()
{
	// ３Dモデル読み込み
	modelHandle = MV1LoadModel("data/player/player.pmx");

	mSpeed = 0.0f;

	// ゴールまでの距離 //
	dCount = 0;       // 進んだ距離
	maxdCount = 360;  // ゴール  
	//                 //

	// スタミナゲージ //
	endStBar = 1150;  // スタミナゲージの長さ(1150 - 780)
	startStBar = 780; // スタミナゲージの始点（左端）
	maxSt = 500;      // スタミナ最大値の設定
	nowSt = 0;        // 現在のスタミナ
	dSt = 0;          // スタミナの減少値

	// アニメーション生成時間初期化
	animNowTime = 0;
	animTotal = 0;
	animIndex = 0;

	mPosition = VGet(0, 18, -150);     // 初期位置設定
	mVelocity = VGet(0, 0, 0);         // 初期速度
	mRotation = VGet(250.0f, 180.0f * DX_PI_F / 180.0f, 0.0f);   // 回転角度

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
	if (CheckHitKey(KEY_INPUT_UP))
	{
		mPosition.z += mSpeed;
		mSpeed += 0.1;
		dSt += 0.25;    // スタミナを減らす
		dCount += 1;
		// 速度が最大値を超えたら
		if (mSpeed >= 1.5f)
		{
			mSpeed = 1.5f;
		}
		// 奥まで行くと
		if (dCount >= 185)
		{
			mSpeed = 0;   // スピードを０に
			dCount -= 1;  // 距離のカウントを相殺
		}
		// 奥を向く
		mRotation = VGet(250.0f, 180.0f * DX_PI_F / 180.0f, 0.0f);
	}
	if (CheckHitKey(KEY_INPUT_DOWN))
	{
		mPosition.z -= mSpeed;
		mSpeed += 0.1;
		dSt += 0.25;  // スタミナを減らす
		dCount += 1;
		// 速度が最大値を超えたら
		if (mSpeed >= 1.5f)
		{
			mSpeed = 1.5f;
		}
		// 初期位置に戻ってきたら
		if (dCount >= 360)
		{
			mSpeed = 0;    // スピードを０に
			dCount = 360;  // 距離のカウントを固定
		}
		// 手前を向く
		mRotation = VGet(250.0f, 0.0f * DX_PI_F / 180.0f, 0.0f);
	}
	// アニメーション情報を取得
	PlayAnim();

	// 現在のスタミナを計算
	nowSt = maxSt - dSt;
	// スタミナゲージの長さ     (nowSt / maxSt で比率計算)
	endStBar = nowSt / maxSt * endStBar;
	// ボタンを押している間だけスタミナ減少
	if (endStBar >= 780)
	{
		dSt = 0;
	}

	// ポジションを更新
	mPosition = VAdd(mPosition, mVelocity);
	MV1SetPosition(modelHandle, mPosition);
}

//描画
void Yamaoka_Actor::DrawActor()
{
	// 3Dモデルの描画
	MV1DrawModel(modelHandle);

	// 3Dモデルの回転角度 (どの方向を向いているか)
	MV1SetRotationXYZ(modelHandle, mRotation);

	// 残りの距離の表示
	DrawFormatString(1500, 850, GetColor(0, 0, 0), "残り  %d m", maxdCount - dCount);
	// スタミナゲージが０以上の時
	if (endStBar >= 780)
	{
		// スタミナゲージを描画
		DrawBox(startStBar, 1000, endStBar, 1035, GetColor(0, 255, 0), TRUE);
		DrawFormatString(780, 1000, GetColor(0, 0, 0)
			,"%d / 370", (int)endStBar - (int)startStBar);
	}
	else
	{
		endStBar = 0;
	}
	// 一往復したら
	if (dCount >= 360)
	{
		DrawFormatString(900, 700, GetColor(255, 0, 0), "GOAL");
	}

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
