#pragma once

#include "interfaces.h"

namespace Chip8 {

class Cpu : public ICpu
{
public:
	Cpu(IRom& rom,
	    IDisplay& display,
	    IController& controller,
		IRandom& random);

	void Execute() override;

private:
	void PrintError(uint16_t op) const;
	uint16_t GetNNN(uint16_t op) const;
	uint16_t GetNN(uint16_t op) const;
	uint16_t GetN(uint16_t op) const;
	uint16_t GetX(uint16_t op) const;
	uint16_t GetY(uint16_t op) const;

	IRom&                mRom;
	IDisplay&            mDisplay;
	IController&         mController;
	IRandom&             mRandom;
	uint16_t             mI;
	std::vector<uint8_t> mV;
	uint8_t              mDelayTimer;
};

} // namespace Chip8
