#include "Articulated.h"

//----------------------------------------------------------------------------
//�w�萔�̘A�Ȃ�֐�(�֐ߌQ)���쐬����
//�����F�쐬�����(int), �e���W����̋���(float), �e���W�𒆐S�Ƃ����X��(float), �X�����x(float)
void Articulated::CreateJointGroup(int num, float dis, float ang, float angSpd, ML::Box2D& hb, const int id)
{
	if (num > 0)
	{
		joint.emplace_back(new Articulated(dis, ang + setAngle, angSpd, hb));
		joint.back()->articID = id;
		joint.back()->CreateJointGroup(--num, dis, ang, angSpd, hb, id + 1);
	}
}

//----------------------------------------------------------------------------
//�S�Ă̎q��(�֐�)���E��
void Articulated::AllKillJoint()
{
	if (joint.empty())
		return;

	for (auto it = joint.begin();
		 it != joint.end();
		 ++it)
	{
		(*it)->AllKillJoint();
		delete(*it);
	}
	joint.clear();
	joint.shrink_to_fit();
}

//----------------------------------------------------------------------------
//���łɂ���֐߂̏��ɃI�t�Z�b�g����
//�����F�I�t�Z�b�g����֐߂̔ԍ�(int), ����(dis), �X��(float), �X�����x(float)
void Articulated::SetJointOffset(int jointNum, float dis, float ang, float angSpd)
{
	joint[jointNum]->dist += dis;
	joint[jointNum]->setAngle += ang;
	joint[jointNum]->angleSpeed += angSpd;
}

//-----------------------------------------------------------------------------
//�֐ߌQ�𓮂���
//�����F�w�肵���ʒu�̊֐߂����𓮂���(int, �f�t�H���g��-1(�S����))
void Articulated::MoveJointGroup(int onlyDirect)
{
	if (joint.empty())
		return;

	for (auto it = joint.begin();
		 it != joint.end();
		 ++it)
	{
		if (onlyDirect < 0 || onlyDirect == (*it)->articID)
		{
			(*it)->setAngle += (*it)->angleSpeed;
		}
		(*it)->angle = (*it)->setAngle + angle;
		(*it)->pos = { pos.x + cos((*it)->angle) * (*it)->dist,
					   pos.y + sin((*it)->angle) * (*it)->dist };

		(*it)->MoveJointGroup(onlyDirect);
	}
}

//----------------------------------------------------------------------------
//�e�֐߂Ƌ�`�̂����蔻��
//�����F������s����`(ML::Box2D)
//�߂�l�F�����������ۂ�
bool Articulated::HitJoint(ML::Box2D targethb)
{
	if (joint.empty())
		return false;

	for (auto it = joint.begin();
		it != joint.end();
		++it)
	{
		if ((*it)->hitBase.OffsetCopy((*it)->pos).Hit(targethb) ||
			(*it)->HitJoint(targethb))
			return true;
	}
	return false;
}

//-----------------------------------------------------------------------------
//�֐ߌQ��`�悷��
void Articulated::RenderJointGroup(string imageName, ML::Box2D& src, ML::Vec2& drawPos)
{
	if (joint.empty())
		return;

	for (auto it = joint.begin();
		 it != joint.end();
		 ++it)
	{
		ML::Box2D draw = src;
		draw.x = -(int)drawPos.x; draw.y = -(int)drawPos.y;
		draw.Offset((*it)->pos);
		DG::Image_Rotation(imageName, (*it)->angle, drawPos);
		DG::Image_Draw(imageName, draw, src);

		(*it)->RenderJointGroup(imageName, src, drawPos);
	}
}