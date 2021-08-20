#include "Nagatomo_GameScene.h"

#include "Nagatomo_Result.h"
#include "Stage.h"
#include "Nagatomo_PlayerActor.h"

#include "DxLib.h"

//	スクリーンのサイズ
const int SCREEN_SIZE_W = 1920;
const int SCREEN_SIZE_H = 1080;

// フェードイン・フェードアウトの速度
const int addAlphaVal = 5;

GameScene::GameScene()
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

GameScene::~GameScene()
{
	StopSoundMem(m_bgmSoundHandle);
	DeleteGraph(m_backGraphHandle);

	delete m_stage;   // ステージのポインタメンバ変数を消去
	delete m_camera;  // カメラのポインタメンバ変数を消去
	delete m_actor;   // アクターのポインタメンバ変数を削除
}

SceneBase* GameScene::Update(float _deltaTime)
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
		m_countUP = (m_tmpTime - m_startTime)-5;

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

		//端まで行くと次のステートへ
		if (m_actor->GetPosX() <= -136)
		{
			m_state = GAME_SCENE_STATE::FADE_OUT;
		}

		break;
	case GAME_SCENE_STATE::FADE_OUT:
		if (m_fadeOutFinishFlag)
		{
			return new Result(m_countUP);				//	リザルトシーンに切り替える
		}

		break;
	default:
		break;
	}
	return this;
}

void GameScene::Draw()
{
	//	背景
	DrawGraph(0, 0, m_backGraphHandle, TRUE);

	// ステージの描画
	m_stage->Draw();
	SetFontSize(40);
	// 
	DrawBox(1450, 800, 1750, 890, GetColor(0, 255, 255), TRUE);
	DrawFormatString(1500, 800, GetColor(0, 0, 0), "TIME   %d", m_countUP);

	// プレイヤー描画
	m_actor->DrawActor();

	// 操作ボタン（仮） プレイヤーの手の斜め上に表示
	DrawBox(750, 600, 850, 700, GetColor(0, 0, 0), FALSE);
	DrawBox(1050, 600, 1150, 700, GetColor(0, 0, 0), FALSE);
	SetFontSize(100);
	DrawFormatString(770, 600, GetColor(0, 0, 0), "←");
	DrawFormatString(1070, 600, GetColor(0, 0, 0), "→");

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

		//プレイヤー
		m_actor->DrawActor();

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

void GameScene::Sound()
{
	if (m_state == GAME_SCENE_STATE::GAME)
	{
		PlaySoundMem(m_bgmSoundHandle, DX_PLAYTYPE_BACK, FALSE);
		ChangeVolumeSoundMem(m_volumePal, m_bgmSoundHandle);
	}
}

void GameScene::Load()
{
	// グラフィックハンドルにセット
	m_backGraphHandle = LoadGraph("data/img/Game/gameBackTest.png");		//	背景

	//	サウンドハンドルにセット
	m_bgmSoundHandle = LoadSoundMem("data/sound/Game/gameBgmTest.mp3");			//	BGM

	// ステージクラスのインスタンスを生成
	m_stage = new Stage;
	// アクタークラスへのインスタンスを生成
	m_actor = new Nagatomo_PlayerActor;
	// カメラクラスへのインスタンスを生成
	m_camera = new Camera(*m_actor);
}
