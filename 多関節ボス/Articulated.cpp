#include "Articulated.h"

//----------------------------------------------------------------------------
//指定数の連なる関節(関節群)を作成する
//引数：作成する量(int), 親座標からの距離(float), 親座標を中心とした傾き(float), 傾く速度(float)
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
//全ての子供(関節)を殺す
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
//すでにある関節の情報にオフセットする
//引数：オフセットする関節の番号(int), 距離(dis), 傾き(float), 傾く速度(float)
void Articulated::SetJointOffset(int id, float dis, float ang, float angSpd)
{
	joint[id]->dist += dis;
	joint[id]->setAngle += ang;
	joint[id]->angleSpeed += angSpd;
}

//-----------------------------------------------------------------------------
//関節群を動かす
//引数：指定した位置の関節だけを動かす(int, デフォルトで-1(全動作))
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
//関節群を描画する
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