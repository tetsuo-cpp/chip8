#pragma once

#include <chip8_lib/cpu.h>

namespace Chip8 {
namespace Test {

class TestCpu : public Cpu
{
public:
	TestCpu(IRom& rom,
	        IDisplay& display,
	        IController& controller,
	        IRandom& random,
	        IClock& clock)
		: Cpu(rom,
		      display,
		      controller,
		      random,
		      clock)
	{}

	uint16_t& GetI() { return mI; }
	std::vector<uint8_t>& GetV() { return mV; }
	uint8_t& GetDelayTimer() { return mDelayTimer; }
};

} // namespace Test
} // namespace Chip8
