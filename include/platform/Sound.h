#pragma once
#include <SDL_mixer.h>

#include "platform/Platform.h"

namespace Platform
{
    class SDL_SpaceInvadersSound final : public ISound
    {
    public:
        ~SDL_SpaceInvadersSound() override;
        SDL_SpaceInvadersSound();
    	explicit SDL_SpaceInvadersSound(const std::string_view &sounds_path);

        bool TryInitComponent() override;
		void SoundPort1(uint8_t data) override;
		void SoundPort2(uint8_t data) override;

	private:
    	std::string _sounds_path = "./";
        Mix_Chunk *_sounds[kNumSounds]{};
    };

} // namespace Platform
