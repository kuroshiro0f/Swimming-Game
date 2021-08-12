#include "Result.h"
#include "Title.h"

#include "DxLib.h"

//	スクリーンのサイズ
const int SCREEN_SIZE_W = 1920;
const int SCREEN_SIZE_H = 1080;

//	タイム表示する白い四角の場所の座標
const int SQUARE_START_X = 30;
const int SQUARE_START_Y = 370;
const int SQUARE_END_X = 1100;
const int SQUARE_END_Y = 1030;

// タイムを表示する場所
const int TIME_X = (SQUARE_END_X - SQUARE_START_X) / 2 - 40;
const int TIME_Y = (SQUARE_END_Y - SQUARE_START_Y) - 10;

//	フェードイン・フェードアウトの速度
const int addAlphaVal = 5;


Result::Result(const int _time)
	: m_alphaVal(255)
	, m_fadeOutFinishFlag(false)
{
	// ※キー入力重複対策のフラグ
	// ENTERキーが押されている間、次のENTERの入力を無効に
	if (CheckHitKey(KEY_INPUT_RETURN))
	{
		m_checkKeyFlag = TRUE;
	}

	//	フェードインから始める
	m_state = RESULT_SCENE_STATE::FADE_IN;
}

Result::~Result()
{
	//	メモリの解放
	DeleteGraph(m_backGraphHandle);
	DeleteSoundMem(m_bgmSoundHandle);
}

SceneBase* Result::Update(float _deltaTime)
{
	switch (m_state)
	{
	case Result::RESULT_SCENE_STATE::FADE_IN:
		break;
	case Result::RESULT_SCENE_STATE::RESULT:
		// ※キー入力重複対策のフラグ
		// ENTERキーから指を離したら、次のENTERの入力を有効に
		if (!CheckHitKey(KEY_INPUT_RETURN))
		{
			m_checkKeyFlag = false;
		}

		// ENTERで次のステートへ
		if (CheckHitKey(KEY_INPUT_RETURN) && m_checkKeyFlag == false)
		{
			// ※キー入力重複対策のフラグ
			m_checkKeyFlag = true;

			//	ステートをフェードアウトに
			m_state = RESULT_SCENE_STATE::FADE_OUT;
		}

		break;
	case Result::RESULT_SCENE_STATE::FADE_OUT:
		//	フェードアウトが終わったらタイトルへ
		if (m_fadeOutFinishFlag)
		{
			return new Title();
		}

		break;
	default:
		break;
	}
	return this;
}

void Result::Draw()
{
	//	描画
	DrawGraph(0, 0, m_backGraphHandle, TRUE);			//	背景
	DrawGraph(0, 0, m_logoGraphHandle, TRUE);			//	ロゴ
	DrawGraph(0, 0, m_evaluationGraphHandle, TRUE);		//	評価
	DrawGraph(0, 0, m_guidanceGraphHandle, TRUE);		//	案内
	DrawGraph(0, 0, m_medalGraphHandle, TRUE);			//	メダル

	// 透過して描画
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 190);
	DrawBox(SQUARE_START_X, SQUARE_START_Y, SQUARE_END_X, SQUARE_END_Y, GetColor(255, 255, 255), TRUE);
	// 透過を元に戻す
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	//	フェードイン処理
	if (m_state == RESULT_SCENE_STATE::FADE_IN)
	{
		// アルファ値の減算
		m_alphaVal -= addAlphaVal;

		// アルファブレンド有効化(ここでアルファ値をセット)
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, m_alphaVal);

		// 画面全体に任意のカラーの四角形を描画
		DrawBox(0, 0, SCREEN_SIZE_W, SCREEN_SIZE_H, GetColor(0, 0, 0), TRUE);

		// アルファブレンド無効化
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

		// アルファ値が最大(255)になったらフェードアウト終了
		if (m_alphaVal <= 0)
		{
			m_state = RESULT_SCENE_STATE::RESULT;
		}
	}

	// フェードアウト処理
	if (m_state == RESULT_SCENE_STATE::FADE_OUT)
	{
		// アルファ値の加算
		m_alphaVal += addAlphaVal;
		// アルファブレンド有効化(ここでアルファ値をセット)
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, m_alphaVal);

		// 画面全体に任意のカラーの四角形を描画
		DrawBox(0, 0, SCREEN_SIZE_W, SCREEN_SIZE_H, GetColor(0, 0, 0), TRUE);

		// アルファブレンド無効化
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

		// アルファ値が最大(255)になったらフェードアウト終了
		if (m_alphaVal >= 255)
		{
			m_fadeOutFinishFlag = true;
		}
	}
}

void Result::Sound()
{
	PlaySoundMem(m_bgmSoundHandle, DX_PLAYTYPE_BACK, FALSE);
	ChangeVolumeSoundMem(m_volumePal, m_bgmSoundHandle);
}

void Result::Load()
{
	//	グラフィックハンドルにセット
	m_backGraphHandle = LoadGraph("data/img/Result/Result_back.png");			//	背景
	m_logoGraphHandle = LoadGraph("data/img/Result/Result_logo.png");			//	ロゴ
	m_evaluationGraphHandle = LoadGraph("data/img/Result/Result_best.png");		//　評価
	m_guidanceGraphHandle = LoadGraph("data/img/Result/Result_guidance.png");	//	案内
	m_medalGraphHandle = LoadGraph("data/img/Result/Result_gold.png");			//	メダル

	//	サウンドハンドルにセット
	m_bgmSoundHandle = LoadSoundMem("data/sound/Result/Result.ogg");			//	BGM
}
