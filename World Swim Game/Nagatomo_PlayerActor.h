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

	Nagatomo_PlayerActor();
	~Nagatomo_PlayerActor();

	//アップデート関数
	void Update(float _deltaTime);
	//アクターのアップデート関数
	void UpdateActor(float _deltaTime) override;
	//スタート処理の関数
	void StartProcess(float _deltaTime);
	//アクターの描画関数
	void DrawActor();
	//アニメーション関数
	void PlayAnim(float _deltaTime);
	//アニメーションのアタッチ関数
	void attachAnim(int _animPlay);

	// スタミナゲージの描画
	void DrawSt(int _st, int _MaxSt, int _MinSt);
	// ゴールまでの距離の描画
	void DrawToGoal(float _playerPos, float _goalPos);
	//ランダムな数字を生成
	void GenRandomKey(bool _randomFlag, bool _ultFlag);
	//自動移動
	void AutoMove(bool _turnFlag ,float _deltaTime);
	// ラストスパート
	void LastSpurt();

	//入力処理
	void ProcessInput(bool _ultFlag,bool _skillFlag);
	//入力成功時の処理
	void SuccessInput(int _st, int _MinSt);
	//入力失敗時の処理
	void FailureInput(int _st, int _MinSt);
	//入力時間を超えたときの処理
	void InputOverTime(bool _inputArrowFlag, float _inputTime, float _inputLimitTime);

	//ターン処理
	void TurnProcess(bool _turnFlag, VECTOR _mPosition);

	//ランダムな数字を生成（ult用）
	void UltNumber(bool _randomFlag);
	//ウルトの入力処理
	void UltProcessInput(int _arrow[], int _size);

	//残りスタミナによる速度
	void StaminaCoefficient(int _st, int _halfSt, int _quarterSt);

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
	float maxdCount;   // どこまで進めるのか
	int NowPos;    // プレイヤーの現在座標

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
	int countDown;		// スタートまでのカウントダウン
	int skillCount;  // スキルの使用カウント
	int skillTime;   // スキルの効果時間

	int stopTime;    // 停止時間

	float inputTime;			//入力時間
	float inputLimitTime;		//入力制限時間

	int arrow[4];					//ラストスパートのキー

	int turnGraphHandle;				//	グラフィックハンドル
	bool turnGraphFlag;					//	表示するか

	int randomKeyNumber;
	bool randomFlag;
	bool inputArrowFlag;				//矢印キー入力フラグ

	bool countDownFinishFlag;		//	カウントダウンが終わったか
	bool ultLimitFlag;				//　ウルトの制限
	bool ultFlag;


	bool turnFlag;						 // ターンフラグ

private:
	int inputStartTime;
	int inputEndTime;
	int inputCount;						//入力回数

	float addStaminaSpeed;				//加算されるスタミナによるスピード

	VECTOR mPrevPosition;

	PLAYER_STATE_ENUM mNowPlayerState;	//プレイヤーの今の状態

	KEY_STATE_ENUM mNowKeyState;		//キーの今の状態
	KEY_STATE_ENUM mPrevKeyState;		//キーの1つ前の状態

	Evaluation	   mEvlt;				//評価

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
	const float maxSpeed = 15.0f;				//最大スピード

	//入力関連
	const float limitTime = 1.5f;				//入力制限時間
	const float maxTime = 0.5f;					//最大時間

	const int dCountUlt = 15;					//ウルトが使えるようになる残り距離

	bool startFlag;
	bool inputSpaceFlag;				//SPACE入力フラグ
	bool mCheckKeyFlag;					//長押し対策フラグ
	bool skillFlag;					  // スキルを使用したかどうか
	bool finishFlag;					// ゴールしたかどうか
	bool m_ultCheckFlag;					//	ラストスパートの矢印を表示し終わったかどうか
	int m_ultFinishFlag;					//	ラストスパートで矢印を押し終わったか
};