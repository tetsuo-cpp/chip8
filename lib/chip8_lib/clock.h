#pragma once

#include "interfaces.h"

namespace Chip8 {

class Clock : public IClock
{
public:
	Clock();

	void WaitForNextCycle() override;
	uint8_t GetDelayTimer() const override;
	void SetDelayTimer(uint8_t timer) override;

private:
	uint8_t mDelayTimer;
	size_t mTicks;
};

} // namespace Chip8
