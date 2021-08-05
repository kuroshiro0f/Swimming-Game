#include "Nagatomo_Camera.h"

// �R���X�g���N�^
Nagatomo_Camera::Nagatomo_Camera(const Nagatomo_PlayerActor& player)
{
	// ���s 1.0�`10000 �܂ł��J�����̕`��͈͂Ƃ���
	SetCameraNearFar(1.0f, 10000.0f);

	pos = VGet(0, 25, -55);
}

// �f�X�g���N�^
Nagatomo_Camera::~Nagatomo_Camera()
{

}

// �X�V
void Nagatomo_Camera::Update(const Nagatomo_PlayerActor& player)
{
	// �J�����Ɉʒu�𔽉f����
	SetCameraPositionAndTargetAndUpVec(pos, VGet(0.0f, 20.0f, 0.0f), VGet(0, 1, 0));

}