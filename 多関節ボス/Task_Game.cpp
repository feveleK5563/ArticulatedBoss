#include "MyPG.h"
#include "MyGameMain.h"
#include "Articulated.h"

//ゲーム本編
namespace Game
{
	//ゲーム情報
	DI::VGamePad	in1;
	Articulated		boss;

	//----------------------------------------------------------------------------
	//背景描画
	void BackRender()
	{
		ML::Box2D draw(0, 0, 480, 270);
		ML::Box2D src(0, 0, 480, 270);
		DG::Image_Draw("Back", draw, src);
	}

	//-----------------------------------------------------------------------------
	//初期化処理
	//機能概要：プログラム起動時に１回実行される（素材などの準備を行う）
	//-----------------------------------------------------------------------------
	void Initialize()
	{
		DG::Image_Create("Back", "./data/image/GameBG.bmp");	//背景
		DG::Image_Create("Boss", "./data/image/Boss.png");		//ボス
		DG::Image_Create("Joint", "./data/image/Joint.png");	//関節

		boss.pos = { 0, 0 };
		boss.speed = { 0, 0 };

		for (int i = 0; i < 4; ++i)
		{
			boss.CreateJointGroup(4, 20.f, ML::ToRadian(1.2f), ML::ToRadian(1.f));
			boss.SetJointOffset(i, 16.f, ML::ToRadian(90.f * i), ML::ToRadian(0.f));
		}
	}
	//-----------------------------------------------------------------------------
	//解放処理
	//機能概要：プログラム終了時に１回実行される（素材などの解放を行う）
	//-----------------------------------------------------------------------------
	void Finalize()
	{
		DG::Image_Erase("Back");
		boss.AllKillJoint();
	}
	//-----------------------------------------------------------------------------
	//実行処理
	//機能概要：ゲームの１フレームに当たる処理
	//-----------------------------------------------------------------------------
	TaskFlag UpDate()
	{
		in1 = DI::GPad_GetState("P1");

		boss.speed = { 0, 0 };
		if (in1.LStick.L.on) { boss.speed.x -= 2; }
		if (in1.LStick.R.on) { boss.speed.x += 2; }
		if (in1.LStick.U.on) { boss.speed.y -= 2; }
		if (in1.LStick.D.on) { boss.speed.y += 2; }
		boss.pos += boss.speed;
		boss.MoveJointGroup(-1);

		TaskFlag rtv = Task_Game;//取りあえず現在のタスクを指定
		if (true == in1.ST.down) {
			rtv = Task_Title;	//次のタスクをタイトルへ
		}
		return rtv;
	}
	//-----------------------------------------------------------------------------
	//描画処理
	//機能概要：ゲームの１フレームに当たる表示処理
	//-----------------------------------------------------------------------------
	void Render()
	{
		BackRender();
		
		{ //足
			ML::Box2D src(0, 0, 32, 32);
			ML::Vec2 drawPos(16, 16);
			boss.RenderJointGroup("Joint", src, drawPos);
		}
		{ //本体
			ML::Box2D draw(-32, -32, 64, 64);
			ML::Box2D src(0, 0, 64, 64);
			draw.Offset(boss.pos);
			DG::Image_Draw("Boss", draw, src);
		}
	}
}