#pragma once
#include "SceneBase.h"

class Ueyama_Title : public SceneBase
{
public:

	enum class TITLE_SCENE_STATE
	{
		FIRST,
		SECOND,
		FADE_IN,
		TITLE,
		MANUAL,
		FADE_OUT
	};

	Ueyama_Title();						//	コンストラクタ
	~Ueyama_Title();					//	デストラクタ

	SceneBase* Update(float _deltaTime) override;
	void Draw() override;
	void Sound() override;
	void Load() override;

	void UpdateTransparent();			//	文字の透過量更新

private:
	TITLE_SCENE_STATE m_state;

	//	モデルハンドル
	int m_swimModelHandle;				//	泳ぐキャラ
	int m_crawlModelHandle;				//	クロールのモーション

	//	グラフィックハンドル
	int m_backGraphHandle;				//	背景
	int m_logoGraphHandle;				//	ロゴ
	int m_startGuideGraphHandle;		//	スタートの案内
	int m_manualGuideGraphHandle;		//	マニュアルへの案内
	int m_gateGraphHandle;				//	GATE
	int m_maouGraphHandle;				//	魔王魂
	int m_bigDropGraphHandle;			//	大きな水滴
	int m_smallDropGraphHandle;			//	小さな水滴

	//	サウンドハンドル
	int m_backSoundHandle;				//	BGM
	int m_click_sound_handle;			//  ボタンを押したときの音

	//	アルファ値
	int m_alphaVal;
	int m_addAlphaVal;
	int m_addAlphaVal2;

	//	水滴の動き
	double m_bigDropAngle;				//	大きな水滴
	double m_smallDropAngle;			//	小さな水滴

	//	文字の透過
	int m_transpVal;				//	透過量
	int m_fadeTransVal;				//	フェードアウト中の透過量
	int m_permeationAmount;			//	毎フレーム透過量

	bool m_checkKeyFlag;			//	キーが押されたままかを判定するフラグ
	bool m_fadeOutFinishFlag;		//	フェードアウトの終了判定フラグ
};