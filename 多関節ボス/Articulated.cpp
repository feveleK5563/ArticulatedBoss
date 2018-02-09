#include "Articulated.h"

//----------------------------------------------------------------------------
//�w�萔�̘A�Ȃ�֐�(�֐ߌQ)���쐬����
//�����F�쐬�����(int), �e���W����̋���(float), �e���W�𒆐S�Ƃ����X��(float), �X�����x(float)
void Articulated::CreateJointGroup(int num, float dis, float ang, float angSpd, int id)
{
	if (num > 0)
	{
		joint.emplace_back(new Articulated(dis, ang + setAngle, angSpd));
		joint.back()->articID = id;
		joint.back()->CreateJointGroup(--num, dis, ang, angSpd, ++id);
	}
}

//----------------------------------------------------------------------------
//�S�Ă̎q��(�֐�)���E��
void Articulated::AllKillJoint()
{
	if (joint.empty())
		return;

	for (vector<Articulated*>::iterator it = joint.begin();
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
void Articulated::SetJointOffset(int id, float dis, float ang, float angSpd)
{
	joint[id]->dist += dis;
	joint[id]->setAngle += ang;
	joint[id]->angleSpeed += angSpd;
}

//-----------------------------------------------------------------------------
//�֐ߌQ�𓮂���
//�����F�w�肵���ʒu�̊֐߂����𓮂���(int, �f�t�H���g��-1(�S����))
void Articulated::MoveJointGroup(int onlyDirect)
{
	if (joint.empty())
		return;

	for (vector<Articulated*>::iterator it = joint.begin();
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

//-----------------------------------------------------------------------------
//�֐ߌQ��`�悷��
void Articulated::RenderJointGroup(string imageName, ML::Box2D& src, ML::Vec2& drawPos)
{
	if (joint.empty())
		return;

	for (vector<Articulated*>::iterator it = joint.begin();
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