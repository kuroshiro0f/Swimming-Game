#include "Title.h"
#include "Yamaoka_GameScene.h"
#include "Yamaoka_LoadingScene.h"

#include "DxLib.h"

//	ウインドウのサイズ
const int SCREEN_SIZE_W = 1920;
const int SCREEN_SIZE_H = 1080;

//	最大透過量
const int MAX_TRANSP_VAL = 255;
//	透過量変化用ベクトル
const int TRANSP_MODERATION = -1;
//	最初の透過量
const int FIRST_TRANS_VAL = 100;

//	フェードイン・フェードアウトの速度
const int ADD_ALPHA_VAL = 2;
const int ADD_ALPHA_VAL_2 = 5;

Title::Title()
	: m_state(TITLE_SCENE_STATE::FIRST)
	, m_alphaVal(255)
	, m_fadeOutFinishFlag(false)
	, m_addAlphaVal(ADD_ALPHA_VAL)
	, m_addAlphaVal2(ADD_ALPHA_VAL_2)
{
	if (CheckHitKey(KEY_INPUT_RETURN))
	{
		m_checkKeyFlag = TRUE;
	}

	// 透過量変数を122に設定
	m_transpVal = MAX_TRANSP_VAL;
	m_fadeTransVal = FIRST_TRANS_VAL;
	// 毎透過量変数を2に設定
	m_permeationAmount = 2;

	SetFontSize(m_normalFontSize);
}

Title::~Title()
{
	//	曲を止める
	StopSoundMem(m_backSoundHandle);

	//	メモリの解放
	DeleteGraph(m_backGraphHandle);
	DeleteGraph(m_logoGraphHandle);
	DeleteGraph(m_startGuideGraphHandle);
	DeleteGraph(m_manualGuideGraphHandle);
	DeleteGraph(m_gateGraphHandle);
	DeleteGraph(m_maouGraphHandle);
	DeleteSoundMem(m_backSoundHandle);
}

SceneBase* Title::Update(float _deltaTime)
{
	// ステートメントごとに処理を変更
	switch (m_state)
	{
	case TITLE_SCENE_STATE::FIRST:
		break;
	case TITLE_SCENE_STATE::SECOND:
		break;
	case TITLE_SCENE_STATE::FADE_IN:
		// ※キー入力重複対策のフラグ
		// ENTERキーから指を離したら、次のENTERの入力を有効に
		if (!CheckHitKey(KEY_INPUT_RETURN))
		{
			m_checkKeyFlag = false;
		}

		// ENTERで最後のステートへ
		if (CheckHitKey(KEY_INPUT_RETURN) && m_checkKeyFlag == false)
		{
			// ※キー入力重複対策のフラグ
			m_checkKeyFlag = true;

			m_state = TITLE_SCENE_STATE::FADE_OUT;
		}
		break;
	case TITLE_SCENE_STATE::TITLE:
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

			m_state = TITLE_SCENE_STATE::FADE_OUT;
		}

		break;
	case TITLE_SCENE_STATE::MANUAL:
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

			m_state = TITLE_SCENE_STATE::TITLE;
		}

		break;
	case TITLE_SCENE_STATE::FADE_OUT:
		if (m_fadeOutFinishFlag)
		{
			return new Yamaoka_LoadingScene();
		}
		break;
	default:
		break;
	}
	return this;
}

void Title::Draw()
{
	// 透過量更新
	UpdateTransparent();

	if (m_state != TITLE_SCENE_STATE::FIRST && m_state != TITLE_SCENE_STATE::SECOND)
	{

		// 描画
		DrawGraph(0, 0, m_backGraphHandle, TRUE);			//	背景
		DrawGraph(0, 0, m_logoGraphHandle, TRUE);			//	ロゴ

		// 透過して描画
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, m_transpVal);
		DrawGraph(0, 0, m_startGuideGraphHandle, TRUE);		//	スタートの案内
		// 透過を元に戻す
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

		// 透過して描画
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, m_transpVal);
		DrawGraph(0, 0, m_manualGuideGraphHandle, TRUE);	//	マニュアルへの案内
		// 透過を元に戻す
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	}

	if (m_state == TITLE_SCENE_STATE::MANUAL)
	{
		// 透過して描画
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, 190);
		DrawBox(0, 0, SCREEN_SIZE_W, SCREEN_SIZE_H, GetColor(0, 0, 0), TRUE);
		// 透過を元に戻す
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	}

	if (m_state == TITLE_SCENE_STATE::FIRST)
	{
		//	Titleの場合

		////	描画
		//DrawGraph(0, 0, m_gateGraphHandle, TRUE);			//	GATE

		//// アルファ値の減算
		//m_alphaVal -= m_addAlphaVal;

		//// アルファブレンド有効化(ここでアルファ値をセット)
		//SetDrawBlendMode(DX_BLENDMODE_ALPHA, m_alphaVal);

		//// 画面全体に任意のカラーの四角形を描画
		//DrawBox(0, 0, SCREEN_SIZE_W, SCREEN_SIZE_H, GetColor(0, 0, 0), TRUE);

		//// アルファブレンド無効化
		//SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

		//// アルファ値が最大(255)になったらフェードアウト終了
		//if (m_alphaVal <= 92)
		//{
		//	m_state = TITLE_SCENE_STATE::SECOND;
		//	m_alphaVal = 255;
		//}

		//	Title2の場合

		//	描画
		DrawGraph(0, 0, m_gateGraphHandle, TRUE);			//	GATE

		// アルファ値の減算
		m_alphaVal -= m_addAlphaVal;

		// アルファブレンド有効化(ここでアルファ値をセット)
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, m_alphaVal);

		// 画面全体に任意のカラーの四角形を描画
		DrawBox(0, 0, SCREEN_SIZE_W, SCREEN_SIZE_H, GetColor(0, 0, 0), TRUE);

		// アルファブレンド無効化
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

		// アルファ値が最大(255)になったらフェードアウト終了
		if (m_alphaVal <= 70)
		{
			m_state = TITLE_SCENE_STATE::SECOND;
			m_alphaVal = 255;
		}
	}

	if (m_state == TITLE_SCENE_STATE::SECOND)
	{
		//	Titleの場合

		////	描画
		//DrawGraph(0, 0, m_maouGraphHandle, TRUE);			//	魔王魂

		//// アルファ値の減算
		//m_alphaVal -= m_addAlphaVal;

		//// アルファブレンド有効化(ここでアルファ値をセット)
		//SetDrawBlendMode(DX_BLENDMODE_ALPHA, m_alphaVal);

		//// 画面全体に任意のカラーの四角形を描画
		//DrawBox(0, 0, SCREEN_SIZE_W, SCREEN_SIZE_H, GetColor(0, 0, 0), TRUE);

		//// アルファブレンド無効化
		//SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 60);

		//// アルファ値が最大(255)になったらフェードアウト終了
		//if (m_alphaVal <= 92)
		//{
		//	m_state = TITLE_SCENE_STATE::FADE_IN;
		//	m_alphaVal = 255;
		//}

		//	Title2の場合

		//	描画
		DrawGraph(0, 0, m_maouGraphHandle, TRUE);			//	魔王魂

		// アルファ値の減算
		m_alphaVal -= m_addAlphaVal;

		// アルファブレンド有効化(ここでアルファ値をセット)
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, m_alphaVal);

		// 画面全体に任意のカラーの四角形を描画
		DrawBox(0, 0, SCREEN_SIZE_W, SCREEN_SIZE_H, GetColor(0, 0, 0), TRUE);

		// アルファブレンド無効化
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 60);

		// アルファ値が最大(255)になったらフェードアウト終了
		if (m_alphaVal <= 70)
		{
			m_state = TITLE_SCENE_STATE::FADE_IN;
			m_alphaVal = 255;
		}
	}

	//	フェードイン処理
	if (m_state == TITLE_SCENE_STATE::FADE_IN)
	{
		// アルファ値の減算
		m_alphaVal -= m_addAlphaVal;

		// アルファブレンド有効化(ここでアルファ値をセット)
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, m_alphaVal);

		// 画面全体に任意のカラーの四角形を描画
		DrawBox(0, 0, SCREEN_SIZE_W, SCREEN_SIZE_H, GetColor(0, 0, 0), TRUE);

		// アルファブレンド無効化
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

		// アルファ値が最大(255)になったらフェードアウト終了
		if (m_alphaVal <= 0)
		{
			m_state = TITLE_SCENE_STATE::TITLE;
		}
	}

	// フェードアウト処理
	if (m_state == TITLE_SCENE_STATE::FADE_OUT)
	{
		// アルファ値の加算
		m_alphaVal += m_addAlphaVal2;
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

void Title::Sound()
{
	//	BGMを流す
	PlaySoundMem(m_backSoundHandle, DX_PLAYTYPE_BACK, FALSE);
	ChangeVolumeSoundMem(m_volumePal, m_backSoundHandle);
}

void Title::Load()
{
	//	グラフィックハンドルにセット
	m_backGraphHandle = LoadGraph("data/img/Title/Title_back.png");				//	背景
	m_logoGraphHandle = LoadGraph("data/img/Title/Title_logo.png");				//	ロゴ
	m_startGuideGraphHandle = LoadGraph("data/img/Title/Title_start.png");		//	スタートの案内
	m_manualGuideGraphHandle = LoadGraph("data/img/Title/Title_manual.png");	//	マニュアルへの案内
	m_gateGraphHandle = LoadGraph("data/img/Title/GATE.png");					//	GATE
	m_maouGraphHandle = LoadGraph("data/img/Title/maou.png");					//	魔王魂

	//	サウンドハンドルにセット
	m_backSoundHandle = LoadSoundMem("data/sound/Title/Title2.ogg");		//	BGM
}

/// 点滅エフェクトに用いる透過量の更新処理
void Title::UpdateTransparent()
{
	// 透過量が122より大きくなったら
	if (m_transpVal >= MAX_TRANSP_VAL)
	{
		// 透過量を255に設定
		m_transpVal = MAX_TRANSP_VAL - 1;
		// 毎透過量を-1にする
		m_permeationAmount *= TRANSP_MODERATION;
	}
	// 透過量が0より小さければ
	else if (MAX_TRANSP_VAL / 2 >= m_transpVal)
	{
		// 透過量を設定
		m_transpVal = MAX_TRANSP_VAL / 2 + 1;
		// 毎透過量を1にする
		m_permeationAmount *= TRANSP_MODERATION;
	}
	// 毎透過量を透過量に加算する
	m_transpVal += m_permeationAmount;
}