#include "Ueyama_GameScene.h"
#include "Ueyama_Result.h"
#include "Stage.h"


#include "DxLib.h"

//	スクリーンのサイズ
const int SCREEN_SIZE_W = 1920;
const int SCREEN_SIZE_H = 1080;

// フェードイン・フェードアウトの速度
const int addAlphaVal = 5;

Ueyama_GameScene::Ueyama_GameScene()
	: m_alphaVal(255)
	, m_fadeOutFinishFlag(false)
	, m_stage(nullptr)
	, m_camera(nullptr)
	, m_actor(nullptr)
{
	// ステートセット(フェードインから)
	m_state = GAME_SCENE_STATE::FADE_IN;
}

Ueyama_GameScene::~Ueyama_GameScene()
{
	StopSoundMem(m_bgmSoundHandle);
	DeleteGraph(m_backGraphHandle);

	delete m_stage;   // ステージのポインタメンバ変数を消去
	delete m_camera;  // カメラのポインタメンバ変数を消去
	delete m_actor;   // アクターのポインタメンバ変数を削除
}

SceneBase* Ueyama_GameScene::Update(float _deltaTime)
{
	//	カメラをセット
	m_camera->Update(*m_actor);

	switch (m_state)
	{
	case GAME_SCENE_STATE::FADE_IN:
		//	ステージをセット
		m_stage->Update();
		//	アクターをセット
		m_actor->Update(_deltaTime);
		break;
	case GAME_SCENE_STATE::GAME:
		//	アクターを更新
		m_actor->UpdateActor(_deltaTime);

		//	カウントダウン開始
		m_actor->countDown--;

		// ※キー入力重複対策のフラグ
		// ENTERキーから指を離したら、次のENTERの入力を有効に
		if (!CheckHitKey(KEY_INPUT_RETURN))
		{
			m_checkKeyFlag = false;
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
			//	経過時間をリザルトに渡す
			return new Ueyama_Result(m_actor->countUP);	//	リザルトシーンに切り替える
		}
		break;
	default:
		break;
	}
	return this;
}

void Ueyama_GameScene::Draw()
{
	//	背景
	DrawGraph(0, 0, m_backGraphHandle, TRUE);

	// ステージの描画
	m_stage->Draw();
	SetFontSize(40);

	//	タイムの表示
	DrawBox(1550, 830, 1850, 880, GetColor(255, 255, 0), TRUE);
	DrawFormatString(1600, 835, GetColor(0, 0, 0), "TIME   %d", m_actor->countUP);

	// プレイヤー描画
	m_actor->DrawActor();

	// 操作ボタン（仮）
	if (CheckHitKey(KEY_INPUT_RIGHT))
	{
		DrawBox(1050, 600, 1150, 700, GetColor(255, 255, 255), TRUE);
	}
	if (CheckHitKey(KEY_INPUT_LEFT))
	{
		DrawBox(750, 600, 850, 700, GetColor(255, 255, 255), TRUE);
	}
	DrawBox(750, 600, 850, 700, GetColor(0, 0, 0), FALSE);
	DrawBox(1050, 600, 1150, 700, GetColor(0, 0, 0), FALSE);
	SetFontSize(100);
	DrawFormatString(750, 600, GetColor(0, 0, 0), "←");
	DrawFormatString(1050, 600, GetColor(0, 0, 0), "→");

	SetFontSize(35);
	// スタミナゲージの表示
	m_actor->DrawSt(m_actor->st, m_actor->MaxSt, m_actor->MinSt);
	// 残り距離の表示
	m_actor->DrawToGoal(m_actor->dCount, m_actor->maxdCount);

	// カウントダウンの表示
	if (m_actor->countDown >= 0)
	{
		SetFontSize(150);
		DrawFormatString(800, 400, GetColor(0, 0, 0), " %d ", m_actor->countDown / 60 + 1);
	}

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

void Ueyama_GameScene::Sound()
{
	if (m_state == GAME_SCENE_STATE::GAME)
	{
		PlaySoundMem(m_bgmSoundHandle, DX_PLAYTYPE_BACK, FALSE);
		ChangeVolumeSoundMem(m_volumePal, m_bgmSoundHandle);
	}
}

void Ueyama_GameScene::Load()
{
	// グラフィックハンドルにセット
	m_backGraphHandle = LoadGraph("data/img/Game/gameBackTest.png");		//	背景

	//	サウンドハンドルにセット
	m_bgmSoundHandle = LoadSoundMem("data/sound/Game/Game.ogg");			//	BGM

	// ステージクラスのインスタンスを生成
	m_stage = new Stage();
	// アクタークラスへのインスタンスを生成
	m_actor = new Ueyama_PlayerActor;
	// カメラクラスへのインスタンスを生成
	m_camera = new Ueyama_Camera(*m_actor);
}
