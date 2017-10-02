#pragma once

#include "interfaces.h"

#include <memory>

namespace Chip8 {

class Chip8
{
public:
	Chip8();
	~Chip8();

	void Run(const std::string& path);

private:
	std::unique_ptr<ICpu>        mCpu;
	std::unique_ptr<IRom>        mRom;
	std::unique_ptr<IDisplay>    mDisplay;
	std::unique_ptr<IController> mController;
	std::unique_ptr<IRandom>     mRandom;
	std::unique_ptr<IClock>      mClock;
};

} // namespace Chip8
