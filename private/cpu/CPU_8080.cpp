#include "cpu/CPU_8080.h"

#include <cstring>
#include <fstream>
#include <iostream>

namespace Intel8080
{
	const char *OpcodeStrings[CPU_8080::kNumInstructions] = {
		"NOP",		   "LXI B",		  "STAX B",	  "INX B",		 "INR B",	 "DCR B",		 "MVI B",	 "RLC",
		"UNDEFINED 1", "DAD B",		  "LDAX B",	  "DCX B",		 "INR C",	 "DCR C",		 "MVI C",	 "RRC",
		"UNDEFINED 2", "LXI D",		  "STAX D",	  "INX D",		 "INR D",	 "DCR D",		 "MVI D",	 "RAL",
		"UNDEFINED 3", "DAD D",		  "LDAX D",	  "DCX D",		 "INR E",	 "DCR E",		 "MVI E",	 "RAR",
		"UNDEFINED 4", "LXI H",		  "SHLD",	  "INX H",		 "INR H",	 "DCR H",		 "MVI H",	 "DAA",
		"UNDEFINED 5", "DAD H",		  "LHLD",	  "DCX H",		 "INR L",	 "DCR L",		 "MVI L",	 "CMA",
		"UNDEFINED 6", "LXI SP",	  "STA",	  "INX SP",		 "INR M",	 "DCR M",		 "MVI M",	 "STC",
		"UNDEFINED 7", "DAD SP",	  "LDA",	  "DCX SP",		 "INR A",	 "DCR A",		 "MVI A",	 "CMC",
		"MOV B, B",	   "MOV B, C",	  "MOV B, D", "MOV B, E",	 "MOV B, H", "MOV B, L",	 "MOV B, M", "MOV B, A",
		"MOV C, B",	   "MOV C, C",	  "MOV C, D", "MOV C, E",	 "MOV C, H", "MOV C, L",	 "MOV C, M", "MOV C, A",
		"MOV D, B",	   "MOV D, C",	  "MOV D, D", "MOV D, E",	 "MOV D, H", "MOV D, L",	 "MOV D, M", "MOV D, A",
		"MOV E, B",	   "MOV E, C",	  "MOV E, D", "MOV E, E",	 "MOV E, H", "MOV E, L",	 "MOV E, M", "MOV E, A",
		"MOV H, B",	   "MOV H, C",	  "MOV H, D", "MOV H, E",	 "MOV H, H", "MOV H, L",	 "MOV H, M", "MOV H, A",
		"MOV L, B",	   "MOV L, C",	  "MOV L, D", "MOV L, E",	 "MOV L, H", "MOV L, L",	 "MOV L, M", "MOV L, A",
		"MOV M, B",	   "MOV M, C",	  "MOV M, D", "MOV M, E",	 "MOV M, H", "MOV M, L",	 "HLT",		 "MOV M, A",
		"MOV A, B",	   "MOV A, C",	  "MOV A, D", "MOV A, E",	 "MOV A, H", "MOV A, L",	 "MOV A, M", "MOV A, A",
		"ADD B",	   "ADD C",		  "ADD D",	  "ADD E",		 "ADD H",	 "ADD L",		 "ADD M",	 "ADD A",
		"ADC B",	   "ADC C",		  "ADC D",	  "ADC E",		 "ADC H",	 "ADC L",		 "ADC M",	 "ADC A",
		"SUB B",	   "SUB C",		  "SUB D",	  "SUB E",		 "SUB H",	 "SUB L",		 "SUB M",	 "SUB A",
		"SBB B",	   "SBB C",		  "SBB D",	  "SBB E",		 "SBB H",	 "SBB L",		 "SBB M",	 "SBB A",
		"ANA B",	   "ANA C",		  "ANA D",	  "ANA E",		 "ANA H",	 "ANA L",		 "ANA M",	 "ANA A",
		"XRA B",	   "XRA C",		  "XRA D",	  "XRA E",		 "XRA H",	 "XRA L",		 "XRA M",	 "XRA A",
		"ORA B",	   "ORA C",		  "ORA D",	  "ORA E",		 "ORA H",	 "ORA L",		 "ORA M",	 "ORA A",
		"CMP B",	   "CMP C",		  "CMP D",	  "CMP E",		 "CMP H",	 "CMP L",		 "CMP M",	 "CMP A",
		"RNZ",		   "POP B",		  "JNZ",	  "JMP",		 "CNZ",		 "PUSH B",		 "ADI",		 "RST 0",
		"RZ",		   "RET",		  "JZ",		  "UNDEFINED 8", "CZ",		 "CALL",		 "ACI",		 "RST 1",
		"RNC",		   "POP D",		  "JNC",	  "OUT",		 "CNC",		 "PUSH D",		 "SUI",		 "RST 2",
		"RC",		   "UNDEFINED 9", "JC",		  "IN",			 "CC",		 "UNDEFINED 10", "SBI",		 "RST 3",
		"RPO",		   "POP H",		  "JPO",	  "XTHL",		 "CPO",		 "PUSH H",		 "ANI",		 "RST 4",
		"RPE",		   "PCHL",		  "JPE",	  "XCHG",		 "CPE",		 "UNDEFINED 11", "XRI",		 "RST 5",
		"RP",		   "POP PSW",	  "JP",		  "DI",			 "CP",		 "PUSH PSW",	 "ORI",		 "RST 6",
		"RM",		   "SPHL",		  "JM",		  "EI",			 "CM",		 "EXIT",		 "CPI",		 "RST 7"
	};

	void CPU_8080::BindInputPort(const uint8_t port_byte, input_callback callback)
	{
		_input_port[port_byte] = std::move(callback);
	}

	void CPU_8080::BindOutputPort(const uint8_t port_byte, output_callback callback)
	{
		_output_port[port_byte] = std::move(callback);
	}
	uint32_t CPU_8080::GetCycleCounter() const
	{
		return _cycle_counter;
	}
	void CPU_8080::SetCycleCounter(const uint32_t cycle)
	{
		_cycle_counter = cycle;
	}

	void CPU_8080::Tick()
	{
		uint8_t opcode;

		uint8_t operands[kMaxOperands] = { _memory[_pc + 1], _memory[_pc + 2] };

		if (_interrupt >= 0 && GetFlag(IR))
		{
			SetFlag(IR, false);
			opcode = _interrupt;
			_interrupt = -1;
		}
		else
		{
			opcode = _memory[_pc];
			_pc += kOpcodeSyzes[opcode];
		}

		_cycle_counter += kOpcodeCycles[opcode];

		// clang-format off
        switch (opcode)
        {
            // Data Transfer
            case OP_MOV_A_A: MOV_R_R(A, A); break;
            case OP_MOV_A_B: MOV_R_R(A, B); break;
            case OP_MOV_A_C: MOV_R_R(A, C); break;
            case OP_MOV_A_D: MOV_R_R(A, D); break;
            case OP_MOV_A_E: MOV_R_R(A, E); break;
            case OP_MOV_A_H: MOV_R_R(A, H); break;
            case OP_MOV_A_L: MOV_R_R(A, L); break;
            case OP_MOV_B_A: MOV_R_R(B, A); break;
            case OP_MOV_B_B: MOV_R_R(B, B); break;
            case OP_MOV_B_C: MOV_R_R(B, C); break;
            case OP_MOV_B_D: MOV_R_R(B, D); break;
            case OP_MOV_B_E: MOV_R_R(B, E); break;
            case OP_MOV_B_H: MOV_R_R(B, H); break;
            case OP_MOV_B_L: MOV_R_R(B, L); break;
            case OP_MOV_C_A: MOV_R_R(C, A); break;
            case OP_MOV_C_B: MOV_R_R(C, B); break;
            case OP_MOV_C_C: MOV_R_R(C, C); break;
            case OP_MOV_C_D: MOV_R_R(C, D); break;
            case OP_MOV_C_E: MOV_R_R(C, E); break;
            case OP_MOV_C_H: MOV_R_R(C, H); break;
            case OP_MOV_C_L: MOV_R_R(C, L); break;
            case OP_MOV_D_A: MOV_R_R(D, A); break;
            case OP_MOV_D_B: MOV_R_R(D, B); break;
            case OP_MOV_D_C: MOV_R_R(D, C); break;
            case OP_MOV_D_D: MOV_R_R(D, D); break;
            case OP_MOV_D_E: MOV_R_R(D, E); break;
            case OP_MOV_D_H: MOV_R_R(D, H); break;
            case OP_MOV_D_L: MOV_R_R(D, L); break;
            case OP_MOV_E_A: MOV_R_R(E, A); break;
            case OP_MOV_E_B: MOV_R_R(E, B); break;
            case OP_MOV_E_C: MOV_R_R(E, C); break;
            case OP_MOV_E_D: MOV_R_R(E, D); break;
            case OP_MOV_E_E: MOV_R_R(E, E); break;
            case OP_MOV_E_H: MOV_R_R(E, H); break;
            case OP_MOV_E_L: MOV_R_R(E, L); break;
            case OP_MOV_H_A: MOV_R_R(H, A); break;
            case OP_MOV_H_B: MOV_R_R(H, B); break;
            case OP_MOV_H_C: MOV_R_R(H, C); break;
            case OP_MOV_H_D: MOV_R_R(H, D); break;
            case OP_MOV_H_E: MOV_R_R(H, E); break;
            case OP_MOV_H_H: MOV_R_R(H, H); break;
            case OP_MOV_H_L: MOV_R_R(H, L); break;
            case OP_MOV_L_A: MOV_R_R(L, A); break;
            case OP_MOV_L_B: MOV_R_R(L, B); break;
            case OP_MOV_L_C: MOV_R_R(L, C); break;
            case OP_MOV_L_D: MOV_R_R(L, D); break;
            case OP_MOV_L_E: MOV_R_R(L, E); break;
            case OP_MOV_L_H: MOV_R_R(L, H); break;
            case OP_MOV_L_L: MOV_R_R(L, L); break;
            case OP_MOV_A_M: MOV_R_M(A); break;
            case OP_MOV_B_M: MOV_R_M(B); break;
            case OP_MOV_C_M: MOV_R_M(C); break;
            case OP_MOV_D_M: MOV_R_M(D); break;
            case OP_MOV_E_M: MOV_R_M(E); break;
            case OP_MOV_H_M: MOV_R_M(H); break;
            case OP_MOV_L_M: MOV_R_M(L); break;
            case OP_MOV_M_A: MOV_M_R(A); break;
            case OP_MOV_M_B: MOV_M_R(B); break;
            case OP_MOV_M_C: MOV_M_R(C); break;
            case OP_MOV_M_D: MOV_M_R(D); break;
            case OP_MOV_M_E: MOV_M_R(E); break;
            case OP_MOV_M_H: MOV_M_R(H); break;
            case OP_MOV_M_L: MOV_M_R(L); break;
            case OP_MVI_A: MVI_R(A, operands[0]); break;
            case OP_MVI_B: MVI_R(B, operands[0]); break;
            case OP_MVI_C: MVI_R(C, operands[0]); break;
            case OP_MVI_D: MVI_R(D, operands[0]); break;
            case OP_MVI_E: MVI_R(E, operands[0]); break;
            case OP_MVI_H: MVI_R(H, operands[0]); break;
            case OP_MVI_L: MVI_R(L, operands[0]); break;
            case OP_MVI_M: MVI_M(operands[0]); break;
            case OP_LXI_B: LXI_R(B, operands); break;
            case OP_LXI_D: LXI_R(D, operands); break;
            case OP_LXI_H: LXI_R(H, operands); break;
            case OP_LXI_SP: LXI_SP(operands); break;
            case OP_LDA: LDA(operands); break;
            case OP_STA: STA(operands); break;
            case OP_LHLD: LHLD(operands); break;
            case OP_SHLD: SHLD(operands); break;
            case OP_LDAX_B: LDAX_R(B); break;
            case OP_LDAX_D: LDAX_R(D); break;
            case OP_STAX_B: STAX_R(B); break;
            case OP_STAX_D: STAX_R(D); break;
            case OP_XCHG: XCHG(); break;

            // Arithmetic
            case OP_ADD_A: ADD_R( A); break;
            case OP_ADD_B: ADD_R( B); break;
            case OP_ADD_C: ADD_R( C); break;
            case OP_ADD_D: ADD_R( D); break;
            case OP_ADD_E: ADD_R( E); break;
            case OP_ADD_H: ADD_R( H); break;
            case OP_ADD_L: ADD_R( L); break;
            case OP_ADD_M: ADD_M(); break;
            case OP_ADI: ADI(operands[0]); break;
            case OP_ADC_A: ADC_R(A); break;
            case OP_ADC_B: ADC_R(B); break;
            case OP_ADC_C: ADC_R(C); break;
            case OP_ADC_D: ADC_R(D); break;
            case OP_ADC_E: ADC_R(E); break;
            case OP_ADC_H: ADC_R(H); break;
            case OP_ADC_L: ADC_R(L); break;
            case OP_ADC_M: ADC_M(); break;
            case OP_ACI: ACI(operands[0]); break;
            case OP_SUB_A: SUB_R(A); break;
            case OP_SUB_B: SUB_R(B); break;
            case OP_SUB_C: SUB_R(C); break;
            case OP_SUB_D: SUB_R(D); break;
            case OP_SUB_E: SUB_R(E); break;
            case OP_SUB_H: SUB_R(H); break;
            case OP_SUB_L: SUB_R(L);break;
            case OP_SUB_M: SUB_M(); break;
            case OP_SUI: SUI(operands[0]); break;
            case OP_SBB_A: SBB_R(A); break;
            case OP_SBB_B: SBB_R(B); break;
            case OP_SBB_C: SBB_R(C); break;
            case OP_SBB_D: SBB_R(D); break;
            case OP_SBB_E: SBB_R(E); break;
            case OP_SBB_H: SBB_R(H); break;
            case OP_SBB_L: SBB_R(L); break;
            case OP_SBB_M: SBB_M(); break;
            case OP_SBI: SBI(operands[0]); break;
            case OP_INR_A: INR_R(A); break;
            case OP_INR_B: INR_R(B); break;
            case OP_INR_C: INR_R(C); break;
            case OP_INR_D: INR_R(D); break;
            case OP_INR_E: INR_R(E); break;
            case OP_INR_H: INR_R(H); break;
            case OP_INR_L: INR_R(L); break;
            case OP_INR_M: INR_M(); break;
            case OP_DCR_A: DCR_R(A); break;
            case OP_DCR_B: DCR_R(B); break;
            case OP_DCR_C: DCR_R(C); break;
            case OP_DCR_D: DCR_R(D); break;
            case OP_DCR_E: DCR_R(E); break;
            case OP_DCR_H: DCR_R(H); break;
            case OP_DCR_L: DCR_R(L); break;
            case OP_DCR_M: DCR_M(); break;
            case OP_INX_B: INX_R(B); break;
            case OP_INX_D: INX_R(D); break;
            case OP_INX_H: INX_R(H); break;
            case OP_INX_SP: INX_SP(); break;
            case OP_DCX_B: DCX_R(B); break;
            case OP_DCX_D: DCX_R(D); break;
            case OP_DCX_H: DCX_R(H); break;
            case OP_DCX_SP: DCX_SP(); break;
            case OP_DAD_B: DAD_R(B); break;
            case OP_DAD_D: DAD_R(D); break;
            case OP_DAD_H: DAD_R(H); break;
            case OP_DAD_SP: DAD_SP(); break;
            case OP_DAA: DAA(); break;

            // Logical
            case OP_ANA_A: ANA_R(A); break;
            case OP_ANA_B: ANA_R(B); break;
            case OP_ANA_C: ANA_R(C); break;
            case OP_ANA_D: ANA_R(D); break;
            case OP_ANA_E: ANA_R(E); break;
            case OP_ANA_H: ANA_R(H); break;
            case OP_ANA_L: ANA_R(L); break;
            case OP_ANA_M: ANA_M(); break;
            case OP_ANI: ANI(operands[0]); break;
            case OP_XRA_A: XRA_R(A); break;
            case OP_XRA_B: XRA_R(B); break;
            case OP_XRA_C: XRA_R(C); break;
            case OP_XRA_D: XRA_R(D); break;
            case OP_XRA_E: XRA_R(E); break;
            case OP_XRA_H: XRA_R(H); break;
            case OP_XRA_L: XRA_R(L); break;
            case OP_XRA_M: XRA_M(); break;
            case OP_XRI: XRI(operands[0]); break;
            case OP_ORA_A: ORA_R(A); break;
            case OP_ORA_B: ORA_R(B); break;
            case OP_ORA_C: ORA_R(C); break;
            case OP_ORA_D: ORA_R(D); break;
            case OP_ORA_E: ORA_R(E); break;
            case OP_ORA_H: ORA_R(H); break;
            case OP_ORA_L: ORA_R(L); break;
            case OP_ORA_M: ORA_M(); break;
            case OP_ORI: ORI(operands[0]); break;
            case OP_CMP_A: CMP_R(A); break;
            case OP_CMP_B: CMP_R(B); break;
            case OP_CMP_C: CMP_R(C); break;
            case OP_CMP_D: CMP_R(D); break;
            case OP_CMP_E: CMP_R(E); break;
            case OP_CMP_H: CMP_R(H); break;
            case OP_CMP_L: CMP_R(L); break;
            case OP_CMP_M: CMP_M(); break;
            case OP_CPI: CPI(operands[0]); break;
            case OP_RLC: RLC(); break;
            case OP_RRC: RRC(); break;
            case OP_RAL: RAL(); break;
            case OP_RAR: RAR(); break;
            case OP_CMA: CMA(); break;
            case OP_CMC: CMC(); break;
            case OP_STC: STC(); break;

            // Branch
            case OP_JMP:   JMP(operands); break;
            case OP_JNZ:   JCC(Z, false, operands); break;
            case OP_JZ:    JCC(Z, true, operands); break;
            case OP_JNC:   JCC(CY, false, operands); break;
            case OP_JC:    JCC(CY, true, operands); break;
            case OP_JPO:   JCC(P, false, operands); break;
            case OP_JPE:   JCC(P, true, operands); break;
            case OP_JP:    JCC(S, false, operands); break;
            case OP_JM:    JCC(S, true, operands); break;
            case OP_CALL:  CALL(operands); break;
            case OP_CNZ:   CCC(Z, false, operands); break;
            case OP_CZ:    CCC(Z, true, operands); break;
            case OP_CNC:   CCC(CY, false, operands); break;
            case OP_CC:    CCC(CY, true, operands); break;
            case OP_CPO:   CCC(P, false, operands); break;
            case OP_CPE:   CCC(P, true, operands); break;
            case OP_CP:    CCC(S, false, operands); break;
            case OP_CM:    CCC(S, true, operands); break;
            case OP_RET:   RET(); break;
            case OP_RNZ:   RCC(Z, false); break;
            case OP_RZ:    RCC(Z, true); break;
            case OP_RNC:   RCC(CY, false); break;
            case OP_RC:    RCC(CY, true); break;
            case OP_RPO:   RCC(P, false); break;
            case OP_RPE:   RCC(P, true); break;
            case OP_RP:    RCC(S, false); break;
            case OP_RM:    RCC(S, true); break;
            case OP_RST_0: RST_N(0); break;
            case OP_RST_1: RST_N(1); break;
            case OP_RST_2: RST_N(2); break;
            case OP_RST_3: RST_N(3); break;
            case OP_RST_4: RST_N(4); break;
            case OP_RST_5: RST_N(5); break;
            case OP_RST_6: RST_N(6); break;
            case OP_RST_7: RST_N(7); break;
            case OP_PCHL:  PCHL(); break;

            // Stack and IO
            case OP_PUSH_B:   PUSH_R(B); break;
            case OP_PUSH_D:   PUSH_R(D); break;
            case OP_PUSH_H:   PUSH_R(H); break;
            case OP_PUSH_PSW: PUSH_PSW(); break;
            case OP_POP_B:    POP_R(B); break;
            case OP_POP_D:    POP_R(D); break;
            case OP_POP_H:    POP_R(H); break;
            case OP_POP_PSW:  POP_PSW(); break;
            case OP_XTHL:     XTHL(); break;
            case OP_SPHL:     SPHL(); break;
            case OP_IN:       IN(operands[0]); break;
            case OP_OUT:      OUT(operands[0]); break;
            case OP_EI:       EI();  break;
            case OP_DI:       DI(); break;
            case OP_HLT:      HLT(); break;
            case OP_NOP:      NOP(); break;
            case OP_EXIT:     EXIT(); break;
            default: UNDEFINED(); break;
        }
		// clang-format on
	}

	void CPU_8080::Reset()
	{
		_pc = 0;
		_sp = 0;
		std::memset(_registers, 0, sizeof(_registers));
		std::memset(_memory, 0, sizeof(_memory));
		_exit = false;
		_halt = false;
		_cycle_counter = 0;
		_interrupt = -1;
	}

	void CPU_8080::RequiredInterrupt(const uint8_t opcode)
	{
		_interrupt = opcode;
	}

	void CPU_8080::SetFlag(const EFlagBits bit, const bool flag)
	{
		flag ? _registers[FLAGS] |= bit : _registers[FLAGS] &= ~bit;
	}

	bool CPU_8080::GetFlag(const EFlagBits bit) const
	{
		return _registers[FLAGS] & bit;
	}

	bool CPU_8080::IsExit() const
	{
		return _exit or _halt;
	}
	void CPU_8080::SetExitFlag(const bool flag)
	{
		_exit = flag;
		_halt = flag;
	}

	uint8_t *CPU_8080::GetMemory()
	{
		return _memory;
	}

}	 // namespace Intel8080
