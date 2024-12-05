#include "platform/SDL_display.h"

#include <iostream>

namespace Platform
{

	SDL_Display::SDL_Display() = default;
	SDL_Display::SDL_Display(const std::string_view &name_window, const uint16_t scale) :
		_name_window(name_window), _scale(scale)
	{
	}

	SDL_Display::~SDL_Display()
	{
		if (_surface)
			SDL_FreeSurface(_surface);

		if (_window)
			SDL_DestroyWindow(_window);

		SDL_QuitSubSystem(SDL_INIT_VIDEO);
		// SDL_Quit();
	}

	bool SDL_Display::TryInitWindow()
	{
		if (SDL_Init(SDL_INIT_VIDEO) < 0)
		{
			std::cerr << "SDL not initialize, error: " << SDL_GetError() << '\n';
			return false;
		}

		SDL_Window *window = SDL_CreateWindow(
			_name_window.data(),
			SDL_WINDOWPOS_CENTERED,
			SDL_WINDOWPOS_CENTERED,
			kDisplayWidth * _scale,
			kDisplayHeight * _scale,
			SDL_WINDOW_SHOWN | SDL_WINDOW_BORDERLESS | SDL_WINDOW_INPUT_FOCUS | SDL_WINDOW_FULLSCREEN_DESKTOP);

		if (!window)
		{
			std::cerr << "SDL not create window :^( , error: " << SDL_GetError() << '\n';
			return false;
		}

		SDL_Surface *surface = SDL_GetWindowSurface(window);
		if (!surface)
		{
			SDL_DestroyWindow(window);
			std::cerr << "SDL not create surface, error: " << SDL_GetError() << '\n';
			return false;
		}

		_window = window;
		_surface = surface;

		return true;
	}

	void SDL_Display::SetVideoBuffer(uint8_t *video_buffer) {}

	void SDL_Display::Draw()
	{
		int hw;
		int ww;
		SDL_GetWindowSize(_window, &ww, &hw);
		for (int i = 0; i < kDisplayBytes; ++i)
		{
			const uint8_t byte = *(_platform->GetCPU()->GetVideoBuffer() + i);

			const int y = kDisplayHeight * _scale - 1 - i % 32 * (8 * _scale);
			const int x = i / 32 * _scale;

			for (int j = 0; j < 8; ++j)
			{
				const int temp_y = y - j * _scale;

				for (int ys = 0; ys < _scale; ++ys)
				{
					for (int xs = 0; xs < _scale; ++xs)
					{
						int final_x = x + xs;
						int final_y = temp_y - ys;
						long color = 0;

						if (final_y >= 0 && final_y < 32 * _scale)
							color = 0xffffff;
						else if (final_y >= 32 * _scale && final_y < 64 * _scale)
							color = 0xFF0000;
						else if (final_y >= 64 * _scale && final_y < 184 * _scale)
							color = 0xFFFFFF;
						else if (final_y >= 184 * _scale && final_y < 240 * _scale)
							color = 0x00FF00;
						else if (final_y >= 240 * _scale && final_y < 256 * _scale && final_x >= 0 && final_x < 16 * _scale)
							color = 0xFFFFFF;
						else if (final_y >= 240 * _scale && final_y < 256 * _scale && final_x >= 16 * _scale && final_x < 134 * _scale)
							color = 0x00FF00;
						else if (final_y >= 240 * _scale && final_y < 256 * _scale && final_x >= 134 * _scale && final_x < 224 * _scale)
							color = 0xFFFFFF;

						final_x += ww / 2 - kDisplayWidth * _scale / 2;
						final_y += hw / 2 - kDisplayHeight * _scale / 2;

						if (byte & (1 << j))
							SetPixel(final_x, final_y, color);
						else
							SetPixel(final_x, final_y, 0x000000);
					}
				}
			}
		}
		SDL_UpdateWindowSurface(_window);
	}

	void SDL_Display::SetScale(uint16_t scale) {}

	void SDL_Display::SetPixel(const int x, const int y, const long color) const
	{
		auto *pixels = static_cast<Uint32 *>(_surface->pixels);
		pixels[y * (_surface->w) + x] = color;
	}

	bool SDL_Display::TryInitComponent()
	{
		return TryInitWindow();
	}

	uint32_t SDL_Display::GetTicks()
	{
		return SDL_GetTicks();
	}

	void SDL_Display::Delay(const int ticks)
	{
		if (ticks < 0)
			return;

		SDL_Delay(ticks);
	}

}	 // namespace Platform
