#include "Ueyama_GameScene.h"
#include "Ueyama_Result.h"
#include "Stage.h"


#include "DxLib.h"

//-----------------------------------------------------------------------------
//	変更点
//	UI追加（ハート、矢印、スペース、その他もろもろ）
//-----------------------------------------------------------------------------


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

//	矢印の中心座標
const int ARROW_CENTER_X = 990;
const int ARROW_CENTER_Y = 370;

//	矢印の座標調整
const int ARROW_ADJUST_X = 28;
const int ARROW_ADJUST_Y = -170;

//	ラストスパートでの矢印の位置調整
const int FIRST_ARROW_X = -210;
const int SECOND_ARROW_X = -70;
const int THIRD_ARROW_X = 70;
const int FORTH_ARROW_X = 210;

//	スタミナゲージの色が変わる残りゲージ量
const int GREEN = 300;
const int ORANGE = 150;

//	男の子の移動範囲
const int BOY_MIN_Y = -50;

//	タイムの表示
const int TIME_X = 1697;
const int TIME_Y = 970;

//	ハートの表示場所
const int FIRST_HEART_X = 535;
const int FIRST_HEART_Y = 70;

//	ハートの鼓動の速度
const int HEART_SPEED_1 = 0.01;
const int HEART_SPEED_2 = 0.04;
const int HEART_SPEED_3 = 0.07;
const int HEART_SPEED_4 = 0.1;

//	ハートの拡大率の最大と最小
const double HEART_MAX_RATE = 2.0;
const double HEART_MIN_RATE = 0.8;

//	スタミナゲージの表示位置
const int ST_FIRST_X = 665;
const int ST_FIRST_Y = 80;
const int ST_END_X = 1250;
const int ST_END_Y = 1035;
const int ST_HEIGHT = 65;

//	効果音音量調整
const int SE_VOLUME_PAL = 50;

//	円周率
const double PI = 3.1415926535897932384626433832795f;

Ueyama_GameScene::Ueyama_GameScene()
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
	, m_heartFlag(false)
	, m_lastFlag(false)
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
	, m_heartRate(1.0)
	, m_heartSpeed1(HEART_SPEED_1)
	, m_heartSpeed2(HEART_SPEED_2)
	, m_arrowRate(1.0)
	, m_arrowAngle(0)
	, m_arrow2Angle(0)
	, m_arrow3Angle(0)
	, m_arrow4Angle(0)
{
	m_tipsFlag = GetRand(TIPS_NUM - 1);

	// ステートセット(フェードインから)
	m_state = GAME_SCENE_STATE::LOAD;
}

Ueyama_GameScene::~Ueyama_GameScene()
{
	//	メモリの解放
	StopSoundMem(m_bgmSoundHandle);
	DeleteGraph(m_backGraphHandle);
	DeleteGraph(m_tips1GraphHandle);
	DeleteGraph(m_tips2GraphHandle);
	DeleteGraph(m_tips3GraphHandle);
	DeleteGraph(m_boyGraphHandle);
	DeleteGraph(m_boy2GraphHandle);
	DeleteGraph(m_starGraphHandle);
	DeleteGraph(m_sweat1GraphHandle);
	DeleteGraph(m_sweat2GraphHandle);
	DeleteGraph(m_heartGraphHandle);
	DeleteGraph(m_spaceGraphHandle);
	DeleteGraph(m_arrowBaseGraphHandle);
	DeleteGraph(m_scoreBaseGraphHandle);
	DeleteGraph(m_spaceBaseGraphHandle);
	DeleteGraph(m_upArrowGraphHandle);
	DeleteGraph(m_rightArrowGraphHandle);
	DeleteGraph(m_downArrowGraphHandle);
	DeleteGraph(m_leftArrowGraphHandle);
	DeleteSoundMem(m_bgmSoundHandle);
	DeleteSoundMem(m_whistleFinishFlag);
	DeleteSoundMem(m_countDownSoundHandle);
	DeleteSoundMem(m_goalSoundHandle);

	delete m_stage;   // ステージのポインタメンバ変数を消去
	delete m_camera;  // カメラのポインタメンバ変数を消去
	delete m_actor;   // アクターのポインタメンバ変数を削除
	delete m_font;

	m_effect->StopEffect();
	m_effect->Delete();
	delete m_effect;
}

SceneBase* Ueyama_GameScene::Update(float _deltaTime)
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
			return new Ueyama_Result(m_actor->countUP);	//	リザルトシーンに切り替える
		}
		break;
	default:
		break;
	}

	m_deltaTime = _deltaTime;

	return this;
}

void Ueyama_GameScene::Draw()
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
					DrawFormatStringToHandle(m_mojiX, m_mojiY, GetColor(255, 255, 0), m_font->loadFontHandle, "Now Loading");
				}
				if (m_loadingFlag >= LOADING_SPEED && m_loadingFlag < LOADING_SPEED * 2)
				{
					DrawFormatStringToHandle(m_mojiX, m_mojiY, GetColor(255, 255, 0), m_font->loadFontHandle, "Now Loading.");
				}
				if (m_loadingFlag >= LOADING_SPEED * 2 && m_loadingFlag < LOADING_SPEED * 3)
				{
					DrawFormatStringToHandle(m_mojiX, m_mojiY, GetColor(255, 255, 0), m_font->loadFontHandle, "Now Loading..");
				}
				if (m_loadingFlag >= LOADING_SPEED * 3 && m_loadingFlag < LOADING_SPEED * 4)
				{
					DrawFormatStringToHandle(m_mojiX, m_mojiY, GetColor(255, 255, 0), m_font->loadFontHandle, "Now Loading...");
				}
				DrawGraph(0, m_boyY, m_boyGraphHandle, TRUE);
			}
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
			DrawGraph(0, m_boyY, m_boy2GraphHandle, TRUE);
			DrawFormatStringToHandle(m_mojiX, m_mojiY, GetColor(255, 255, 0), m_font->loadFontHandle, "PLEASE ENTER");
		}
	}
	else
	{
		//	背景
		DrawGraph(0, 0, m_backGraphHandle, TRUE);

		// ステージの描画
		m_stage->Draw();
		//SetFontSize(40);

		if (!m_actor->ultFlag)
		{
			//	矢印の土台の描画
			DrawGraph(0, 0, m_arrowBaseGraphHandle, TRUE);
		}

		if (m_actor->st > ORANGE)
		{
			m_heartSpeed1 = HEART_SPEED_2;
			m_heartSpeed2 = -HEART_SPEED_1;
		}
		else if (m_actor->st <= ORANGE)
		{
			m_heartSpeed1 = HEART_SPEED_3;
			m_heartSpeed2 = -HEART_SPEED_2;
		}
		else if (m_actor->st <= ORANGE / 2)
		{
			m_heartSpeed1 = HEART_SPEED_4;
			m_heartSpeed2 = -HEART_SPEED_3;
		}

		//	ハートの表示
		DrawRotaGraph(SCREEN_SIZE_W / 2, SCREEN_SIZE_H / 2, m_heartRate, 0, m_heartGraphHandle, TRUE, FALSE);

		if (m_heartFlag)
		{
			m_heartSpeed1 = m_heartSpeed2;
		}

		m_heartRate += m_heartSpeed1;

		if (m_heartRate >= HEART_MAX_RATE)
		{
			m_heartRate = HEART_MAX_RATE;
			m_heartFlag = true;
		}
		else if (m_heartRate <= HEART_MIN_RATE)
		{
			m_heartRate = HEART_MIN_RATE;
			m_heartFlag = false;
		}

		//	タイムの表示
		//DrawBox(1550, 830, 1850, 880, GetColor(255, 255, 0), TRUE);
		//DrawFormatString(1600, 835, GetColor(0, 0, 0), "TIME   %d", m_actor->countUP);

		//	タイムの表示
		DrawGraph(0, 0, m_scoreBaseGraphHandle, TRUE);		//	タイムと距離の土台
		DrawFormatStringToHandle(TIME_X, TIME_Y, GetColor(255, 255, 255), m_font->gameSceneScoreHandle, "%d", m_actor->countUP);		//	タイムの表示

		//	スタミナの表示
		DrawGraph(0, 0, m_stBaseGraphHandle, TRUE);
		DrawRectGraph(ST_FIRST_X, ST_FIRST_Y, ST_FIRST_X, ST_FIRST_Y, m_actor->st, ST_HEIGHT, m_stGraphHandle, TRUE, FALSE);
		//DrawRectGraph(0, 0, 0, 0, SCREEN_SIZE_W, SCREEN_SIZE_H, m_stGraphHandle, TRUE, FALSE);


		// プレイヤー描画
		m_actor->DrawActor();

		//DrawBox(900, 800, 1000, 900, GetColor(0, 0, 0), FALSE);				//ボックスの表示(1つ用)
		//SetFontSize(100);

		if (m_actor->st > m_actor->MinSt && m_actor->ultLimitFlag == false)
		{
			//ランダムに矢印を表示
			switch (m_actor->randomKeyNumber)
			{
			case 1:		//ランダムに生成した数が STATE_KEY_UP(1) と同じとき
				if (m_actor->inputArrowFlag && m_actor->randomFlag)
				{
					//DrawBox(900, 800, 1000, 900, GetColor(255, 255, 255), TRUE);
					/*DrawRotaGraph(m_starX, m_starY, 1.0, m_starAngle, m_starGraphHandle, TRUE, FALSE);
					m_starX++;
					m_starY--;
					m_starAngle += STAR_ROTA_SPEED;*/

					/*if (m_starX >= STAR_END_X)
					{
						m_starX = STAR_FIRST_X;
						m_starY = STAR_FIRST_Y;
					}*/

					//	矢印を回す
					m_arrowAngle++;

					///	エフェクトの再生
					m_effect->PlayEffekseer(m_actor->GetPos());
				}
				else if (!m_actor->randomFlag)
				{
					m_actor->inputArrowFlag = false;
					m_arrowAngle = 0;
					m_effect->StopEffect();
				}
				else if (CheckHitKey(KEY_INPUT_DOWN) || CheckHitKey(KEY_INPUT_RIGHT) || CheckHitKey(KEY_INPUT_LEFT))
				{
					//DrawBox(900, 800, 1000, 900, GetColor(255, 0, 0), TRUE);
				}
				//DrawFormatString(900, 800, GetColor(0, 0, 0), "↑");
				DrawRotaGraph2(SCREEN_SIZE_W / 2 + ARROW_ADJUST_X, SCREEN_SIZE_H / 2 + ARROW_ADJUST_Y, ARROW_CENTER_X, ARROW_CENTER_Y, m_arrowRate, m_arrowAngle, m_upArrowGraphHandle, TRUE, FALSE);
				break;

			case 2:		//ランダムに生成した数が STATE_KEY_DOWN(2) と同じとき
				if (m_actor->inputArrowFlag && m_actor->randomFlag)
				{
					//DrawBox(900, 800, 1000, 900, GetColor(255, 255, 255), TRUE);
					/*DrawRotaGraph(m_starX, m_starY, 1.0, m_starAngle, m_starGraphHandle, TRUE, FALSE);
					m_starX++;
					m_starY--;
					m_starAngle += STAR_ROTA_SPEED;

					if (m_starX >= STAR_END_X)
					{
						m_starX = STAR_FIRST_X;
						m_starY = STAR_FIRST_Y;
					}*/

					//	矢印を回す
					m_arrowAngle++;

					//	エフェクトの再生
					m_effect->PlayEffekseer(m_actor->GetPos());
				}
				else if (!m_actor->randomFlag)
				{
					m_actor->inputArrowFlag = false;
					m_arrowAngle = 0;
					m_effect->StopEffect();
				}
				else if (CheckHitKey(KEY_INPUT_UP) || CheckHitKey(KEY_INPUT_RIGHT) || CheckHitKey(KEY_INPUT_LEFT))
				{
					//DrawBox(900, 800, 1000, 900, GetColor(255, 0, 0), TRUE);
				}
				//DrawFormatString(900, 800, GetColor(0, 0, 0), "↓");
				DrawRotaGraph2(SCREEN_SIZE_W / 2 + ARROW_ADJUST_X, SCREEN_SIZE_H / 2 + ARROW_ADJUST_Y, ARROW_CENTER_X, ARROW_CENTER_Y, m_arrowRate, m_arrowAngle, m_downArrowGraphHandle, TRUE, FALSE);
				break;

			case 3:		//ランダムに生成した数が STATE_KEY_RIGHT(3) と同じとき
				if (m_actor->inputArrowFlag && m_actor->randomFlag)
				{
					////DrawBox(900, 800, 1000, 900, GetColor(255, 255, 255), TRUE);
					//DrawRotaGraph(m_starX, m_starY, 1.0, m_starAngle, m_starGraphHandle, TRUE, FALSE);
					//m_starX++;
					//m_starY--;
					//m_starAngle += STAR_ROTA_SPEED;

					//if (m_starX >= STAR_END_X)
					//{
					//	m_starX = STAR_FIRST_X;
					//	m_starY = STAR_FIRST_Y;
					//}

					//	矢印を回す
					m_arrowAngle++;

					//	エフェクトの再生
					m_effect->PlayEffekseer(m_actor->GetPos());
				}
				else if (!m_actor->randomFlag)
				{
					m_actor->inputArrowFlag = false;
					m_arrowAngle = 0;
					m_effect->StopEffect();
				}
				else if (CheckHitKey(KEY_INPUT_UP) || CheckHitKey(KEY_INPUT_DOWN) || CheckHitKey(KEY_INPUT_LEFT))
				{
					//DrawBox(900, 800, 1000, 900, GetColor(255, 0, 0), TRUE);
				}
				//DrawFormatString(900, 800, GetColor(0, 0, 0), "→");
				DrawRotaGraph2(SCREEN_SIZE_W / 2 + ARROW_ADJUST_X, SCREEN_SIZE_H / 2 + ARROW_ADJUST_Y, ARROW_CENTER_X, ARROW_CENTER_Y, m_arrowRate, m_arrowAngle, m_rightArrowGraphHandle, TRUE, FALSE);
				break;

			case 4:		//ランダムに生成した数が STATE_KEY_LEFT(4) と同じとき
				if (m_actor->inputArrowFlag && m_actor->randomFlag)
				{
					//DrawBox(900, 800, 1000, 900, GetColor(255, 255, 255), TRUE);
				/*	DrawRotaGraph(m_starX, m_starY, 1.0, m_starAngle, m_starGraphHandle, TRUE, FALSE);
					m_starX++;
					m_starY--;
					m_starAngle += STAR_ROTA_SPEED;

					if (m_starX >= STAR_END_X)
					{
						m_starX = STAR_FIRST_X;
						m_starY = STAR_FIRST_Y;
					}*/

					//	矢印を回す
					m_arrowAngle++;

					//	エフェクトの再生
					m_effect->PlayEffekseer(m_actor->GetPos());
				}
				else if (!m_actor->randomFlag)
				{
					m_actor->inputArrowFlag = false;
					m_arrowAngle = 0;
					m_effect->StopEffect();
				}
				else if (CheckHitKey(KEY_INPUT_UP) || CheckHitKey(KEY_INPUT_DOWN) || CheckHitKey(KEY_INPUT_RIGHT))
				{
					//DrawBox(900, 800, 1000, 900, GetColor(255, 0, 0), TRUE);
				}
				//DrawFormatString(900, 800, GetColor(0, 0, 0), "←");
				DrawRotaGraph2(SCREEN_SIZE_W / 2 + ARROW_ADJUST_X, SCREEN_SIZE_H / 2 + ARROW_ADJUST_Y, ARROW_CENTER_X, ARROW_CENTER_Y, m_arrowRate, m_arrowAngle, m_leftArrowGraphHandle, TRUE, FALSE);
				break;
			default:
				break;
			}
		}

		if (m_actor->ultFlag)
		{
			if (!m_lastFlag)
			{
				m_arrowAngle = 0;
				m_lastFlag = true;
			}
			switch (m_actor->arrow[0])
			{
			case 1:
				DrawRotaGraph2(SCREEN_SIZE_W / 2 + ARROW_ADJUST_X + FIRST_ARROW_X, SCREEN_SIZE_H / 2 + ARROW_ADJUST_Y, ARROW_CENTER_X, ARROW_CENTER_Y, m_arrowRate, m_arrowAngle, m_upArrowGraphHandle, TRUE, FALSE);
				m_last1SucFlag = m_actor->ultSucFlag;
				break;
			case 2:
				DrawRotaGraph2(SCREEN_SIZE_W / 2 + ARROW_ADJUST_X + FIRST_ARROW_X, SCREEN_SIZE_H / 2 + ARROW_ADJUST_Y, ARROW_CENTER_X, ARROW_CENTER_Y, m_arrowRate, m_arrowAngle, m_downArrowGraphHandle, TRUE, FALSE);
				m_last1SucFlag = m_actor->ultSucFlag;
				break;
			case 3:
				DrawRotaGraph2(SCREEN_SIZE_W / 2 + ARROW_ADJUST_X + FIRST_ARROW_X, SCREEN_SIZE_H / 2 + ARROW_ADJUST_Y, ARROW_CENTER_X, ARROW_CENTER_Y, m_arrowRate, m_arrowAngle, m_rightArrowGraphHandle, TRUE, FALSE);
				m_last1SucFlag = m_actor->ultSucFlag;
				break;
			case 4:
				DrawRotaGraph2(SCREEN_SIZE_W / 2 + ARROW_ADJUST_X + FIRST_ARROW_X, SCREEN_SIZE_H / 2 + ARROW_ADJUST_Y, ARROW_CENTER_X, ARROW_CENTER_Y, m_arrowRate, m_arrowAngle, m_leftArrowGraphHandle, TRUE, FALSE);
				m_last1SucFlag = m_actor->ultSucFlag;
				break;
			}

			switch (m_actor->arrow[1])
			{
			case 1:
				DrawRotaGraph2(SCREEN_SIZE_W / 2 + ARROW_ADJUST_X + SECOND_ARROW_X, SCREEN_SIZE_H / 2 + ARROW_ADJUST_Y, ARROW_CENTER_X, ARROW_CENTER_Y, m_arrowRate, m_arrow2Angle, m_upArrowGraphHandle, TRUE, FALSE);
				m_last2SucFlag = m_actor->ultSucFlag;
				break;
			case 2:
				DrawRotaGraph2(SCREEN_SIZE_W / 2 + ARROW_ADJUST_X + SECOND_ARROW_X, SCREEN_SIZE_H / 2 + ARROW_ADJUST_Y, ARROW_CENTER_X, ARROW_CENTER_Y, m_arrowRate, m_arrow2Angle, m_downArrowGraphHandle, TRUE, FALSE);
				m_last2SucFlag = m_actor->ultSucFlag;
				break;
			case 3:
				DrawRotaGraph2(SCREEN_SIZE_W / 2 + ARROW_ADJUST_X + SECOND_ARROW_X, SCREEN_SIZE_H / 2 + ARROW_ADJUST_Y, ARROW_CENTER_X, ARROW_CENTER_Y, m_arrowRate, m_arrow2Angle, m_rightArrowGraphHandle, TRUE, FALSE);
				m_last2SucFlag = m_actor->ultSucFlag;
				break;
			case 4:
				DrawRotaGraph2(SCREEN_SIZE_W / 2 + ARROW_ADJUST_X + SECOND_ARROW_X, SCREEN_SIZE_H / 2 + ARROW_ADJUST_Y, ARROW_CENTER_X, ARROW_CENTER_Y, m_arrowRate, m_arrow2Angle, m_leftArrowGraphHandle, TRUE, FALSE);
				m_last2SucFlag = m_actor->ultSucFlag;
				break;
			}
			switch (m_actor->arrow[2])
			{
			case 1:
				DrawRotaGraph2(SCREEN_SIZE_W / 2 + ARROW_ADJUST_X + THIRD_ARROW_X, SCREEN_SIZE_H / 2 + ARROW_ADJUST_Y, ARROW_CENTER_X, ARROW_CENTER_Y, m_arrowRate, m_arrow3Angle, m_upArrowGraphHandle, TRUE, FALSE);
				m_last3SucFlag = m_actor->ultSucFlag;
				break;
			case 2:
				DrawRotaGraph2(SCREEN_SIZE_W / 2 + ARROW_ADJUST_X + THIRD_ARROW_X, SCREEN_SIZE_H / 2 + ARROW_ADJUST_Y, ARROW_CENTER_X, ARROW_CENTER_Y, m_arrowRate, m_arrow3Angle, m_downArrowGraphHandle, TRUE, FALSE);
				m_last3SucFlag = m_actor->ultSucFlag;
				break;
			case 3:
				DrawRotaGraph2(SCREEN_SIZE_W / 2 + ARROW_ADJUST_X + THIRD_ARROW_X, SCREEN_SIZE_H / 2 + ARROW_ADJUST_Y, ARROW_CENTER_X, ARROW_CENTER_Y, m_arrowRate, m_arrow3Angle, m_rightArrowGraphHandle, TRUE, FALSE);
				m_last3SucFlag = m_actor->ultSucFlag;
				break;
			case 4:
				DrawRotaGraph2(SCREEN_SIZE_W / 2 + ARROW_ADJUST_X + THIRD_ARROW_X, SCREEN_SIZE_H / 2 + ARROW_ADJUST_Y, ARROW_CENTER_X, ARROW_CENTER_Y, m_arrowRate, m_arrow3Angle, m_leftArrowGraphHandle, TRUE, FALSE);
				m_last3SucFlag = m_actor->ultSucFlag;
				break;
			}
			switch (m_actor->arrow[3])
			{
				case 1:
				DrawRotaGraph2(SCREEN_SIZE_W / 2 + ARROW_ADJUST_X + FORTH_ARROW_X, SCREEN_SIZE_H / 2 + ARROW_ADJUST_Y, ARROW_CENTER_X, ARROW_CENTER_Y, m_arrowRate, m_arrow4Angle, m_upArrowGraphHandle, TRUE, FALSE);
				m_last4SucFlag = m_actor->ultSucFlag;
				break;
			case 2:
				DrawRotaGraph2(SCREEN_SIZE_W / 2 + ARROW_ADJUST_X + FORTH_ARROW_X, SCREEN_SIZE_H / 2 + ARROW_ADJUST_Y, ARROW_CENTER_X, ARROW_CENTER_Y, m_arrowRate, m_arrow4Angle, m_downArrowGraphHandle, TRUE, FALSE);
				m_last4SucFlag = m_actor->ultSucFlag;
				break;
			case 3:
				DrawRotaGraph2(SCREEN_SIZE_W / 2 + ARROW_ADJUST_X + FORTH_ARROW_X, SCREEN_SIZE_H / 2 + ARROW_ADJUST_Y, ARROW_CENTER_X, ARROW_CENTER_Y, m_arrowRate, m_arrow4Angle, m_rightArrowGraphHandle, TRUE, FALSE);
				m_last4SucFlag = m_actor->ultSucFlag;
				break;
			case 4:
				DrawRotaGraph2(SCREEN_SIZE_W / 2 + ARROW_ADJUST_X + FORTH_ARROW_X, SCREEN_SIZE_H / 2 + ARROW_ADJUST_Y, ARROW_CENTER_X, ARROW_CENTER_Y, m_arrowRate, m_arrow4Angle, m_leftArrowGraphHandle, TRUE, FALSE);
				m_last4SucFlag = m_actor->ultSucFlag;
				break;
			}

			if (m_last1SucFlag)
			{
				m_arrowAngle++;
			}
			else if (!m_last1SucFlag)
			{

			}
			if (m_last2SucFlag)
			{
				m_arrow2Angle++;
			}
			else if (!m_last1SucFlag)
			{

			}
			if (m_last3SucFlag)
			{
				m_arrow3Angle++;
			}
			else if (!m_last1SucFlag)
			{

			}
			if (m_last4SucFlag)
			{
				m_arrow4Angle++;
			}
			else if (!m_last1SucFlag)
			{

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

		//// 息継ぎキーの表示
		//DrawBox(600, 800, 700, 900, GetColor(0, 0, 0), FALSE);				//ボックスの表示(1つ用)
		DrawGraph(0, 0, m_breathGraphHandle, TRUE);
		if (CheckHitKey(KEY_INPUT_C))
		{
			m_actor->inputArrowFlag = true;
			//DrawBox(600, 800, 700, 900, GetColor(255, 255, 255), TRUE);
		}
		/*else if (CheckHitKey(KEY_INPUT_UP) || CheckHitKey(KEY_INPUT_DOWN) || CheckHitKey(KEY_INPUT_RIGHT) || CheckHitKey(KEY_INPUT_LEFT))
		{
			DrawBox(600, 800, 700, 900, GetColor(255, 0, 0), TRUE);
		}*/
		//DrawFormatString(625, 800, GetColor(0, 0, 0), "C");

		//スペースキーのBOX描画
		if (-90 >= m_actor->GetPosX() && m_actor->GetPosX() > -136 && m_actor->GetInputSpaceFlag() == false && m_actor->GetTurnFlag() == false)
		{
			//αブレンドモード
			//SetDrawBlendMode(DX_BLENDMODE_ALPHA, 128);

			DrawGraph(0, 0, m_spaceBaseGraphHandle, TRUE);

			/*if (CheckHitKey(KEY_INPUT_SPACE))
			{
				DrawBox(810, 900, 1100, 1000, GetColor(255, 255, 255), TRUE);
			}*/
			////ターンの評価が BAD の範囲
			//if (-90 >= m_actor->GetPosX() && m_actor->GetPosX() > -120)
			//{
			//	DrawBox(810, 900, 1100, 1000, GetColor(255, 0, 0), TRUE);
			//}
			//ターンの評価が NORMAL の範囲
			if (-120 >= m_actor->GetPosX() && m_actor->GetPosX() > -130 || -130 >= m_actor->GetPosX() && m_actor->GetPosX() > -140)
			{
				////DrawBox(810, 900, 1100, 1000, GetColor(255, 255, 0), TRUE);
				DrawGraph(0, 0, m_spaceGraphHandle, TRUE);
			}
			////ターンの評価が GOOD の範囲
			//if (-130 >= m_actor->GetPosX() && m_actor->GetPosX() > -140)
			//{
			//	DrawBox(810, 900, 1100, 1000, GetColor(0, 100, 0), TRUE);
			//}

			//ブレンドモードをデフォルトに戻す
			//SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255);

			//DrawFormatString(820, 900, GetColor(0, 0, 0), "SPACE");
		}

		//SetFontSize(35);
		// スタミナゲージの表示
		//m_actor->DrawSt(m_actor->st, m_actor->MaxSt, m_actor->MinSt);
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
			//SetFontSize(150);
			DrawFormatStringToHandle(800, 400, GetColor(0, 0, 0),m_font->startHandle, " %d ", m_actor->countDown / 60 + 1);
		}

		//	START!!の表示
		if (m_actor->countDown <= 0 && !m_startFinishFlag)
		{
			DrawFormatStringToHandle(700, 400, GetColor(255, 0, 0), m_font->startHandle, "START!!");
			m_startCount++;
			if (m_startCount >= 50)
			{
				m_startFinishFlag = true;
			}
		}

		//	GOAL!!の表示
		if (m_gameFinishFlag)
		{
			DrawFormatStringToHandle(700, 400, GetColor(255, 0, 0), m_font->startHandle, "GOAL!!");
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

void Ueyama_GameScene::Sound()
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

void Ueyama_GameScene::Load()
{
	//	非同期読み込みしないファイル

	// グラフィックハンドルにセット
	m_tips1GraphHandle = LoadGraph("data/img/Load/TIPS1.png");
	m_tips2GraphHandle = LoadGraph("data/img/Load/TIPS2.png");
	m_tips3GraphHandle = LoadGraph("data/img/Load/TIPS3.png");
	m_boyGraphHandle = LoadGraph("data/img/Load/boy.png");

	//	エフェクト生成
	m_effect = new PlayEffect("data/effects/water4.efk", 3.0f);

	//	フォントクラスへのインスタンスを生成
	m_font = new Font();

	//	非同期読み込み開始
	SetUseASyncLoadFlag(TRUE);

	// グラフィックハンドルにセット
	m_backGraphHandle = LoadGraph("data/img/Game/gameBackTest.png");			//	背景
	m_starGraphHandle = LoadGraph("data/img/Game/star.png");					//	星
	m_sweat1GraphHandle = LoadGraph("data/img/Game/Sweat1.png");				//	汗1
	m_sweat2GraphHandle = LoadGraph("data/img/Game/Sweat2.png");				//	汗2
	m_boy2GraphHandle = LoadGraph("data/img/Load/boy2.png");					//	男の子2
	m_heartGraphHandle = LoadGraph("data/img/Game/Game_heart.png");				//	ハート
	m_upArrowGraphHandle = LoadGraph("data/img/Game/Game_up.png");				//	上矢印
	m_rightArrowGraphHandle = LoadGraph("data/img/Game/Game_right.png");		//	右矢印
	m_downArrowGraphHandle = LoadGraph("data/img/Game/Game_down.png");			//	下矢印
	m_leftArrowGraphHandle = LoadGraph("data/img/Game/Game_left.png");			//	左矢印
	m_spaceGraphHandle = LoadGraph("data/img/Game/Game_space.png");				//	スペース
	m_stGraphHandle = LoadGraph("data/img/Game/Game_st.png");					//	スタミナ
	m_breathGraphHandle = LoadGraph("data/img/Game/Game_breath.png");			//	息継ぎ
	m_arrowBaseGraphHandle = LoadGraph("data/img/Game/Game_arrowBase.png");		//	矢印の土台
	m_scoreBaseGraphHandle = LoadGraph("data/img/Game/Game_score.png");			//	スコアの土台
	m_spaceBaseGraphHandle = LoadGraph("data/img/Game/Game_spaceBase.png");		//	スペースの土台
	m_stBaseGraphHandle = LoadGraph("data/img/Game/Game_stBase.png");			//	スタミナの土台

	//	サウンドハンドルにセット
	m_bgmSoundHandle = LoadSoundMem("data/sound/Game/Game.ogg");				//	BGM
	m_whistleSoundHandle = LoadSoundMem("data/sound/Game/whistle.ogg");			//	笛
	m_countDownSoundHandle = LoadSoundMem("data/sound/Game/Countdown2.ogg");	//	カウントダウン
	m_goalSoundHandle = LoadSoundMem("data/sound/Game/goalWhistle.ogg");		//	ゴール

	// ステージクラスのインスタンスを生成
	m_stage = new Stage();
	// アクタークラスへのインスタンスを生成
	m_actor = new Ueyama_PlayerActor;
	// カメラクラスへのインスタンスを生成
	m_camera = new Ueyama_Camera(*m_actor);
}
