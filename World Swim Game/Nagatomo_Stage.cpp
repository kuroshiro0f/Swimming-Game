#include "Nagatomo_Stage.h"

// �R���X�g���N�^
Nagatomo_Stage::Nagatomo_Stage()
	: m_stageHandle(-1)
	, m_woterHandle(-1)
{
	// �X�e�[�W���f���̓ǂݍ���
	m_stageHandle = MV1LoadModel("data/stage/�����v�[���i�Z���H�jv0_8_2.pmx");
	m_woterHandle = MV1LoadModel("data/woter/Suimen_D603.pmx");

	// �����ʒu��ݒ�
	m_pos = VGet(0, 0, 0);
	m_pos2 = VGet(0, 20, 0);
}

// �f�X�g���N�^
Nagatomo_Stage::~Nagatomo_Stage()
{
	// �X�e�[�W�̃A�����[�h
	MV1DeleteModel(m_stageHandle);
	MV1DeleteModel(m_woterHandle);
}

// �X�V
void Nagatomo_Stage::Update()
{
	// �|�W�V�������X�V
	MV1SetPosition(m_stageHandle, m_pos);
	MV1SetPosition(m_woterHandle, m_pos2);
}

// �`��
void Nagatomo_Stage::Draw()
{
	// �X�e�[�W�̕`��
	SetDraw3DScale(1.0f);
	MV1DrawModel(m_stageHandle);
	SetDraw3DScale(1.0f);
	MV1SetOpacityRate(m_woterHandle, 0.5f);   // ���ʂ̃��f���𔼓����ŕ\��
	MV1DrawModel(m_woterHandle);
}
