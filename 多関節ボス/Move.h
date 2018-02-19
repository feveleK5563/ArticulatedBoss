#pragma once
#include "MyPG.h"

class Move
{
public:
	ML::Vec2	pos;	//基準座標
	float		speed;	//1フレームの移動量
	float		angle;	//角度
	float		angSpd;	//回転速度
	float		dist;	//座標からの距離


};