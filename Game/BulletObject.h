#pragma once
#include "BaseObject.h"
#include "CommonFunc.h"

class BulletObject : public BaseObject
{
public:
	BulletObject();
	~BulletObject();

	enum BulletDir
	{
		DIR_RIGHT = 20,
		DIR_LEFT = 21,
		DIR_UP = 22,
		DIR_UP_LEFT = 23,
		DIR_UP_RIGHT = 24,
		DIR_DOWN_LEFT = 25,
		DIR_DOWN_RIGHT = 26,
	};
	enum BulletType
	{
		MAIN_BULLET_L = 50,
		MAIN_BULLET_R = 52,
		TANK_BULLET = 51,
	};

	void set_x_val(const int& xVal) { x_val_ = xVal; }
	void set_y_val(const int& yVal) { y_val_ = yVal; }
	int get_x_val() const { return x_val_; }
	int get_y_val() const { return y_val_; }

	void set_is_move(const bool& isMove) { is_move_ = isMove; }
	bool get_is_move() const { return is_move_; }

	void set_bullet_dir(const unsigned int& BullerDir) { bullet_dir_ = BullerDir; }
	unsigned int get_bullet_dir() const { return bullet_dir_; }

	void set_bullet_type(const unsigned int& BulletType) { bullet_type_ = BulletType; }
	unsigned int get_bullet_type() const { return bullet_type_; }

	void HandleMove(const int& x_boder, const int& y_boder);// khi dan ban vuot qua man hien thi thi no xoa vien dan
	bool LoadImgBullet(SDL_Renderer* des);
private:
	int x_val_;
	int y_val_;
	bool is_move_;
	unsigned int bullet_dir_;
	unsigned int bullet_type_;
};