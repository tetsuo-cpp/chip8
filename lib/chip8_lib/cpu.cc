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
         IRandom& random)
	: mRom(rom),
	  mDisplay(display),
	  mController(controller),
	  mRandom(random),
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
		case 0x00E0:
		{
			mDisplay.Clear();
			break;
		}
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
	case 0x1000:
	{
		mRom.GoTo(GetNNN(op));
		break;
	}
	case 0x2000:
	{
		mRom.Call(GetNNN(op));
		break;
	}
	case 0x3000:
	{
		if (mV[GetX(op)] == GetNN(op))
		{
			mRom.SkipOp();
		}

		break;
	}
	case 0x4000:
	{
		if (mV[GetX(op)] != GetNN(op))
		{
			mRom.SkipOp();
		}

		break;
	}
	case 0x5000:
	{
		if (mV[GetX(op)] == mV[GetY(op)])
		{
			mRom.SkipOp();
		}

		break;
	}
	case 0x6000:
	{
		mV[GetX(op)] = GetNN(op);
		break;
	}
	case 0x7000:
	{
		mV[GetX(op)] += GetNN(op);
		break;
	}
	case 0x8000:
	{
		switch (op & 0x000F)
		{
		case 0x0000:
		{
			mV[GetX(op)] = mV[GetY(op)];
			break;
		}
		case 0x0001:
		{
			mV[GetX(op)] |= mV[GetY(op)];
			break;
		}
		case 0x0002:
		{
			mV[GetX(op)] &= mV[GetY(op)];
			break;
		}
		case 0x0003:
		{
			mV[GetX(op)] ^= mV[GetY(op)];
			break;
		}
		case 0x0004:
		{
			mV[0xF] = (mV[GetY(op)] > (0xFF - mV[GetX(op)]));
			mV[GetX(op)] += mV[GetY(op)];
			break;
		}
		case 0x0005:
		{
			mV[0xF] = (mV[GetY(op)] > mV[GetX(op)]);
			mV[GetX(op)] -= mV[GetY(op)];
			break;
		}
		case 0x0006:
		{
			mV[0xF] = (mV[GetX(op)] & 0x1);
			mV[GetX(op)] /= 2;
			break;
		}
		case 0x0007:
		{
			mV[0xF] = (mV[GetX(op)] > mV[GetY(op)]);
			mV[GetX(op)] = mV[GetY(op)] - mV[GetX(op)];
			break;
		}
		case 0x000E:
		{
			mV[0xF] = (mV[GetX(op)] & 0x80);
			mV[GetX(op)] *= 2;
			break;
		}
		default:
			PrintError(op);
			break;
		}
		break;
	}
	case 0x9000:
	{
		if (mV[GetX(op)] != mV[GetY(op)])
		{
			mRom.SkipOp();
		}

		break;
	}
	case 0xA000:
	{
		mI = GetNNN(op);
		break;
	}
	case 0xB000:
	{
		mRom.GoTo(mV[0x0] + GetNNN(op));
		break;
	}
	case 0xC000:
	{
		mV[GetX(op)] = GetNN(op) & mRandom();
		break;
	}
	case 0xD000:
	{
		std::vector<uint8_t> sprite(GetN(op));
		mRom.Load(mI, sprite, GetN(op));
		mV[0xF] = mDisplay.DrawSprite(mV[GetX(op)],
		                              mV[GetY(op)],
		                              sprite) ? 0x1 : 0x0;
		break;
	}
	case 0xE000:
	{
		switch (op & 0x00FF)
		{
		case 0x009E:
		{
			if (mController.IsKeyPressed(mV[GetX(op)]))
			{
				mRom.SkipOp();
			}

			break;
		}
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
		case 0x0007:
		{
			mV[GetX(op)] = mDelayTimer;
			break;
		}
		case 0x000A:
		{
			mV[GetX(op)] = mController.WaitForKeyPress();
			break;
		}
		case 0x0015:
		{
			mDelayTimer = mV[GetX(op)];
			break;
		}
		case 0x0018:
		{
			// Set sound timer.
			break;
		}
		case 0x001E:
		{
			mI += mV[GetX(op)];
			break;
		}
		case 0x0029:
		{
			mI = mV[GetX(op)] * 0x5;
			break;
		}
		case 0x0033:
		{
			std::vector<uint8_t> decimal;
			decimal.push_back(mV[GetX(op)] / 100);
			decimal.push_back((mV[GetX(op)] / 10) % 10);
			decimal.push_back((mV[GetX(op)] % 100) % 100);
			mRom.Dump(mI, decimal, decimal.size() - 1);
			break;
		}
		case 0x0055:
		{
			mRom.Dump(mI, mV, GetX(op) + 1);
			mI += GetX(op) + 1;
			break;
		}
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

	if (mDelayTimer > 0)
	{
		--mDelayTimer;
	}

	mDisplay.Render();
}

void Cpu::PrintError(uint16_t op) const
{
	std::cerr << "cpu: Unrecognised op: " << std::hex << op << std::endl;
}

} // namespace Chip8
