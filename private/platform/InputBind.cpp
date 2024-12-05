#include "platform/InputBind.h"

#include "port_bits/space_invaders.h"

#include <SDL.h>
#include <iostream>

namespace Platform
{
	SDL_SpaceInvadersInput::SDL_SpaceInvadersInput() = default;

	bool SDL_SpaceInvadersInput::HandlerInput()
	{
		while (SDL_PollEvent(&_event))
		{
			const SDL_Keycode keyc = _event.key.keysym.sym;

			switch (_event.type)
			{
			case SDL_QUIT:
				return false;
			case SDL_KEYDOWN:
			{
				switch (keyc)
				{
				case SDLK_RETURN:
					_input1_register |= PortBits::SP_Start1P;
					break;
				case SDLK_c:
					_input1_register |= PortBits::SP_Credit;
					break;
				case SDLK_t:
					_input2_register |= PortBits::SP_Tilt;
					break;
				case SDLK_SPACE:
					_input1_register |= PortBits::SP_Shot1P;
					_input2_register |= PortBits::SP_Shot2P;
					break;
				case SDLK_RIGHT:
					_input1_register |= PortBits::SP_Right1P;
					_input2_register |= PortBits::SP_Right2P;
					break;
				case SDLK_LEFT:
					_input1_register |= PortBits::SP_Left1P;
					_input2_register |= PortBits::SP_Left2P;
					break;
				case SDLK_2:
					_input1_register |= PortBits::SP_Start2P;
					break;
				default:
					break;
				}
				break;
			}
			case SDL_KEYUP:
				switch (keyc)
				{
				case SDLK_ESCAPE:
					return false;
				case SDLK_RETURN:
					_input1_register &= ~PortBits::SP_Start1P;
					break;
				case SDLK_c:
					_input1_register &= ~PortBits::SP_Credit;
					break;
				case SDLK_t:
					_input2_register &= ~PortBits::SP_Tilt;
					break;
				case SDLK_SPACE:
					_input1_register &= ~PortBits::SP_Shot1P;
					_input2_register &= ~PortBits::SP_Shot2P;
					break;
				case SDLK_RIGHT:
					_input1_register &= ~PortBits::SP_Right1P;
					_input2_register &= ~PortBits::SP_Right2P;
					break;
				case SDLK_LEFT:
					_input1_register &= ~PortBits::SP_Left1P;
					_input2_register &= ~PortBits::SP_Left2P;
					break;
				case SDLK_2:
					_input1_register &= ~PortBits::SP_Start2P;
					break;
				default:
					break;
				}
				break;
			default:
				break;
			}
		}

		return true;
	}

	bool SDL_SpaceInvadersInput::TryInitComponent()
	{
		if (SDL_Init(SDL_INIT_VIDEO) < 0)
		{
			std::cerr << "SDL not initialize, error: " << SDL_GetError() << '\n';
			return false;
		}

		return true;
	}

	uint8_t SDL_SpaceInvadersInput::GetInputReg1()
	{
		return _input1_register;
	}

	uint8_t SDL_SpaceInvadersInput::GetInputReg2()
	{
		return _input2_register;
	}

}	 // namespace Platform
