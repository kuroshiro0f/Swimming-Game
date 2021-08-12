#ifndef _YAMAOKA_STAGE_H_
#define _YAMAOKA_STAGE_H_

#include "DxLib.h"

class Yamaoka_Stage
{
public:
	// コンストラクタ
	Yamaoka_Stage();
	// デストラクタ
	~Yamaoka_Stage();

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

#endif // !YAMAOKA__STAGE_H_
