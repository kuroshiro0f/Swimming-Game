#pragma once
#include "Actor.h"
#include "Font.h"
#include "Ueyama_GameScene.h"

//-----------------------------------------------------------------------------
// 　変更なし
//-----------------------------------------------------------------------------

class Ueyama_PlayerActor : public Actor
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

	Ueyama_PlayerActor();
	~Ueyama_PlayerActor();

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


	// ラストスパート
	void LastSpurt();
	void UltNumber(bool _randomFlag);
	//void ProcessInput(int _randomKeyNumber);
	void UltProcessInput(int _arrow[], int _size);

	//turnFlagのゲッター
	bool GetTurnFlag() { return turnFlag; };

	//inputSpaceFlagのゲッター
	bool GetInputSpaceFlag() { return inputSpaceFlag; };

	//inputArrowFlagのゲッター
	bool GetInputArrpwFlag() { return inputArrowFlag; };

	void UltArrowMotion(int _ult1, int _ult2, int _ult3, int _ult4);

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
	int ultSucFlag[4];

	bool arrowFailFlag;				//	矢印の入力に成功したか

	int arrowAngle[4];				//	矢印の角度

	bool turnFlag;						 // ターンフラグ

private:
	int inputStartTime;
	int inputEndTime;
	int inputCount;						//入力回数

	int countSpeed;

	float addStaminaSpeed;				//加算されるスタミナによるスピード

	VECTOR mPrevPosition;

	PLAYER_STATE_ENUM mNowPlayerState;	//プレイヤーの今の状態

	KEY_STATE_ENUM mNowKeyState;		//キーの今の状態
	KEY_STATE_ENUM mPrevKeyState;		//キーの1つ前の状態

	Evaluation	   mEvlt;				//評価

	int m_stGraphHandle;				//	スタミナのグラフィックハンドル
	int m_failGraphHandle;				//	失敗

	bool startFlag;
	bool inputSpaceFlag;				//SPACE入力フラグ
	bool mCheckKeyFlag;					//長押し対策フラグ
	bool skillFlag;					  // スキルを使用したかどうか
	bool finishFlag;					// ゴールしたかどうか
	bool m_ultCheckFlag;					//	ラストスパートの矢印を表示し終わったかどうか
	int m_ultFinishFlag;					//	ラストスパートで矢印を押し終わったか

	class Font* m_font;					//	フォントクラスへのポインタメンバ変数
	class Ueyama_GameScene* m_game;		//	ゲームシーンへのポインタメンバ変数
};