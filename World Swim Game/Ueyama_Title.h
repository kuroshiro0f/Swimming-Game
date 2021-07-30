#pragma once
#include "Ueyama_SceneBase.h"

class Ueyama_Title : public Ueyama_SceneBase
{
public:

	enum class TITLE_SCENE_STATE
	{
		FADE_IN,
		TITLE,
		MANUAL,
		FADE_OUT
	};

	Ueyama_Title();					//	コンストラクタ
	~Ueyama_Title();					//	デストラクタ

	Ueyama_SceneBase* Update(float _deltaTime) override;
	void Draw() override;
	void Sound() override;
	void Load() override;

private:
	TITLE_SCENE_STATE m_state;

	int m_backGraphHandle;				//	タイトル画面・背景のグラフィックハンドル
	int m_soundHandle;					//	タイトル画面・サウンドハンドル
	int m_click_sound_handle;			//  ENTERで進むときのサウンドハンドル
										
	int m_permeationAmount;				// 毎フレーム透過量

	//	アルファ値
	int m_alphaVal;

	// 透過量
	int m_transpVal;
	int m_fadeTransVal;				//	フェードアウト中

	bool m_checkKeyFlag;			//	キーが押されたままかを判定するフラグ
	bool m_fadeOutFinishFlag;		//	フェードアウトの終了判定フラグ
};