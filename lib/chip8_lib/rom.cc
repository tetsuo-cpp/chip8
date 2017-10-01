#include "rom.h"

#include <cassert>
#include <fstream>
#include <iostream>

namespace {

static const uint16_t MAX_MEMORY_ADDR = 4096;
static const uint16_t ROM_ADDR        = 512;

static const std::vector<uint8_t> FONT_SET =
{
	0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
	0x20, 0x60, 0x20, 0x20, 0x70, // 1
	0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
	0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
	0x90, 0x90, 0xF0, 0x10, 0x10, // 4
	0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
	0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
	0xF0, 0x10, 0x20, 0x40, 0x40, // 7
	0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
	0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
	0xF0, 0x90, 0xF0, 0x90, 0x90, // A
	0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
	0xF0, 0x80, 0x80, 0x80, 0xF0, // C
	0xE0, 0x90, 0x90, 0x90, 0xE0, // D
	0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
	0xF0, 0x80, 0xF0, 0x80, 0x80  // F
};

} // namespace anonymous

namespace Chip8 {

Rom::Rom(const std::string& path)
	: mMemory(MAX_MEMORY_ADDR, 0x0),
	  mAddr(ROM_ADDR)
{
	std::ifstream file(path, std::ios::binary);
	if (!file.is_open())
	{
		throw std::runtime_error("rom: File could not be opened.");
	}

	file.seekg(0, std::ios::end);
	size_t fileSize = file.tellg();
	if (fileSize > (MAX_MEMORY_ADDR - ROM_ADDR))
	{
		file.close();
		throw std::runtime_error("rom: File exceeds maximum ROM size.");
	}

	file.seekg(0, std::ios::beg);
	file.read(reinterpret_cast<char*>(&mMemory[ROM_ADDR]),
	          fileSize);
	file.close();

	std::copy(FONT_SET.begin(),
	          FONT_SET.end(),
	          mMemory.begin());
}

uint16_t Rom::ReadOp()
{
	assert(mAddr < mMemory.size());

	uint16_t op = mMemory[mAddr] << 8 | mMemory[mAddr + 1];
	mAddr += 2;

	return op;
}

void Rom::SkipOp()
{
	mAddr += 2;
}

void Rom::GoTo(uint16_t addr)
{
	mAddr = addr;
}

void Rom::Call(uint16_t addr)
{
	mCalls.push(mAddr);
	mAddr = addr;
}

void Rom::Return()
{
	assert(!mCalls.empty());

	mAddr = mCalls.top();
	mCalls.pop();
}

void Rom::Dump(uint16_t addr,
               const std::vector<uint8_t>& reg,
               size_t length)
{
	assert((addr + length) <= mMemory.size());
	assert(length <= reg.size());

	std::copy(reg.begin(),
	          reg.begin() + length,
	          mMemory.begin() + addr);
}

void Rom::Load(uint16_t addr,
               std::vector<uint8_t>& reg,
               size_t length) const
{
	assert((addr + length) <= mMemory.size());
	assert(length <= reg.size());

	std::copy(mMemory.begin() + addr,
	          mMemory.begin() + addr + length,
	          reg.begin());
}

} // namespace Chip8
