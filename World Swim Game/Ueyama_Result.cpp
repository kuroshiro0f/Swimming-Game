#include "Ueyama_Result.h"
#include "Ueyama_Title.h"
#include "Save.h"

#include "DxLib.h"

//-----------------------------------------------------------------------------
//	・変更点
//-----------------------------------------------------------------------------

//	スクリーンのサイズ
const int SCREEN_SIZE_W = 1920;
const int SCREEN_SIZE_H = 1080;

//	タイム表示する白い四角の場所の座標
const int SQUARE_START_X = 30;
const int SQUARE_START_Y = 370;
const int SQUARE_END_X = 1100;
const int SQUARE_END_Y = 1030;

// タイムを表示する場所
const int TIME_X = 350;
const int TIME_Y = 450;

// RECORDを表示する場所
const int RECORD_X = 300;				//	RECORDの文字	
const int RECORD_Y = 610;
const int RECORD_TIME_X = 350;			//	歴代タイムのX座標
const int FIRST_TIME_Y = 635;			//	一位の場所
const int SECOND_TIME_Y = 790;			//	二位の場所
const int THIRD_TIME_Y = 950;			//	三位の場所

//	リザルトの遷移する時間
//const int TIME = 100;
//const int MEDAL = 150;
//const int EVA = 200;
//const int ENTER = 250;
const int TIME = 100;
const int MEDAL = 150;
const int EVA = 250;
const int ENTER = 300;
const int END = 350;


//	最大透過量
const int MAX_TRANSP_VAL = 255;
//	透過量変化用ベクトル
const int TRANSP_MODERATION = -1;
//	最初の透過量
const int FIRST_TRANS_VAL = 100;

//	評価の基準となる秒数
const int FIRST = 45;
const int SECOND = 55;
const int THIRD = 65;

//	ランクの表示用
const int RANK_FIRST_X = -300;
const int RANK_END_X = 60;
const int RANK_SPEED = 20;

//	星の拡大縮小範囲
const int STAR_MIN = -30;

//	フェードイン・フェードアウトの速度
const int addAlphaVal = 5;

Ueyama_Result::Ueyama_Result(const int _time)
	: m_alphaVal(255)
	, m_fadeOutFinishFlag(false)
	, m_time(_time)
	, m_resultFlag(0)
	, m_resultTime(0)
	, m_bigStarX1(0)
	, m_bigStarX2(SCREEN_SIZE_W)
	, m_bigStarY1(0)
	, m_bigStarY2(SCREEN_SIZE_H)
	, m_smallStarX1(0)
	, m_smallStarX2(SCREEN_SIZE_W)
	, m_smallStarY1(0)
	, m_smallStarY2(SCREEN_SIZE_H)
	, m_rank1X(RANK_FIRST_X)
	, m_rank2X(RANK_FIRST_X)
	, m_rank3X(RANK_FIRST_X)
	, m_starScallFlag(TRUE)
	, m_rank1Flag(false)
	, m_rank2Flag(false)
	, m_rank3Flag(false)
{
	// ※キー入力重複対策のフラグ
	// ENTERキーが押されている間、次のENTERの入力を無効に
	if (CheckHitKey(KEY_INPUT_RETURN))
	{
		m_checkKeyFlag = TRUE;
	}

	SAVE->SetTime(_time);

	// 透過量変数を122に設定
	m_transpVal = MAX_TRANSP_VAL;
	m_fadeTransVal = FIRST_TRANS_VAL;
	// 毎透過量変数を2に設定
	m_permeationAmount = 2;

	//	フェードインから始める
	m_state = RESULT_SCENE_STATE::FADE_IN;
}

Ueyama_Result::~Ueyama_Result()
{
	//	メモリの解放
	DeleteGraph(m_backGraphHandle);
	DeleteGraph(m_logoGraphHandle);
	DeleteGraph(m_guidanceGraphHandle);
	DeleteGraph(m_medalGraphHandle);
	DeleteGraph(m_evaluationGraphHandle);
	DeleteGraph(m_bigStarGraphHandle);
	DeleteGraph(m_smallStarGraphHandle);
	DeleteGraph(m_nowGraphHandle);
	DeleteGraph(m_firstGraphHandle);
	DeleteGraph(m_secondGraphHandle);
	DeleteGraph(m_thirdGraphHandle);
	DeleteSoundMem(m_bgmSoundHandle);
	DeleteSoundMem(m_se1SoundHandle);
	DeleteSoundMem(m_se2SoundHandle);
	DeleteSoundMem(m_se3SoundHandle);
	DeleteSoundMem(m_se4SoundHandle);
}

SceneBase* Ueyama_Result::Update(float _deltaTime)
{
	//	リザルトの経過時間を進める
	m_resultTime++;

	switch (m_state)
	{
	case RESULT_SCENE_STATE::FADE_IN:
		break;
	case RESULT_SCENE_STATE::RESULT:
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

			//	次のシーンへ移行
			m_resultFlag++;

			//	ステートをタイムに
			m_state = RESULT_SCENE_STATE::TIME;
		}

		if (m_resultTime >= TIME)
		{
			//	次のシーンへ移行
			m_resultFlag++;
			m_state = RESULT_SCENE_STATE::TIME;
		}

		break;
	case RESULT_SCENE_STATE::TIME:
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

			//	次のシーンへ移行
			m_resultFlag++;

			//	ステートをメダルに
			m_state = RESULT_SCENE_STATE::MEDAL;
		}

		if (m_resultTime >= MEDAL)
		{
			// 次のシーンへ移行
			m_resultFlag++;
			m_state = RESULT_SCENE_STATE::MEDAL;
		}

		break;
	case RESULT_SCENE_STATE::MEDAL:
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

			//	次のシーンへ移行
			m_resultFlag++;

			//	ステートをエンターに
			m_state = RESULT_SCENE_STATE::EVA;
		}

		if (m_resultTime >= EVA)
		{
			//	次のシーンへ移行
			m_resultFlag++;
			m_state = RESULT_SCENE_STATE::EVA;
		}

		break;
	case RESULT_SCENE_STATE::EVA:
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

			//	次のシーンへ移行
			m_resultFlag++;

			//	ステートをエンターに
			m_state = RESULT_SCENE_STATE::ENTER;
		}

		if (m_resultTime >= ENTER)
		{
			//	次のシーンへ移行
			m_resultFlag++;
			m_state = RESULT_SCENE_STATE::ENTER;
		}

		break;
	case RESULT_SCENE_STATE::ENTER:
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

			//	ステートをタイムに
			m_state = RESULT_SCENE_STATE::FADE_OUT;
		}

		if (m_resultTime >= END)
		{
			m_resultFlag++;
		}

		break;
	case RESULT_SCENE_STATE::FADE_OUT:
		//	フェードアウトが終わったらタイトルへ
		if (m_fadeOutFinishFlag)
		{
			return new Ueyama_Title(true);
		}
		break;
	default:
		break;
	}
	return this;
}

void Ueyama_Result::Draw()
{
	// 透過量更新
	UpdateTransparent();

	DrawGraph(0, 0, m_backGraphHandle, TRUE);			//	背景
	DrawGraph(0, 0, m_logoGraphHandle, TRUE);			//	ロゴ

	//// 透過して描画
	//SetDrawBlendMode(DX_BLENDMODE_ALPHA, 190);
	//DrawBox(SQUARE_START_X, SQUARE_START_Y, SQUARE_END_X, SQUARE_END_Y, GetColor(255, 255, 255), TRUE);
	//// 透過を元に戻す
	//SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);


	if (m_resultFlag >= 1)
	{
		//	クリアタイムを表示
		DrawGraph(0, 0, m_nowGraphHandle, TRUE);

		DrawFormatStringToHandle(TIME_X, TIME_Y, GetColor(255, 255, 255), m_font->recordHandle, "%d", m_time);
	}
	if (m_resultFlag == 2)
	{
		DrawGraph(m_rank1X, 0, m_firstGraphHandle, TRUE);
		DrawFormatStringToHandle(m_rank1X + RECORD_TIME_X, FIRST_TIME_Y, GetColor(255, 255, 255), m_font->recordHandle, "%d", SAVE->GetFirstTime());

		RankMove(m_rank1X, m_rank1Flag, false);
	}
	if (m_resultFlag == 3)
	{
		DrawGraph(m_rank1X, 0, m_firstGraphHandle, TRUE);
		DrawFormatStringToHandle(m_rank1X + RECORD_TIME_X, FIRST_TIME_Y, GetColor(255, 255, 255), m_font->recordHandle, "%d", SAVE->GetFirstTime());
		DrawGraph(m_rank2X, 0, m_secondGraphHandle, TRUE);
		DrawFormatStringToHandle(m_rank2X + RECORD_TIME_X, SECOND_TIME_Y, GetColor(255, 255, 255), m_font->recordHandle, "%d", SAVE->GetSecondTime());

		RankMove(m_rank2X, m_rank2Flag, false);
	}
	if (m_resultFlag == 4)
	{
		DrawGraph(m_rank1X, 0, m_firstGraphHandle, TRUE);
		DrawFormatStringToHandle(m_rank1X + RECORD_TIME_X, FIRST_TIME_Y, GetColor(255, 255, 255), m_font->recordHandle, "%d", SAVE->GetFirstTime());
		DrawGraph(m_rank2X, 0, m_secondGraphHandle, TRUE);
		DrawFormatStringToHandle(m_rank2X + RECORD_TIME_X, SECOND_TIME_Y, GetColor(255, 255, 255), m_font->recordHandle, "%d", SAVE->GetSecondTime());
		DrawGraph(m_rank3X, 0, m_thirdGraphHandle, TRUE);
		DrawFormatStringToHandle(m_rank3X + RECORD_TIME_X, THIRD_TIME_Y, GetColor(255, 255, 255), m_font->recordHandle, "%d", SAVE->GetThirdTime());

		RankMove(m_rank3X, m_rank3Flag, false);
	}
	if (m_resultFlag >= 5)
	{
		DrawGraph(0, 0, m_firstGraphHandle, TRUE);
		DrawFormatStringToHandle(RECORD_TIME_X, FIRST_TIME_Y, GetColor(255, 255, 255), m_font->recordHandle, "%d", SAVE->GetFirstTime());
		DrawGraph(0, 0, m_secondGraphHandle, TRUE);
		DrawFormatStringToHandle(RECORD_TIME_X, SECOND_TIME_Y, GetColor(255, 255, 255), m_font->recordHandle, "%d", SAVE->GetSecondTime());
		DrawGraph(0, 0, m_thirdGraphHandle, TRUE);
		DrawFormatStringToHandle(RECORD_TIME_X, THIRD_TIME_Y, GetColor(255, 255, 255), m_font->recordHandle, "%d", SAVE->GetThirdTime());
	}
	if (m_resultFlag >= 6)
	{
		DrawGraph(0, 0, m_medalGraphHandle, TRUE);			//	メダル
	}
	if (m_resultFlag >= 7)
	{
		DrawGraph(0, 0, m_evaluationGraphHandle, TRUE);		//	評価
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, m_transpVal);	//	透過して描画
		DrawGraph(0, 0, m_guidanceGraphHandle, TRUE);		//	案内
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);			//	透過を元に戻す
		if (m_time <= SAVE->GetSecondTime())
		{
			DrawExtendGraph(m_bigStarX1, m_bigStarY1, m_bigStarX2, m_bigStarY2, m_bigStarGraphHandle, TRUE);			//	大きな星
		}
		if(m_time <= SAVE->GetFirstTime() || m_time > SAVE->GetSecondTime() && m_time <= SAVE->GetThirdTime())
		DrawExtendGraph(m_smallStarX1, m_smallStarY1, m_smallStarX2, m_smallStarY2, m_smallStarGraphHandle, TRUE);	//	小さな星


		if (m_bigStarX1 <= STAR_MIN)
		{
			m_starScallFlag = FALSE;
		}
		if (m_bigStarX1 >= 0)
		{
			m_starScallFlag = TRUE;
		}

		if (m_starScallFlag == TRUE)
		{
			//	拡大する
			m_bigStarX1--;
			m_bigStarX2++;
			m_bigStarY1--;
			m_bigStarY2++;
			m_smallStarX1--;
			m_smallStarX2++;
			m_smallStarY1--;
			m_smallStarY2++;
		}
		else if (m_starScallFlag == FALSE)
		{
			//	縮小する
			m_bigStarX1++;
			m_bigStarX2--;
			m_bigStarY1++;
			m_bigStarY2--;
			m_smallStarX1++;
			m_smallStarX2--;
			m_smallStarY1++;
			m_smallStarY2--;
		}
	}

	//	フェードイン処理
	if (m_state == RESULT_SCENE_STATE::FADE_IN)
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
			m_state = RESULT_SCENE_STATE::RESULT;
		}
	}

	// フェードアウト処理
	if (m_state == RESULT_SCENE_STATE::FADE_OUT)
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

void Ueyama_Result::Sound()
{
	PlaySoundMem(m_bgmSoundHandle, DX_PLAYTYPE_BACK, FALSE);
	switch (m_resultFlag)
	{
	case 1:
		//	効果音を流す
		PlaySoundMem(m_se1SoundHandle, DX_PLAYTYPE_BACK, FALSE);
		break;
	case 2:
		//	効果音を流す
		PlaySoundMem(m_se2SoundHandle, DX_PLAYTYPE_BACK, FALSE);
		StopSoundMem(m_se1SoundHandle);
		break;
	case 3:
		//	効果音を流す
		PlaySoundMem(m_se2SoundHandle, DX_PLAYTYPE_BACK, FALSE);
		break;
	case 4:
		//	効果音を流す
		PlaySoundMem(m_se2SoundHandle, DX_PLAYTYPE_BACK, FALSE);
		break;
	case 6:
		//	効果音を流す
		PlaySoundMem(m_se3SoundHandle, DX_PLAYTYPE_BACK, FALSE);
		StopSoundMem(m_se2SoundHandle);
		break;
	case 7:
		//	効果音を流す
		PlaySoundMem(m_se4SoundHandle, DX_PLAYTYPE_BACK, FALSE);
		StopSoundMem(m_se3SoundHandle);
		break;
	case 9:
		StopSoundMem(m_se4SoundHandle);
	default:
		break;
	}
}

void Ueyama_Result::Load()
{
	//	グラフィックハンドルにセット
	m_backGraphHandle = LoadGraph("data/img/Result/Result_back.png");				//	背景
	m_logoGraphHandle = LoadGraph("data/img/Result/Result_logo.png");				//	ロゴ
	m_guidanceGraphHandle = LoadGraph("data/img/Result/Result_guidance.png");		//	案内
	if (m_time <= SAVE->GetFirstTime())
	{
		m_medalGraphHandle = LoadGraph("data/img/Result/Result_gold.png");			//	金メダル
		m_evaluationGraphHandle = LoadGraph("data/img/Result/Result_best.png");		//　CONGRATURATION
		m_bigStarGraphHandle = LoadGraph("data/img/Result/bigStar.png");			//	大きな星
		m_smallStarGraphHandle = LoadGraph("data/img/Result/smallStar.png");		//	小さな星
	}
	if (m_time > SAVE->GetFirstTime() && m_time <= SAVE->GetSecondTime())
	{
		m_medalGraphHandle = LoadGraph("data/img/Result/Result_silver.png");		//	銀メダル
		m_evaluationGraphHandle = LoadGraph("data/img/Result/Result_great.png");	//	GREAT
		m_bigStarGraphHandle = LoadGraph("data/img/Result/bigStar.png");			//	大きな星
	}
	if (m_time > SAVE->GetSecondTime() && m_time <= SAVE->GetThirdTime())
	{
		m_medalGraphHandle = LoadGraph("data/img/Result/Result_copper.png");		//	銅メダル
		m_evaluationGraphHandle = LoadGraph("data/img/Result/Result_good.png");		//　GOOD
		m_smallStarGraphHandle = LoadGraph("data/img/Result/smallStar.png");		//	小さな星
	}
	if (m_time > SAVE->GetThirdTime())
	{
		//m_medalGraphHandle = LoadGraph("data/img/Result/Result_copper.png");		//	銅メダル
		m_evaluationGraphHandle = LoadGraph("data/img/Result/Result_ngu.png");		//　GOOD
	}
	m_nowGraphHandle = LoadGraph("data/img/Result/Result_timeBase.png");			//	今回のタイム
	m_firstGraphHandle = LoadGraph("data/img/Result/Result_first.png");				//	一位のタイム
	m_secondGraphHandle = LoadGraph("data/img/Result/Result_second.png");			//	二位のタイム
	m_thirdGraphHandle = LoadGraph("data/img/Result/Result_third.png");				//	三位のタイム


	//	サウンドハンドルにセット
	m_bgmSoundHandle = LoadSoundMem("data/sound/Result/Result.ogg");				//	BGM
	m_se1SoundHandle = LoadSoundMem("data/sound/Result/TestSE.mp3");				//	効果音
	m_se2SoundHandle = LoadSoundMem("data/sound/Result/TestSE.mp3");				//	効果音
	m_se3SoundHandle = LoadSoundMem("data/sound/Result/TestSE.mp3");				//	効果音
	m_se4SoundHandle = LoadSoundMem("data/sound/Result/TestSE.mp3");				//	効果音

	//	音量調整
	ChangeVolumeSoundMem(m_volumePal, m_bgmSoundHandle);
	ChangeVolumeSoundMem(m_volumePal, m_se1SoundHandle);
	ChangeVolumeSoundMem(m_volumePal, m_se2SoundHandle);
	ChangeVolumeSoundMem(m_volumePal, m_se3SoundHandle);
	ChangeVolumeSoundMem(m_volumePal, m_se4SoundHandle);

	m_font = new Font();
}

void Ueyama_Result::RankMove(int& _x, bool& _flag, bool _flag2)
{
	if (_x >= RANK_END_X)
	{
		_flag = true;
	}
	
	/*_x += RANK_SPEED;
	if (_x >= 0)
	{
		_x = 0;
		_flag = true;
	}
	if (_flag == true && _flag2 == false)
	{
		m_resultFlag++;
		_flag2 = true;
	}*/

	if (_flag == false)
	{
		_x += RANK_SPEED;
	}
	else if (_flag == true)
	{
		_x -= RANK_SPEED / 2;
		if (_x <= 0)
		{
			_x = 0;
			if (_flag2 == false)
			{
				m_resultFlag++;
				_flag2 = true;
			}
		}
	}
}

void Ueyama_Result::UpdateTransparent()
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
