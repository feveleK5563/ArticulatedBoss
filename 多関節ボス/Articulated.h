#pragma once
#include "MyPG.h"

class Articulated
{
private:
	int		articID;	//���g���e�̒��S���琔���ĉ��Ԗڂ̊֐߂�

	float	angle;			//���ۂ̌X��

	float	dist;			//�e���W����̋���
	float	setAngle;		//�����̌X��
	float	angleSpeed;		//�X�����x

	vector<Articulated*> joint;		//�����Ɉʒu����q��(�֐�)

public:
	ML::Vec2 pos;		//���W
	ML::Vec2 speed;		//�ړ����x

	Articulated():
		articID(0),
		pos(0.f, 0.f),
		speed(0.f, 0.f),
		angle(0.f),
		dist(0.f),
		setAngle(0.f),
		angleSpeed(0.f){}

	Articulated(float dis, float ang, float angSpd):
		articID(0),
		pos(0.f, 0.f),
		speed(0.f, 0.f),
		angle(0.f),
		dist(dis),
		setAngle(ang),
		angleSpeed(angSpd){}

	//----------------------------------------------------------------------------
	//�w�萔�̘A�Ȃ�֐�(�֐ߌQ)���쐬����
	//�����F�쐬�����(int), �e���W����̋���(float), �e���W�𒆐S�Ƃ����X��(float), �X�����x(float)
	//�����Ӂ� id�̈����͐�΂ɕύX���Ȃ��ł�������
	void CreateJointGroup(int num, float dis, float ang, float angSpd, int id = 0);

	//----------------------------------------------------------------------------
	//�S�Ă̎q��(�֐�)���E��
	void AllKillJoint();

	//----------------------------------------------------------------------------
	//���łɂ���֐߂̏��ɃI�t�Z�b�g����
	//�����F�I�t�Z�b�g����֐߂̔ԍ�(int), ����(dis), �X��(float), �X�����x(float)
	void SetJointOffset(int id, float dis, float ang, float angSpd);

	//----------------------------------------------------------------------------
	//�֐ߌQ�𓮂���
	//�����F�w�肵���ʒu�̊֐߂����𓮂���(int, �f�t�H���g��-1(�S����))
	void MoveJointGroup(int onlyDirect = -1);

	//----------------------------------------------------------------------------
	//�֐ߌQ��`�悷��
	//�����F�摜��(string), �摜��`(ML::Box2D), �`��Ɖ�]�(ML::Vec2)
	void RenderJointGroup(string imageName, ML::Box2D& src, ML::Vec2& drawPos);
};