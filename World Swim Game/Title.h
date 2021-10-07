#pragma once
#include "SceneBase.h"
#include "Volume.h"

class Title : public SceneBase
{
public:

enum class TITLE_SCENE_STATE
	{
		FIRST,
		SECOND,
		FADE_IN,
		TITLE,
		MANUAL,
		MANUAL2,
		MANUAL3,
		FADE_OUT
	};

	Title(bool _roundFlag);					//	コンストラクタ
	~Title();					//	デストラクタ

	SceneBase* Update(float _deltaTime) override;
	void Draw() override;
	void Sound() override;
	void Load() override;

	void UpdateTransparent();			//	文字の透過量更新
	void DlphinMotion();				//	イルカの動き

private:
	TITLE_SCENE_STATE m_state;

	//	グラフィックハンドル
	int m_backGraphHandle;				//	背景
	int m_logoGraphHandle;				//	ロゴ
	int m_startGuideGraphHandle;		//	スタートの案内
	int m_manualGuideGraphHandle;		//	マニュアルへの案内
	int m_arrowGraphHandle;				//	矢印
	int m_blueDolphinGraphHandle;		//	青のイルカ
	int m_yellowDolphinGraphHandle;		//	黄色のイルカ
	int m_gateGraphHandle;				//	GATE
	int m_maouGraphHandle;				//	魔王魂
	int m_manualGraphHandle;			//	マニュアル
	int m_manual2GraphHandle;			//	マニュアル2ページ目
	int m_manual3GraphHandle;			//	マニュアル3ページ目

	//	サウンドハンドル
	int m_backSoundHandle;				//	BGM
	int m_entClickSoundHandle;			//  ENTキーを押したときの音
	int m_spaClickSoundHandle;			//	SPACEキーを押したときの音

	//	アルファ値
	int m_alphaVal;
	int m_addAlphaVal;
	int m_addAlphaVal2;

	////	水滴の動き
	//double m_bigDropAngle;				//	大きな水滴
	//double m_smallDropAngle;			//	小さな水滴

	// 文字の透過
	int m_transpVal;				//	透過量
	int m_fadeTransVal;				//	フェードアウト中の透過量
	int m_permeationAmount;			//	毎フレーム透過量

	//	GATEと魔王魂の透過
	int m_transpVal2;				//	透過量
	int m_displayCount;				//	表示時間
	bool m_transpDownFlag;			//	フェードインかフェードアウトか
	bool m_titleFlag;				//	タイトル画面に行くか

	//	モード選択
	int m_cursolNum;				//	カーソル番号
	int m_arrowY;					//	矢印のY座標

	//	イルカの移動
	int m_blueY;					//	青イルカ		
	int m_yellowY;					//	黄イルカ
	bool m_blueUpFlag;				//	青イルカが上がるか
	bool m_yellowUpFlag;			//	黄イルカが上がるか

	//	デルタタイム
	//float m_deltaTime;

	//	フラグ
	bool m_checkKeyFlag;			//	キーが押されたままかを判定するフラグ
	bool m_fadeOutFinishFlag;		//	フェードアウトの終了判定フラグ

	class Volume* m_volume;
};