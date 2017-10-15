#pragma once

#include "interfaces.h"

#include <random>

namespace Chip8 {

class Random : public IRandom
{
private:
	uint8_t Generate() override
	{
		return mDist(mEngine);
	}

	std::default_random_engine             mEngine;
	std::uniform_int_distribution<uint8_t> mDist;
};

} // namespace Chip8
