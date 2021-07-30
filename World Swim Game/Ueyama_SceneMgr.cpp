#include "Ueyama_SceneMgr.h"
#include "Ueyama_Title.h"
#include "Ueyama_GameScene.h"


Ueyama_SceneMgr::Ueyama_SceneMgr()
{
	m_scene = nullptr;		//	メンバ変数の初期化
}

Ueyama_SceneMgr::~Ueyama_SceneMgr()
{
	delete m_scene;			//	メンバ変数の消去
}

void Ueyama_SceneMgr::Update(float _deltaTime)
{
	Ueyama_SceneBase* tmpScene = m_scene->Update(_deltaTime);		//	現在のシーンを保存
	if (tmpScene != m_scene)						//	シーンが切り替わった時
	{
		delete m_scene;								//	直前のシーンを消す
		m_scene = tmpScene;							//	シーンを更新する
		m_scene->Load();							//	シーンの初期化
	}
}

void Ueyama_SceneMgr::Draw()
{
	m_scene->Draw();		//	シーンを描画
}

void Ueyama_SceneMgr::Sound()
{
	m_scene->Sound();		//	BGMを流す
}

void Ueyama_SceneMgr::SetScene(Ueyama_SceneBase* _Scene)
{
	if (m_scene != nullptr)	//	m_Sceneが空ではない場合
	{
		delete m_scene;		//	m_Sceneを消去
	}
	m_scene = _Scene;		//	シーンの更新
	m_scene->Load();		//	シーンの初期化
}
