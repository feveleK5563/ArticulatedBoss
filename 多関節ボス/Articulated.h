#pragma once
#include "MyPG.h"

class Articulated
{
private:
	int		articID;	//自身が親の中心から数えて何番目の関節か

	float	angle;			//実際の傾き

	float	dist;			//親座標からの距離
	float	setAngle;		//自分の傾き
	float	angleSpeed;		//傾く速度

	vector<Articulated*> joint;		//直下に位置する子供(関節)

public:
	ML::Vec2 pos;		//座標
	ML::Vec2 speed;		//移動速度

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
	//指定数の連なる関節(関節群)を作成する
	//引数：作成する量(int), 親座標からの距離(float), 親座標を中心とした傾き(float), 傾く速度(float)
	//※注意※ idの引数は絶対に変更しないでください
	void CreateJointGroup(int num, float dis, float ang, float angSpd, int id = 0);

	//----------------------------------------------------------------------------
	//全ての子供(関節)を殺す
	void AllKillJoint();

	//----------------------------------------------------------------------------
	//すでにある関節の情報にオフセットする
	//引数：オフセットする関節の番号(int), 距離(dis), 傾き(float), 傾く速度(float)
	void SetJointOffset(int id, float dis, float ang, float angSpd);

	//----------------------------------------------------------------------------
	//関節群を動かす
	//引数：指定した位置の関節だけを動かす(int, デフォルトで-1(全動作))
	void MoveJointGroup(int onlyDirect = -1);

	//----------------------------------------------------------------------------
	//関節群を描画する
	//引数：画像名(string), 画像矩形(ML::Box2D), 描画と回転基準(ML::Vec2)
	void RenderJointGroup(string imageName, ML::Box2D& src, ML::Vec2& drawPos);
};