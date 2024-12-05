#pragma once
#include <cstdint>
#include <functional>

namespace Intel8080
{
	extern const char *OpcodeStrings[];

	enum Opcodes : uint8_t
	{
		OP_NOP = 0x00,
		OP_LXI_B,
		OP_STAX_B,
		OP_INX_B,
		OP_INR_B,
		OP_DCR_B,
		OP_MVI_B,
		OP_RLC,
		OP_UNDEFINED_1,
		OP_DAD_B,
		OP_LDAX_B,
		OP_DCX_B,
		OP_INR_C,
		OP_DCR_C,
		OP_MVI_C,
		OP_RRC,
		OP_UNDEFINED_2,
		OP_LXI_D,
		OP_STAX_D,
		OP_INX_D,
		OP_INR_D,
		OP_DCR_D,
		OP_MVI_D,
		OP_RAL,
		OP_UNDEFINED_3,
		OP_DAD_D,
		OP_LDAX_D,
		OP_DCX_D,
		OP_INR_E,
		OP_DCR_E,
		OP_MVI_E,
		OP_RAR,
		OP_UNDEFINED_4,
		OP_LXI_H,
		OP_SHLD,
		OP_INX_H,
		OP_INR_H,
		OP_DCR_H,
		OP_MVI_H,
		OP_DAA,
		OP_UNDEFINED_5,
		OP_DAD_H,
		OP_LHLD,
		OP_DCX_H,
		OP_INR_L,
		OP_DCR_L,
		OP_MVI_L,
		OP_CMA,
		OP_UNDEFINED_6,
		OP_LXI_SP,
		OP_STA,
		OP_INX_SP,
		OP_INR_M,
		OP_DCR_M,
		OP_MVI_M,
		OP_STC,
		OP_UNDEFINED_7,
		OP_DAD_SP,
		OP_LDA,
		OP_DCX_SP,
		OP_INR_A,
		OP_DCR_A,
		OP_MVI_A,
		OP_CMC,
		OP_MOV_B_B,
		OP_MOV_B_C,
		OP_MOV_B_D,
		OP_MOV_B_E,
		OP_MOV_B_H,
		OP_MOV_B_L,
		OP_MOV_B_M,
		OP_MOV_B_A,
		OP_MOV_C_B,
		OP_MOV_C_C,
		OP_MOV_C_D,
		OP_MOV_C_E,
		OP_MOV_C_H,
		OP_MOV_C_L,
		OP_MOV_C_M,
		OP_MOV_C_A,
		OP_MOV_D_B,
		OP_MOV_D_C,
		OP_MOV_D_D,
		OP_MOV_D_E,
		OP_MOV_D_H,
		OP_MOV_D_L,
		OP_MOV_D_M,
		OP_MOV_D_A,
		OP_MOV_E_B,
		OP_MOV_E_C,
		OP_MOV_E_D,
		OP_MOV_E_E,
		OP_MOV_E_H,
		OP_MOV_E_L,
		OP_MOV_E_M,
		OP_MOV_E_A,
		OP_MOV_H_B,
		OP_MOV_H_C,
		OP_MOV_H_D,
		OP_MOV_H_E,
		OP_MOV_H_H,
		OP_MOV_H_L,
		OP_MOV_H_M,
		OP_MOV_H_A,
		OP_MOV_L_B,
		OP_MOV_L_C,
		OP_MOV_L_D,
		OP_MOV_L_E,
		OP_MOV_L_H,
		OP_MOV_L_L,
		OP_MOV_L_M,
		OP_MOV_L_A,
		OP_MOV_M_B,
		OP_MOV_M_C,
		OP_MOV_M_D,
		OP_MOV_M_E,
		OP_MOV_M_H,
		OP_MOV_M_L,
		OP_HLT,
		OP_MOV_M_A,
		OP_MOV_A_B,
		OP_MOV_A_C,
		OP_MOV_A_D,
		OP_MOV_A_E,
		OP_MOV_A_H,
		OP_MOV_A_L,
		OP_MOV_A_M,
		OP_MOV_A_A,
		OP_ADD_B,
		OP_ADD_C,
		OP_ADD_D,
		OP_ADD_E,
		OP_ADD_H,
		OP_ADD_L,
		OP_ADD_M,
		OP_ADD_A,
		OP_ADC_B,
		OP_ADC_C,
		OP_ADC_D,
		OP_ADC_E,
		OP_ADC_H,
		OP_ADC_L,
		OP_ADC_M,
		OP_ADC_A,
		OP_SUB_B,
		OP_SUB_C,
		OP_SUB_D,
		OP_SUB_E,
		OP_SUB_H,
		OP_SUB_L,
		OP_SUB_M,
		OP_SUB_A,
		OP_SBB_B,
		OP_SBB_C,
		OP_SBB_D,
		OP_SBB_E,
		OP_SBB_H,
		OP_SBB_L,
		OP_SBB_M,
		OP_SBB_A,
		OP_ANA_B,
		OP_ANA_C,
		OP_ANA_D,
		OP_ANA_E,
		OP_ANA_H,
		OP_ANA_L,
		OP_ANA_M,
		OP_ANA_A,
		OP_XRA_B,
		OP_XRA_C,
		OP_XRA_D,
		OP_XRA_E,
		OP_XRA_H,
		OP_XRA_L,
		OP_XRA_M,
		OP_XRA_A,
		OP_ORA_B,
		OP_ORA_C,
		OP_ORA_D,
		OP_ORA_E,
		OP_ORA_H,
		OP_ORA_L,
		OP_ORA_M,
		OP_ORA_A,
		OP_CMP_B,
		OP_CMP_C,
		OP_CMP_D,
		OP_CMP_E,
		OP_CMP_H,
		OP_CMP_L,
		OP_CMP_M,
		OP_CMP_A,
		OP_RNZ,
		OP_POP_B,
		OP_JNZ,
		OP_JMP,
		OP_CNZ,
		OP_PUSH_B,
		OP_ADI,
		OP_RST_0,
		OP_RZ,
		OP_RET,
		OP_JZ,
		OP_UNDEFINED_8,
		OP_CZ,
		OP_CALL,
		OP_ACI,
		OP_RST_1,
		OP_RNC,
		OP_POP_D,
		OP_JNC,
		OP_OUT,
		OP_CNC,
		OP_PUSH_D,
		OP_SUI,
		OP_RST_2,
		OP_RC,
		OP_UNDEFINED_9,
		OP_JC,
		OP_IN,
		OP_CC,
		OP_UNDEFINED_10,
		OP_SBI,
		OP_RST_3,
		OP_RPO,
		OP_POP_H,
		OP_JPO,
		OP_XTHL,
		OP_CPO,
		OP_PUSH_H,
		OP_ANI,
		OP_RST_4,
		OP_RPE,
		OP_PCHL,
		OP_JPE,
		OP_XCHG,
		OP_CPE,
		OP_UNDEFINED_11,
		OP_XRI,
		OP_RST_5,
		OP_RP,
		OP_POP_PSW,
		OP_JP,
		OP_DI,
		OP_CP,
		OP_PUSH_PSW,
		OP_ORI,
		OP_RST_6,
		OP_RM,
		OP_SPHL,
		OP_JM,
		OP_EI,
		OP_CM,
		OP_EXIT,	// Orginally undefined so using this to signal to main to exit
		OP_CPI,
		OP_RST_7
	};


	// clang-format off
	enum ERegister : uint8_t
	{
		A, FLAGS,
		B, C,
		D, E,
		H, L,
	};
	// clang-format on

	enum EFlagBits
	{
		S = 1 << 5,		// Sign Bit
		Z = 1 << 4,		// Zero Bit
		AC = 1 << 3,	// Aux Carry Bit
		P = 1 << 2,		// Parity Bit
		CY = 1 << 1,	// Carry Flag
		IR = 1 << 0,	// Interrupt Bit
	};

	class CPU_8080
	{
	public:
		typedef std::function<void(uint8_t)> output_callback;
		typedef std::function<uint8_t()> input_callback;

	public:
		static constexpr uint8_t kMaxOperands = 2;
		static constexpr uint32_t kMaxMemory = 0x10000;
		static constexpr uint16_t kVideoBufferAddress = 0x2400;
		static constexpr uint16_t kNumInstructions = 256;
		static constexpr uint8_t kNumIO = 0xFF;
		static constexpr uint8_t kNumRegisters = 8;

		// clang-format off
		static constexpr uint8_t kOpcodeCycles[kNumInstructions] = {
			4,	10, 7,	5,	5,	5,	7,	4,	4,	10, 7,	5,	5,	5,	7,	4,
			4,	10, 7,	5,	5,	5,	7,	4,	4,	10, 7,	5,	5,	5,	7,	4,
			4,	10, 16, 5,	5,	5,	7,	4,	4,	10, 16, 5,	5,	5,	7,	4,
			4,	10, 13, 5, 10, 10, 10,	4,	4,	10, 13, 5,	5,	5,	7,	4,

			5,	5,	5,	5,	5,	5,	7,	5,	5,	5,	5,	5,	5,	5,	7,	5,
			5,	5,	5,	5,	5,	5,	7,	5,	5,	5,	5,	5,	5,	5,	7,	5,
			5,	5,	5,	5,	5,	5,	7,	5,	5,	5,	5,	5,	5,	5,	7,	5,
			7,	7,	7,	7,	7,	7,	7,	7,	5,	5,	5,	5,	5,	5,	7,	5,

			4,	4,	4,	4,	4,	4,	7,	4,	4,	4,	4,	4,	4,	4,	7,	4,
			4,	4,	4,	4,	4,	4,	7,	4,	4,	4,	4,	4,	4,	4,	7,	4,
			4,	4,	4,	4,	4,	4,	7,	4,	4,	4,	4,	4,	4,	4,	7,	4,
			4,	4,	4,	4,	4,	4,	7,	4,	4,	4,	4,	4,	4,	4,	7,	4,

			5,	10, 10, 10, 11, 11, 7,	11, 5,	10, 10, 10, 11, 17, 7,	11,
			5,	10, 10, 10, 11, 11, 7,	11, 5,	10, 10, 10, 11, 17, 7,	11,
			5,	10, 10, 18, 11, 11, 7,	11, 5,	5,	10, 5,	11, 17, 7,	11,
			5,	10, 10, 4,	11, 11, 7,	11, 5,	5,	10, 4,	11, 17, 7,	11
		};

		static constexpr uint8_t kOpcodeSyzes[kNumInstructions] = {
			1, 3, 1, 1, 1, 1, 2, 1, 1, 1, 1, 1, 1, 1, 2, 1,
			1, 3, 1, 1, 1, 1, 2, 1, 1, 1, 1, 1, 1, 1, 2, 1,
			1, 3, 3, 1, 1, 1, 2, 1, 1, 1, 3, 1, 1, 1, 2, 1,
			1, 3, 3, 1, 1, 1, 2, 1, 1, 1, 3, 1, 1, 1, 2, 1,

			1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
			1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
			1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
			1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,

			1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
			1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
			1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
			1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,

			1, 1, 3, 3, 3, 1, 2, 1, 1, 1, 3, 3, 3, 3, 2, 1,
			1, 1, 3, 2, 3, 1, 2, 1, 1, 1, 3, 2, 3, 3, 2, 1,
			1, 1, 3, 1, 3, 1, 2, 1, 1, 1, 3, 1, 3, 3, 2, 1,
			1, 1, 3, 1, 3, 1, 2, 1, 1, 1, 3, 1, 3, 3, 2, 1,
		};
		// clang-format on

	public:
		void Tick();
		void Reset();
		void RequiredInterrupt(uint8_t opcode);

	public:	   // Help function
		[[nodiscard]] static inline uint16_t GetExtendedOperands(const uint8_t operands[]);

		[[nodiscard]] uint16_t GetExtendedRegister(ERegister first_reg, ERegister second_reg) const;
		[[nodiscard]] uint16_t GetExtendedRegister(ERegister reg) const;

		void SetExtendedRedister(ERegister fisrt, ERegister second, uint16_t value);
		void SetExtendedRedister(ERegister reg, uint16_t value);
		uint8_t *GetVideoBuffer();

		void SetFlag(EFlagBits bit, bool flag);
		[[nodiscard]] bool GetFlag(EFlagBits bit) const;
		[[nodiscard]] bool IsExit() const;
		void SetExitFlag(bool flag);
		uint8_t *GetMemory();

		[[nodiscard]] uint8_t GetSW() const;

		void BindInputPort(uint8_t port_byte, input_callback callback);
		void BindOutputPort(uint8_t port_byte, output_callback callback);

		[[nodiscard]] uint32_t GetCycleCounter() const;
		void SetCycleCounter(uint32_t);

	private:	// Opcodes
		void NOP();

		// Data Transfer
		void LXI_R(ERegister reg, const uint8_t operands[]);
		void LXI_SP(const uint8_t operands[]);
		void LDAX_R(ERegister src);
		void XCHG();

		void LDA(const uint8_t operands[]);
		void STA(const uint8_t operands[]);
		void LHLD(const uint8_t operands[]);
		void SHLD(const uint8_t operands[]);
		void STAX_R(ERegister src);
		void MOV_R_R(ERegister dest, ERegister src);
		void MOV_R_M(ERegister dest);
		void MOV_M_R(ERegister src);
		void MVI_R(ERegister dest, uint8_t operand);
		void MVI_M(uint8_t operand);

		// Arithmetic
		void ADD_R(ERegister src);
		void ADD_M();
		void ADI(uint8_t operand);
		void ADC_R(ERegister src);
		void ADC_M();
		void ACI(uint8_t operand);
		void SUB_R(ERegister src);
		void SUB_M();
		void SUI(uint8_t operand);
		void SBB_R(ERegister src);
		void SBB_M();
		void SBI(uint8_t operand);
		void INR_R(ERegister dest);
		void INR_M();
		void DCR_R(ERegister dest);
		void DCR_M();
		void INX_R(ERegister dest);
		void INX_SP();
		void DCX_R(ERegister dest);
		void DCX_SP();
		void DAD_R(ERegister dest);
		void DAD_SP();
		void DAA();

		// Logical
		void ANA_R(ERegister src);
		void ANA_M();
		void ANI(uint8_t operand);
		void XRA_R(ERegister src);
		void XRA_M();
		void XRI(uint8_t operand);
		void ORA_R(ERegister src);
		void ORA_M();
		void ORI(uint8_t operand);
		void CMP_R(ERegister src);
		void CMP_M();
		void CPI(uint8_t operand);
		void RLC();
		void RRC();
		void RAL();
		void RAR();
		void CMA();
		void CMC();
		void STC();

		// Branches
		void JMP(const uint8_t operands[]);
		void JCC(EFlagBits flag, bool cmp, const uint8_t operands[]);
		void CALL(const uint8_t operands[]);
		void CCC(EFlagBits flag, bool cmp, uint8_t operands[]);
		void RET();
		void RCC(EFlagBits flag, bool cmp);
		void RST_N(uint8_t n);
		void PCHL();

		// IO
		void PUSH_R(ERegister src);
		void PUSH_PSW();
		void POP_R(ERegister src);
		void POP_PSW();
		void XTHL();
		void SPHL();
		void IN(uint8_t operand);
		void OUT(uint8_t operand) const;
		void EI();
		void DI();
		void HLT();
		void EXIT();
		void UNDEFINED();

	public:
		uint8_t _registers[kNumRegisters] = {};
		uint16_t _sp = 0;
		uint16_t _pc = 0;

		uint8_t _memory[kMaxMemory] = {};

		// Helpers
		uint64_t _cycle_counter = 0;
		bool _exit = false;	   // Signals main to exit loop
		bool _halt = false;	   // Signals CPU to halt until interrupt (not functional)

		int _interrupt = -1;

		// IO
		input_callback _input_port[kNumIO]{};
		output_callback _output_port[kNumIO]{};
	};

}	 // namespace Intel8080
