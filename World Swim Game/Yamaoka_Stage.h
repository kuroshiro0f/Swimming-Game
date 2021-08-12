#ifndef _YAMAOKA_STAGE_H_
#define _YAMAOKA_STAGE_H_

#include "DxLib.h"

class Yamaoka_Stage
{
public:
	// �R���X�g���N�^
	Yamaoka_Stage();
	// �f�X�g���N�^
	~Yamaoka_Stage();

	// �X�V����
	void Update();
	// �`��
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
	int m_stageHandle;     // �X�e�[�W���f��
	int m_woterHandle;     // ���ʂ̃��f��
	VECTOR m_pos;          // �v�[���̃|�W�V����
	VECTOR m_pos2;         // ���ʂ̃|�W�V����
	VECTOR m_rot;          // ��]    

};

#endif // !YAMAOKA__STAGE_H_
