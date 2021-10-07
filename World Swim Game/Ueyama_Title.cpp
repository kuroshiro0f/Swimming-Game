#include "Ueyama_Title.h"
#include "Ueyama_GameScene.h"
#include "GameScene.h"

#include "DxLib.h"

//-----------------------------------------------------------------------------
//	・変更点
//-----------------------------------------------------------------------------

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

//	止める時間
const int STOP_TIME = 50;

//	フェードイン・フェードアウトの速度
const int ADD_ALPHA_VAL = 5;
const int ADD_ALPHA_VAL_2 = 10;

//	イルカの移動
const int DOLPHIN_MIN_Y = -30;

//	矢印のY座標
const int ARROW_UP_Y = 0;
const int ARROW_DOWN_Y = 130;

Ueyama_Title::Ueyama_Title(bool _roundFlag)
	: m_alphaVal(300)
	, m_fadeOutFinishFlag(false)
	, m_titleFlag(false)
	, m_addAlphaVal(ADD_ALPHA_VAL)
	, m_addAlphaVal2(ADD_ALPHA_VAL_2)
	, m_transpVal2(0)
	, m_displayCount(0)
	, m_transpDownFlag(false)
	, m_cursolNum(0)
	, m_arrowY(ARROW_UP_Y)
	, m_blueY(0)
	, m_yellowY(DOLPHIN_MIN_Y)
	, m_blueUpFlag(true)
	, m_yellowUpFlag(false)
{
	if (CheckHitKey(KEY_INPUT_RETURN))
	{
		m_checkKeyFlag = true;
	}

	if (_roundFlag)
	{
		m_state = TITLE_SCENE_STATE::FADE_IN;
	}
	else if (!_roundFlag)
	{
		m_state = TITLE_SCENE_STATE::FIRST;
	}

	// 透過量変数を122に設定
	m_transpVal = MAX_TRANSP_VAL;
	m_fadeTransVal = FIRST_TRANS_VAL;
	// 毎透過量変数を2に設定
	m_permeationAmount = 2;
}

Ueyama_Title::~Ueyama_Title()
{
	StopSoundMem(m_backSoundHandle);

	//	メモリの解放
	DeleteGraph(m_backGraphHandle);
	DeleteGraph(m_logoGraphHandle);
	DeleteGraph(m_startGuideGraphHandle);
	DeleteGraph(m_manualGuideGraphHandle);
	DeleteGraph(m_arrowGraphHandle);
	DeleteGraph(m_blueDolphinGraphHandle);
	DeleteGraph(m_yellowDolphinGraphHandle);
	DeleteGraph(m_gateGraphHandle);
	DeleteGraph(m_maouGraphHandle);
	DeleteGraph(m_manualGraphHandle);
	DeleteGraph(m_manual2GraphHandle);
	DeleteGraph(m_manual3GraphHandle);
	DeleteSoundMem(m_backSoundHandle);
	DeleteSoundMem(m_entClickSoundHandle);
	DeleteSoundMem(m_spaClickSoundHandle);

	delete m_volume;
}

SceneBase* Ueyama_Title::Update(float _deltaTime)
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
		if (!CheckHitKey(KEY_INPUT_RETURN) && !CheckHitKey(KEY_INPUT_UP) && !CheckHitKey(KEY_INPUT_DOWN))
		{
			m_checkKeyFlag = false;
		}

		//	モード選択
		if (CheckHitKey(KEY_INPUT_UP) && !m_checkKeyFlag || CheckHitKey(KEY_INPUT_DOWN) && !m_checkKeyFlag)
		{
			//	SEを流す
			PlaySoundMem(m_spaClickSoundHandle, DX_PLAYTYPE_BACK, TRUE);

			// ※キー入力重複対策のフラグ
			m_checkKeyFlag = true;

			if (m_cursolNum == 0)
			{
				m_cursolNum = 1;
			}
			else if (m_cursolNum == 1)
			{
				m_cursolNum = 0;
			}
		}

		// ENTERで選択されたステートへ
		if (CheckHitKey(KEY_INPUT_RETURN) && m_checkKeyFlag == false)
		{
			// ※キー入力重複対策のフラグ
			m_checkKeyFlag = true;

			//	SEを流す
			PlaySoundMem(m_entClickSoundHandle, DX_PLAYTYPE_BACK, TRUE);

			if (m_cursolNum == 0)
			{
				m_state = TITLE_SCENE_STATE::FADE_OUT;
			}
			else if (m_cursolNum == 1)
			{
				m_state = TITLE_SCENE_STATE::MANUAL;
			}
		}
		break;
	case TITLE_SCENE_STATE::TITLE:
		// ※キー入力重複対策のフラグ
		// ENTERキーから指を離したら、次のENTERの入力を有効に
		if (!CheckHitKey(KEY_INPUT_RETURN) && !CheckHitKey(KEY_INPUT_UP) && !CheckHitKey(KEY_INPUT_DOWN))
		{
			m_checkKeyFlag = false;
		}

		//	モード選択
		if (CheckHitKey(KEY_INPUT_UP) && !m_checkKeyFlag || CheckHitKey(KEY_INPUT_DOWN) && !m_checkKeyFlag)
		{
			//	SEを流す
			PlaySoundMem(m_spaClickSoundHandle, DX_PLAYTYPE_BACK, TRUE);

			// ※キー入力重複対策のフラグ
			m_checkKeyFlag = true;

			if (m_cursolNum == 0)
			{
				m_cursolNum = 1;
			}
			else if (m_cursolNum == 1)
			{
				m_cursolNum = 0;
			}
		}

		// ENTERで選択されたステートへ
		if (CheckHitKey(KEY_INPUT_RETURN) && m_checkKeyFlag == false)
		{
			// ※キー入力重複対策のフラグ
			m_checkKeyFlag = true;

			//	SEを流す
			PlaySoundMem(m_entClickSoundHandle, DX_PLAYTYPE_BACK, TRUE);

			if (m_cursolNum == 0)
			{
				m_state = TITLE_SCENE_STATE::FADE_OUT;
			}
			else if (m_cursolNum == 1)
			{
				m_state = TITLE_SCENE_STATE::MANUAL;
			}
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

			m_state = TITLE_SCENE_STATE::MANUAL3;
		}

		break;
	case TITLE_SCENE_STATE::MANUAL3:
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

			m_cursolNum = 0;

			m_state = TITLE_SCENE_STATE::TITLE;
		}

		break;
	case TITLE_SCENE_STATE::FADE_OUT:
		if (m_fadeOutFinishFlag)
		{
			return new Ueyama_GameScene();
			//return new GameScene();
		}
		break;
	default:
		break;
	}

	/*m_deltaTime = _deltaTime;*/

	return this;
}

void Ueyama_Title::Draw()
{

	// 透過量更新
	UpdateTransparent();

	if (m_state != TITLE_SCENE_STATE::FIRST && m_state != TITLE_SCENE_STATE::SECOND)
	{
		// 描画
		DrawGraph(0, 0, m_backGraphHandle, TRUE);			//	背景
		if (m_state != TITLE_SCENE_STATE::MANUAL && m_state != TITLE_SCENE_STATE::MANUAL2 && m_state != TITLE_SCENE_STATE::MANUAL3)
		{
			DrawGraph(0, 0, m_logoGraphHandle, TRUE);			//	ロゴ

			/*m_bigDropAngle += DROP_SPEED;
			m_smallDropAngle -= DROP_SPEED;

			DrawRotaGraph(DROP1_X, DROP1_Y, 1.0, m_bigDropAngle, m_bigDropGraphHandle, TRUE, FALSE);
			DrawRotaGraph(DROP2_X, DROP2_Y, 1.0, m_smallDropAngle, m_smallDropGraphHandle, TRUE, FALSE);*/

			DrawGraph(0, m_arrowY, m_arrowGraphHandle, TRUE);

			if (m_cursolNum == 0)
			{
				m_arrowY = ARROW_UP_Y;
			}
			else if (m_cursolNum == 1)
			{
				m_arrowY = ARROW_DOWN_Y;
			}

			if (m_cursolNum == 0)
			{
				// 透過して描画
				SetDrawBlendMode(DX_BLENDMODE_ALPHA, m_transpVal);
			}
			DrawGraph(0, 0, m_startGuideGraphHandle, TRUE);		//	スタートの案内
			if (m_cursolNum == 0)
			{
				// 透過を元に戻す
				SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
			}

			if (m_cursolNum == 1)
			{
				// 透過して描画
				SetDrawBlendMode(DX_BLENDMODE_ALPHA, m_transpVal);
			}
			DrawGraph(0, 0, m_manualGuideGraphHandle, TRUE);	//	マニュアルへの案内
			if (m_cursolNum == 1)
			{
				// 透過を元に戻す
				SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
			}

			DrawGraph(0, m_blueY, m_blueDolphinGraphHandle, TRUE);
			DrawGraph(0, m_yellowY, m_yellowDolphinGraphHandle, TRUE);

			DlphinMotion();
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
	if (m_state == TITLE_SCENE_STATE::MANUAL3)
	{
		// 透過して描画
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, 190);
		DrawBox(0, 0, SCREEN_SIZE_W, SCREEN_SIZE_H, GetColor(0, 0, 0), TRUE);
		// 透過を元に戻す
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

		//	描画
		DrawGraph(0, 0, m_manual3GraphHandle, TRUE);
	}

	if (m_state == TITLE_SCENE_STATE::FIRST || m_state == TITLE_SCENE_STATE::SECOND)
	{
		if (!m_titleFlag)
		{
			// アルファ値の減算
			m_alphaVal -= m_addAlphaVal;
		}
		else if (m_titleFlag)
		{
			m_alphaVal += m_addAlphaVal;
		}

		// 画面全体に任意のカラーの四角形を描画
		DrawBox(0, 0, SCREEN_SIZE_W, SCREEN_SIZE_H, GetColor(255, 255, 255), TRUE);

		// アルファブレンド有効化(ここでアルファ値をセット)
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, m_alphaVal);

		// 画面全体に任意のカラーの四角形を描画
		DrawBox(0, 0, SCREEN_SIZE_W, SCREEN_SIZE_H, GetColor(0, 0, 0), TRUE);

		// アルファブレンド無効化
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

		// アルファ値が0になったらフェードイン終了
		if (m_alphaVal <= 0 && !m_titleFlag)
		{
			m_alphaVal = 0;
		}
		if (m_alphaVal >= 255 && m_titleFlag)
		{
			m_alphaVal = 255;
			m_state = TITLE_SCENE_STATE::FADE_IN;
		}
	}

	if (m_state == TITLE_SCENE_STATE::FIRST)
	{
		if (m_alphaVal <= 0)
		{
			// 透過して描画
			SetDrawBlendMode(DX_BLENDMODE_ALPHA, m_transpVal2);
			//	描画
			DrawGraph(0, 0, m_gateGraphHandle, TRUE);			//	GATE
			// 透過を元に戻す
			SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

			if (m_transpDownFlag)
			{
				m_transpVal2 -= m_addAlphaVal;
			}
			else if (!m_transpDownFlag)
			{
				m_transpVal2 += m_addAlphaVal;
			}

			if (m_transpVal2 >= MAX_TRANSP_VAL && !m_transpDownFlag)
			{
				m_displayCount++;
				m_transpVal2 = MAX_TRANSP_VAL;
				if (m_displayCount >= STOP_TIME)
				{
					m_transpDownFlag = true;
					m_displayCount = 0;
				}
			}
			if (m_transpVal2 <= 0 && m_transpDownFlag)
			{
				m_transpVal2 = 0;
				m_transpDownFlag = false;
				m_state = TITLE_SCENE_STATE::SECOND;
			}
		}
	}

	if (m_state == TITLE_SCENE_STATE::SECOND)
	{
		if (m_alphaVal <= 0)
		{
			// 透過して描画
			SetDrawBlendMode(DX_BLENDMODE_ALPHA, m_transpVal2);
			//	描画
			DrawGraph(0, 0, m_maouGraphHandle, TRUE);			//	GATE
			// 透過を元に戻す
			SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

			if (m_transpDownFlag)
			{
				m_transpVal2 -= m_addAlphaVal;
			}
			else if (!m_transpDownFlag)
			{
				m_transpVal2 += m_addAlphaVal;
			}

			if (m_transpVal2 >= MAX_TRANSP_VAL && !m_transpDownFlag)
			{
				m_displayCount++;
				m_transpVal2 = MAX_TRANSP_VAL;
				if (m_displayCount >= STOP_TIME)
				{
					m_transpDownFlag = true;
					m_displayCount = 0;
				}
			}
			if (m_transpVal2 <= 0 && m_transpDownFlag)
			{
				m_transpVal2 = 0;
				m_transpDownFlag = false;
				m_titleFlag = true;
			}
		}
	}

	//	フェードイン処理
	if (m_state == TITLE_SCENE_STATE::FADE_IN)
	{
		// アルファ値の減算
		m_alphaVal -= m_addAlphaVal2;

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

void Ueyama_Title::Sound()
{
	//	音量調整
	ChangeVolumeSoundMem(m_volume->GetVolumePal(), m_backSoundHandle);
	ChangeVolumeSoundMem(m_volume->GetVolumePal() + DECVOLUMEPAL, m_spaClickSoundHandle);
	ChangeVolumeSoundMem(m_volume->GetVolumePal() + DECVOLUMEPAL, m_entClickSoundHandle);

	if (m_state != TITLE_SCENE_STATE::FIRST && m_state != TITLE_SCENE_STATE::SECOND)
	{
		//	BGMを流す
		PlaySoundMem(m_backSoundHandle, DX_PLAYTYPE_BACK, FALSE);
	}
}

void Ueyama_Title::Load()
{
	//	グラフィックハンドルにセット
	m_backGraphHandle = LoadGraph("data/img/Title/Title_back.png");				//	背景
	m_logoGraphHandle = LoadGraph("data/img/Title/Title_logo.png");				//	ロゴ
	m_startGuideGraphHandle = LoadGraph("data/img/Title/Title_start.png");		//	スタートの案内
	m_manualGuideGraphHandle = LoadGraph("data/img/Title/Title_manual.png");	//	マニュアルへの案内
	m_arrowGraphHandle = LoadGraph("data/img/Title/Title_arrow.png");			//	矢印
	m_blueDolphinGraphHandle = LoadGraph("data/img/Title/Title_blue.png");		//	青のイルカ
	m_yellowDolphinGraphHandle = LoadGraph("data/img/Title/Title_yellow.png");	//	黄色のイルカ
	m_gateGraphHandle = LoadGraph("data/img/Title/GATE.png");					//	GATE
	m_maouGraphHandle = LoadGraph("data/img/Title/maou.png");					//	魔王魂
	m_manualGraphHandle = LoadGraph("data/img/Manual/Manual_1.png");				//	マニュアル
	m_manual2GraphHandle = LoadGraph("data/img/Manual/Manual_2.png");			//	マニュアル2
	m_manual3GraphHandle = LoadGraph("data/img/Manual/Manual_3.png");			//	マニュアル3

	//	サウンドハンドルにセット
	m_backSoundHandle = LoadSoundMem("data/sound/Title/Title4.ogg");			//	BGM
	m_entClickSoundHandle = LoadSoundMem("data/sound/Title/SE2.mp3");		//	ENTERキーを押したとき
	m_spaClickSoundHandle = LoadSoundMem("data/sound/Title/SE1.mp3");		//	SPACEキーを押したとき

	m_volume = new Volume();
}

/// 点滅エフェクトに用いる透過量の更新処理
void Ueyama_Title::UpdateTransparent()
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

void Ueyama_Title::DlphinMotion()
{
	if (m_blueY <= DOLPHIN_MIN_Y)
	{
		m_blueUpFlag = false;
	}
	else if (m_blueY >= 0)
	{
		m_blueUpFlag = true;
	}
	if (m_blueUpFlag)
	{
		m_blueY--;
	}
	else if (!m_blueUpFlag)
	{
		m_blueY++;
	}

	if (m_yellowY <= DOLPHIN_MIN_Y)
	{
		m_yellowUpFlag = false;
	}
	else if (m_yellowY >= 0)
	{
		m_yellowUpFlag = true;
	}
	if (m_yellowUpFlag)
	{
		m_yellowY--;
	}
	else if (!m_yellowUpFlag)
	{
		m_yellowY++;
	}
}
