#pragma once

#include "interfaces.h"

#include <random>

namespace Chip8 {

class Random : public IRandom
{
public:
	uint8_t Generate() override;

private:
	std::default_random_engine mEngine;
	std::uniform_int_distribution<uint8_t> mDist;
};

} // namespace Chip8
