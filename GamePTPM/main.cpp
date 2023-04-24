#include "SDL.h"
#include "BaseObject.h"
#include "CommonFunc.h"
#include "GameMap.h"
#include "MainObject.h"
#include "ImpTimer.h"
#include "ThreatObject.h"
#include "ExplosionObj.h"
#include "Text.h"
#include "GamePower.h"
#include "BossObj.h"
#include "Sound.h"
BaseObject g_background;
TTF_Font* font_common;
TTF_Font* font_menu = NULL;
bool is_quit = false;


void close()
{
	g_background.Free();
	SDL_DestroyRenderer(g_screen);
	g_screen = NULL;
	SDL_DestroyWindow(g_Window);
	g_Window = NULL;
	IMG_Quit();
	SDL_Quit();
	Mix_CloseAudio();
}

bool InitData()
{
	bool success = true;
	int ret = SDL_Init(SDL_INIT_VIDEO);
	if (ret < 0) return false;

	SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1");
	g_Window = SDL_CreateWindow("Battle_of_Warrior", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
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
		if (TTF_Init() == -1)
		{
			success = false;
		}
		font_common = TTF_OpenFont("font//ObelixProB-cyr.ttf", 20);
		if (font_common == NULL)
		{
			success = false;
		}
	}
	return success;
}
//======= Start: Load IMG : normal========
SDL_Texture* LoadTexture(std::string path)
{
	SDL_Texture* newTexture = NULL;
	SDL_Surface* loadedSurface = IMG_Load(path.c_str());
	if (loadedSurface == NULL) std::cout << "Unable to load image " << path << " SDL_image Error: " << IMG_GetError() << std::endl;
	else
	{
		newTexture = SDL_CreateTextureFromSurface(g_screen, loadedSurface);
		if (newTexture == NULL) std::cout << "Unable to create texture from " << path << " SDL Error: " << SDL_GetError() << std::endl;
		SDL_FreeSurface(loadedSurface);
	}
	return newTexture;
}
//======= End: Load IMG : normal========

//======= Start: LoadMenu IntroGame========

bool LoadMenu()
{
	bool succes = true;
	g_texture = LoadTexture("img//introIMG.jpg");
	if (g_texture == NULL)
	{
		std::cout << "Error image" << std::endl;
		succes = false;
	}
	return succes;
}

bool createImage(SDL_Texture* texture)
{
	if (texture == NULL) return false;
	SDL_RenderCopy(g_screen, texture, NULL, NULL);
	return true;
}
int showMenu()
{
	if (TTF_Init() < 0)
	{
		std::cout << TTF_GetError();
		return 0;
	}
	font_menu = TTF_OpenFont("font//ObelixProB-cyr.ttf", 60);

	int x, y;

	const int numMenu = 2;
	SDL_Color colorMenu[numMenu] = { {243, 156, 18}, {255, 0, 0} };
	std::string text[numMenu] = { "Play", "Exit" };
	bool selected[numMenu] = { 0,0 };

	SDL_Surface* surface[numMenu];
	for (int i = 0; i < numMenu; i++) surface[i] = TTF_RenderText_Solid(font_menu, text[i].c_str(), colorMenu[0]);

	SDL_Texture* texture[numMenu];
	for (int i = 0; i < numMenu; i++) texture[i] = SDL_CreateTextureFromSurface(g_screen, surface[i]);
	for (int i = 0; i < numMenu; i++) SDL_FreeSurface(surface[i]);

	SDL_Rect scrRest[numMenu];
	SDL_Rect desRest[numMenu];
	for (int i = 0; i < numMenu; i++) TTF_SizeText(font_menu, text[i].c_str(), &scrRest[i].w, &scrRest[i].h);

	scrRest[0].x = 0;
	scrRest[0].y = 0;

	desRest[0].x = 150;
	desRest[0].y = 250;

	desRest[0].w = scrRest[0].w;
	desRest[0].h = scrRest[0].h;

	scrRest[1].x = 0;
	scrRest[1].y = 0;

	desRest[1].x = 150;
	desRest[1].y = 330;

	desRest[1].w = scrRest[1].w;
	desRest[1].h = scrRest[1].h;
	while (!is_quit)
	{
		while (SDL_PollEvent(&g_event))
		{
			switch (g_event.type)
			{
			case SDL_QUIT:
			{
				is_quit = true;;
				return 1;
			}
			case SDL_MOUSEMOTION:
				x = g_event.motion.x;
				y = g_event.motion.y;
				for (int i = 0; i < numMenu; i++)
				{
					if (x >= desRest[i].x && x <= desRest[i].x + desRest[i].w && y >= desRest[i].y && y <= desRest[i].y + desRest[i].h)
					{
						if (!selected[i])
						{
							MixSelect();
							selected[i] = 1;
							surface[i] = TTF_RenderText_Solid(font_menu, text[i].c_str(), colorMenu[1]);
							texture[i] = SDL_CreateTextureFromSurface(g_screen, surface[i]);
						}
					}
					else
					{
						if (selected[i])
						{
							selected[i] = 0;
							surface[i] = TTF_RenderText_Solid(font_menu, text[i].c_str(), colorMenu[0]);
							texture[i] = SDL_CreateTextureFromSurface(g_screen, surface[i]);
						}
					}
				}
				break;
			case SDL_MOUSEBUTTONDOWN:
				x = g_event.button.x;
				y = g_event.button.y;
				for (int i = 0; i < numMenu; i++)
				{
					if (x >= desRest[i].x && x <= desRest[i].x + desRest[i].w && y >= desRest[i].y && y <= desRest[i].y + desRest[i].h)
					{
						return i;
					}
				}
				break;
			case SDL_KEYDOWN:
				if (g_event.key.keysym.sym == SDLK_ESCAPE)
				{
					return 1;
				}
			default:
			{
				break;
			}
			}
		}
		for (int i = 0; i < numMenu; i++)
		{
			SDL_RenderCopy(g_screen, texture[i], &scrRest[i], &desRest[i]);
			SDL_RenderPresent(g_screen);
		}
	}
	return 1;
}

//=========End: Show: Menu============
//=========Start: Load Sound Effect========

bool LoadMedia()
{
	if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
	{
		return false;
	}
	if (CheckSound == false)
	{
		return false;
	}
}
//=========End: Load Sound Effect========
 

//=========Start: Load Background========
bool LoadBackGround()
{
	bool ret = g_background.LoadImg("img//bk1.jpg", g_screen);
	if (ret == false)
		return false;
	return true;
}

//==========Start: Load Dynamic threat (No Bullet)
std::vector<ThreatObject*> MakeThreatList()
{
	std::vector<ThreatObject*> list_threats;// static threat
	ThreatObject* dynamic_threats = new ThreatObject[20];
	srand(time(0));
	for (int i = 1; i < 20; i++)
	{
		ThreatObject* p_threat = (dynamic_threats + i);
		if (p_threat != NULL)
		{
			p_threat->LoadImg("img//threat_left.png", g_screen);
			p_threat->set_clips();
			p_threat->set_type_move_(ThreatObject::MOVE_IN_SPACE_THREAT);// trang thai threat: di chuyen
			p_threat->set_x_pos(rand() %926 + i * 1250);
			p_threat->set_y_pos(399);

			int pos1 = p_threat->get_x_pos() - 120;
			int pos2 = p_threat->get_x_pos() + 120;
			p_threat->setAnimationPos(pos1, pos2);// di chuyen trong pham vi pos1-->pos2
			p_threat->set_input_left(1);


			list_threats.push_back(p_threat);
		}
	}
//==========Start: Threat have bullets============
	ThreatObject* threats_objs = new ThreatObject[20];
	for (int i = 1; i < 18; i++)
	{
		ThreatObject* p_threat = (threats_objs + i);
		if (p_threat != NULL)
		{
			p_threat->LoadImg("img//threat_level.png", g_screen);
			p_threat->set_clips();
			p_threat->set_x_pos(rand()%1930 + i * 890);
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
//=========================================
#undef main 
int main(int argc, char* argv[])
{
	ImpTimer fps_timer;
// Khoi tao tat ca cac doi tuong==========
	if (InitData() == false)
		return -1;
	if (LoadMenu() == true)
	{
		createImage(g_texture);
		SDL_RenderPresent(g_screen);
	}
	if (LoadBackGround() == false)
		return -1;
	if (LoadMedia() == false)
		return -1;
	MixBackGround();
//========================
//======LoadMap===========
	GameMap game_map;
	char nameFileMap[] = "map/map01.dat";
	game_map.LoadMap(nameFileMap);
	game_map.LoadTiles(g_screen);
//======LoadPlayer========
	//Image
	MainObject p_player;
	p_player.LoadImg("img//goku_right.png", g_screen);
	p_player.set_clip();
	p_player.setScore(0);
	//Heart icon
	GamePower player_heart;
	player_heart.Init(g_screen);
	//Coin icon
	Coin player_money;
	player_money.Init(g_screen);
	player_money.setPos(SCREEN_WIDTH / 2 - 100, 12);

	std::vector<ThreatObject*> threats_list = MakeThreatList();
	//Boss:
	BossObject BossObj;
	BossObj.LoadImg("img//boss_object.png", g_screen);
	BossObj.set_clips();
	int PosBoss = MAX_MAP_X * TILE_SIZE - SCREEN_WIDTH;
	BossObj.set_x_pos(PosBoss);
	BossObj.set_y_pos(20);
	int boss_blood = 100;


	//Khoi tao vu no
	ExplosionObj exp_threat;
	bool tRet = exp_threat.LoadImg("img//exp3.png", g_screen);
	if (!tRet)
	{
		return false;
	}
	else {
		exp_threat.set_clip();
	}
	ExplosionObj exp_player;
	bool epRet = exp_player.LoadImg("img//exp33.png", g_screen);
	if (!epRet)
	{
		return false;
	}
	else {
		exp_player.set_clip();
	}
	ExplosionObj exp_boss;
	bool tRetBoss = exp_boss.LoadImg("Img//boss_bullet.png", g_screen);
	if (!tRetBoss)
	{
		return false;
	}
	else
	{
		exp_boss.set_clip();
	}
	int heart = 3;

	//Text trong Game:
	TextObj time_game;
	time_game.SetColor(TextObj::RED_TXT);
	TextObj Coin_txt;
	Coin_txt.SetColor(TextObj::RED_TXT);
	TextObj score_txt;
	score_txt.SetColor(TextObj::RED_TXT);


	bool boss = true;
	int tmp = showMenu();
	if (tmp == 1)
	{
		is_quit = false;
		return 0;
	}
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
		//Check Player outside map
		if (p_player.GetRect().y > 630)
		{
			heart--;
			p_player.SetRect(0, 0);
			p_player.set_comeback_time(40);
			SDL_Delay(600);
			player_heart.Decrease();
			player_heart.Render(g_screen);
			continue;
		}
		if (heart == 0)
		{
			if (MessageBox(NULL, L"GameOver!!", L"InFo", MB_OK | MB_ICONSTOP) == IDOK)
			{
				close();
				SDL_Quit();
				return 0;
			}
		}
//==End: Init Player=====================================
		//Map
		game_map.setMap(map_data);
		game_map.DrawMap(g_screen);


		//show Mang:

		player_heart.Show(g_screen);

		//Show coin
		player_money.Show(g_screen);

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

				SDL_Rect rect_player = p_player.GetRectFrame();
				bool bCol1 = false;
				std::vector<BulletObject*> tBullet_list = p_threat->get_bullet_list();
				for (int j = 0; j < tBullet_list.size(); ++j)
				{
					BulletObject* pt_bullet = tBullet_list.at(j);
					if (pt_bullet)
					{
						bCol1 = SDLCommonFunc::CheckCollision(pt_bullet->GetRect(), rect_player);
						if (bCol1 && heart ==0)
						{
							MixHit();
							p_threat->RemoveBullet(j);
							break;
						}
						else if (bCol1 && heart >= 1)
						{
							MixHit();
							break;
						}
					}
				}
//=============Check va cham dan threat vs Player
				SDL_Rect rect_threat = p_threat->GetRectFrame();
				bool bCol2 = SDLCommonFunc::CheckCollision(rect_player, rect_threat);
				if (bCol1 || bCol2 )
				{
					//Die
					int frame_die_w = exp_player.get_frame_width();
					int frame_die_h = exp_player.get_frame_height();

					for (int ex = 0; ex < 8; ex++)
					{
						int x_pos = p_player.GetRect().x + p_player.get_frame_width() * 0.5 - frame_die_w * 0.5;
						int y_pos = p_player.GetRect().y + p_player.get_frame_height() * 0.5 - frame_die_h * 0.5;

						exp_threat.set_frame(ex);
						exp_threat.SetRect(x_pos, y_pos);
						exp_threat.Show(g_screen);
						SDL_RenderPresent(g_screen);
					}

					heart--;
					if (heart >=1)
					{
						p_player.SetRect(0, 0);
						p_player.set_comeback_time(40);
						SDL_Delay(600);
						player_heart.Decrease();
						player_heart.Render(g_screen);
						continue;
					}
					else
					{
						if (MessageBox(NULL, L"GameOver!!", L"InFo", MB_OK | MB_ICONSTOP) == IDOK)
						{
							p_threat->Free();
							close();
							SDL_Quit();
							return 0;
						}
					}

					
				}

			}
		}
		//check va cham dan player trung threat
		int frame_exp_width = exp_threat.get_frame_width();
		int frame_exp_height = exp_threat.get_frame_height();

		std::vector<BulletObject*> bullet_arr = p_player.get_bullet_list();
		for (int r = 0; r < bullet_arr.size(); r++)
		{
			BulletObject* p_bullet = bullet_arr.at(r);
			if (p_bullet != NULL)
			{
				for (int t = 0; t < threats_list.size(); t++)
				{
					ThreatObject* obj_threat = threats_list.at(t);
					if (obj_threat != NULL)
					{
						SDL_Rect tRect;
						tRect.x = obj_threat->GetRect().x;
						tRect.y = obj_threat->GetRect().y;
						tRect.w = obj_threat->get_width_frame();// neu dung getRect se lay ra width = 480px;
						tRect.h = obj_threat->get_height_frame();


						SDL_Rect bRect = p_bullet->GetRect();//bRect: toa do dan player
						bool bCol = SDLCommonFunc::CheckCollision(bRect, tRect);//tRect : Toa do threat

						if (bCol)
						{
							p_player.inCrease15Point();
							MixHit();
							for (int ex = 0; ex < 8; ex++)
							{
								int x_pos = p_bullet->GetRect().x - frame_exp_width * 0.2;
								int y_pos = p_bullet->GetRect().y - frame_exp_height * 0.5;

								exp_threat.set_frame(ex);
								exp_threat.SetRect(x_pos, y_pos);
								exp_threat.Show(g_screen);
								SDL_RenderPresent(g_screen);
							}

							p_player.RemoveBullet(r);
							obj_threat->Free();
							threats_list.erase(threats_list.begin() + t);
						}
					}
				}
			}
		}


		//Show Boss
		BossObj.SetMapXY(map_data.start_x_, map_data.start_y_);
		BossObj.DoPlayer(map_data, boss_blood);
		BossObj.MakeBullet(g_screen, SCREEN_WIDTH, SCREEN_HEIGHT);
		BossObj.Show(g_screen, boss_blood);

		//Check Boss_Player
		SDL_Rect rect_player = p_player.GetRectFrame();
		bool bCol1 = false;
		std::vector<BulletObject*> boss_bullets = BossObj.get_bullet_list();
		for (int id = 0; id < boss_bullets.size(); id++)
		{
			BulletObject* boss_bullet = boss_bullets[id];
			if (boss_bullet)
			{
				SDL_Rect boss_bullet_rect = boss_bullet->GetRect();
				bCol1 = SDLCommonFunc::CheckCollision(rect_player, boss_bullet_rect);
				if (bCol1)
				{
					BossObj.RemoveBullet(id);
					break;
				}
			}
		}
		//Dan cua Boss trung player
		SDL_Rect rect_boss = BossObj.GetRectFrame();
		bool bCol2 = SDLCommonFunc::CheckCollision(rect_boss, rect_player);
		if (bCol1 || bCol2)
		{
			int width_exp_frame = exp_player.get_frame_width();
			int height_exp_frame = exp_player.get_frame_height();
			for (int ex = 0; ex < 8; ex++)
			{
				int x_pos = (p_player.GetRect().x + p_player.get_frame_width() * 0.5) - width_exp_frame * 0.5;
				int y_pos = (p_player.GetRect().y + p_player.get_frame_height() * 0.5) - height_exp_frame * 0.5;

				exp_player.set_frame(ex);
				exp_player.SetRect(x_pos, y_pos);
				exp_player.Show(g_screen);
				SDL_RenderPresent(g_screen);
			}
			heart--;

			if (heart >= 1)
			{
				p_player.SetRect(0, 0);
				p_player.set_comeback_time(100);
				SDL_Delay(500);
				player_heart.Decrease();
				player_heart.Render(g_screen);
				continue;
			}
			else
			{
				//Game Won
			}
		}
		//Dan player trung boss
		int frame_exp_width_boss = exp_boss.get_frame_width();
		int frame_exp_height_boss = exp_boss.get_frame_height();

		//std::vector<BulletObject*> bullet_arr = p_player.get_bullet_list();
		for (int r = 0; r < bullet_arr.size(); r++)
		{
			BulletObject* p_bullet = bullet_arr[r];
			if (p_bullet != NULL)
			{
				SDL_Rect bossRect;
				bossRect.x = BossObj.GetRect().x;
				bossRect.y = BossObj.GetRect().y;
				bossRect.w = BossObj.get_width_frame();
				bossRect.h = BossObj.get_height_frame();

				SDL_Rect bRect = p_bullet->GetRect();

				bool bCol = SDLCommonFunc::CheckCollision(bRect, bossRect);

				if (bCol == true)
				{
					MixHit();
					for (int ex = 0; ex < NUM_FRAME_EXP; ex++)
					{
						int x_pos = p_bullet->GetRect().x - frame_exp_width_boss * 0.5;
						int y_pos = p_bullet->GetRect().y - frame_exp_height_boss * 0.5;

						exp_boss.set_frame(ex);
						exp_boss.SetRect(x_pos, y_pos);
						exp_boss.Show(g_screen);
						SDL_RenderPresent(g_screen);
					}

					boss_blood -= 2;
					exp_boss.Free();
					p_player.RemoveBullet(r);
					if (boss_blood == 0)
					{
						p_player.inCrease50Point();
						BossObj.Free();
						boss = false;
						std::vector<BulletObject*> tBullet_list = BossObj.get_bullet_list();
						for (int jj = 0; jj < tBullet_list.size(); jj++)
							BossObj.RemoveBullet(jj);

						is_quit = true;
					}

				}

			}
		}


		int val = MAX_MAP_X * TILE_SIZE - (map_data.start_x_ + p_player.GetRect().x) - 750;
		if (val <= SCREEN_WIDTH && boss == true)
		{
			BossObj.ShowHP(g_screen, boss_blood);
		}




		//=====================SHOW TEXT==========
		// show Text time:
		std::string str_time = "Time: ";
		Uint32 time_value = SDL_GetTicks() / 1000;
		Uint32 value_time = 180 - time_value;
		if (value_time <= 0)
		{
			if (MessageBox(NULL, L"GameOver!!", L"Alert", MB_OK | MB_ICONSTOP) == IDOK)
			{
				is_quit = true;
				break;
				/*close();
				SDL_Quit();
				return 0;*/
			}
		}
		else {
			std::string str_val = std::to_string(value_time);
			str_time += str_val;

			time_game.SetText(str_time);
			time_game.LoadFromRenderText(font_common, g_screen);
			time_game.RenderText(g_screen, SCREEN_WIDTH - 200, 12);
		}

		//show coin
		int cur_money = p_player.GetMoney();
		std::string show_money = ": ";
		std::string money = std::to_string(cur_money);
		show_money += money;
		Coin_txt.SetText(show_money);
		Coin_txt.LoadFromRenderText(font_common, g_screen);
		Coin_txt.RenderText(g_screen, SCREEN_WIDTH / 2 - 70, 15);

		//show SCORE
		int cur_score = p_player.getScore();
		std::string show_score = "Score: ";
		std::string sc = std::to_string(cur_score);
		show_score += sc;
		score_txt.SetText(show_score);
		score_txt.LoadFromRenderText(font_common, g_screen);
		score_txt.RenderText(g_screen, 20, 40);

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