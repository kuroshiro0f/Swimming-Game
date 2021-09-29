#pragma once

#include "SceneBase.h"
#include "Font.h"

//-----------------------------------------------------------------------------
// 　変更なし
//-----------------------------------------------------------------------------

class Ueyama_Result : public SceneBase
{
public:

	enum class RESULT_SCENE_STATE
	{
		FADE_IN,
		RESULT,
		TIME,
		MEDAL,
		EVA,
		ENTER,
		FADE_OUT
	};

	typedef struct {
		int TIME_FIRST;			//	一位のタイム
		int TIME_SECOND;		//	二位のタイム
		int TIME_THIRD;			//	三位のタイム
	}ranking;

	Ueyama_Result(const int _time);			//	コンストラクタ
	~Ueyama_Result();			//　デストラクタ

	SceneBase* Update(float _deltaTime)override;	//	更新
	void Draw()override;			//	描画
	void Sound()override;			//	音楽
	void Load()override;			//	初期化

	void UpdateTransparent();			//	文字の透過量更新

private:
	RESULT_SCENE_STATE m_state;

	//	アルファ値
	int m_alphaVal;

	//	クリアタイム
	int m_time;

	//	時間管理
	int m_resultTime;

	//	星の拡大縮小の管理
	int m_bigStarX1;
	int m_bigStarX2;
	int m_bigStarY1;
	int m_bigStarY2;
	int m_smallStarX1;
	int m_smallStarX2;
	int m_smallStarY1;
	int m_smallStarY2;

	// 文字の透過
	int m_transpVal;				//	透過量
	int m_fadeTransVal;				//	フェードアウト中の透過量
	int m_permeationAmount;			//	毎フレーム透過量

	//	グラフィックハンドル
	int m_backGraphHandle;				//	背景
	int m_logoGraphHandle;				//	ロゴ
	int m_evaluationGraphHandle;		//	評価
	int m_guidanceGraphHandle;			//	次のシーンへの案内
	int m_medalGraphHandle;				//	メダル
	int m_bigStarGraphHandle;			//	大きな星
	int m_smallStarGraphHandle;			//	小さな星
	int m_nowGraphHandle;				//	今のタイム
	int m_firstGraphHandle;				//	一位のタイム
	int m_secondGraphHandle;			//	二位のタイム
	int m_thirdGraphHandle;				//	三位のタイム

	//	サウンドハンドル
	int m_bgmSoundHandle;				//	BGM
	int m_se1SoundHandle;				//	効果音
	int m_se2SoundHandle;				//	効果音
	int m_se3SoundHandle;				//	効果音
	int m_se4SoundHandle;				//	効果音

	//	フラグ
	int m_resultFlag;					//	リザルトの推移を管理する
	bool m_starScallFlag;				//	星を拡大するか（しない場合は縮小）
	bool m_checkKeyFlag;				//	キーが押されたままか
	bool m_fadeOutFinishFlag;			//	フェードアウト終了

	class Save* m_save;
	class Font* m_font;
};