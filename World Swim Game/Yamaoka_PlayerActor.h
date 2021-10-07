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

	//アップデート関数
	void Update(float _deltaTime);
	//アクターのアップデート関数
	void UpdateActor(float _deltaTime) override;
	//スタート処理の関数
	void StartProcess(float _deltaTime);
	//アクターの描画関数
	void DrawActor();

	//------------------------YAMAOKA--------------------------↓
	/// <summary>
	/// アニメーション関数
	/// アニメーションの再生時間の処理をする関数
	/// </summary> 
	/// <param name="_deltaTime"> デルタタイム　</param>
	void PlayAnim(float _deltaTime);

	/// <summary>
	/// アニメーション関数
	/// 何番目のアニメーションを再生するかを選択する関数
	/// </summary>
	/// <param name="_animPlay">　再生するアニメーションの番号　</param>
	void attachAnim(int _animPlay);

	/// <summary>
	/// スタミナ描画
	/// </summary>
	/// <param name="_st">　現在のスタミナの値　</param>
	/// <param name="_MaxSt">　スタミナの最大値　</param>
	/// <param name="_MinSt">　スタミナの最小値　</param>
	void DrawSt(int _st, int _MaxSt, int _MinSt);

	/// <summary>
	/// ゴールまでの距離の描画
	/// </summary>
	/// <param name="_playerPos">　プレイヤーの現在の座標の値　</param>
	/// <param name="_goalPos">　ゴールの座標　</param>
	void DrawToGoal(float _playerPos, float _goalPos);

	/// <summary>
	/// ラストスパートの処理
	/// </summary>
	void LastSpurt();
	//------------------------YAMAOKA--------------------------↑

	void UltNumber(bool _randomFlag);
	//void ProcessInput(int _randomKeyNumber);
	void UltProcessInput(int _arrow[], int _size);

	//turnFlagのゲッター
	bool GetTurnFlag() { return turnFlag; };

	//inputSpaceFlagのゲッター
	bool GetInputSpaceFlag() { return inputSpaceFlag; };

	//inputArrowFlagのゲッター
	bool GetInputArrpwFlag() { return inputArrowFlag; };

	//リターンの評価するためのプレイヤーの座標を補完
	float mPosX;

	//------------------------YAMAOKA--------------------------↓
	// ゴールまでの距離 //
	float dCount;      // どれだけ進んだか        
	float maxdCount;   // どこまで進めるのか
	int NowPos;    // プレイヤーの現在座標

	//スタミナ関連
	int st;						// スタミナ
	int MaxSt;					// スタミナ最大値
	int MinSt;					// スタミナ最小値
	//------------------------YAMAOKA--------------------------↑

	int halfSt;					//スタミナ（50%）
	int quarterSt;				//スタミナ（25%）
	
	//------------------------YAMAOKA--------------------------↓
	int startTime;   // ゲーム開始時間
	int tmpTime;     // 現在時間を一時保存
	int countUP;     // 経過時間

	int count;       // 次のシーンに行くまでのカウント    
	int countDown;		// スタートまでのカウントダウン
	int skillCount;  // スキルの使用カウント
	int skillTime;   // スキルの効果時間

	int stopTime;    // 停止時間
	//------------------------YAMAOKA--------------------------↑

	float inputTime;			//入力時間
	float inputLimitTime;		//入力制限時間

	int arrow[4];				//ラストスパートのキー

	int turnGraphHandle;		//	グラフィックハンドル
	bool turnGraphFlag;			//	表示するか

	int randomKeyNumber;
	bool randomFlag;
	bool inputArrowFlag;			//矢印キー入力フラグ

	bool countDownFinishFlag;		//	カウントダウンが終わったか
	bool ultLimitFlag;				//　ウルトの制限
	bool ultFlag;

	//------------------------YAMAOKA--------------------------↓
	bool turnFlag;					// ターンフラグ
	//------------------------YAMAOKA--------------------------↑
private:
	int inputStartTime;
	int inputEndTime;
	int inputCount;					//入力回数

	int countSpeed;

	float addStaminaSpeed;			//加算されるスタミナによるスピード

	VECTOR mPrevPosition;

	PLAYER_STATE_ENUM mNowPlayerState;	//プレイヤーの今の状態

	KEY_STATE_ENUM mNowKeyState;		//キーの今の状態
	KEY_STATE_ENUM mPrevKeyState;		//キーの1つ前の状態

	Evaluation	   mEvlt;				//評価

	bool inputSpaceFlag;				//SPACE入力フラグ
	bool mCheckKeyFlag;					//長押し対策フラグ
	bool m_ultCheckFlag;					//	ラストスパートの矢印を表示し終わったかどうか
	int m_ultFinishFlag;					//	ラストスパートで矢印を押し終わったか

	//------------------------YAMAOKA--------------------------↓
	bool skillFlag;					  // スキルを使用したかどうか
	bool startFlag;
	bool finishFlag;					// ゴールしたかどうか
	//------------------------YAMAOKA--------------------------↑
};