#include "BulletObject.h"

BulletObject::BulletObject()
{
	x_val_ = 0;
	y_val_ = 0;
	is_move_ = false;
	bullet_type_ = MAIN_BULLET_L;

}
BulletObject::~BulletObject()
{

}
bool BulletObject::LoadImgBullet(SDL_Renderer* des)
{
	bool ret = false;
	if (bullet_type_ == TANK_BULLET)
	{
		ret = LoadImg("img//tank_bullet.png", des);
	}
	else if (bullet_type_ == MAIN_BULLET_L)
	{
		ret = LoadImg("img//bulletL.png", des);
	}
	else if (bullet_type_ == MAIN_BULLET_R)
	{
		ret = LoadImg("img//bulletR.png", des);
	}
	return ret;
}

void BulletObject::HandleMove(const int& x_boder, const int& y_boder)
{
	if (bullet_dir_ == DIR_RIGHT)
	{
		rect_.x += x_val_;// dan ban theo chieu ngang
		if (rect_.x > x_boder)
		{
			is_move_ = false;
		}
	}
	else if (bullet_dir_ == DIR_LEFT)
	{
		rect_.x -= x_val_;// dan ban theo chieu ngang
		if (rect_.x < 0)
		{
			is_move_ = false;
		}
	}
	else if (bullet_dir_ == DIR_UP)
	{
		rect_.y -= y_val_;
		if (rect_.y < 0)
		{
			is_move_ = false;
		}
	}
	else if (bullet_dir_ == DIR_UP_LEFT)
	{
		rect_.y -= y_val_;
		rect_.x -= x_val_;
		if (rect_.y < 0 || rect_.x < 0)
		{
			is_move_ = false;
		}
	}
	else if (bullet_dir_ == DIR_UP_RIGHT)
	{
		rect_.x += x_val_;
		rect_.y -= y_val_;
		if (rect_.y <0 || rect_.x > x_boder)
		{
			is_move_ = false;
		}
	}
	else if (bullet_dir_ == DIR_DOWN_LEFT)
	{
		rect_.x -= x_val_;
		rect_.y += y_val_;
		if (rect_.y > y_boder || rect_.x < 0)
		{
			is_move_ = false;
		}
	}
	else if (bullet_dir_ == DIR_DOWN_RIGHT)
	{
		rect_.x += x_val_;
		rect_.y += y_val_;
		if (rect_.y > y_boder || rect_.x > x_boder)
		{
			is_move_ = false;
		}
	}
}