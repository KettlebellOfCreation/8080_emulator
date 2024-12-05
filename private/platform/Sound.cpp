#include "platform/Sound.h"

#include "port_bits/space_invaders.h"

#include <iostream>

namespace Platform
{
	enum SpaceInvaders_Sound
	{
		SP_UfoMove,
		SP_ShootLazer,
		SP_PlayerDeath,
		SP_InvaderDeath,
		SP_Fleet1,
		SP_Fleet2,
		SP_Fleet3,
		SP_Fleet4,
		SP_UfoHit,
		SP_ExtraLife
	};

	SDL_SpaceInvadersSound::~SDL_SpaceInvadersSound()
	{
		Mix_Quit();
	}

	SDL_SpaceInvadersSound::SDL_SpaceInvadersSound() = default;
	SDL_SpaceInvadersSound::SDL_SpaceInvadersSound(const std::string_view &sounds_path) : _sounds_path(sounds_path)
	{
		if (!_sounds_path.ends_with('/'))
			_sounds_path.push_back('/');

		_sounds_path.append("sounds/");
	}

	bool SDL_SpaceInvadersSound::TryInitComponent()
	{
		if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
		{
			std::cerr << "Could not initialize SDL mixer.\n" << std::endl;
			return false;
		}

		_sounds[SP_UfoMove] = Mix_LoadWAV((_sounds_path + "ufo_move.wav").data());
		_sounds[SP_ShootLazer] = Mix_LoadWAV((_sounds_path + "shot.wav").data());
		_sounds[SP_PlayerDeath] = Mix_LoadWAV((_sounds_path + "player_died.wav").data());
		_sounds[SP_InvaderDeath] = Mix_LoadWAV((_sounds_path + "invader_died.wav").data());
		_sounds[SP_Fleet1] = Mix_LoadWAV((_sounds_path + "fleet_1.wav").data());
		_sounds[SP_Fleet2] = Mix_LoadWAV((_sounds_path + "fleet_2.wav").data());
		_sounds[SP_Fleet3] = Mix_LoadWAV((_sounds_path + "fleet_3.wav").data());
		_sounds[SP_Fleet4] = Mix_LoadWAV((_sounds_path + "fleet_4.wav").data());
		_sounds[SP_UfoHit] = Mix_LoadWAV((_sounds_path + "ufo_hit.wav").data());
		_sounds[SP_ExtraLife] = Mix_LoadWAV((_sounds_path + "extra_life.wav").data());

		for (int i = 0; i < kNumSounds; i++) {
			if (!_sounds[i]) {
				fprintf(stderr, "Failed to load sound effect %d\n", i);
			}
		}

		return true;
	}

	static int ufo_move_chan = -1;

	void SDL_SpaceInvadersSound::SoundPort1(const uint8_t data)
	{
		if (data & PortBits::SP_Ufo && _sounds[SP_UfoMove])
			ufo_move_chan = Mix_PlayChannel(-1, _sounds[SP_UfoMove], 0);

		if (!(_sound_register_1 & PortBits::SP_Shot) && (data & PortBits::SP_Shot) && _sounds[SP_ShootLazer])
			Mix_PlayChannel(-1, _sounds[SP_ShootLazer], 0);

		if (!(_sound_register_1 & PortBits::SP_PlayerDie) && (data & PortBits::SP_PlayerDie) && _sounds[SP_PlayerDeath])
			Mix_PlayChannel(-1, _sounds[SP_PlayerDeath], 0);

		if (!(_sound_register_1 & PortBits::SP_InvaderDie) && (data & PortBits::SP_InvaderDie) && _sounds[SP_InvaderDeath])
			Mix_PlayChannel(-1, _sounds[SP_InvaderDeath], 0);

		if (!(_sound_register_1 & PortBits::SP_ExtraLife) && (data & PortBits::SP_ExtraLife) && _sounds[SP_ExtraLife])
			Mix_PlayChannel(-1, _sounds[SP_ExtraLife], 0);

		_sound_register_1 = data;
	}

	void SDL_SpaceInvadersSound::SoundPort2(const uint8_t data)
	{
		if (!(_sound_register_2 & PortBits::SP_FleetMove1) && (data & PortBits::SP_FleetMove1) && _sounds[SP_Fleet1])
			Mix_PlayChannel(-1, _sounds[SP_Fleet1], 0);

		if (!(_sound_register_2 & PortBits::SP_FleetMove2) && (data & PortBits::SP_FleetMove2) && _sounds[SP_Fleet1])
			Mix_PlayChannel(-1, _sounds[SP_Fleet2], 0);

		if (!(_sound_register_2 & PortBits::SP_FleetMove3) && (data & PortBits::SP_FleetMove3) && _sounds[SP_Fleet3])
			Mix_PlayChannel(-1, _sounds[SP_Fleet3], 0);

		if (!(_sound_register_2 & PortBits::SP_FleetMove4) && (data & PortBits::SP_FleetMove4) && _sounds[SP_Fleet4])
			Mix_PlayChannel(-1, _sounds[SP_Fleet4], 0);

		if (!(_sound_register_2 & PortBits::SP_UfoHit) && (data & PortBits::SP_UfoHit) && _sounds[SP_UfoHit])
		{
			Mix_HaltChannel(ufo_move_chan);
			Mix_PlayChannel(-1, _sounds[SP_UfoHit], 0);
		}

		_sound_register_2 = data;
	}

}	 // namespace Platform
