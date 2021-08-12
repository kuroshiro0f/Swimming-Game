#ifndef _STAGE_H_
#define _STAGE_H_

#include "DxLib.h"

class Stage
{
public:
	// コンストラクタ
	Stage();
	// デストラクタ
	~Stage();

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
	VECTOR m_pos;          // プールのポジション
	VECTOR m_pos2;         // 水面のポジション
	VECTOR m_rot;          // 回転    

};

#endif // !_STAGE_H_
