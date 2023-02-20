#include"SDL.h"
#include"BaseObject.h"
#include "CommonFunc.h"
#include"GameMap.h"
#include"MainObject.h"
#include"ImpTimer.h"
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
	bool ret = g_background.LoadImg("img/bk1.png", g_screen);
	if (ret == false)
		return false;
	return true;
}

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
	p_player.LoadImg("img//player_right.png", g_screen);
	p_player.set_clip();



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

		p_player.HandleBullet(g_screen);
		p_player.SetMapXY(map_data.start_x_, map_data.start_y_);
		p_player.DoPlayer(map_data);
		p_player.Show(g_screen);

		game_map.setMap(map_data);
		game_map.DrawMap(g_screen);

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
			std::cout << FPS << std::endl;
		}
	}
	close();
	return 0;
}