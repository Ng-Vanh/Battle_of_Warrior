#pragma once

#include <vector>
#include "CommonFunc.h";
#include "BaseObject.h";
#include "BulletObject.h"

#define GRAVITY_SPEED 0.5
#define MAX_FAIL_SPEED 10
#define PLAYER_SPEED 10
#define PLAYER_JUMP_VALUE 15

class MainObject :public BaseObject
{
public:
	MainObject();
	~MainObject();

	enum WalkType
	{
		WALK_NONE = 0,
		WALK_RIGHT = 1,
		WALK_LEFT = 2,
	};
	bool LoadImg(std::string path, SDL_Renderer* screen);
	void Show(SDL_Renderer* des);
	void HandleInputAction(SDL_Event events, SDL_Renderer* screen);
	void HandleBullet(SDL_Renderer* des);
	void set_clip();
	void set_comeback_time(const int& cbTime)
	{
		come_back_time = cbTime;
	}
	int get_frame_width() const { return width_frame_; }
	int get_frame_height() const { return height_frame_; }

	void DoPlayer(Map& map_data);
	void CheckToMap(Map& map_data);// kiem tra map 
	void SetMapXY(const int map_x, const int map_y) // luu vi tri ban do
	{
		map_x_ = map_x;
		map_y_ = map_y;
	}
	void CenterEnityOnMap(Map& map_data);
	void UpDateImagePlayer(SDL_Renderer* des);
	SDL_Rect GetRectFrame();

	void set_bullet_list(std::vector<BulletObject*> bullet_list)
	{
		p_bullet_list = bullet_list;
	}
	std::vector<BulletObject*> get_bullet_list() const { return p_bullet_list; }
	void RemoveBullet(const int& idx);
	void InCreaseMoney();
	int GetMoney()const { return money_count; }
	void setScore(const int& sc) { score = sc; }
	int getScore()const { return score; }
	void inCrease15Point() { score += 15; }
	void inCrease5Point() { score += 5; }
	void inCrease50Point() { score += 50; }
private:
	std::vector<BulletObject*> p_bullet_list;

	float x_val_;// khi bam di chuyen 1 doan bang x_val
	float y_val_;

	float x_pos_;
	float y_pos_;

	int width_frame_;
	int height_frame_;

	SDL_Rect frame_clip_[8];// luu cac frame. 8mla 1 anh co 8 chuyen dong
	Input input_type_;
	int frame_;
	int status_;// trang thai kieu di chuyen hien tai
	bool on_ground_;

	int map_x_;
	int map_y_;


	int come_back_time = 0;

	int money_count;
	int score;
};
