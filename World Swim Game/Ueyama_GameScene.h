#pragma once

#include "SceneBase.h"
#include "Ueyama_PlayerActor.h"
#include "Ueyama_Camera.h"

class Ueyama_PlayerActor;			//PlayerActorへの参照
class Ueyama_Camera;				//Cameraへの参照

class Ueyama_GameScene :public SceneBase
{
	Ueyama_PlayerActor* player;
	Ueyama_Camera* camera;

public:

	enum class GAME_SCENE_STATE
	{
		FADE_IN,
		GAME,
		FADE_OUT
	};

	Ueyama_GameScene();				//	コンストラクタ
	~Ueyama_GameScene();			//	デストラクタ

	SceneBase* Update(float _deltaTime)override;	//	更新
	void Draw()override;			//	描画
	void Sound()override;			//	音楽
	void Load()override;			//	初期化

private:
	GAME_SCENE_STATE m_state;
	int m_backGraphHandle;			//	背景のグラフィックハンドル
	int m_bgmSoundHandle;			//	ゲーム画面・サウンドハンドル

	//	アルファ値
	int m_alphaVal;

	bool m_checkKeyFlag;			//	キーが押されたままかを判定するフラグ
	bool m_fadeOutFinishFlag;		//	フェードアウトの終了判定フラグ

	class Stage* m_stage;   // ステージクラスへのポインタメンバ変数
	class Ueyama_Camera* m_camera;	// カメラクラスへのポインタメンバ変数
	class Ueyama_PlayerActor* m_actor;   // アクタークラスへのポインタメンバ変数
};