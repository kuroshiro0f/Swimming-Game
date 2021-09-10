#pragma once
#include "SceneBase.h"
#include "Yamaoka_PlayerActor.h"
#include "Yamaoka_Camera.h"

class PlayerActor;			//PlayerActorへの参照
class Camera;				//Cameraへの参照

class GameScene :public SceneBase
{
	PlayerActor* player;
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

	GameScene();			//	コンストラクタ
	~GameScene();			//	デストラクタ

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

	//	グラフィックハンドル
	int m_backGraphHandle;			//	背景
	int m_loadGraphHandle;			//	ロード
	int m_tips1GraphHandle;			//	TIPS1
	int m_tips2GraphHandle;			//	TIPS2
	int m_tips3GraphHandle;			//	TIPS3
	int m_boyGraphHandle;			//	男の子

	//	サウンドハンドル
	int m_bgmSoundHandle;			//	BGM

	//	アルファ値
	int m_alphaVal;

	//	フラグ
	int m_tipsFlag;					//	TIPSの表示変える
	int m_loadingFlag;				//	ローディングの文字の表示を変える
	bool m_checkKeyFlag;			//	キーが押されたままか
	bool m_fadeOutFinishFlag;		//	フェードアウトの終了判定
	bool m_loadFlag;				//	ロード画面を表示するか
	bool m_loadFinishFlag;			//	ロードが終わったか
	bool m_boyPlusFlag;				//	男の子をうごかすフラグ

	class Stage* m_stage;   // ステージクラスへのポインタメンバ変数
	class Camera* m_camera;	// カメラクラスへのポインタメンバ変数
	class Yamaoka_PlayerActor* m_actor;   // アクタークラスへのポインタメンバ変数
};