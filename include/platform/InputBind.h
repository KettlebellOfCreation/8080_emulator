#pragma once
#include <SDL_events.h>

#include "platform/Platform.h"

namespace Platform
{
    class SDL_SpaceInvadersInput final : public IInput
    {
    public:
        SDL_SpaceInvadersInput();
        bool HandlerInput() override;
        bool TryInitComponent() override;
		uint8_t GetInputReg1() override;
		uint8_t GetInputReg2() override;

	private:
        SDL_Event _event{};
    };

} // namespace Platform
