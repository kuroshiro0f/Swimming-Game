#include "Stage.h"

// コンストラクタ
Stage::Stage()
	: m_stageHandle(-1)
	, m_woterHandle(-1)
{
	// ステージモデルの読み込み
	m_stageHandle = MV1LoadModel("data/stage/屋内プール（短水路）v0_8_2.pmx");
	m_woterHandle = MV1LoadModel("data/woter/Suimen_D603.pmx");

	// 初期位置を設定
	m_pos = VGet(0, 0, 0);
	m_pos2 = VGet(0, 20, 0);
	m_rot = VGet(0.0f, 90.0f * DX_PI_F / 180.0f, 0.0f);   // 回転角度
}

// デストラクタ
Stage::~Stage()
{
	// ステージのアンロード
	MV1DeleteModel(m_stageHandle);
	MV1DeleteModel(m_woterHandle);
}

// 更新
void Stage::Update()
{
	// ポジションを更新
	MV1SetPosition(m_stageHandle, m_pos);
	MV1SetPosition(m_woterHandle, m_pos2);

	// 3Dモデルの回転角度 (どの方向を向いているか)
	MV1SetRotationXYZ(m_stageHandle, m_rot);
}

// 描画
void Stage::Draw()
{
	// ステージの描画
	SetDraw3DScale(1.0f);
	MV1DrawModel(m_stageHandle);
	SetDraw3DScale(1.0f);
	MV1SetOpacityRate(m_woterHandle, 0.5f);   // 水面のモデルを半透明で表示
	MV1DrawModel(m_woterHandle);
}
