#pragma once

#include "SceneBase.h"

class Result : public SceneBase
{
public:

	enum class RESULT_SCENE_STATE
	{
		FADE_IN,
		RESULT,
		FADE_OUT
	};

	Result();			//	コンストラクタ
	~Result();			//　デストラクタ

	SceneBase* Update(float _deltaTime)override;	//	更新
	void Draw()override;			//	描画
	void Sound()override;			//	音楽
	void Load()override;			//	初期化

private:
	RESULT_SCENE_STATE m_state;

	//	アルファ値
	int m_alphaVal;

	int m_backGraphHandle;				//	背景のグラフィックハンドル
	int m_bgmSoundHandle;				//	BGMのサウンドハンドル
	bool m_checkKeyFlag;				//	キーが押されたままかを判定するフラグ
	bool m_fadeOutFinishFlag;			//	フェードアウトの終了判定フラグ
};