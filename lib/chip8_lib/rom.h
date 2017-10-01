#pragma once

#include "interfaces.h"

#include <stack>

namespace Chip8 {

class Rom : public IRom
{
public:
	Rom(const std::string& path);

	uint16_t ReadOp() override;
	void SkipOp() override;
	void GoTo(uint16_t addr) override;
	void Call(uint16_t addr) override;
	void Return() override;
	void Dump(uint16_t addr,
	          const std::vector<uint8_t>& reg,
	          size_t length) override;
	void Load(uint16_t addr,
	          std::vector<uint8_t>& reg,
	          size_t length) const override;

private:
	std::vector<uint8_t> mMemory;
	std::stack<uint16_t> mCalls;
	uint16_t             mAddr;
};

} // namespace Chip8
