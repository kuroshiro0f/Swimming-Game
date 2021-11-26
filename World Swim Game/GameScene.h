#pragma once
#include "SceneBase.h"
#include "PlayEffect.h"
#include "PlayerActor.h"
#include "Camera.h"
#include "Font.h"

class PlayerActor;			//PlayerActorへの参照
class Camera;				//Cameraへの参照

class GameScene :public SceneBase
{
	PlayerActor* player;
	Camera* camera;

public:
	//int m_countUP;                  // 経過時間

	enum class GAME_SCENE_STATE
	{
		LOAD,
		FADE_IN,
		GAME,
		FADE_OUT
	};

	GameScene();			//	コンストラクタ
	~GameScene();			//	デストラクタ

	SceneBase* Update(float _deltaTime)override;	//	更新
	void Draw()override;			//	描画
	void Sound()override;			//	音楽
	void Load()override;			//	初期化

private:
	GAME_SCENE_STATE m_state;

	//	文字の位置
	int m_mojiX;		//	ロード文字のX座標
	int m_mojiY;		//	ロード文字のY座標

	//	男の子の移動
	int m_boyY;

	//	文字の表示時間の操作
	int m_startCount;			//	スタート
	int m_finishCount;			//	ゴール
	bool m_startFinishFlag;

	//	星の移動
	int m_starX;				//	X方向
	int m_starY;				//	Y方向
	double m_starAngle;			//	角度

	//	ハートの拡大率
	double m_heartRate;
	double m_heartSpeed1;
	double m_heartSpeed2;

	//	矢印の表示関係
	double m_arrowRate;			//	拡大率
	double m_arrowAngle;		//	角度
	double m_arrow2Angle;		//	角度
	double m_arrow3Angle;		//	角度
	double m_arrow4Angle;		//	角度

	//	息継ぎの拡大縮小
	int m_breathX, m_breathY;
	int m_breathX2, m_breathY2;
	int m_breathCount;
	bool m_breathFlag;

	//	汗の移動
	int m_sweat1X, m_sweat1Y;	//	汗1
	int m_sweat2X, m_sweat2Y;	//	汗2

	//	ラストの演出用
	int m_lastX, m_lastY;

	//	グラフィックハンドル
	int m_backGraphHandle;				//	背景
	int m_loadGraphHandle;				//	ロード
	int m_tips1GraphHandle;				//	TIPS1
	int m_tips2GraphHandle;				//	TIPS2
	int m_tips3GraphHandle;				//	TIPS3
	int m_boyGraphHandle;				//	男の子
	int m_boy2GraphHandle;				//	男の子2
	int m_starGraphHandle;				//	星
	int m_sweat1GraphHandle;			//	汗1
	int m_sweat2GraphHandle;			//	汗2
	int m_heartGraphHandle;				//	ハート
	int m_upArrowGraphHandle;			//	上矢印
	int m_rightArrowGraphHandle;		//	右矢印
	int m_downArrowGraphHandle;			//	下矢印
	int m_leftArrowGraphHandle;			//	左矢印
	int m_failGraphHandle;				//	失敗
	int m_spaceGraphHandle;				//	スペース
	int m_stGraphHandle;				//	スタミナ
	int m_st2GraphHandle;				//	スタミナ2
	int m_st3GraphHandle;				//	スタミナ3
	int m_breathGraphHandle;			//	息継ぎ
	int m_arrowBaseGraphHandle;			//	矢印の土台
	int m_scoreBaseGraphHandle;			//	スコアの土台
	int m_spaceBaseGraphHandle;			//	スペースの土台
	int m_spaceBase2GraphHandle;		//	スペースの土台2
	int m_stBaseGraphHandle;			//	スタミナの土台
	int m_finishArrowBaseGraphHandle;	//	ラストの矢印の土台
	int m_lastGraphHandle;				//	ラストの演出


	//	サウンドハンドル
	int m_bgmSoundHandle;			//	BGM
	int m_whistleSoundHandle;		//	笛
	int m_countDownSoundHandle;		//	カウントダウン
	int m_goalSoundHandle;			//	ゴール
	int m_arrowSoundHandle;			//	矢印
	int m_arrowFailSoundHandle;		//	矢印の失敗
	int m_spaceSoundHandle;			//	スペース
	int m_breathSoundHandle;		//	息継ぎ
	int m_loadSoundHandle;			//	ロード

	//	時間の経過
	int m_timeElapsed;

	//	ターンの評価関連
	int m_turnEvaX, m_turnEvaY;				//	座標

	//	アルファ値
	int m_alphaVal;

	//	デルタタイム
	float m_deltaTime;

	//	フラグ
	int m_tipsFlag;					//	TIPSの表示変える
	int m_loadingFlag;				//	ローディングの文字の表示を変える
	bool m_checkKeyFlag;			//	キーが押されたままか
	bool m_fadeOutFinishFlag;		//	フェードアウトの終了判定
	bool m_loadFlag;				//	ロード画面を表示するか
	bool m_loadFinishFlag;			//	ロードが終わったか
	bool m_boyPlusFlag;				//	男の子をうごかすフラグ
	bool m_whistleFinishFlag;		//	笛を鳴らし終わったか
	bool m_gameFinishFlag;			//	ゲームが終わったか
	bool m_fadeOutFlag;				//	フェードアウトを始めるか
	bool m_sweat1Flag;				//	汗1を表示するか
	bool m_sweat2Flag;				//	汗2を表示するか
	bool m_heartFlag;				//	ハートの拡大と縮小の変更用
	bool m_lastFlag;				//	ラストスパートが始まったか
	bool m_last1SucFlag;
	bool m_last2SucFlag;
	bool m_last3SucFlag;
	bool m_last4SucFlag;

	class Stage* m_stage;   // ステージクラスへのポインタメンバ変数
	class Camera* m_camera;	// カメラクラスへのポインタメンバ変数
	class PlayerActor* m_actor;   // アクタークラスへのポインタメンバ変数
	class PlayEffect* m_effect;				//	エフェクトプレーヤー
	class Font* m_font;
};