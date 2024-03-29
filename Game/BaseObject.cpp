﻿#include"BaseObject.h"

BaseObject::BaseObject()
{
	p_object = NULL;
	rect_.x = 0;
	rect_.y = 0;
	rect_.w = 0;
	rect_.h = 0;
}
BaseObject:: ~BaseObject()
{
	Free();
}
bool BaseObject::LoadImg(std::string path, SDL_Renderer* screen)
{
	//Free();
	SDL_Texture* new_texture = NULL;

	SDL_Surface* load_surface = IMG_Load(path.c_str());
	if (load_surface != NULL)
	{
		SDL_SetColorKey(load_surface, SDL_TRUE, SDL_MapRGB(load_surface->format, COLOR_KEY_R, COLOR_KEY_G, COLOR_KEY_B));
		// 167			175				180	
		new_texture = SDL_CreateTextureFromSurface(screen, load_surface);
		if (new_texture != NULL)
		{
			rect_.w = load_surface->w;
			rect_.h = load_surface->h;
		}
		SDL_FreeSurface(load_surface);
	}
	p_object = new_texture;

	return p_object != NULL;
}
void BaseObject::Render(SDL_Renderer* des, const SDL_Rect* clip)
{
	SDL_Rect renderquad = { rect_.x, rect_.y, rect_.w, rect_.h };
	SDL_RenderCopy(des, p_object, clip, &renderquad);

}
void BaseObject::Free()
{
	if (p_object != NULL)
	{
		SDL_DestroyTexture(p_object);
		p_object = NULL;
		rect_.w = 0;
		rect_.h = 0;
	}
}
void BaseObject::RenderLoss(SDL_Renderer* render)
{
	SDL_Texture* newTexture = NULL;
	SDL_Surface* loadedSurface = IMG_Load("img//gameover.png");
	newTexture = SDL_CreateTextureFromSurface(render, loadedSurface);
	SDL_FreeSurface(loadedSurface);

	SDL_Rect picLoss;
	picLoss.w = loadedSurface->w;
	picLoss.h = loadedSurface->h;
	picLoss.x = SCREEN_WIDTH / 2 - loadedSurface->w / 2;
	picLoss.y = SCREEN_HEIGHT / 2 - loadedSurface->h / 2;

	SDL_RenderCopy(render, newTexture, NULL, &picLoss);
	SDL_RenderPresent(render);
	SDL_Delay(1000);
}
void BaseObject::RenderWin(SDL_Renderer* render)
{
	SDL_Texture* newTexture = NULL;
	SDL_Surface* loadedSurface = IMG_Load("img//gamewin.png");
	newTexture = SDL_CreateTextureFromSurface(render, loadedSurface);
	SDL_FreeSurface(loadedSurface);

	SDL_Rect picWin;
	picWin.w = loadedSurface->w;
	picWin.h = loadedSurface->h;
	picWin.x = SCREEN_WIDTH / 2 - loadedSurface->w / 2;
	picWin.y = SCREEN_HEIGHT / 2 - loadedSurface->h / 2;

	SDL_RenderCopy(render, newTexture, NULL, &picWin);
	SDL_RenderPresent(render);
	SDL_Delay(1000);
}