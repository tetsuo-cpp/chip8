#include "clock.h"

#include <chrono>
#include <thread>

namespace {

static const size_t MICROS_IN_SECOND = 1000000;
static const size_t CPU_HZ           = 540;
static const size_t DELAY_HZ         = 60;

static constexpr size_t DELAY_TICKS = CPU_HZ / DELAY_HZ;

} // anonymous namespace

namespace Chip8 {

Clock::Clock()
	: mDelayTimer(0x0),
	  mTicks(0)
{}

void Clock::WaitForNextCycle()
{
	std::this_thread::sleep_for(
		std::chrono::microseconds(MICROS_IN_SECOND / CPU_HZ));

	++mTicks;
	if (mTicks == DELAY_TICKS)
	{
		mTicks = 0;
		if (mDelayTimer > 0)
		{
			--mDelayTimer;
		}
	}
}

uint8_t Clock::GetDelayTimer() const
{
	return mDelayTimer;
}

void Clock::SetDelayTimer(uint8_t timer)
{
	mDelayTimer = timer;
}

} // namespace Chip8
