
#include "platform/Platform.h"

#include <fstream>

class ComponentInitError final : public std::exception
{
public:
	explicit ComponentInitError(const char *msg) : message(msg) {}

	[[nodiscard]] const char *what() const noexcept override { return message.c_str(); }

private:
	std::string message;
};

namespace Platform
{
	Platform8080::Platform8080(IDisplay *display, ISound *sound, IInput *input) :
		_display_system(display), _sound_system(sound), _input_system(input)
	{
		if (!_display_system->TryInitComponent())
			throw ComponentInitError("display system init error");

		if (!_sound_system->TryInitComponent())
			throw ComponentInitError("sound system init error");

		if (!_input_system->TryInitComponent())
			throw ComponentInitError("input system init error");

		_display_system->SetPlatform(this);
		_sound_system->SetPlatform(this);
		_input_system->SetPlatform(this);

		_cpu.BindInputPort(CPU_Input1, [this] { return _input_system->GetInputReg1(); });
		_cpu.BindInputPort(CPU_Input2, [this] { return _input_system->GetInputReg2(); });

		_cpu.BindInputPort(CPU_ShiftIN, [this] { return _shift_register >> (8 - _shift_amnt_register); });
		_cpu.BindOutputPort(CPU_ShiftOUT, [this](const uint8_t data) { WriteShift(data); });
		_cpu.BindOutputPort(CPU_ShiftAMNT, [this](const uint8_t data) { _shift_amnt_register = data & 7; });

		_cpu.BindOutputPort(CPU_Sound1, [this](const uint8_t data) { _sound_system->SoundPort1(data); });
		_cpu.BindOutputPort(CPU_Sound2, [this](const uint8_t data) { _sound_system->SoundPort2(data); });

		_cpu.BindOutputPort(CPU_WatchDog, [this](const uint8_t data) { WriteWatchdog(data); });
	}

	Platform8080::~Platform8080()
	= default;

	void Platform8080::Start()
	{
		while (!_cpu.IsExit())
		{
			_display_system->Draw();
			_cpu.SetExitFlag(!_input_system->HandlerInput());

			const uint32_t ticks = _display_system->GetTicks();

			while (_cpu.GetCycleCounter() < IDisplay::kVBlankRate / 2)
				_cpu.Tick();

			_cpu.RequiredInterrupt(Intel8080::OP_RST_1);

			while (_cpu.GetCycleCounter() < IDisplay::kVBlankRate)
				_cpu.Tick();

			_cpu.RequiredInterrupt(Intel8080::OP_RST_2);



			_cpu.SetCycleCounter(0);
			_display_system->Delay(1000 / IDisplay::kDisplayRate - (_display_system->GetTicks() - ticks));
		}
	}

	bool Platform8080::TryLoadROM(const std::string_view &file_name, const uint16_t start_addres)
	{
		std::ifstream rom_file(file_name.data(), std::ios::binary);

		if (!rom_file.is_open())
			return false;

		rom_file.readsome(reinterpret_cast<char *>(_cpu.GetMemory() + start_addres), Intel8080::CPU_8080::kMaxMemory);
		rom_file.close();
		return true;
	}

	bool Platform8080::TryLoadPartsROM(const std::string_view &directory_path, const std::string_view &parts_name)
	{
		std::string file_path(directory_path);

		if (!directory_path.ends_with('/'))
			file_path.push_back('/');

		file_path += parts_name;

		if (!TryLoadROM(file_path + ".h", 0x0000) || !TryLoadROM(file_path + ".g", 0x0800) ||
			!TryLoadROM(file_path + ".f", 0x1000) || !TryLoadROM(file_path + ".e", 0x1800))
			return false;

		return true;
	}

	Intel8080::CPU_8080 *Platform8080::GetCPU()
	{
		return &_cpu;
	}
	void Platform8080::WriteShift(const uint8_t data)
	{
		_shift_register >>= 8;
		_shift_register |= (data << 8);
	}

	void Platform8080::WriteWatchdog(uint8_t data)
	{
		/* Space Invaders will call this periodically to reset the watchdog timer.
		 * Since our emulation has no use for a watchdog timer, we can ignore it.
		 */
	}

}	 // namespace Platform
