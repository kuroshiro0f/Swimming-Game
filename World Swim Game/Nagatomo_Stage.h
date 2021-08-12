#ifndef _NAGATOMO_STAGE_H_
#define _NAGATOMO_STAGE_H_

#include "DxLib.h"

class Nagatomo_Stage
{
public:
	// コンストラクタ
	Nagatomo_Stage();
	// デストラクタ
	~Nagatomo_Stage();

	// 更新処理
	void Update();
	// 描画
	void Draw();

	int GetModelHandle()
	{
		return m_stageHandle;
	}
	int GetModelHandle2()
	{
		return m_woterHandle;
	}

private:
	int m_stageHandle;     // ステージモデル
	int m_woterHandle;     // 水面のモデル
	VECTOR m_pos;            // プールのポジション
	VECTOR m_pos2;           // 水面のポジション

};

#endif
