#pragma once
#include "SceneBase.h"
#include "Nagatomo_PlayerActor.h"
#include "Nagatomo_Camera.h"

class Nagatomo_PlayerActor;			//PlayerActorへの参照
class Nagatomo_Camera;				//Cameraへの参照

class Nagatomo_GameScene : public SceneBase
{
	Nagatomo_PlayerActor* player;
	Nagatomo_Camera* camera;

public:

	enum class GAME_SCENE_STATE
	{
		FADE_IN,
		GAME,
		FADE_OUT
	};

	Nagatomo_GameScene();				//	コンストラクタ
	~Nagatomo_GameScene();			    //	デストラクタ

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

	class Nagatomo_Stage* m_stage;   // ステージクラスへのポインタメンバ変数
	class Nagatomo_Camera* m_camera; // カメラクラスへのポインタメンバ変数
	class Nagatomo_PlayerActor* m_actor;   // アクタークラスへのポインタメンバ変数

	int m_startTime;                // ゲーム開始時間
	int m_tmpTime;                  // 現在時間を一時保存
	int m_countUP;                  // 経過時間
};