#include "Nagatomo_Result.h"
#include "Nagatomo_Title.h"

#include "DxLib.h"

const int SCREEN_SIZE_W = 1920;
const int SCREEN_SIZE_H = 1080;

const int addAlphaVal = 5;

Nagatomo_Result::Nagatomo_Result()
	: m_alphaVal(255)
	, m_fadeOutFinishFlag(false)
{
	if (CheckHitKey(KEY_INPUT_RETURN))
	{
		m_checkKeyFlag = TRUE;
	}

	m_state = RESULT_SCENE_STATE::FADE_IN;
}

Nagatomo_Result::~Nagatomo_Result()
{
	DeleteGraph(m_backGraphHandle);
	DeleteSoundMem(m_bgmSoundHandle);
}

SceneBase* Nagatomo_Result::Update(float _deltaTime)
{
	switch (m_state)
	{
	case Nagatomo_Result::RESULT_SCENE_STATE::FADE_IN:
		break;
	case Nagatomo_Result::RESULT_SCENE_STATE::RESULT:
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

			m_state = RESULT_SCENE_STATE::FADE_OUT;
		}

		break;
	case Nagatomo_Result::RESULT_SCENE_STATE::FADE_OUT:
		if (m_fadeOutFinishFlag)
		{
			return new Nagatomo_Title();
		}

		break;
	default:
		break;
	}
	return this;
}

void Nagatomo_Result::Draw()
{
	// 背景
	DrawGraph(0, 0, m_backGraphHandle, TRUE);

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

void Nagatomo_Result::Sound()
{
	PlaySoundMem(m_bgmSoundHandle, DX_PLAYTYPE_BACK, FALSE);
	ChangeVolumeSoundMem(m_volumePal, m_bgmSoundHandle);
}

void Nagatomo_Result::Load()
{
	//	グラフィックハンドルにセット
	m_backGraphHandle = LoadGraph("data/img/resultBackTest.png");			//	背景

	//	サウンドハンドルにセット
	m_bgmSoundHandle = LoadSoundMem("data/sound/resultBgmTest.mp3");			//	BGM
}
