#pragma once
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


TTF_Font* font_menu = NULL;
bool is_quit = false;

static SDL_Texture* LoadTexture(std::string path)
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

static bool createImage(SDL_Texture* texture)
{
	if (texture == NULL) return false;
	SDL_RenderCopy(g_screen, texture, NULL, NULL);
	return true;
}

static bool LoadMenu()
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


static int showMenu()
{
	if (TTF_Init() < 0)
	{
		std::cout << TTF_GetError();
		return 0;
	}
	font_menu = TTF_OpenFont("font//ObelixProB-cyr.ttf", 60);

	int x, y;

	const int numMenu = 2;
	SDL_Color colorMenu[numMenu] = { {142, 22, 222}, {255, 0, 0} };
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

	desRest[0].x = SCREEN_WIDTH/2 - 100;
	desRest[0].y = 250;

	desRest[0].w = scrRest[0].w;
	desRest[0].h = scrRest[0].h;

	scrRest[1].x = 0;
	scrRest[1].y = 0;

	desRest[1].x = SCREEN_WIDTH/2 - 100;
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
					if (x >= desRest[i].x && x <= desRest[i].x + desRest[i].w 
						&& y >= desRest[i].y && y <= desRest[i].y + desRest[i].h)
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
					if (x >= desRest[i].x && x <= desRest[i].x + desRest[i].w 
						&& y >= desRest[i].y && y <= desRest[i].y + desRest[i].h)
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
