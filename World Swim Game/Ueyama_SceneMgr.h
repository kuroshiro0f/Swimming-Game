#ifndef _UEYAMA_SCENEMGR_H_
#define _UEYAMA_SCENEMGR_H_

#include "DxLib.h"

class Ueyama_SceneMgr
{
public:
	Ueyama_SceneMgr();			//	コンストラクタ
	~Ueyama_SceneMgr();		//	デストラクタ

	void Update(float _deltaTime);		//	更新
	void Draw();		//	描画
	void Sound();		//　音楽

	void SetScene(class Ueyama_SceneBase* _Scene);		//	set関数

private:
	class Ueyama_SceneBase* m_scene;	//	シーンベースへのポインタメンバ変数
};

#endif // _UEYAMA_SCENEMGR_H_
