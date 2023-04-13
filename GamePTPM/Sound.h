#pragma once
#include "SDL_mixer.h"

static bool CheckSound = true;
static Mix_Chunk* mix_kame = NULL;
static Mix_Chunk* mix_hit_threat = NULL ;
static Mix_Chunk* get_coin = NULL;
static void MixKame()
{
	mix_kame = Mix_LoadWAV("sound//laser_shot.wav");
	if (mix_kame == NULL)
	{
		CheckSound = false;
	}
	else
	{
		Mix_PlayChannel(-1, mix_kame, 0);
		Mix_VolumeChunk(mix_kame, 50);
	}
}
static void MixGetCoin()
{
	get_coin = Mix_LoadWAV("sound//getMoney.wav");
	if (get_coin == NULL)
	{
		CheckSound = false;
	}
	else
	{
		Mix_PlayChannel(-1, get_coin, 0);
	}
}
static void MixHit()
{
	mix_hit_threat = Mix_LoadWAV("sound//hit.wav");
	if (mix_hit_threat == NULL)
	{
		CheckSound = false;
	}
	else
	{
		Mix_PlayChannel(-1, mix_hit_threat, 0);
	}
}