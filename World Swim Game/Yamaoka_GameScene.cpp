#include "Yamaoka_GameScene.h"
#include "Result.h"
#include "Stage.h"
#include "Yamaoka_PlayerActor.h"

#include "DxLib.h"

//	スクリーンのサイズ
const int SCREEN_SIZE_W = 1920;
const int SCREEN_SIZE_H = 1080;

// フェードイン・フェードアウトの速度
const int addAlphaVal = 5;

//	カウントダウンの速度
const int COUNTDOWN_SPEED = 50;

//	ロードするファイル数
const int LOAD_FAILE_NUM = 5;

//	TIPSの数
const int TIPS_NUM = 3;

//	LOADINGの文字の切り替わる速さ
const int LOADING_SPEED = 60;

//	タイムの評価の表示時間
const int TURN_EVA_TIME = 80;

//	星の初期位置と消える位置
const int STAR_FIRST_X = 960;
const int STAR_FIRST_Y = 750;
const int STAR_END_X = 1000;
const int STAR_END_Y = 700;

//	星の回転速度
const double STAR_ROTA_SPEED = 0.1;

//	汗の移動範囲
const int SWEAT1_END_X = 40;
const int SWEAT1_END_Y = -40;
const int SWEAT2_END_X = -40;
const int SWEAT2_END_Y = -40;

//	汗の反転量
const int SWEAT_REV = 500;

//	スタミナゲージの色が変わる残りゲージ量
const int GREEN = 300;
const int ORANGE = 150;

//	男の子の移動範囲
const int BOY_MIN_Y = -50;

//	効果音音量調整
const int SE_VOLUME_PAL = 50;

//	円周率
const double PI = 3.1415926535897932384626433832795f;

GameScene::GameScene()
	: m_alphaVal(255)
	, m_fadeOutFinishFlag(false)
	, m_loadFlag(true)
	, m_loadFinishFlag(false)
	, m_loadingFlag(0)
	, m_boyPlusFlag(false)
	, m_whistleFinishFlag(false)
	, m_startFinishFlag(false)
	, m_gameFinishFlag(false)
	, m_fadeOutFlag(false)
	, m_sweat1Flag(true)
	, m_sweat2Flag(false)
	, m_stage(nullptr)
	, m_camera(nullptr)
	, m_actor(nullptr)
	, m_mojiX(1450)
	, m_mojiY(1000)
	, m_boyY(0)
	, m_sweat1X(0)
	, m_sweat1Y(0)
	, m_sweat2X(0)
	, m_sweat2Y(0)
	, m_timeElapsed(0)
	, m_starX(STAR_FIRST_X)
	, m_starY(STAR_FIRST_Y)
	, m_starAngle(PI)
{
	m_tipsFlag = GetRand(TIPS_NUM - 1);

	// ステートセット(フェードインから)
	m_state = GAME_SCENE_STATE::LOAD;
}

GameScene::~GameScene()
{
	//	メモリの解放
	StopSoundMem(m_bgmSoundHandle);
	DeleteGraph(m_backGraphHandle);
	DeleteGraph(m_tips1GraphHandle);
	DeleteGraph(m_tips2GraphHandle);
	DeleteGraph(m_tips3GraphHandle);
	DeleteGraph(m_boyGraphHandle);
	DeleteGraph(m_starGraphHandle);
	DeleteGraph(m_sweat1GraphHandle);
	DeleteGraph(m_sweat2GraphHandle);
	DeleteSoundMem(m_bgmSoundHandle);
	DeleteSoundMem(m_whistleFinishFlag);
	DeleteSoundMem(m_countDownSoundHandle);
	DeleteSoundMem(m_goalSoundHandle);

	delete m_stage;   // ステージのポインタメンバ変数を消去
	delete m_camera;  // カメラのポインタメンバ変数を消去
	delete m_actor;   // アクターのポインタメンバ変数を削除

	m_effect->StopEffect();
	m_effect->Delete();
	delete m_effect;
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
				//	非同期読み込みを終了し、次のステートへ
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
		if (m_actor->countDown >= 0)
		{
			m_actor->countDown -= COUNTDOWN_SPEED * _deltaTime;
		}

		// ※キー入力重複対策のフラグ
		// ENTERキーから指を離したら、次のENTERの入力を有効に
		if (!CheckHitKey(KEY_INPUT_RETURN))
		{
			m_checkKeyFlag = false;
		}

		//端まで戻ってきてかつ、turnFlag が true ならゴールの文字を表示
		if (m_actor->GetPosX() >= 130 && m_actor->GetTurnFlag() == true)
		{
			m_gameFinishFlag = true;
		}

		if (m_fadeOutFlag)
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

	m_deltaTime = _deltaTime;

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
				break;
			case 1:
				DrawGraph(0, 0, m_tips2GraphHandle, TRUE);
				break;
			case 2:
				DrawGraph(0, 0, m_tips3GraphHandle, TRUE);
				break;
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

		DrawBox(900, 800, 1000, 900, GetColor(0, 0, 0), FALSE);				//ボックスの表示(1つ用)
		SetFontSize(100);

		if (m_actor->st > m_actor->MinSt && m_actor->ultLimitFlag == false)
		{
			//ランダムに矢印を表示
			switch (m_actor->randomKeyNumber)
			{
			case 1:		//ランダムに生成した数が STATE_KEY_UP(1) と同じとき
				if (m_actor->inputArrowFlag && m_actor->randomFlag)
				{
					DrawBox(900, 800, 1000, 900, GetColor(255, 255, 255), TRUE);
					DrawRotaGraph(m_starX, m_starY, 1.0, m_starAngle, m_starGraphHandle, TRUE, FALSE);
					m_starX++;
					m_starY--;
					m_starAngle += STAR_ROTA_SPEED;

					if (m_starX >= STAR_END_X)
					{
						m_starX = STAR_FIRST_X;
						m_starY = STAR_FIRST_Y;
					}

					///	エフェクトの再生
					m_effect->PlayEffekseer(m_actor->GetPos());
				}
				else if (!m_actor->randomFlag)
				{
					m_actor->inputArrowFlag = false;
					m_effect->StopEffect();
				}
				else if (CheckHitKey(KEY_INPUT_DOWN) || CheckHitKey(KEY_INPUT_RIGHT) || CheckHitKey(KEY_INPUT_LEFT))
				{
					DrawBox(900, 800, 1000, 900, GetColor(255, 0, 0), TRUE);
				}
				DrawFormatString(900, 800, GetColor(0, 0, 0), "↑");
				break;

			case 2:		//ランダムに生成した数が STATE_KEY_DOWN(2) と同じとき
				if (m_actor->inputArrowFlag && m_actor->randomFlag)
				{
					DrawBox(900, 800, 1000, 900, GetColor(255, 255, 255), TRUE);
					DrawRotaGraph(m_starX, m_starY, 1.0, m_starAngle, m_starGraphHandle, TRUE, FALSE);
					m_starX++;
					m_starY--;
					m_starAngle += STAR_ROTA_SPEED;

					if (m_starX >= STAR_END_X)
					{
						m_starX = STAR_FIRST_X;
						m_starY = STAR_FIRST_Y;
					}

					//	エフェクトの再生
					m_effect->PlayEffekseer(m_actor->GetPos());
				}
				else if (!m_actor->randomFlag)
				{
					m_actor->inputArrowFlag = false;
					m_effect->StopEffect();
				}
				else if (CheckHitKey(KEY_INPUT_UP) || CheckHitKey(KEY_INPUT_RIGHT) || CheckHitKey(KEY_INPUT_LEFT))
				{
					DrawBox(900, 800, 1000, 900, GetColor(255, 0, 0), TRUE);
				}
				DrawFormatString(900, 800, GetColor(0, 0, 0), "↓");
				break;

			case 3:		//ランダムに生成した数が STATE_KEY_RIGHT(3) と同じとき
				if (m_actor->inputArrowFlag && m_actor->randomFlag)
				{
					DrawBox(900, 800, 1000, 900, GetColor(255, 255, 255), TRUE);
					DrawRotaGraph(m_starX, m_starY, 1.0, m_starAngle, m_starGraphHandle, TRUE, FALSE);
					m_starX++;
					m_starY--;
					m_starAngle += STAR_ROTA_SPEED;

					if (m_starX >= STAR_END_X)
					{
						m_starX = STAR_FIRST_X;
						m_starY = STAR_FIRST_Y;
					}

					//	エフェクトの再生
					m_effect->PlayEffekseer(m_actor->GetPos());
				}
				else if (!m_actor->randomFlag)
				{
					m_actor->inputArrowFlag = false;
					m_effect->StopEffect();
				}
				else if (CheckHitKey(KEY_INPUT_UP) || CheckHitKey(KEY_INPUT_DOWN) || CheckHitKey(KEY_INPUT_LEFT))
				{
					DrawBox(900, 800, 1000, 900, GetColor(255, 0, 0), TRUE);
				}
				DrawFormatString(900, 800, GetColor(0, 0, 0), "→");
				break;

			case 4:		//ランダムに生成した数が STATE_KEY_LEFT(4) と同じとき
				if (m_actor->inputArrowFlag && m_actor->randomFlag)
				{
					DrawBox(900, 800, 1000, 900, GetColor(255, 255, 255), TRUE);
					DrawRotaGraph(m_starX, m_starY, 1.0, m_starAngle, m_starGraphHandle, TRUE, FALSE);
					m_starX++;
					m_starY--;
					m_starAngle += STAR_ROTA_SPEED;

					if (m_starX >= STAR_END_X)
					{
						m_starX = STAR_FIRST_X;
						m_starY = STAR_FIRST_Y;
					}

					//	エフェクトの再生
					m_effect->PlayEffekseer(m_actor->GetPos());
				}
				else if (!m_actor->randomFlag)
				{
					m_actor->inputArrowFlag = false;
					m_effect->StopEffect();
				}
				else if (CheckHitKey(KEY_INPUT_UP) || CheckHitKey(KEY_INPUT_DOWN) || CheckHitKey(KEY_INPUT_RIGHT))
				{
					DrawBox(900, 800, 1000, 900, GetColor(255, 0, 0), TRUE);
				}
				DrawFormatString(900, 800, GetColor(0, 0, 0), "←");
				break;
			default:
				break;
			}
		}

		if (m_actor->ultFlag)
		{
			switch (m_actor->arrow[0])
			{
			case 1:
				DrawFormatString(900, 800, GetColor(0, 0, 0), "↑");
				break;
			case 2:
				DrawFormatString(900, 800, GetColor(0, 0, 0), "↓");
				break;
			case 3:
				DrawFormatString(900, 800, GetColor(0, 0, 0), "→");
				break;
			case 4:
				DrawFormatString(900, 800, GetColor(0, 0, 0), "←");
				break;
			}

			switch (m_actor->arrow[1])
			{
			case 1:
				DrawFormatString(1000, 800, GetColor(0, 0, 0), "↑");
				break;
			case 2:
				DrawFormatString(1000, 800, GetColor(0, 0, 0), "↓");
				break;
			case 3:
				DrawFormatString(1000, 800, GetColor(0, 0, 0), "→");
				break;
			case 4:
				DrawFormatString(1000, 800, GetColor(0, 0, 0), "←");
				break;
			}
			switch (m_actor->arrow[2])
			{
			case 1:
				DrawFormatString(1100, 800, GetColor(0, 0, 0), "↑");
				break;
			case 2:
				DrawFormatString(1100, 800, GetColor(0, 0, 0), "↓");
				break;
			case 3:
				DrawFormatString(1100, 800, GetColor(0, 0, 0), "→");
				break;
			case 4:
				DrawFormatString(1100, 800, GetColor(0, 0, 0), "←");
				break;
			}
			switch (m_actor->arrow[3])
			{
			case 1:
				DrawFormatString(1200, 800, GetColor(0, 0, 0), "↑");
				break;
			case 2:
				DrawFormatString(1200, 800, GetColor(0, 0, 0), "↓");
				break;
			case 3:
				DrawFormatString(1200, 800, GetColor(0, 0, 0), "→");
				break;
			case 4:
				DrawFormatString(1200, 800, GetColor(0, 0, 0), "←");
				break;
			}
		}

		//	ターンの評価の表示
		if (m_actor->turnGraphFlag)
		{
			m_timeElapsed++;
			DrawGraph(m_turnEvaX, m_turnEvaY, m_actor->turnGraphHandle, TRUE);
			if (m_timeElapsed == TURN_EVA_TIME)
			{
				m_actor->turnGraphFlag = false;
			}
		}

		// 息継ぎキーの表示
		DrawBox(600, 800, 700, 900, GetColor(0, 0, 0), FALSE);				//ボックスの表示(1つ用)
		if (CheckHitKey(KEY_INPUT_C))
		{
			m_actor->inputArrowFlag = true;
			DrawBox(600, 800, 700, 900, GetColor(255, 255, 255), TRUE);
		}
		else if (CheckHitKey(KEY_INPUT_UP) || CheckHitKey(KEY_INPUT_DOWN) || CheckHitKey(KEY_INPUT_RIGHT) || CheckHitKey(KEY_INPUT_LEFT))
		{
			DrawBox(600, 800, 700, 900, GetColor(255, 0, 0), TRUE);
		}
		DrawFormatString(625, 800, GetColor(0, 0, 0), "C");

		//スペースキーのBOX描画
		if (-90 >= m_actor->GetPosX() && m_actor->GetPosX() > -136 && m_actor->GetInputSpaceFlag() == false && m_actor->GetTurnFlag() == false)
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
		m_actor->LastSpurt();

		//	スタミナ減少時の汗の表示
		if (m_actor->st <= ORANGE)
		{
			if (!m_actor->turnFlag)
			{
				if (m_sweat1Flag)
				{
					DrawGraph(m_sweat1X, m_sweat1Y, m_sweat1GraphHandle, TRUE);
					m_sweat1X++;
					m_sweat1Y--;
					if (m_sweat1X > SWEAT1_END_X)
					{
						m_sweat1X = 0;
						m_sweat1Flag = false;
					}
					if (m_sweat1Y < SWEAT1_END_Y)
					{
						m_sweat1Y = 0;
						m_sweat1Flag = false;
					}
					if (m_sweat1X > SWEAT1_END_X / 2)
					{
						m_sweat2Flag = true;
					}
				}
				if (m_sweat2Flag)
				{
					DrawGraph(m_sweat2X, m_sweat2Y, m_sweat2GraphHandle, TRUE);
					m_sweat2X--;
					m_sweat2Y--;
					if (m_sweat2X < SWEAT2_END_X)
					{
						m_sweat2X = 0;
						m_sweat2Flag = false;
						m_sweat1Flag = true;
					}
					if (m_sweat2Y < SWEAT2_END_Y)
					{
						m_sweat2Y = 0;
						m_sweat2Flag = false;
						m_sweat1Flag = true;
					}
				}
			}
			if (m_actor->turnFlag)
			{
				if (m_sweat1Flag)
				{
					DrawGraph(m_sweat1X - SWEAT_REV, m_sweat1Y, m_sweat1GraphHandle, TRUE);
					m_sweat1X++;
					m_sweat1Y--;
					if (m_sweat1X > SWEAT1_END_X)
					{
						m_sweat1X = 0;
						m_sweat1Flag = false;
					}
					if (m_sweat1Y < SWEAT1_END_Y)
					{
						m_sweat1Y = 0;
						m_sweat1Flag = false;
					}
					if (m_sweat1X > SWEAT1_END_X / 2)
					{
						m_sweat2Flag = true;
					}
				}
				if (m_sweat2Flag)
				{
					DrawGraph(m_sweat2X - SWEAT_REV, m_sweat2Y, m_sweat2GraphHandle, TRUE);
					m_sweat2X--;
					m_sweat2Y--;
					if (m_sweat2X < SWEAT2_END_X)
					{
						m_sweat2X = 0;
						m_sweat2Flag = false;
						m_sweat1Flag = true;
					}
					if (m_sweat2Y < SWEAT2_END_Y)
					{
						m_sweat2Y = 0;
						m_sweat2Flag = false;
						m_sweat1Flag = true;
					}
				}
			}
		}

		// カウントダウンの表示
		if (m_actor->countDown > 0 && m_actor->countDown <= 150)
		{
			SetFontSize(150);
			DrawFormatString(800, 400, GetColor(0, 0, 0), " %d ", m_actor->countDown / 60 + 1);
		}

		//	START!!の表示
		if (m_actor->countDown <= 0 && !m_startFinishFlag)
		{
			DrawFormatStringToHandle(700, 400, GetColor(255, 0, 0), startHandle, "START!!");
			m_startCount++;
			if (m_startCount >= 50)
			{
				m_startFinishFlag = true;
			}
		}

		//	GOAL!!の表示
		if (m_gameFinishFlag)
		{
			DrawFormatStringToHandle(700, 400, GetColor(255, 0, 0), startHandle, "GOAL!!");
			m_finishCount++;
			if (m_startCount >= 50)
			{
				m_fadeOutFlag = true;
			}
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
	if (m_state == GAME_SCENE_STATE::GAME && !m_whistleFinishFlag)
	{
		PlaySoundMem(m_countDownSoundHandle, DX_PLAYTYPE_BACK, FALSE);
		ChangeVolumeSoundMem(m_volumePal + SE_VOLUME_PAL, m_countDownSoundHandle);
	}
	//	カウントダウン終了後、笛を鳴らす
	if (m_actor->countDownFinishFlag)
	{
		PlaySoundMem(m_whistleSoundHandle, DX_PLAYTYPE_BACK, FALSE);
		ChangeVolumeSoundMem(m_volumePal + SE_VOLUME_PAL, m_whistleSoundHandle);
		m_actor->countDownFinishFlag = false;
		m_whistleFinishFlag = true;
	}
	//	笛が鳴り終わったら、BGMを流す
	if (m_state == GAME_SCENE_STATE::GAME && m_whistleFinishFlag)
	{
		PlaySoundMem(m_bgmSoundHandle, DX_PLAYTYPE_BACK, FALSE);
		ChangeVolumeSoundMem(m_volumePal, m_bgmSoundHandle);
	}
	if (m_gameFinishFlag)
	{
		PlaySoundMem(m_goalSoundHandle, DX_PLAYTYPE_BACK, FALSE);
		ChangeVolumeSoundMem(m_volumePal + SE_VOLUME_PAL, m_goalSoundHandle);
	}
}

void GameScene::Load()
{
	// グラフィックハンドルにセット
	m_tips1GraphHandle = LoadGraph("data/img/Load/TIPS1.png");
	m_tips2GraphHandle = LoadGraph("data/img/Load/TIPS2.png");
	m_tips3GraphHandle = LoadGraph("data/img/Load/TIPS3.png");
	m_boyGraphHandle = LoadGraph("data/img/Load/boy.png");

	//	エフェクト生成
	m_effect = new PlayEffect("data/effects/water4.efk", 3.0f);

	SetUseASyncLoadFlag(TRUE);

	// グラフィックハンドルにセット
	m_backGraphHandle = LoadGraph("data/img/Game/gameBackTest.png");			//	背景
	m_starGraphHandle = LoadGraph("data/img/Game/star.png");					//	星
	m_sweat1GraphHandle = LoadGraph("data/img/Game/Sweat1.png");				//	汗1
	m_sweat2GraphHandle = LoadGraph("data/img/Game/Sweat2.png");				//	汗2

	//	サウンドハンドルにセット
	m_bgmSoundHandle = LoadSoundMem("data/sound/Game/Game.ogg");				//	BGM
	m_whistleSoundHandle = LoadSoundMem("data/sound/Game/whistle.ogg");			//	笛
	m_countDownSoundHandle = LoadSoundMem("data/sound/Game/Countdown2.ogg");	//	カウントダウン
	m_goalSoundHandle = LoadSoundMem("data/sound/Game/goalWhistle.ogg");		//	ゴール

	// ステージクラスのインスタンスを生成
	m_stage = new Stage();
	// アクタークラスへのインスタンスを生成
	m_actor = new Yamaoka_PlayerActor;
	// カメラクラスへのインスタンスを生成
	m_camera = new Camera(*m_actor);
}
