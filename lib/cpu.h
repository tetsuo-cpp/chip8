#pragma once

#include "controller.h"
#include "display.h"
#include "rom.h"

namespace Chip8 {

class Cpu
{
public:
	Cpu(Rom& rom,
	    Display& display,
	    Controller& controller);

	void EmulateCycle();

private:
	void PrintError(uint16_t op) const;
	uint16_t GetNNN(uint16_t op) const;
	uint16_t GetNN(uint16_t op) const;
	uint16_t GetN(uint16_t op) const;
	uint16_t GetX(uint16_t op) const;
	uint16_t GetY(uint16_t op) const;

	Rom&                 mRom;
	Display&             mDisplay;
	Controller&          mController;
	uint16_t             mI;
	std::vector<uint8_t> mV;
	uint8_t              mDelayTimer;
};

} // namespace Chip8
