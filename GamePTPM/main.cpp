#include "SDL.h"
#include "BaseObject.h"
#include "CommonFunc.h"
#include "GameMap.h"
#include "MainObject.h"
#include "ImpTimer.h"
#include "ThreatObject.h"
BaseObject g_background;

void close()
{
	g_background.Free();
	SDL_DestroyRenderer(g_screen);
	g_screen = NULL;
	SDL_DestroyWindow(g_Window);
	g_Window = NULL;
	IMG_Quit();
	SDL_Quit();
}

bool InitData()
{
	bool success = true;
	int ret = SDL_Init(SDL_INIT_VIDEO);
	if (ret < 0) return false;

	SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1");
	g_Window = SDL_CreateWindow("Gamecpp", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
		SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
	if (g_Window == NULL)
	{
		success = false;
	}
	else
	{
		g_screen = SDL_CreateRenderer(g_Window, -1, SDL_RENDERER_ACCELERATED);
		if (g_screen == NULL) success = false;
		else
		{
			SDL_SetRenderDrawColor(g_screen, RENDER_DRAW_COLOR, RENDER_DRAW_COLOR, RENDER_DRAW_COLOR, RENDER_DRAW_COLOR);
			int imgFlags = IMG_INIT_PNG;
			if (!(IMG_Init(imgFlags) && imgFlags))
				success = false;
		}
	}
	return success;
}
bool LoadBackGround()
{
	bool ret = g_background.LoadImg("img/bk1.jpg", g_screen);
	if (ret == false)
		return false;
	return true;
}
std::vector<ThreatObject*> MakeThreatList()
{
	std::vector<ThreatObject*> list_threats;// static threat


	ThreatObject* dynamic_threats = new ThreatObject[20];
	for (int i = 0; i < 20; i++)
	{
		ThreatObject* p_threat = (dynamic_threats + i);
		if (p_threat != NULL)
		{
			p_threat->LoadImg("img//threat_left.png", g_screen);
			p_threat->set_clips();
			p_threat->set_type_move_(ThreatObject::MOVE_IN_SPACE_THREAT);// trang thai threat: di chuyen
			p_threat->set_x_pos(550 + i * 1250);
			p_threat->set_y_pos(399);

			int pos1 = p_threat->get_x_pos() - 60;
			int pos2 = p_threat->get_x_pos() + 60;
			p_threat->setAnimationPos(pos1, pos2);// di chuyen trong pham vi pos1-->pos2
			p_threat->set_input_left(1);


			list_threats.push_back(p_threat);
		}
	}

	ThreatObject* threats_objs = new ThreatObject[20];
	for (int i = 0; i < 20; i++)
	{
		ThreatObject* p_threat = (threats_objs + i);
		if (p_threat != NULL)
		{
			p_threat->LoadImg("img//threat_level.png", g_screen);
			p_threat->set_clips();
			p_threat->set_x_pos(900 + i * 1200);
			p_threat->set_y_pos(250);
			p_threat->set_type_move_(ThreatObject::STATIC_THREAT);
			p_threat->set_input_left(0);
			//Threat ban dan: 
			BulletObject* p_bullet = new BulletObject;
			p_threat->InitBullet(p_bullet, g_screen);

			list_threats.push_back(p_threat);
		}
	}
	return list_threats;
}

#undef main 
int main(int argc, char* argv[])
{
	ImpTimer fps_timer;



	if (InitData() == false)
		return -1;
	if (LoadBackGround() == false)
		return -1;

	GameMap game_map;
	char nameFileMap[] = "map/map01.dat";
	game_map.LoadMap(nameFileMap);
	game_map.LoadTiles(g_screen);

	MainObject p_player;
	p_player.LoadImg("img//goku_right.png", g_screen);
	p_player.set_clip();

	std::vector<ThreatObject*> threats_list = MakeThreatList();


	bool is_quit = false;
	while (!is_quit)
	{
		fps_timer.start();
		while (SDL_PollEvent(&g_event) != 0)
		{
			if (g_event.type == SDL_QUIT)
			{
				is_quit = true;
			}
			p_player.HandleInputAction(g_event, g_screen);
		}
		SDL_SetRenderDrawColor(g_screen, RENDER_DRAW_COLOR, RENDER_DRAW_COLOR, RENDER_DRAW_COLOR, RENDER_DRAW_COLOR);
		SDL_RenderClear(g_screen);

		g_background.Render(g_screen, NULL);

		Map map_data = game_map.getMap();

		// Player
		p_player.HandleBullet(g_screen);
		p_player.SetMapXY(map_data.start_x_, map_data.start_y_);
		p_player.DoPlayer(map_data);
		p_player.Show(g_screen);

		//Map
		game_map.setMap(map_data);
		game_map.DrawMap(g_screen);

		//Threat
		for (int i = 0; i < threats_list.size(); i++)
		{
			ThreatObject* p_threat = threats_list.at(i);
			if (p_threat != NULL)
			{
				p_threat->SetMapXY(map_data.start_x_, map_data.start_y_);
				p_threat->ImpMoveType(g_screen);
				p_threat->DoPlayer(map_data);
				p_threat->MakeBullet(g_screen, SCREEN_WIDTH, SCREEN_HEIGHT);
				p_threat->Show(g_screen);
			}
		}


		SDL_RenderPresent(g_screen);

		int real_ipm_time = fps_timer.get_ticks();
		int time_one_frame = 1000 / FPS;// ms
		if (real_ipm_time < time_one_frame)
		{
			int delay_time = time_one_frame - real_ipm_time;
			if (delay_time >= 0)
			{
				SDL_Delay(delay_time);
			}
		}
	}
	for (int i = 0; i < threats_list.size(); i++)
	{
		ThreatObject* p_threat = threats_list.at(i);
		if (p_threat != NULL)
		{
			p_threat->Free();
			p_threat = NULL;
		}
	}
	threats_list.clear();

	close();
	return 0;
}