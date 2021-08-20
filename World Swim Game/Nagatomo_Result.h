#pragma once
#include "SceneBase.h"
#include "Nagatomo_GameScene.h"

class GameScene;

#pragma once

#include "SceneBase.h"

class Result : public SceneBase
{
public:

	enum class RESULT_SCENE_STATE
	{
		FADE_IN,
		RESULT,
		TIME,
		MEDAL,
		ENTER,
		FADE_OUT
	};

	Result(const int _time);			//	コンストラクタ
	~Result();			//　デストラクタ

	SceneBase* Update(float _deltaTime)override;	//	更新
	void Draw()override;			//	描画
	void Sound()override;			//	音楽
	void Load()override;			//	初期化

private:
	RESULT_SCENE_STATE m_state;

	//	アルファ値
	int m_alphaVal;

	//	グラフィックハンドル
	int m_time;							//　タイム
	int m_backGraphHandle;				//	背景
	int m_logoGraphHandle;				//	ロゴ
	int m_evaluationGraphHandle;		//	評価
	int m_guidanceGraphHandle;			//	次のシーンへの案内
	int m_medalGraphHandle;				//	メダル

	//	サウンドハンドル
	int m_bgmSoundHandle;				//	BGM

	//	フラグ
	bool m_checkKeyFlag;				//	キーが押されたままか
	bool m_fadeOutFinishFlag;			//	フェードアウト終了
};