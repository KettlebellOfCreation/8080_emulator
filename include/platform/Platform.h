#pragma once
#include <string>
#include "cpu/CPU_8080.h"

namespace Platform
{
    class Platform8080;

	enum PortBytes
	{
		CPU_Input1 = 1,
		CPU_Input2 = 2,
		CPU_ShiftAMNT = 2,
		CPU_ShiftIN = 3,
		CPU_ShiftOUT = 4,
		CPU_Sound1 = 3,
		CPU_Sound2 = 5,
		CPU_WatchDog = 6
	};

    class IComponent
    {
    public:
        virtual ~IComponent() = default;
        virtual bool TryInitComponent() = 0;
        void SetPlatform(Platform8080 *platform)
        {
            _platform = platform;
        }

    protected:
        Platform8080 *_platform = nullptr;
    };

    class IDisplay : public IComponent
    {
    public:
        static constexpr uint16_t kDisplayHeight = 256;
        static constexpr uint16_t kDisplayWidth = 224;
        static constexpr uint16_t kDisplayBytes = kDisplayHeight * 28;

        static constexpr uint16_t kDisplayRate = 60;
        static constexpr uint32_t kCpuClock = 2'000'000;
    	static constexpr uint32_t kVBlankRate = kCpuClock / kDisplayRate;

    public:
        virtual void SetVideoBuffer(uint8_t *video_buffer) = 0;
        virtual void Draw() = 0;
        virtual void SetScale(uint16_t scale) = 0;
        virtual bool TryInitWindow() = 0;
    	virtual uint32_t GetTicks() = 0;
    	virtual void Delay(int) = 0;
    };

    class IInput : public IComponent
    {

    public:
        virtual bool HandlerInput() = 0;
    	virtual uint8_t GetInputReg1() = 0;
    	virtual uint8_t GetInputReg2() = 0;

    protected:
        uint8_t _input1_register = 0;
        uint8_t _input2_register = 0;
    };

    class ISound : public IComponent
    {
    public:
        static constexpr uint8_t kNumSounds = 10;

    public:
    	virtual void SoundPort1(uint8_t data) = 0;
    	virtual void SoundPort2(uint8_t data) = 0;

    protected:
    	uint8_t _sound_register_1 = 0;
    	uint8_t _sound_register_2 = 0;
    };

    class Platform8080
    {
    public:
        Platform8080(IDisplay *display, ISound *sound, IInput *input);
        ~Platform8080();

    public:
        void Start();
        [[nodiscard]] bool TryLoadROM(const std::string_view &file_name, uint16_t start_addres = 0);
        bool TryLoadPartsROM(const std::string_view &directory_path, const std::string_view &parts_name);
        Intel8080::CPU_8080 *GetCPU();
    	void WriteShift(uint8_t data);
    	void WriteWatchdog(uint8_t data);

    private:
        IDisplay *_display_system;
        ISound *_sound_system;
        IInput *_input_system;
        Intel8080::CPU_8080 _cpu{};
    	uint16_t _shift_register = 0;
    	uint8_t _shift_amnt_register = 0;
    };

} // namespace Platform
