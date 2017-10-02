#pragma once

#include "interfaces.h"

namespace Chip8 {

class Cpu : public ICpu
{
public:
	Cpu(IRom& rom,
	    IDisplay& display,
	    IController& controller,
	    IRandom& random,
	    IClock& clock);

	void Execute() override;

private:
	void PrintError(uint16_t op) const;

	IRom&                mRom;
	IDisplay&            mDisplay;
	IController&         mController;
	IRandom&             mRandom;
	IClock&              mClock;
	uint16_t             mI;
	std::vector<uint8_t> mV;
	uint8_t              mDelayTimer;
};

} // namespace Chip8
