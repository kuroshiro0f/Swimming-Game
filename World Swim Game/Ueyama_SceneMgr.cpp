#include "Ueyama_SceneMgr.h"
#include "Ueyama_Title.h"
#include "Ueyama_GameScene.h"


Ueyama_SceneMgr::Ueyama_SceneMgr()
{
	m_scene = nullptr;		//	�����o�ϐ��̏�����
}

Ueyama_SceneMgr::~Ueyama_SceneMgr()
{
	delete m_scene;			//	�����o�ϐ��̏���
}

void Ueyama_SceneMgr::Update(float _deltaTime)
{
	Ueyama_SceneBase* tmpScene = m_scene->Update(_deltaTime);		//	���݂̃V�[����ۑ�
	if (tmpScene != m_scene)						//	�V�[�����؂�ւ������
	{
		delete m_scene;								//	���O�̃V�[��������
		m_scene = tmpScene;							//	�V�[�����X�V����
		m_scene->Load();							//	�V�[���̏�����
	}
}

void Ueyama_SceneMgr::Draw()
{
	m_scene->Draw();		//	�V�[����`��
}

void Ueyama_SceneMgr::Sound()
{
	m_scene->Sound();		//	BGM�𗬂�
}

void Ueyama_SceneMgr::SetScene(Ueyama_SceneBase* _Scene)
{
	if (m_scene != nullptr)	//	m_Scene����ł͂Ȃ��ꍇ
	{
		delete m_scene;		//	m_Scene������
	}
	m_scene = _Scene;		//	�V�[���̍X�V
	m_scene->Load();		//	�V�[���̏�����
}
