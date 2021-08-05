#include "Yamaoka_Camera.h"
#include "Yamaoka_Actor.h"

// �R���X�g���N�^
Yamaoka_Camera::Yamaoka_Camera(const Yamaoka_Actor& _actor)
{
	// ���s 1.0�`10000 �܂ł��J�����̕`��͈͂Ƃ���
	SetCameraNearFar(1.0f, 10000.0f);

	// �|�W�V������ݒ� 
	m_pos = VGet(0, 60, -200);
}

// �f�X�g���N�^
Yamaoka_Camera::~Yamaoka_Camera()
{
}

// �X�V
void Yamaoka_Camera::Update(const Yamaoka_Actor& _actor)
{
	// �J�����Ɉʒu�𔽉f����
	SetCameraPositionAndTargetAndUpVec(m_pos, VGet(0.0f, 20.0f, 0.0f), VGet(0, 1, 0));
}
