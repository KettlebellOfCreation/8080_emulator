#include "cpu/CPU_8080.h"

#include <iostream>
#include <utility>

namespace Intel8080
{
	uint8_t CPU_8080::GetSW() const
	{
		return GetFlag(S) << 7 | GetFlag(Z) << 6 | GetFlag(AC) << 4 | GetFlag(P) << 2 | (1 << 1) | GetFlag(CY);
	}

	uint16_t CPU_8080::GetExtendedRegister(const ERegister reg) const
	{
		return _registers[reg] << 8 | _registers[reg + 1];
	}

	uint16_t CPU_8080::GetExtendedRegister(const ERegister first_reg, const ERegister second_reg) const
	{
		return _registers[first_reg] << 8 | _registers[second_reg];
	}

	void CPU_8080::SetExtendedRedister(const ERegister fisrt, const ERegister second, const uint16_t value)
	{
		_registers[fisrt] = value >> 8;
		_registers[second] = value & 0xFF;
	}

	void CPU_8080::SetExtendedRedister(const ERegister reg, const uint16_t value)
	{
		_registers[reg] = value >> 8;
		_registers[reg + 1] = value & 0xFF;
	}

	uint8_t *CPU_8080::GetVideoBuffer()
	{
		return &_memory[kVideoBufferAddress];
	}

	inline uint16_t CPU_8080::GetExtendedOperands(const uint8_t operands[kMaxOperands])
	{
		return operands[1] << 8 | operands[0];
	}

	// Flags Updates
	static void UpdateFlagZ(CPU_8080 *CPU, const uint8_t result)
	{
		CPU->SetFlag(Z, !result);
	}

	static void UpdateFlagS(CPU_8080 *CPU, const uint8_t result)
	{
		CPU->SetFlag(S, result & 1 << 7);
	}

	static void UpdateFlagP(CPU_8080 *CPU, uint8_t result)
	{
		int high_bits = 0;

		while (result)
		{
			high_bits += result & 1;
			result >>= 1;
		}

		CPU->SetFlag(P, !(high_bits % 2));
	}

	static void UpdateFlagAC_ADD(CPU_8080 *CPU, const uint8_t first, const uint8_t second, const bool add_carry)
	{
		const bool carry = add_carry ? CPU->GetFlag(CY) : 0;
		CPU->SetFlag(AC, (first & 0xF) + (second & 0xF) + carry > 0xF);
	}

	static void UpdateFlagAC_SUB(CPU_8080 *CPU, const uint8_t first, const uint8_t second, const bool sub_borrow)
	{
		const bool borrow = sub_borrow ? CPU->GetFlag(CY) : false;
		CPU->SetFlag(AC, ((first & 0xF) + (~second & 0xF) + !borrow) > 0xF);
	}

	static void UpdateFlagCY_ADD(CPU_8080 *CPU, const uint8_t first, const uint8_t second, const bool add_carry)
	{
		const bool carry = add_carry ? CPU->GetFlag(CY) : 0;
		const uint16_t result = first + second + carry;
		CPU->SetFlag(CY, result > 0xFF);
	}

	static void UpdateFlagCY_SUB(CPU_8080 *CPU, const uint8_t first, const uint8_t second, const bool sub_borrow)
	{
		const bool borrow = sub_borrow ? CPU->GetFlag(CY) : false;
		const uint16_t result = first + ~second + !borrow;
		CPU->SetFlag(CY, result > 0xFF);
	}

	static void UpdateFlagsADD(CPU_8080 *CPU, const uint8_t first, const uint8_t second, const bool carry)
	{
		const uint8_t result = first + second + (carry ? CPU->GetFlag(CY) : 0);
		UpdateFlagZ(CPU, result);
		UpdateFlagP(CPU, result);
		UpdateFlagS(CPU, result);
		UpdateFlagAC_ADD(CPU, first, second, carry);
		UpdateFlagCY_ADD(CPU, first, second, carry);
	}

	static void UpdateFlagsSUB(CPU_8080 *CPU, const uint8_t first, const uint8_t second, const bool carry)
	{
		const uint8_t result = first - second - (carry ? CPU->GetFlag(CY) : false);
		UpdateFlagZ(CPU, result);
		UpdateFlagP(CPU, result);
		UpdateFlagS(CPU, result);
		UpdateFlagAC_SUB(CPU, first, second, carry);
		UpdateFlagCY_SUB(CPU, first, second, carry);
	}

	static void UpdateFlagsINC(CPU_8080 *CPU, const uint8_t value)
	{
		const uint16_t result = value + 1;
		UpdateFlagZ(CPU, result);
		UpdateFlagP(CPU, result);
		UpdateFlagS(CPU, result);
		UpdateFlagAC_ADD(CPU, value, 1, false);
	}

	static void UpdateFlagsDEC(CPU_8080 *CPU, const uint8_t value)
	{
		const uint8_t result = value - 1;
		UpdateFlagZ(CPU, result);
		UpdateFlagP(CPU, result);
		UpdateFlagS(CPU, result);
		UpdateFlagAC_SUB(CPU, value, 1, false);
	}

	static void UpdateFlagsLogic(CPU_8080 *CPU, const uint8_t value)
	{
		UpdateFlagZ(CPU, value);
		UpdateFlagP(CPU, value);
		UpdateFlagS(CPU, value);
	}

	static void UpdateFlagsAND(CPU_8080 *CPU, const uint8_t first, const uint8_t second)
	{
		UpdateFlagsLogic(CPU, first & second);
		CPU->SetFlag(CY, false);
		CPU->SetFlag(AC, (first | second) >> 3 & 1);
	}

	static void UpdateFlagsOR(CPU_8080 *CPU, const uint8_t value)
	{
		UpdateFlagsLogic(CPU, value);
		CPU->SetFlag(CY, false);
		CPU->SetFlag(AC, false);
	}

	static void UpdateFlagsCMP(CPU_8080 *CPU, const uint8_t first, const uint8_t second)
	{
		const uint8_t result = first - second;
		UpdateFlagZ(CPU, result);
		UpdateFlagP(CPU, result);
		UpdateFlagS(CPU, result);
		UpdateFlagAC_SUB(CPU, first, second, false);
		UpdateFlagCY_SUB(CPU, first, second, false);
	}

	// Opcodes
	void CPU_8080::NOP() {}

	void CPU_8080::LXI_R(const ERegister reg, const uint8_t operands[kMaxOperands])
	{
		SetExtendedRedister(reg, GetExtendedOperands(operands));
	}

	void CPU_8080::LXI_SP(const uint8_t operands[kMaxOperands])
	{
		_sp = GetExtendedOperands(operands);
	}

	void CPU_8080::STAX_R(const ERegister src)
	{
		_memory[GetExtendedRegister(src)] = _registers[A];
	}

	void CPU_8080::MOV_R_R(const ERegister dest, const ERegister src)
	{
		_registers[dest] = _registers[src];
	}

	void CPU_8080::MOV_R_M(const ERegister dest)
	{
		_registers[dest] = _memory[GetExtendedRegister(H, L)];
	}

	void CPU_8080::MOV_M_R(const ERegister src)
	{
		_memory[GetExtendedRegister(H, L)] = _registers[src];
	}

	void CPU_8080::MVI_R(const ERegister dest, const uint8_t operand)
	{
		_registers[dest] = operand;
	}

	void CPU_8080::MVI_M(const uint8_t operand)
	{
		_memory[GetExtendedRegister(H, L)] = operand;
	}

	void CPU_8080::LDA(const uint8_t operands[kMaxOperands])
	{
		_registers[A] = _memory[GetExtendedOperands(operands)];
	}

	void CPU_8080::STA(const uint8_t operands[kMaxOperands])
	{
		_memory[GetExtendedOperands(operands)] = _registers[A];
	}

	void CPU_8080::LHLD(const uint8_t operands[kMaxOperands])
	{
		const uint16_t ext_op = GetExtendedOperands(operands);
		_registers[L] = _memory[ext_op];
		_registers[H] = _memory[ext_op + 1];
	}

	void CPU_8080::SHLD(const uint8_t operands[kMaxOperands])
	{
		const uint16_t ext_op = GetExtendedOperands(operands);
		_memory[ext_op] = _registers[L];
		_memory[ext_op + 1] = _registers[H];
	}

	void CPU_8080::LDAX_R(const ERegister src)
	{
		_registers[A] = _memory[GetExtendedRegister(src)];
	}

	void CPU_8080::XCHG()
	{
		std::swap(_registers[H], _registers[D]);
		std::swap(_registers[L], _registers[E]);
	}

	void CPU_8080::ADD_R(const ERegister src)
	{
		UpdateFlagsADD(this, _registers[A], _registers[src], false);
		_registers[A] += _registers[src];
	}

	void CPU_8080::ADD_M()
	{
		const uint8_t value = _memory[GetExtendedRegister(H, L)];
		UpdateFlagsADD(this, _registers[A], value, false);
		_registers[A] += value;
	}

	void CPU_8080::ADI(const uint8_t operand)
	{
		UpdateFlagsADD(this, _registers[A], operand, false);
		_registers[A] += operand;
	}

	void CPU_8080::ADC_R(const ERegister src)
	{
		const bool carry = GetFlag(CY);
		UpdateFlagsADD(this, _registers[A], _registers[src], true);
		_registers[A] += _registers[src] + carry;
	}

	void CPU_8080::ADC_M()
	{
		const uint8_t value = _memory[GetExtendedRegister(H, L)];
		const bool carry = GetFlag(CY);
		UpdateFlagsADD(this, _registers[A], value, true);
		_registers[A] += value + carry;
	}

	void CPU_8080::ACI(const uint8_t operand)
	{
		const bool carry = GetFlag(CY);
		UpdateFlagsADD(this, _registers[A], operand, true);
		_registers[A] += operand + carry;
	}

	void CPU_8080::SUB_R(const ERegister src)
	{
		UpdateFlagsSUB(this, _registers[A], _registers[src], false);
		_registers[A] -= _registers[src];
	}

	void CPU_8080::SUB_M()
	{
		const uint8_t value = _memory[GetExtendedRegister(H, L)];
		UpdateFlagsSUB(this, _registers[A], value, false);
		_registers[A] -= value;
	}

	void CPU_8080::SUI(const uint8_t operand)
	{
		UpdateFlagsSUB(this, _registers[A], operand, false);
		_registers[A] -= operand;
	}

	void CPU_8080::SBB_R(const ERegister src)
	{
		const bool carry = GetFlag(CY);
		UpdateFlagsSUB(this, _registers[A], _registers[src], true);
		_registers[A] -= _registers[src] + carry;
	}

	void CPU_8080::SBB_M()
	{
		const uint8_t value = _memory[GetExtendedRegister(H, L)];
		const bool carry = GetFlag(CY);
		UpdateFlagsSUB(this, _registers[A], value, true);
		_registers[A] -= value + carry;
	}

	void CPU_8080::SBI(const uint8_t operand)
	{
		const bool carry = GetFlag(CY);
		UpdateFlagsSUB(this, _registers[A], operand, true);
		_registers[A] -= operand + carry;
	}

	void CPU_8080::INR_R(const ERegister dest)
	{
		UpdateFlagsINC(this, _registers[dest]);
		++_registers[dest];
	}

	void CPU_8080::INR_M()
	{
		UpdateFlagsINC(this, _memory[GetExtendedRegister(H, L)]);
		++_memory[GetExtendedRegister(H, L)];
	}

	void CPU_8080::DCR_R(const ERegister dest)
	{
		UpdateFlagsDEC(this, _registers[dest]);
		--_registers[dest];
	}

	void CPU_8080::DCR_M()
	{
		UpdateFlagsDEC(this, _memory[GetExtendedRegister(H, L)]);
		--_memory[GetExtendedRegister(H, L)];
	}

	void CPU_8080::INX_R(const ERegister dest)
	{
		const uint16_t value = GetExtendedRegister(dest) + 1;
		SetExtendedRedister(dest, value);
	}

	void CPU_8080::INX_SP()
	{
		++_sp;
	}

	void CPU_8080::DCX_R(const ERegister dest)
	{
		const uint16_t value = GetExtendedRegister(dest) - 1;
		SetExtendedRedister(dest, value);
	}

	void CPU_8080::DCX_SP()
	{
		--_sp;
	}

	void CPU_8080::DAD_R(const ERegister dest)
	{
		const uint32_t value = GetExtendedRegister(H, L) + GetExtendedRegister(dest);

		SetFlag(CY, value > 0xFFFF);
		SetExtendedRedister(H, L, value);
	}

	void CPU_8080::DAD_SP()
	{
		const uint32_t result = GetExtendedRegister(H, L) + _sp;
		SetFlag(CY, result > 0xFFFF);
		SetExtendedRedister(H, L, result);
	}

	void CPU_8080::DAA()
	{
		uint8_t value = 0;
		const uint8_t msb = _registers[A] >> 4;
		const uint8_t lsb = _registers[A] & 0xF;

		if (GetFlag(AC) || lsb > 9)
			value += 0x06;

		if (GetFlag(CY) || msb > 9 || (msb >= 9 && lsb > 9))
		{
			value += 0x60;
			SetFlag(CY, true);
		}

		const uint8_t result = _registers[A] + value;
		UpdateFlagP(this, result);
		UpdateFlagZ(this, result);
		UpdateFlagS(this, result);
		UpdateFlagAC_ADD(this, _registers[A], value, false);
		_registers[A] = result;
	}

	void CPU_8080::ANA_R(const ERegister src)
	{
		UpdateFlagsAND(this, _registers[A], _registers[src]);
		_registers[A] &= _registers[src];
	}

	void CPU_8080::ANA_M()
	{
		const uint8_t value = _memory[GetExtendedRegister(H, L)];
		UpdateFlagsAND(this, _registers[A], value);
		_registers[A] &= value;
	}

	void CPU_8080::ANI(const uint8_t operand)
	{
		UpdateFlagsAND(this, _registers[A], operand);
		_registers[A] &= operand;
	}

	void CPU_8080::XRA_R(const ERegister src)
	{
		const uint8_t result = _registers[A] ^ _registers[src];
		UpdateFlagsOR(this, result);
		_registers[A] = result;
	}

	void CPU_8080::XRA_M()
	{
		const uint8_t result = _registers[A] ^ _memory[GetExtendedRegister(H, L)];
		UpdateFlagsOR(this, result);
		_registers[A] = result;
	}

	void CPU_8080::XRI(const uint8_t operand)
	{
		const uint8_t result = _registers[A] ^ operand;
		UpdateFlagsOR(this, result);
		_registers[A] = result;
	}

	void CPU_8080::ORA_R(const ERegister src)
	{
		const uint8_t result = _registers[A] | _registers[src];
		UpdateFlagsOR(this, result);
		_registers[A] = result;
	}

	void CPU_8080::ORA_M()
	{
		const uint8_t result = _registers[A] | _memory[GetExtendedRegister(H, L)];
		UpdateFlagsOR(this, result);
		_registers[A] = result;
	}

	void CPU_8080::ORI(const uint8_t operand)
	{
		const uint8_t result = _registers[A] | operand;
		UpdateFlagsOR(this, result);
		_registers[A] = result;
	}

	void CPU_8080::CMP_R(const ERegister src)
	{
		UpdateFlagsCMP(this, _registers[A], _registers[src]);
	}

	void CPU_8080::CMP_M()
	{
		UpdateFlagsCMP(this, _registers[A], _memory[GetExtendedRegister(H, L)]);
	}

	void CPU_8080::CPI(const uint8_t operand)
	{
		UpdateFlagsCMP(this, _registers[A], operand);
	}

	void CPU_8080::RLC()
	{
		const uint8_t msb = _registers[A] >> 7;
		_registers[A] <<= 1;
		msb ? _registers[A] |= 1 : _registers[A] &= ~1;
		SetFlag(CY, msb);
	}

	void CPU_8080::RRC()
	{
		const uint8_t lsb = _registers[A] & 1;
		_registers[A] >>= 1;
		lsb ? _registers[A] |= 1 << 7 : _registers[A] &= ~(1 << 7);
		SetFlag(CY, lsb);
	}

	void CPU_8080::RAL()
	{
		const uint8_t msb = _registers[A] >> 7;
		_registers[A] <<= 1;
		GetFlag(CY) ? _registers[A] |= 1 : _registers[A] &= ~1;
		SetFlag(CY, msb);
	}

	void CPU_8080::RAR()
	{
		const uint8_t lsb = _registers[A] & 1;
		_registers[A] >>= 1;
		GetFlag(CY) ? _registers[A] |= 1 << 7 : _registers[A] &= ~(1 << 7);
		SetFlag(CY, lsb);
	}

	void CPU_8080::CMA()
	{
		_registers[A] = ~_registers[A];
	}

	void CPU_8080::CMC()
	{
		SetFlag(CY, !GetFlag(CY));
	}

	void CPU_8080::STC()
	{
		SetFlag(CY, true);
	}

	void CPU_8080::JMP(const uint8_t operands[kMaxOperands])
	{
		_pc = GetExtendedOperands(operands);
	}

	void CPU_8080::JCC(const EFlagBits flag, const bool cmp, const uint8_t operands[kMaxOperands])
	{
		if (GetFlag(flag) == cmp)
			JMP(operands);
	}

	void CPU_8080::CALL(const uint8_t operands[kMaxOperands])
	{
		_memory[_sp - 1] = _pc >> 8;
		_memory[_sp - 2] = _pc & 0xFF;
		_sp -= 2;
		_pc = GetExtendedOperands(operands);
	}

	void CPU_8080::CCC(const EFlagBits flag, const bool cmp, uint8_t operands[kMaxOperands])
	{
		if (GetFlag(flag) == cmp)
		{
			CALL(operands);
			_cycle_counter += 6;
		}
	}

	void CPU_8080::RET()
	{
		_pc = _memory[_sp + 1] << 8 | _memory[_sp];
		_sp += 2;
	}

	void CPU_8080::RCC(const EFlagBits flag, const bool cmp)
	{
		if (GetFlag(flag) == cmp)
		{
			RET();
			_cycle_counter += 6;
		}
	}

	void CPU_8080::RST_N(const uint8_t n)
	{
		_memory[_sp - 1] = _pc >> 8;
		_memory[_sp - 2] = _pc & 0xFF;
		_sp -= 2;
		_pc = 8 * n;
	}

	void CPU_8080::PCHL()
	{
		_pc = GetExtendedRegister(H, L);
	}

	void CPU_8080::PUSH_R(const ERegister src)
	{
		_memory[_sp - 1] = _registers[src];
		_memory[_sp - 2] = _registers[src + 1];
		_sp -= 2;
	}

	void CPU_8080::PUSH_PSW()
	{
		_memory[_sp - 1] = _registers[A];
		_memory[_sp - 2] = GetSW();
		_sp -= 2;
	}

	void CPU_8080::POP_R(const ERegister src)
	{
		_registers[src + 1] = _memory[_sp];
		_registers[src] = _memory[_sp + 1];
		_sp += 2;
	}

	void CPU_8080::POP_PSW()
	{
		const uint8_t psw = _memory[_sp];

		SetFlag(CY, psw & 1);
		SetFlag(P, psw & 1 << 2);
		SetFlag(AC, psw & 1 << 4);
		SetFlag(Z, psw & 1 << 6);
		SetFlag(S, psw & 1 << 7);

		_registers[A] = _memory[_sp + 1];
		_sp += 2;
	}

	void CPU_8080::XTHL()
	{
		std::swap(_registers[L], _memory[_sp]);
		std::swap(_registers[H], _memory[_sp + 1]);
	}

	void CPU_8080::SPHL()
	{
		_sp = GetExtendedRegister(H, L);
	}

	void CPU_8080::IN(const uint8_t operand)
	{
		_registers[A] = _input_port[operand]();
	}

	void CPU_8080::OUT(const uint8_t operand) const
	{
		_output_port[operand](_registers[A]);
	}

	void CPU_8080::EI()
	{
		SetFlag(IR, true);
	}

	void CPU_8080::DI()
	{
		SetFlag(IR, false);
	}

	void CPU_8080::HLT()
	{
		_halt = true;
	}

	void CPU_8080::EXIT()
	{
		_exit = true;
	}

	void CPU_8080::UNDEFINED()
	{
		std::cerr << "Encountered undefined opcode at cycle: " << _cycle_counter << '\n';
		EXIT();
	}

}	 // namespace Intel8080
