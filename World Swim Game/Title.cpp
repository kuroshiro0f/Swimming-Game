#include "Title.h"
#include "GameScene.h"

#include "DxLib.h"

//	ウインドウのサイズ
const int SCREEN_SIZE_W = 1920;
const int SCREEN_SIZE_H = 1080;

//	効果音音量調整
int DECVOLUMEPAL = 40;

//	最大透過量
const int MAX_TRANSP_VAL = 255;
//	透過量変化用ベクトル
const int TRANSP_MODERATION = -1;
//	最初の透過量
const int FIRST_TRANS_VAL = 100;

//	フェードイン・フェードアウトの速度
const float ADD_ALPHA_VAL = 6.0f;
const float ADD_ALPHA_VAL_2 = 30.0f;

////	円周率
//const double PI = 3.1415926535897932384626433832795f;

Title::Title()
	: m_state(TITLE_SCENE_STATE::FIRST)
	, m_alphaVal(300)
	, m_fadeOutFinishFlag(false)
	, m_addAlphaVal(ADD_ALPHA_VAL)
	, m_addAlphaVal2(ADD_ALPHA_VAL_2)
	/*, m_bigDropAngle(PI)
	, m_smallDropAngle(PI)*/
{
	if (CheckHitKey(KEY_INPUT_RETURN))
	{
		m_checkKeyFlag = true;
	}

	// 透過量変数を122に設定
	m_transpVal = MAX_TRANSP_VAL;
	m_fadeTransVal = FIRST_TRANS_VAL;
	// 毎透過量変数を2に設定
	m_permeationAmount = 2;
}

Title::~Title()
{
	StopSoundMem(m_backSoundHandle);

	//	メモリの解放
	DeleteGraph(m_backGraphHandle);
	DeleteGraph(m_logoGraphHandle);
	DeleteGraph(m_startGuideGraphHandle);
	DeleteGraph(m_manualGuideGraphHandle);
	DeleteGraph(m_gateGraphHandle);
	DeleteGraph(m_maouGraphHandle);
	DeleteGraph(m_bigDropGraphHandle);
	DeleteGraph(m_smallDropGraphHandle);
	DeleteGraph(m_manualGraphHandle);
	DeleteSoundMem(m_backSoundHandle);
	DeleteSoundMem(m_entClickSoundHandle);
	DeleteSoundMem(m_spaClickSoundHandle);
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

			//	SEを流す
			PlaySoundMem(m_entClickSoundHandle, DX_PLAYTYPE_BACK, TRUE);

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

		// SPACEでマニュアルへ
		if (CheckHitKey(KEY_INPUT_SPACE) && m_checkKeyFlag == false)
		{
			// ※キー入力重複対策のフラグ
			m_checkKeyFlag = true;

			//	SEを流す
			PlaySoundMem(m_spaClickSoundHandle, DX_PLAYTYPE_BACK, TRUE);

			m_state = TITLE_SCENE_STATE::MANUAL;
		}

		// ENTERで次のステートへ
		if (CheckHitKey(KEY_INPUT_RETURN) && m_checkKeyFlag == false)
		{
			// ※キー入力重複対策のフラグ
			m_checkKeyFlag = true;

			//	SEを流す
			PlaySoundMem(m_entClickSoundHandle, DX_PLAYTYPE_BACK, TRUE);

			m_state = TITLE_SCENE_STATE::FADE_OUT;
		}

		break;
	case TITLE_SCENE_STATE::MANUAL:
		if (!CheckHitKey(KEY_INPUT_RETURN))
		{
			m_checkKeyFlag = false;
		}

		if (CheckHitKey(KEY_INPUT_RETURN) && m_checkKeyFlag == false)
		{
			// ※キー入力重複対策のフラグ
			m_checkKeyFlag = true;

			//	SEを流す
			PlaySoundMem(m_spaClickSoundHandle, DX_PLAYTYPE_BACK, TRUE);

			m_state = TITLE_SCENE_STATE::MANUAL2;
		}

		break;
	case TITLE_SCENE_STATE::MANUAL2:
		if (!CheckHitKey(KEY_INPUT_RETURN))
		{
			m_checkKeyFlag = false;
		}

		if (CheckHitKey(KEY_INPUT_RETURN) && m_checkKeyFlag == false)
		{
			// ※キー入力重複対策のフラグ
			m_checkKeyFlag = true;

			//	SEを流す
			PlaySoundMem(m_spaClickSoundHandle, DX_PLAYTYPE_BACK, TRUE);

			m_state = TITLE_SCENE_STATE::TITLE;
		}

		break;
	case TITLE_SCENE_STATE::FADE_OUT:
		if (m_fadeOutFinishFlag)
		{
			//return new Ueyama_GameScene();
			return new GameScene();
		}
		break;
	default:
		break;
	}

	//m_deltaTime = _deltaTime;

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
		if (m_state != TITLE_SCENE_STATE::MANUAL && m_state != TITLE_SCENE_STATE::MANUAL2)
		{
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
	}

	if (m_state == TITLE_SCENE_STATE::MANUAL)
	{
		// 透過して描画
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, 190);
		DrawBox(0, 0, SCREEN_SIZE_W, SCREEN_SIZE_H, GetColor(0, 0, 0), TRUE);
		// 透過を元に戻す
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

		//	描画
		DrawGraph(0, 0, m_manualGraphHandle, TRUE);
	}
	if (m_state == TITLE_SCENE_STATE::MANUAL2)
	{
		// 透過して描画
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, 190);
		DrawBox(0, 0, SCREEN_SIZE_W, SCREEN_SIZE_H, GetColor(0, 0, 0), TRUE);
		// 透過を元に戻す
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

		//	描画
		DrawGraph(0, 0, m_manual2GraphHandle, TRUE);
	}

	if (m_state == TITLE_SCENE_STATE::FIRST)
	{
		// アルファ値の減算
		m_alphaVal -= m_addAlphaVal;
		if (m_alphaVal <= 255)
		{
			//	描画
			DrawGraph(0, 0, m_gateGraphHandle, TRUE);			//	GATE

			// アルファブレンド有効化(ここでアルファ値をセット)
			SetDrawBlendMode(DX_BLENDMODE_ALPHA, m_alphaVal);

			// 画面全体に任意のカラーの四角形を描画
			DrawBox(0, 0, SCREEN_SIZE_W, SCREEN_SIZE_H, GetColor(0, 0, 0), TRUE);

			// アルファブレンド無効化
			SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

			// アルファ値が0になったらフェードイン終了
			if (m_alphaVal <= 0)
			{
				m_state = TITLE_SCENE_STATE::SECOND;
				m_alphaVal = 255;
			}
		}
	}

	if (m_state == TITLE_SCENE_STATE::SECOND)
	{
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

		// アルファ値が0になったらフェードイン終了
		if (m_alphaVal <= 0)
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
			m_alphaVal = 0;
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
			m_alphaVal = 255;
			m_fadeOutFinishFlag = true;
		}
	}
}

void Title::Sound()
{
	//	BGMを流す
	PlaySoundMem(m_backSoundHandle, DX_PLAYTYPE_BACK, FALSE);
}

void Title::Load()
{
	//	グラフィックハンドルにセット
	m_backGraphHandle = LoadGraph("data/img/Title/Title_back2.png");			//	背景
	m_logoGraphHandle = LoadGraph("data/img/Title/Title_logo.png");				//	ロゴ
	m_startGuideGraphHandle = LoadGraph("data/img/Title/Title_start.png");		//	スタートの案内
	m_manualGuideGraphHandle = LoadGraph("data/img/Title/Title_manual.png");	//	マニュアルへの案内
	m_gateGraphHandle = LoadGraph("data/img/Title/GATE.png");					//	GATE
	m_maouGraphHandle = LoadGraph("data/img/Title/maou.png");					//	魔王魂
	//m_bigDropGraphHandle = LoadGraph("data/img/Title/drop1.png");				//	大きな水滴
	//m_smallDropGraphHandle = LoadGraph("data/img/Title/drop2.png");			//	小さな水滴
	m_manualGraphHandle = LoadGraph("data/img/Manual/Manual.png");				//	マニュアル
	m_manual2GraphHandle = LoadGraph("data/img/Manual/Manual2.png");			//	マニュアル2

	//	サウンドハンドルにセット
	m_backSoundHandle = LoadSoundMem("data/sound/Title/Title4.ogg");			//	BGM
	m_entClickSoundHandle = LoadSoundMem("data/sound/Title/SE2.mp3");		//	ENTERキーを押したとき
	m_spaClickSoundHandle = LoadSoundMem("data/sound/Title/SE1.mp3");		//	SPACEキーを押したとき

	//	音量調整
	ChangeVolumeSoundMem(m_volumePal, m_backSoundHandle);
	ChangeVolumeSoundMem(m_volumePal + DECVOLUMEPAL, m_spaClickSoundHandle);
	ChangeVolumeSoundMem(m_volumePal + DECVOLUMEPAL, m_entClickSoundHandle);
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