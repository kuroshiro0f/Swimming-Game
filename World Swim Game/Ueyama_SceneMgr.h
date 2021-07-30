#ifndef _UEYAMA_SCENEMGR_H_
#define _UEYAMA_SCENEMGR_H_

#include "DxLib.h"

class Ueyama_SceneMgr
{
public:
	Ueyama_SceneMgr();			//	�R���X�g���N�^
	~Ueyama_SceneMgr();		//	�f�X�g���N�^

	void Update(float _deltaTime);		//	�X�V
	void Draw();		//	�`��
	void Sound();		//�@���y

	void SetScene(class Ueyama_SceneBase* _Scene);		//	set�֐�

private:
	class Ueyama_SceneBase* m_scene;	//	�V�[���x�[�X�ւ̃|�C���^�����o�ϐ�
};

#endif // _UEYAMA_SCENEMGR_H_
