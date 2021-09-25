#pragma once

#include "Actor.h"

class Yamaoka_PlayerActor : public Actor
{
public:

	typedef enum PLAYER_STATE_ENUM
	{
		STATE_IDLE,
		STATE_SWIM,

		STATE_NUM,
	}PLAYER_STATE_ENUM;

	typedef enum KEY_STATE_ENUM
	{
		STATE_KEY_IDLE = 0,
		STATE_KEY_UP = 1,
		STATE_KEY_DOWN = 2,
		STATE_KEY_RIGHT = 3,
		STATE_KEY_LEFT = 4,
		STATE_KEY_C = 5,
		STATE_KEY_S,
		STATE_KEY_A,
		STATE_KEY_SPACE,

		STATE_KEY_ENUM,
	}KEY_STATE_ENUM;

	//評価
	typedef enum Evaluation
	{
		NONE,
		BAD,
		NORMAL,
		GOOD
	}Evaluation;

	Yamaoka_PlayerActor();
	~Yamaoka_PlayerActor();

	// アップデート関数
	void Update(float _deltaTime);
	// アクターのアップデート関数
	void UpdateActor(float _deltaTime) override;
	// スタート処理の関数
	void StartProcess(float _deltaTime);
	// アクターの描画関数
	void DrawActor();
	// アニメーション関数
	void PlayAnim(float _deltaTime);
	// アニメーションのアタッチ関数
	void attachAnim(int _animPlay);

	// スタミナゲージの描画
	void DrawSt(int _st, int _MaxSt, int _MinSt);
	// ゴールまでの距離の描画
	void DrawToGoal(float _playerPos, float _goalPos);

	//turnFlagのゲッター
	bool GetTurnFlag() { return turnFlag; };

	// ラストスパート
	void LastSpurt();

	// inputSpaceFlagのゲッター
	bool GetInputSpaceFlag() { return inputSpaceFlag; };

	// inputArrowFlagのゲッター
	bool GetInputArrpwFlag() { return inputArrowFlag; };

	// リターンの評価するためのプレイヤーの座標を補完
	float mPosX;

	// ゴールまでの距離 //
	float dCount;      // どれだけ進んだか
	float maxdCount;   // どこまで進めるのか
	int NowPos;        // プレイヤーの現在座標

	//スタミナ関連
	int st;						// スタミナ
	int MaxSt;					// スタミナ最大値
	int MinSt;					// スタミナ最小値
	int halfSt;					//スタミナ（50%）
	int quarterSt;				//スタミナ（25%）

	int startTime;   // ゲーム開始時間
	int tmpTime;     // 現在時間を一時保存
	int countUP;     // 経過時間

	int count;       // 次のシーンに行くまでのカウント
	int countDown;   // スタートまでのカウントダウン
	int skillCount;  // スキルの使用カウント

	int stopTime;    // 停止時間

	float inputTime;			//入力時間
	float inputLimitTime;		//入力制限時間

	int turnGraphHandle;				//	グラフィックハンドル
	bool turnGraphFlag;					//	表示するか

	int randomKeyNumber;
	bool randomFlag;
	bool inputArrowFlag;			//矢印キー入力フラグ

	bool countDownFinishFlag;		//	カウントダウンが終わったか

private:
	int inputStartTime;
	int inputEndTime;

	float addStaminaSpeed;				//加算されるスタミナによるスピード

	VECTOR mPrevPosition;

	PLAYER_STATE_ENUM mNowPlayerState;	//プレイヤーの今の状態

	KEY_STATE_ENUM mNowKeyState;		//キーの今の状態
	KEY_STATE_ENUM mPrevKeyState;		//キーの1つ前の状態

	Evaluation	   mEvlt;				//評価

	bool startFlag;
	bool turnFlag;						// ターンフラグ
	bool inputSpaceFlag;				// SPACE入力フラグ
	bool mCheckKeyFlag;					// 長押し対策フラグ
	bool skillFlag;						// スキルを使用したかどうか
	bool stFlag;						// スタミナがスキル使用中に切れたかどうか

	bool finishFlag;					// ゴールしたかどうか
};