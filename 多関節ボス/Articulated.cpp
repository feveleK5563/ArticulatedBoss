#include "Articulated.h"

//----------------------------------------------------------------------------
//指定数の連なる関節(関節群)を作成する
//引数：作成する量(int), 親座標からの距離(float), 親座標を中心とした傾き(float), 傾く速度(float)
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
//全ての子供(関節)を殺す
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
//すでにある関節の情報にオフセットする
//引数：オフセットする関節の番号(int), 距離(dis), 傾き(float), 傾く速度(float)
void Articulated::SetJointOffset(int jointNum, float dis, float ang, float angSpd)
{
	joint[jointNum]->dist += dis;
	joint[jointNum]->setAngle += ang;
	joint[jointNum]->angleSpeed += angSpd;
}

//-----------------------------------------------------------------------------
//関節群を動かす
//引数：指定した位置の関節だけを動かす(int, デフォルトで-1(全動作))
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
//各関節と矩形のあたり判定
//引数：判定を行う矩形(ML::Box2D)
//戻り値：当たったか否か
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
//関節群を描画する
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