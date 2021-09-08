#include "GameScene.h"
#include "Result.h"
#include "Stage.h"
#include "PlayerActor.h"

#include "DxLib.h"

//	スクリーンのサイズ
const int SCREEN_SIZE_W = 1920;
const int SCREEN_SIZE_H = 1080;

// フェードイン・フェードアウトの速度
const int addAlphaVal = 5;

//	ロードするファイル数
const int LOAD_FAILE_NUM = 5;

//	TIPSの数
const int TIPS_NUM = 3;

//	LOADINGの文字の切り替わる速さ
const int LOADING_SPEED = 60;

//	男の子の移動範囲
const int BOY_MIN_Y = -50;

GameScene::GameScene()
	: m_alphaVal(255)
	, m_fadeOutFinishFlag(false)
	, m_loadFlag(true)
	, m_loadFinishFlag(false)
	, m_tipsFlag(GetRand(TIPS_NUM - 1))
	, m_loadingFlag(0)
	, m_boyPlusFlag(false)
	, m_stage(nullptr)
	, m_camera(nullptr)
	, m_actor(nullptr)
	, m_mojiX(1450)
	, m_mojiY(1000)
	, m_boyY(0)
{
	// ステートセット(フェードインから)
	m_state = GAME_SCENE_STATE::LOAD;
}

GameScene::~GameScene()
{
	StopSoundMem(m_bgmSoundHandle);
	DeleteGraph(m_backGraphHandle);
	DeleteGraph(m_tips1GraphHandle);
	DeleteGraph(m_tips2GraphHandle);
	DeleteGraph(m_tips3GraphHandle);
	DeleteGraph(m_boyGraphHandle);

	delete m_stage;   // ステージのポインタメンバ変数を消去
	delete m_camera;  // カメラのポインタメンバ変数を消去
	delete m_actor;   // アクターのポインタメンバ変数を削除
}

SceneBase* GameScene::Update(float _deltaTime)
{
	//	カメラをセット
	m_camera->Update(*m_actor);

	switch (m_state)
	{
	case GAME_SCENE_STATE::LOAD:
		//	ロードが終わったらゲーム画面へ
		if (GetASyncLoadNum() == 0)
		{
			m_loadFinishFlag = true;

			// ※キー入力重複対策のフラグ
			// ENTERキーから指を離したら、次のENTERの入力を有効に
			if (!CheckHitKey(KEY_INPUT_RETURN))
			{
				m_checkKeyFlag = false;
			}

			if (CheckHitKey(KEY_INPUT_RETURN))
			{
				SetUseASyncLoadFlag(FALSE);
				m_state = GAME_SCENE_STATE::FADE_IN;
			}
		}
		break;
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

		////端まで行くと次のステートへ
		//if (m_actor->GetPosX() <= -136)
		//{
		//	m_state = GAME_SCENE_STATE::FADE_OUT;
		//}

		//端まで戻ってきてかつ、turnFlag が true なら次のステートへ
		if (m_actor->GetPosX() >= 130 && m_actor->GetTurnFlag() == true)
		{
			m_state = GAME_SCENE_STATE::FADE_OUT;
		}

		//	デバッグ用
		if (CheckHitKey(KEY_INPUT_RETURN))
		{
			m_state = GAME_SCENE_STATE::FADE_OUT;
		}
		break;
	case GAME_SCENE_STATE::FADE_OUT:
		if (m_fadeOutFinishFlag)
		{
			//	経過時間をリザルトに渡す
			return new Result(m_actor->countUP);	//	リザルトシーンに切り替える
		}
		break;
	default:
		break;
	}
	return this;
}

void GameScene::Draw()
{
	if (m_state == GAME_SCENE_STATE::LOAD)
	{
		m_loadingFlag++;
		if (m_loadingFlag >= LOADING_SPEED * 4)
		{
			m_loadingFlag = 0;
		}
		//	ロード画面
		if (m_loadFlag)
		{
			switch (m_tipsFlag)
			{
			case 0:
				DrawGraph(0, 0, m_tips1GraphHandle, TRUE);
			case 1:
				DrawGraph(0, 0, m_tips2GraphHandle, TRUE);
			case 2:
				DrawGraph(0, 0, m_tips3GraphHandle, TRUE);
			default:
				break;
			}
			if (!m_loadFinishFlag)
			{
				if (m_loadingFlag >= 0 && m_loadingFlag < LOADING_SPEED)
				{
					DrawFormatStringToHandle(m_mojiX, m_mojiY, GetColor(255, 255, 0), loadFontHandle, "Now Loading");
				}
				if (m_loadingFlag >= LOADING_SPEED && m_loadingFlag < LOADING_SPEED * 2)
				{
					DrawFormatStringToHandle(m_mojiX, m_mojiY, GetColor(255, 255, 0), loadFontHandle, "Now Loading.");
				}
				if (m_loadingFlag >= LOADING_SPEED * 2 && m_loadingFlag < LOADING_SPEED * 3)
				{
					DrawFormatStringToHandle(m_mojiX, m_mojiY, GetColor(255, 255, 0), loadFontHandle, "Now Loading..");
				}
				if (m_loadingFlag >= LOADING_SPEED * 3 && m_loadingFlag < LOADING_SPEED * 4)
				{
					DrawFormatStringToHandle(m_mojiX, m_mojiY, GetColor(255, 255, 0), loadFontHandle, "Now Loading...");
				}
			}
			DrawGraph(0, m_boyY, m_boyGraphHandle, TRUE);
			if (m_boyPlusFlag == true)
			{
				m_boyY++;
				if (m_boyY >= 0)
				{
					m_boyY = 0;
					m_boyPlusFlag = false;
				}
			}
			if (m_boyPlusFlag == false)
			{
				m_boyY--;
				if (m_boyY <= BOY_MIN_Y)
				{
					m_boyY = BOY_MIN_Y;
					m_boyPlusFlag = true;
				}
			}
		}

		if (m_loadFinishFlag)
		{
			DrawFormatStringToHandle(m_mojiX, m_mojiY, GetColor(255, 255, 0), loadFontHandle, "PLEASE ENTER");
		}
	}
	else
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

		//// 操作ボタン（仮）
		//if (CheckHitKey(KEY_INPUT_RIGHT))
		//{
		//	DrawBox(1050, 600, 1150, 700, GetColor(255, 255, 255), TRUE);
		//}
		//if (CheckHitKey(KEY_INPUT_LEFT))
		//{
		//	DrawBox(750, 600, 850, 700, GetColor(255, 255, 255), TRUE);
		//}
		//DrawBox(750, 600, 850, 700, GetColor(0, 0, 0), FALSE);
		//DrawBox(1050, 600, 1150, 700, GetColor(0, 0, 0), FALSE);
		//SetFontSize(100);
		//DrawFormatString(750, 600, GetColor(0, 0, 0), "←");
		//DrawFormatString(1050, 600, GetColor(0, 0, 0), "→");

		DrawBox(900, 800, 1000, 900, GetColor(0, 0, 0), FALSE);				//ボックスの表示(1つ用)
		SetFontSize(100);

		//ランダムに矢印を表示
		if (m_actor->randomKeyNumber == 1)		//ランダムに生成した数が STATE_KEY_UP(1) と同じとき
		{
			if (CheckHitKey(KEY_INPUT_UP))
			{
				DrawBox(900, 800, 1000, 900, GetColor(255, 255, 255), TRUE);
			}
			else if (CheckHitKey(KEY_INPUT_DOWN) || CheckHitKey(KEY_INPUT_RIGHT) || CheckHitKey(KEY_INPUT_LEFT))
			{
				DrawBox(900, 800, 1000, 900, GetColor(255, 0, 0), TRUE);
			}
			DrawFormatString(900, 800, GetColor(0, 0, 0), "↑");
		}
		if (m_actor->randomKeyNumber == 2)		//ランダムに生成した数が STATE_KEY_DOWN(1) と同じとき
		{
			if (CheckHitKey(KEY_INPUT_DOWN))
			{
				DrawBox(900, 800, 1000, 900, GetColor(255, 255, 255), TRUE);
			}
			else if (CheckHitKey(KEY_INPUT_UP) || CheckHitKey(KEY_INPUT_RIGHT) || CheckHitKey(KEY_INPUT_LEFT))
			{
				DrawBox(900, 800, 1000, 900, GetColor(255, 0, 0), TRUE);
			}
			DrawFormatString(900, 800, GetColor(0, 0, 0), "↓");
		}
		if (m_actor->randomKeyNumber == 3)		//ランダムに生成した数が STATE_KEY_LEFT(1) と同じとき
		{
			if (CheckHitKey(KEY_INPUT_RIGHT))
			{
				DrawBox(900, 800, 1000, 900, GetColor(255, 255, 255), TRUE);
			}
			else if (CheckHitKey(KEY_INPUT_UP) || CheckHitKey(KEY_INPUT_DOWN) || CheckHitKey(KEY_INPUT_LEFT))
			{
				DrawBox(900, 800, 1000, 900, GetColor(255, 0, 0), TRUE);
			}
			DrawFormatString(900, 800, GetColor(0, 0, 0), "→");
		}
		if (m_actor->randomKeyNumber == 4)		//ランダムに生成した数が STATE_KEY_RIGHT(1) と同じとき
		{
			if (CheckHitKey(KEY_INPUT_LEFT))
			{
				DrawBox(900, 800, 1000, 900, GetColor(255, 255, 255), TRUE);
			}
			else if (CheckHitKey(KEY_INPUT_UP) || CheckHitKey(KEY_INPUT_DOWN) || CheckHitKey(KEY_INPUT_RIGHT))
			{
				DrawBox(900, 800, 1000, 900, GetColor(255, 0, 0), TRUE);
			}
			DrawFormatString(900, 800, GetColor(0, 0, 0), "←");
		}

		//スペースキーのBOX描画
		if (-90 >= m_actor->GetPosX() && m_actor->GetPosX() > -136 && m_actor->GetTurnFlag() == false)
		{
			//αブレンドモード
			SetDrawBlendMode(DX_BLENDMODE_ALPHA, 128);

			if (CheckHitKey(KEY_INPUT_SPACE))
			{
				DrawBox(810, 900, 1100, 1000, GetColor(255, 255, 255), TRUE);
			}
			//ターンの評価が BAD の範囲
			if (-90 >= m_actor->GetPosX() && m_actor->GetPosX() > -120)
			{
				DrawBox(810, 900, 1100, 1000, GetColor(255, 0, 0), TRUE);
			}
			//ターンの評価が NORMAL の範囲
			if (-120 >= m_actor->GetPosX() && m_actor->GetPosX() > -130)
			{
				DrawBox(810, 900, 1100, 1000, GetColor(255, 255, 0), TRUE);
			}
			//ターンの評価が GOOD の範囲
			if (-130 >= m_actor->GetPosX() && m_actor->GetPosX() > -140)
			{
				DrawBox(810, 900, 1100, 1000, GetColor(0, 100, 0), TRUE);
			}

			//ブレンドモードをデフォルトに戻す
			SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255);

			DrawFormatString(820, 900, GetColor(0, 0, 0), "SPACE");
		}

		SetFontSize(35);
		// スタミナゲージの表示
		m_actor->DrawSt(m_actor->st, m_actor->MaxSt, m_actor->MinSt);
		// 残り距離の表示
		m_actor->DrawToGoal(m_actor->dCount, m_actor->maxdCount);

		// スキル
		m_actor->Skill(m_actor->dCount, m_actor->maxdCount);

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
	m_tips1GraphHandle = LoadGraph("data/img/Load/TIPS1.png");
	m_tips2GraphHandle = LoadGraph("data/img/Load/TIPS2.png");
	m_tips3GraphHandle = LoadGraph("data/img/Load/TIPS3.png");
	m_boyGraphHandle = LoadGraph("data/img/Load/boy.png");

	SetUseASyncLoadFlag(TRUE);

	// グラフィックハンドルにセット
	m_backGraphHandle = LoadGraph("data/img/Game/gameBackTest.png");		//	背景

	//	サウンドハンドルにセット
	m_bgmSoundHandle = LoadSoundMem("data/sound/Game/Game.ogg");			//	BGM

	// ステージクラスのインスタンスを生成
	m_stage = new Stage();
	// アクタークラスへのインスタンスを生成
	m_actor = new PlayerActor;
	// カメラクラスへのインスタンスを生成
	m_camera = new Camera(*m_actor);
}
