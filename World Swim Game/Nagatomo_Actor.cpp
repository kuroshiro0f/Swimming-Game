#include "Nagatomo_Actor.h"

int Nagatomo_Actor::modelHandle = NULL;

//�R���X�g���N�^
Nagatomo_Actor::Nagatomo_Actor()
	:mPosition(VGet(0, 0, 0))
	, mVelosity(VGet(1, 0, 0))
{
}

// Update�֐��@Game����R�[�������
void Nagatomo_Actor::Update(float _deltaTime)
{
	UpdateActor(_deltaTime);
}

//�A�N�^�[�̍X�V
void Nagatomo_Actor::UpdateActor(float _deltaTime)
{
}

//�`��
void Nagatomo_Actor::DrawActor()
{

}

//�A�j���[�V�����Đ��֐�
void Nagatomo_Actor::PlayAnim()
{
}

//�Đ�����A�j���[�V�����̔ԍ����擾����֐�
void Nagatomo_Actor::attachAnim(int _animPlay)
{
	if (animIndex != -1)
	{   // �A�j���[�V�������폜����
		MV1DetachAnim(modelHandle, animIndex);
	}
	// �A�j���[�V�������A�^�b�`����
	animIndex = MV1AttachAnim(modelHandle, _animPlay);
	// �A�j���[�V�����̑��Đ����Ԃ��擾����
	animTotal = MV1GetAnimTotalTime(modelHandle, animIndex);
}
