#pragma once

#include <cstdint>
#include <stack>
#include <string>
#include <vector>

namespace Chip8 {

class Rom
{
public:
	Rom();

	bool Init(const std::string& path);
	uint16_t ReadOp();
	void SkipOp();
	void GoTo(uint16_t addr);
	void Call(uint16_t addr);
	void Return();
	void Dump(uint16_t addr,
	          const std::vector<uint8_t>& reg,
	          size_t index);
	void Load(uint16_t addr,
	          std::vector<uint8_t>& reg,
	          size_t index) const;

private:
	std::vector<uint8_t> mMemory;
	std::stack<uint16_t> mCalls;
	uint16_t             mAddr;
};

} // namespace Chip8
