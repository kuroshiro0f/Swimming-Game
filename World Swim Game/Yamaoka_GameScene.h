#pragma once
#include "SceneBase.h"
#include "Yamaoka_PlayerActor.h"
#include "Yamaoka_Camera.h"

class Yamaoka_PlayerActor;			//PlayerActorへの参照
class Camera;				//Cameraへの参照

class GameScene :public SceneBase
{
	Yamaoka_PlayerActor* player;
	Camera* camera;

public:
	//int m_countUP;                  // 経過時間

	enum class GAME_SCENE_STATE
	{
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
	int m_backGraphHandle;			//	背景のグラフィックハンドル
	int m_bgmSoundHandle;				//	ゲーム画面・サウンドハンドル

	//	アルファ値
	int m_alphaVal;

	bool m_checkKeyFlag;			//	キーが押されたままかを判定するフラグ
	bool m_fadeOutFinishFlag;		//	フェードアウトの終了判定フラグ

	class Stage* m_stage;   // ステージクラスへのポインタメンバ変数
	class Camera* m_camera;	// カメラクラスへのポインタメンバ変数
	class Yamaoka_PlayerActor* m_actor;   // アクタークラスへのポインタメンバ変数
};