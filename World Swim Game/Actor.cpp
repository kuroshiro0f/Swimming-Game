//#include "Actor.h"
//
////�R���X�g���N�^
//Actor::Actor()
//{
//}
//
////�f�X�g���N�^
//Actor::~Actor()
//{
//}
//
//// Update�֐��@Game����R�[�������
//void Actor::Update(float _deltaTime)
//{
//	UpdateActor(_deltaTime);
//}
//
////�A�N�^�[�̍X�V
//void Actor::UpdateActor(float _deltaTime)
//{
//}
//
////�`��
//void Actor::DrawActor()
//{
//	// 3D���f���̕`��
//	MV1DrawModel(modelHandle);
//
//	// 3D���f���̉�]�p�x (�ǂ̕����������Ă��邩)
//	MV1SetRotationXYZ(modelHandle, mRotation);
//}
//
////�A�j���[�V�����Đ��֐�
//void Actor::PlayAnim()
//{
//}
//
////�Đ�����A�j���[�V�����̔ԍ����擾����֐�
//void Actor::attachAnim(int _animPlay)
//{
//	if (animIndex != -1)
//	{   // �A�j���[�V�������폜����
//		MV1DetachAnim(modelHandle, animIndex);
//	}
//	// �A�j���[�V�������A�^�b�`����
//	animIndex = MV1AttachAnim(modelHandle, _animPlay);
//	// �A�j���[�V�����̑��Đ����Ԃ��擾����
//	animTotal = MV1GetAnimTotalTime(modelHandle, animIndex);
//}
