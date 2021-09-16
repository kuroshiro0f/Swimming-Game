#pragma once
#include "SceneBase.h"
#include "Yamaoka_PlayerActor.h"
#include "Yamaoka_Camera.h"

class Yamaoka_PlayerActor;			//PlayerActorへの参照
class Camera;				//Cameraへの参照

class Yamaoka_GameScene :public SceneBase
{
	Yamaoka_PlayerActor* player;
	Camera* camera;

public:
	//int m_countUP;                  // 経過時間

	enum class GAME_SCENE_STATE
	{
		LOAD,
		FADE_IN,
		GAME,
		FADE_OUT
	};

	Yamaoka_GameScene();			//	コンストラクタ
	~Yamaoka_GameScene();			//	デストラクタ

	SceneBase* Update(float _deltaTime)override;	//	更新
	void Draw()override;			//	描画
	void Sound()override;			//	音楽
	void Load()override;			//	初期化

private:
	GAME_SCENE_STATE m_state;

	//	文字の位置
	int m_mojiX;		//	ロード文字のX座標
	int m_mojiY;		//	ロード文字のY座標

	//	男の子の移動
	int m_boyY;

	//	文字の表示時間の操作
	int m_startCount;			//	スタート
	int m_finishCount;			//	ゴール
	bool m_startFinishFlag;

	//	グラフィックハンドル
	int m_backGraphHandle;			//	背景
	int m_loadGraphHandle;			//	ロード
	int m_tips1GraphHandle;			//	TIPS1
	int m_tips2GraphHandle;			//	TIPS2
	int m_tips3GraphHandle;			//	TIPS3
	int m_boyGraphHandle;			//	男の子

	//	サウンドハンドル
	int m_bgmSoundHandle;			//	BGM
	int m_whistleSoundHandle;		//	笛
	int m_countDownSoundHandle;		//	カウントダウン
	int m_goalSoundHandle;			//	ゴール

	//	アルファ値
	int m_alphaVal;

	//	デルタタイム
	float m_deltaTime;

	//	フラグ
	int m_tipsFlag;					//	TIPSの表示変える
	int m_loadingFlag;				//	ローディングの文字の表示を変える
	bool m_checkKeyFlag;			//	キーが押されたままか
	bool m_fadeOutFinishFlag;		//	フェードアウトの終了判定
	bool m_loadFlag;				//	ロード画面を表示するか
	bool m_loadFinishFlag;			//	ロードが終わったか
	bool m_boyPlusFlag;				//	男の子をうごかすフラグ
	bool m_whistleFinishFlag;		//	笛を鳴らし終わったか
	bool m_gameFinishFlag;			//	ゲームが終わったか
	bool m_fadeOutFlag;				//	フェードアウトを始めるか

	class Stage* m_stage;   // ステージクラスへのポインタメンバ変数
	class Camera* m_camera;	// カメラクラスへのポインタメンバ変数
	class Yamaoka_PlayerActor* m_actor;   // アクタークラスへのポインタメンバ変数
};