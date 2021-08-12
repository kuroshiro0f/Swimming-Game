//#pragma once
//#include "SceneBase.h"
//
//class Ueyama_Title : public SceneBase
//{
//public:
//
//	enum class TITLE_SCENE_STATE
//	{
//		FADE_IN,
//		TITLE,
//		MANUAL,
//		FADE_OUT
//	};
//
//	Ueyama_Title();						//	コンストラクタ
//	~Ueyama_Title();					//	デストラクタ
//
//	SceneBase* Update(float _deltaTime) override;
//	void Draw() override;
//	void Sound() override;
//	void Load() override;
//
//private:
//	TITLE_SCENE_STATE m_state;
//
//	//	モデルハンドル
//	int m_swimModelHandle;				//	泳ぐキャラ
//	int m_crawlModelHandle;				//	クロールのモーション
//
//	//	グラフィックハンドル
//	int m_backGraphHandle;				//	背景
//	int m_logoGraphHandle;				//	ロゴ
//	int m_startGuideGraphHandle;		//	スタートの案内
//	int m_manualGuideGraphHandle;		//	マニュアルへの案内
//
//	//	サウンドハンドル
//	int m_backSoundHandle;				//	BGM
//	int m_click_sound_handle;			//  ボタンを押したときの音
//
//	//	アルファ値
//	int m_alphaVal;
//
//	bool m_checkKeyFlag;			//	キーが押されたままかを判定するフラグ
//	bool m_fadeOutFinishFlag;		//	フェードアウトの終了判定フラグ
//};