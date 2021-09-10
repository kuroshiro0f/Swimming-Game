#pragma once

#include "Nagatomo_Actor.h"

class Nagatomo_PlayerActor : public Nagatomo_Actor
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
		STATE_KEY_IDLE  = 0,
		STATE_KEY_UP    = 1,
		STATE_KEY_DOWN  = 2,
		STATE_KEY_RIGHT = 3,
		STATE_KEY_LEFT  = 4,

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

	Nagatomo_PlayerActor();
	~Nagatomo_PlayerActor();

	void Update(float _deltaTime);
	void UpdateActor(float _deltaTime) override;
	void StartProcess(float _deltaTime);
	void DrawActor();
	void PlayAnim(float _deltaTime);
	void attachAnim(int _animPlay);

	// スタミナゲージの描画
	void DrawSt(int _st, int _MaxSt, int _MinSt);
	// ゴールまでの距離の描画
	void DrawToGoal(int _dCount);

	//ランダムキー
	void RandomKey();

	//turnFlagのゲッター
	bool GetTurnFlag() { return turnFlag; };

	//inputSpaceFlagのゲッター
	bool GetInputSpaceFlag() { return inputSpaceFlag; };

	//inputArrowFlagのゲッター
	bool GetInputArrpwFlag() { return inputArrowFlag; };

	//リターンの評価するためのプレイヤーの座標を補完
	float mPosX;

	// ゴールまでの距離 //
	float dCount;      // どれだけ進んだか        
	int maxdCount;   // どこまで進めるのか
	int NowPos;		 // プレイヤーの現在座標

	int st;      // スタミナ
	int MaxSt;   // スタミナ最大値
	int MinSt;   // スタミナ最小値

	int startTime;   // ゲーム開始時間
	int tmpTime;     // 現在時間を一時保存
	int countUP;     // 経過時間

	int count;       // 次のシーンに行くまでのカウント    
	int countDown;

	int inputTime;

	int randomKeyNumber;
	bool randomFlag;
	bool inputArrowFlag;				//矢印キー入力フラグ

private:
	int inputStartTime;
	int inputEndTime;

	VECTOR mPrevPosition;

	PLAYER_STATE_ENUM mNowPlayerState;	//プレイヤーの今の状態

	KEY_STATE_ENUM mNowKeyState;		//キーの今の状態
	KEY_STATE_ENUM mPrevKeyState;		//キーの1つ前の状態
	//KEY_STATE_ENUM mRandomKeyState;	//ランダムキーの状態

	Evaluation	   mEvlt;				//評価

	bool startFlag;
	bool turnFlag;						//ターンフラグ
	bool inputSpaceFlag;				//SPACE入力フラグ
};