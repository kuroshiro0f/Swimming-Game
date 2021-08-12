#include "Yamaoka_GameScene.h"
#include "Yamaoka_Result.h"
#include "Yamaoka_Stage.h"
#include "Yamaoka_Camera.h"
#include "Yamaoka_Actor.h"

#include "DxLib.h"

//	スクリーンのサイズ
const int SCREEN_SIZE_W = 1920;
const int SCREEN_SIZE_H = 1080;

// フェードイン・フェードアウトの速度
const int addAlphaVal = 5;

Yamaoka_GameScene::Yamaoka_GameScene()
	: m_alphaVal(255)
	, m_fadeOutFinishFlag(false)
	, m_stage(nullptr)
	, m_camera(nullptr)
	, m_actor(nullptr)
	, m_startTime(0)
	, m_tmpTime(0)
	, m_countUP(0)
{
	// 開始時の時間を取得
	m_startTime = GetNowCount() / 1000;

	// ステートセット(フェードインから)
	m_state = GAME_SCENE_STATE::FADE_IN;
}

Yamaoka_GameScene::~Yamaoka_GameScene()
{
	delete m_stage;   // ステージのポインタメンバ変数を消去
	delete m_camera;  // カメラのポインタメンバ変数を消去
	delete m_actor;   // アクターのポインタメンバ変数を削除
	StopSoundMem(m_bgmSoundHandle);
	DeleteGraph(m_backGraphHandle);	
}

SceneBase* Yamaoka_GameScene::Update(float _deltaTime)
{
	m_camera->Update(*m_actor);

	switch (m_state)
	{
	case GAME_SCENE_STATE::FADE_IN:
		break;
	case GAME_SCENE_STATE::GAME:
		m_stage->Update();
		m_actor->Update(_deltaTime);
		m_actor->UpdateActor(_deltaTime);
		//現在時刻を取得
		m_tmpTime = GetNowCount() / 1000;
		// 経過時間を計算  (-〇 は応急処置)
		m_countUP = (m_tmpTime - m_startTime) - 9;
		

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

			m_state = GAME_SCENE_STATE::FADE_OUT;
		}

		// プレイヤーがゴールについたら
		if (m_actor->dCount <= 500)
		{
			WaitTimer(600);
			m_state = GAME_SCENE_STATE::FADE_OUT;
		}

		break;
	case GAME_SCENE_STATE::FADE_OUT:
		if (m_fadeOutFinishFlag)
		{
			//                        経過時間をリザルトに渡す
			return new Yamaoka_Result(m_countUP);	//	リザルトシーンに切り替える
		}
		break;
	default:
		break;
	}
	return this;
}

void Yamaoka_GameScene::Draw()
{
	//	背景
	DrawGraph(0, 0, m_backGraphHandle, TRUE);

	// ステージの描画
	m_stage->Draw();
	SetFontSize(40);
	// 
	DrawBox(1550, 830, 1850, 880, GetColor(0, 255, 255), TRUE);
	DrawFormatString(1600, 835, GetColor(0,0,0), "TIME   %d", m_countUP);

	// プレイヤー描画
	m_actor->DrawActor();

	// 操作ボタン（仮）
	if (CheckHitKey(KEY_INPUT_RIGHT))
	{
		DrawBox(1050, 800, 1150, 900, GetColor(255, 255, 255), TRUE);
	}
	if (CheckHitKey(KEY_INPUT_LEFT))
	{
		DrawBox(750, 800, 850, 900, GetColor(255, 255, 255), TRUE);
	}
	DrawBox(750, 800, 850, 900, GetColor(0, 0, 0), FALSE);
	DrawBox(1050, 800, 1150, 900, GetColor(0, 0, 0), FALSE);
	SetFontSize(100);
	DrawFormatString(750, 800, GetColor(0, 0, 0), "←");
	DrawFormatString(1050, 800, GetColor(0, 0, 0), "→");


	//	フェードイン処理
	if (m_state == GAME_SCENE_STATE::FADE_IN)
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
			m_state = GAME_SCENE_STATE::GAME;
		}
	}

	// フェードアウト処理
	if (m_state == GAME_SCENE_STATE::FADE_OUT)
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

void Yamaoka_GameScene::Sound()
{
	if (m_state == GAME_SCENE_STATE::GAME)
	{
		PlaySoundMem(m_bgmSoundHandle, DX_PLAYTYPE_BACK, FALSE);
		ChangeVolumeSoundMem(m_volumePal, m_bgmSoundHandle);
	}
}

void Yamaoka_GameScene::Load()
{
	// グラフィックハンドルにセット
	m_backGraphHandle = LoadGraph("data/img/gameBackTest.png");		//	背景

	//	サウンドハンドルにセット
	m_bgmSoundHandle = LoadSoundMem("data/sound/gameBgmTest.mp3");			//	BGM

	// ステージクラスのインスタンスを生成
	m_stage = new Yamaoka_Stage;
	// アクタークラスへのインスタンスを生成
	m_actor = new Yamaoka_Actor;
	// カメラクラスへのインスタンスを生成
	m_camera = new Yamaoka_Camera(*m_actor);
}
