#pragma once
#include "SceneBase.h"

class Yamaoka_GameScene : public SceneBase
{
public:

	enum class GAME_SCENE_STATE
	{
		FADE_IN,
		GAME,
		FADE_OUT
	};

	Yamaoka_GameScene();				//	コンストラクタ
	~Yamaoka_GameScene();			    //	デストラクタ

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

	class Yamaoka_Stage* m_stage;   // ステージクラスへのポインタメンバ変数
	class Yamaoka_Camera* m_camera; // カメラクラスへのポインタメンバ変数
	class Yamaoka_Actor* m_actor;   // アクタークラスへのポインタメンバ変数
	
	int m_startTime;                // ゲーム開始時間
	int m_tmpTime;                  // 現在時間を一時保存
	int m_countUP;                  // 経過時間
};