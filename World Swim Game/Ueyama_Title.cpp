#include "Ueyama_Title.h"
#include "Ueyama_GameScene.h"
#include "Yamaoka_GameScene.h"

#include "DxLib.h"

//	ウインドウのサイズ
const int SCREEN_SIZE_W = 1920;
const int SCREEN_SIZE_H = 1080;

//	フェードイン・フェードアウトの速度
const int addAlphaVal = 5;


Ueyama_Title::Ueyama_Title()
	: m_state(TITLE_SCENE_STATE::TITLE)
	, m_alphaVal(0)
	, m_fadeOutFinishFlag(false)
{
	if (CheckHitKey(KEY_INPUT_RETURN))
	{
		m_checkKeyFlag = TRUE;
	}

	SetFontSize(m_normalFontSize);
}

Ueyama_Title::~Ueyama_Title()
{
	StopSoundMem(m_backSoundHandle);

	DeleteGraph(m_backGraphHandle);
	DeleteSoundMem(m_backSoundHandle);
}

SceneBase* Ueyama_Title::Update(float _deltaTime)
{
	// ステートメントごとに処理を変更
	switch (m_state)
	{
	case TITLE_SCENE_STATE::FADE_IN:
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
		if (!CheckHitKey(KEY_INPUT_RETURN))
		{
			m_checkKeyFlag = false;
		}

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
			//return new Ueyama_GameScene();
			return new Yamaoka_GameScene();
		}
		break;
	default:
		break;
	}
	return this;
}

void Ueyama_Title::Draw()
{
	// 描画
	DrawGraph(0, 0, m_backGraphHandle, TRUE);			//	背景
	DrawGraph(0, 0, m_logoGraphHandle, TRUE);			//	ロゴ
	DrawGraph(0, 0, m_startGuideGraphHandle, TRUE);		//	スタートの案内
	DrawGraph(0, 0, m_manualGuideGraphHandle, TRUE);	//	マニュアルへの案内


	//// 画面に映る位置に３Ｄモデルを移動
	//MV1SetPosition(m_swimModelHandle, VGet(320.0f, -300.0f, 600.0f));


	//// 3Dモデルのスケールを拡大
	//MV1SetScale(m_swimModelHandle, VGet(50.0f, 50.0f, 50.0f));

	////	クロールのモデルを描画
	//MV1DrawModel(m_swimModelHandle);


	if (m_state == TITLE_SCENE_STATE::MANUAL)
	{
		// 透過して描画
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, 190);
		DrawBox(0, 0, SCREEN_SIZE_W, SCREEN_SIZE_H, GetColor(0, 0, 0), TRUE);
		// 透過を元に戻す
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	}

	//	フェードイン処理
	if (m_state == TITLE_SCENE_STATE::FADE_IN)
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
			m_state = TITLE_SCENE_STATE::TITLE;
		}
	}

	// フェードアウト処理
	if (m_state == TITLE_SCENE_STATE::FADE_OUT)
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

void Ueyama_Title::Sound()
{
	//	BGMを流す
	PlaySoundMem(m_backSoundHandle, DX_PLAYTYPE_BACK, FALSE);
	ChangeVolumeSoundMem(m_volumePal, m_backSoundHandle);
}

void Ueyama_Title::Load()
{
	//	グラフィックハンドルにセット
	m_backGraphHandle = LoadGraph("data/img/Title/Title_back.png");				//	背景
	m_logoGraphHandle = LoadGraph("data/img/Title/Title_logo.png");				//	ロゴ
	m_startGuideGraphHandle = LoadGraph("data/img/Title/Title_start.png");		//	スタートの案内
	m_manualGuideGraphHandle = LoadGraph("data/img/Title/Title_manual.png");	//	マニュアルへの案内

	//	モデルハンドルにセット
	m_swimModelHandle = MV1LoadModel("data/asset/swim/natu12b.pmx");			//	泳ぐキャラ
	m_crawlModelHandle = MV1LoadModel("data/asset/クロール.vmd");				//	クロール

	// アニメーションをアタッチ
	MV1AttachAnim(m_swimModelHandle,0,m_crawlModelHandle,FALSE);				//	クロール

	//	サウンドハンドルにセット
	m_backSoundHandle = LoadSoundMem("data/sound/Title/titleBgmTest.mp3");		//	BGM
}
