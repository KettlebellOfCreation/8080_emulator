#pragma once
#define SDL_MAIN_HANDLED true

#include "Platform.h"
#include "SDL2/SDL.h"

struct SDL_Window;
namespace Platform
{

    class SDL_Display final : public IDisplay
    {
    public:
        SDL_Display();
		SDL_Display(const std::string_view &name_window, uint16_t scale);
        ~SDL_Display() override;
        bool TryInitWindow() override;
        void SetVideoBuffer(uint8_t *video_buffer) override;
        void Draw() override;
        void SetScale(uint16_t scale) override;

    private:
        void SetPixel(int x, int y, long color) const;

    public:
        bool TryInitComponent() override;
		uint32_t GetTicks() override;
		void Delay(const int ticks) override;

	private:
        std::string_view _name_window = "undefined";
        uint16_t _scale = 3;
        SDL_Surface *_surface{};
        SDL_Window *_window{};
    };

} // namespace Platform

