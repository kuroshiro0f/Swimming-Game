#ifndef _STAGE_H_
#define _STAGE_H_

#include "DxLib.h"

class Stage
{
public:
	// �R���X�g���N�^
	Stage();
	// �f�X�g���N�^
	~Stage();

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

#endif // !_STAGE_H_
