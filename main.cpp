#include "platform/InputBind.h"
#include "platform/Platform.h"
#include "platform/SDL_display.h"
#include "platform/Sound.h"

#include <iostream>
#include <windows.h>

int main(int argc, char *argv[])
{
	if (argc != 3)
	{
		argv[1] = new char[100];
		argv[2] = new char[100];
		std::cerr << "Input path to directory: ";
		scanf("%s", argv[1]);
		std::cout << "Input name ROM files: ";
		scanf("%s", argv[2]);

		HWND consoleWindow = GetConsoleWindow();
		if (consoleWindow != NULL)
			ShowWindow(consoleWindow, SW_MINIMIZE);	   // Сворачиваем окно
	}

	Platform::SDL_Display sdl_display("Space Invaders", 3);
	Platform::SDL_SpaceInvadersInput sdl_input;
	Platform::SDL_SpaceInvadersSound sdl_sound(argv[1]);

	Platform::Platform8080 platform(&sdl_display, &sdl_sound, &sdl_input);

	if (!platform.TryLoadPartsROM(argv[1], argv[2]))
	{
		std::cerr << "Can't load ROM. check path" << '\n';
		return -1;
	}

	// if (!platform.TryLoadPartsROM("./rom/SpaceInvaders/", "invaders"))
	// {
	// 	std::cerr << "Can't load ROM. check path" << '\n';
	// 	return -1;
	// }

	platform.Start();

	return 0;
}
