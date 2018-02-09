#include "MyPG.h"
#include "MyGameMain.h"
#include "Articulated.h"

//�Q�[���{��
namespace Game
{
	//�Q�[�����
	DI::VGamePad	in1;
	Articulated		boss;

	//----------------------------------------------------------------------------
	//�w�i�`��
	void BackRender()
	{
		ML::Box2D draw(0, 0, 480, 270);
		ML::Box2D src(0, 0, 480, 270);
		DG::Image_Draw("Back", draw, src);
	}

	//-----------------------------------------------------------------------------
	//����������
	//�@�\�T�v�F�v���O�����N�����ɂP����s�����i�f�ނȂǂ̏������s���j
	//-----------------------------------------------------------------------------
	void Initialize()
	{
		DG::Image_Create("Back", "./data/image/GameBG.bmp");	//�w�i
		DG::Image_Create("Boss", "./data/image/Boss.png");		//�{�X
		DG::Image_Create("Joint", "./data/image/Joint.png");	//�֐�

		boss.pos = { 0, 0 };
		boss.speed = { 0, 0 };

		for (int i = 0; i < 4; ++i)
		{
			boss.CreateJointGroup(4, 20.f, ML::ToRadian(1.2f), ML::ToRadian(1.f));
			boss.SetJointOffset(i, 16.f, ML::ToRadian(90.f * i), ML::ToRadian(0.f));
		}
	}
	//-----------------------------------------------------------------------------
	//�������
	//�@�\�T�v�F�v���O�����I�����ɂP����s�����i�f�ނȂǂ̉�����s���j
	//-----------------------------------------------------------------------------
	void Finalize()
	{
		DG::Image_Erase("Back");
		boss.AllKillJoint();
	}
	//-----------------------------------------------------------------------------
	//���s����
	//�@�\�T�v�F�Q�[���̂P�t���[���ɓ����鏈��
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

		TaskFlag rtv = Task_Game;//��肠�������݂̃^�X�N���w��
		if (true == in1.ST.down) {
			rtv = Task_Title;	//���̃^�X�N���^�C�g����
		}
		return rtv;
	}
	//-----------------------------------------------------------------------------
	//�`�揈��
	//�@�\�T�v�F�Q�[���̂P�t���[���ɓ�����\������
	//-----------------------------------------------------------------------------
	void Render()
	{
		BackRender();
		
		{ //��
			ML::Box2D src(0, 0, 32, 32);
			ML::Vec2 drawPos(16, 16);
			boss.RenderJointGroup("Joint", src, drawPos);
		}
		{ //�{��
			ML::Box2D draw(-32, -32, 64, 64);
			ML::Box2D src(0, 0, 64, 64);
			draw.Offset(boss.pos);
			DG::Image_Draw("Boss", draw, src);
		}
	}
}