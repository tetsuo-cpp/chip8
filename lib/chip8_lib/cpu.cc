#include "cpu.h"

#include <cassert>
#include <ctime>
#include <iostream>
#include <iterator>

namespace {

uint16_t GetNNN(uint16_t op)
{
	return (op & 0x0FFF);
}

uint16_t GetNN(uint16_t op)
{
	return (op & 0x00FF);
}

uint16_t GetN(uint16_t op)
{
	return (op & 0x000F);
}

uint16_t GetX(uint16_t op)
{
	return (op & 0x0F00) >> 8;
}

uint16_t GetY(uint16_t op)
{
	return (op & 0x00F0) >> 4;
}

} // anonymous namespace

namespace Chip8 {

Cpu::Cpu(IRom& rom,
         IDisplay& display,
         IController& controller,
         IRandom& random,
         IClock& clock)
	: mRom(rom),
	  mDisplay(display),
	  mController(controller),
	  mRandom(random),
	  mClock(clock),
	  mI(0x0),
	  mV(16, 0x0),
	  mDelayTimer(0x0)
{}

void Cpu::Execute()
{
	uint16_t op = mRom.ReadOp();
	switch (op & 0xF000)
	{
	case 0x0000:
	{
		switch (op & 0x00FF)
		{
		// 00E0: Clear display.
		case 0x00E0:
		{
			mDisplay.Clear();
			mDisplay.Render();
			break;
		}
		// 00EE: Return from current subroutine.
		case 0x00EE:
		{
			mRom.Return();
			break;
		}
		default:
			PrintError(op);
			break;
		}
		break;
	}
	// 1NNN: Jump to address NNN.
	case 0x1000:
	{
		mRom.GoTo(GetNNN(op));
		break;
	}
	// 2NNN: Call subroutine at address NNN.
	case 0x2000:
	{
		mRom.Call(GetNNN(op));
		break;
	}
	// 3XNN: If VX == NN, then skip next op.
	case 0x3000:
	{
		if (mV[GetX(op)] == GetNN(op))
		{
			mRom.SkipOp();
		}

		break;
	}
	// 4XNN: If VX != NN, then skip the next op.
	case 0x4000:
	{
		if (mV[GetX(op)] != GetNN(op))
		{
			mRom.SkipOp();
		}

		break;
	}
	// 5XYN: If VX == VY, then skip the next op.
	case 0x5000:
	{
		if (mV[GetX(op)] == mV[GetY(op)])
		{
			mRom.SkipOp();
		}

		break;
	}
	// 6XNN: Set VX = NN.
	case 0x6000:
	{
		mV[GetX(op)] = GetNN(op);
		break;
	}
	// 7XNN: Set VX = VX + NN.
	case 0x7000:
	{
		mV[GetX(op)] += GetNN(op);
		break;
	}
	case 0x8000:
	{
		switch (op & 0x000F)
		{
		// 8XY0: Set VX = VY.
		case 0x0000:
		{
			mV[GetX(op)] = mV[GetY(op)];
			break;
		}
		// 8XY1: Set VX = VX | VY.
		case 0x0001:
		{
			mV[GetX(op)] |= mV[GetY(op)];
			break;
		}
		// 8XY2: Set VX = VX & VY.
		case 0x0002:
		{
			mV[GetX(op)] &= mV[GetY(op)];
			break;
		}
		// 8XY3: Set VX = VX ^ VY.
		case 0x0003:
		{
			mV[GetX(op)] ^= mV[GetY(op)];
			break;
		}
		// 8XY4: Set VX = VX + VY.
		// Set VF to 1 if there is a carry and 0 if not.
		case 0x0004:
		{
			mV[0xF] = (mV[GetY(op)] >= (0xFF - mV[GetX(op)]));
			mV[GetX(op)] += mV[GetY(op)];
			break;
		}
		// 8XY5: Set VX = VX - VY.
		// Set VF to 0 if there is a borrow and 1 if not.
		case 0x0005:
		{
			mV[0xF] = (mV[GetX(op)] >= mV[GetY(op)]);
			mV[GetX(op)] -= mV[GetY(op)];
			break;
		}
		// 8XY6: Set VX = VX >> 1.
		// Set VF to the least significant bit.
		// The correct behaviour according to spec is to set VX = VY = VY >> 1, however some roms rely on the current implementation.
		case 0x0006:
		{
			mV[0xF] = (mV[GetX(op)] & 0x01) ? 0x1 : 0x0;
			mV[GetX(op)] >>= 1;
			break;
		}
		// 8XY7: Set VX = VY - VX.
		// Set VF to 0 if there is a borrow and 1 if not.
		case 0x0007:
		{
			mV[0xF] = (mV[GetY(op)] >= mV[GetX(op)]);
			mV[GetX(op)] = mV[GetY(op)] - mV[GetX(op)];
			break;
		}
		// 8XYE: Set VX = VX << 1.
		// Set VF to the most significant bit.
		// The correct behaviour according to spec is to set VX = VY = VY << 1, however some roms rely on the current implementation.
		case 0x000E:
		{
			mV[0xF] = (mV[GetX(op)] & 0x80) ? 0x1 : 0x0;
			mV[GetX(op)] <<= 1;
			break;
		}
		default:
			PrintError(op);
			break;
		}
		break;
	}
	// 9XYN: If VX != VY, then skip the next op.
	case 0x9000:
	{
		if (mV[GetX(op)] != mV[GetY(op)])
		{
			mRom.SkipOp();
		}

		break;
	}
	// ANNN: Set I = NNN.
	case 0xA000:
	{
		mI = GetNNN(op);
		break;
	}
	// BNNN: Jump to address V0 + NNN.
	case 0xB000:
	{
		mRom.GoTo(mV[0x0] + GetNNN(op));
		break;
	}
	// CXNN: VX = NN & rand().
	case 0xC000:
	{
		mV[GetX(op)] = GetNN(op) & mRandom();
		break;
	}
	// DXYN: Draw sprite at I of height N at coordinates (X, Y).
	case 0xD000:
	{
		std::vector<uint8_t> sprite(GetN(op));
		mRom.Load(mI, sprite, GetN(op));
		mV[0xF] = mDisplay.DrawSprite(mV[GetX(op)],
		                              mV[GetY(op)],
		                              sprite);
		mDisplay.Render();
		break;
	}
	case 0xE000:
	{
		switch (op & 0x00FF)
		{
		// EX9E: If key in VX is pressed, then skip the next op.
		case 0x009E:
		{
			if (mController.IsKeyPressed(mV[GetX(op)]))
			{
				mRom.SkipOp();
			}

			break;
		}
		// EXA1: If key in VX is not pressed, then skip the next op.
		case 0x00A1:
		{
			if (!mController.IsKeyPressed(mV[GetX(op)]))
			{
				mRom.SkipOp();
			}

			break;
		}
		default:
			PrintError(op);
			break;
		}
		break;
	}
	case 0xF000:
	{
		switch (op & 0x00FF)
		{
		// FX07: Set VX = delay timer.
		case 0x0007:
		{
			mV[GetX(op)] = mClock.GetDelayTimer();
			break;
		}
		// FX0A: Set VX = next key press.
		case 0x000A:
		{
			mV[GetX(op)] = mController.WaitForKeyPress();
			break;
		}
		// FX15: Set delay timer = VX.
		case 0x0015:
		{
			mClock.SetDelayTimer(mV[GetX(op)]);
			break;
		}
		// FX18: Set sound timer = VX.
		case 0x0018:
		{
			// Not implemented.
			break;
		}
		// FX1E: Set I = I + VX.
		case 0x001E:
		{
			mI += mV[GetX(op)];
			break;
		}
		// FX29: Set I = address of character sprite in VX.
		case 0x0029:
		{
			mI = mV[GetX(op)] * 5;
			break;
		}
		case 0x0033:
		{
			std::vector<uint8_t> decimal;
			decimal.push_back(mV[GetX(op)] / 100);
			decimal.push_back((mV[GetX(op)] / 10) % 10);
			decimal.push_back((mV[GetX(op)] % 100) % 100);
			mRom.Dump(mI, decimal, decimal.size());
			break;
		}
		// FX55: Dump V0 to VX to memory at address I.
		// Increment I for every value written.
		case 0x0055:
		{
			mRom.Dump(mI, mV, GetX(op) + 1);
			mI += GetX(op) + 1;
			break;
		}
		// FX65: Load V0 to VX from memory at address I.
		// Increment I for every value written.
		case 0x0065:
		{
			mRom.Load(mI, mV, GetX(op) + 1);
			mI += GetX(op) + 1;
			break;
		}
		default:
			PrintError(op);
			break;
		}
		break;
	}
	default:
		PrintError(op);
		break;
	}
}

void Cpu::PrintError(uint16_t op) const
{
	std::cerr << "cpu: Unrecognised op: " << std::hex << op << std::endl;
}

} // namespace Chip8
