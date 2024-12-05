
#include "cpu/CPU_8080.h"
#include "platform/Platform.h"

#include <cstdio>
#include <fstream>
#include <iostream>

Intel8080::CPU_8080 cpu;


int c = 0;

void PRINT(const uint8_t data)
{
	(void)data;
	if (cpu._registers[Intel8080::C] == 9)
	{
		auto str = reinterpret_cast<char *>(&cpu._memory[cpu.GetExtendedRegister(Intel8080::D, Intel8080::E)]);
		while (*str != '$')
			printf("%c", *str++);
		c++;

	}
	else if (cpu._registers[Intel8080::C] == 2)
		printf("%c", cpu._registers[Intel8080::E]);

	std::cout << std::flush;
}

bool TryLoadROM(Intel8080::CPU_8080 *cpu, const std::string_view &file_name, const uint16_t start_addres = 0)
{
	std::ifstream rom_file(file_name.data(), std::ios::binary);

	if (!rom_file.is_open())
		return false;

	rom_file.readsome(reinterpret_cast<char *>(cpu->GetMemory() + start_addres), Intel8080::CPU_8080::kMaxMemory);
	rom_file.close();
	return true;
}

static bool debug = false;
static bool quit = false;

void EXIT_TEST(const uint8_t data)
{
	(void)data;
	quit = true;
}

void cpu_print_debug()
{
	printf(
		"PC: %04X, AF: %04X, BC: %04X, DE: %04X, HL: %04X, SP: %04X, CYC: %lu (%s), OPER1: %02X, OPER2: %02X\n",
		cpu._pc,
		(cpu._registers[Intel8080::A] << 8) | cpu.GetSW(),
		(cpu._registers[Intel8080::B] << 8) | cpu._registers[Intel8080::C],
		(cpu._registers[Intel8080::D] << 8) | cpu._registers[Intel8080::E],
		(cpu._registers[Intel8080::H] << 8) | cpu._registers[Intel8080::L],
		cpu._sp,
		static_cast<unsigned long>(cpu._cycle_counter),
		Intel8080::OpcodeStrings[cpu._memory[cpu._pc]],
		cpu._memory[cpu._pc + 2],
		cpu._memory[cpu._pc + 1]);

	std::cout << std::flush;
}

#define PRINT_ADDR 0x5

/* Runs a test ROM. */
void run_test(const char *filename, const int test_num)
{
	printf("TEST %d:\n", test_num);

	if (!TryLoadROM(&cpu, filename, 0x100))
	{
		fprintf(stderr, "Unable to open %s\n", filename);
	}
	// These test ROMs expect to be loaded into 0x100
	cpu._pc = 0x100;

	// Inject opcodes to simulate OS calls
	cpu._output_port[0] = EXIT_TEST;
	cpu._memory[0] = Intel8080::OP_OUT;
	cpu._memory[1] = 0x00;

	cpu._output_port[1] = PRINT;
	cpu._memory[PRINT_ADDR] = Intel8080::OP_OUT;
	cpu._memory[PRINT_ADDR + 1] = 0x01;
	cpu._memory[PRINT_ADDR + 2] = Intel8080::OP_RET;

	unsigned long num_instr = 0;
	while (!cpu._exit)
	{
		if (c == 8)
		{
			cpu._cycle_counter = 0;
			c++;
		}
		if (debug && c >= 9 && cpu._cycle_counter > 4294965500)
		{
			cpu_print_debug();
			// getchar();

		}
		if (quit)
		{
			quit = false;
			break;
		}

		num_instr++;
		cpu.Tick();
	}

	printf("\n\nInstructions: %lu\n", num_instr);
	printf("Total Cycles: %llu\n\n\n", cpu._cycle_counter);
	cpu.Reset();
}

int main()
{
	// debug = true;
	run_test("./rom/tests/TST8080.COM", 1);
	run_test("./rom/tests/8080PRE.COM", 2);
	run_test("./rom/tests/CPUTEST.COM", 3);
	run_test("./rom/tests/8080EXM.COM", 4);
}
