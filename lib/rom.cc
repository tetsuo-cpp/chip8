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

Rom::Rom()
	: mMemory(MAX_MEMORY_ADDR, 0x0),
	  mAddr(ROM_ADDR)
{}

bool Rom::Init(const std::string& path)
{
	std::ifstream file(path, std::ios::binary);
	if (!file.is_open())
	{
		std::cerr << "File could not be opened." << std::endl;
		return false;
	}

	file.seekg(0, std::ios::end);
	size_t fileSize = file.tellg();
	if (fileSize > (MAX_MEMORY_ADDR - ROM_ADDR))
	{
		std::cerr << "File exceeds maximum ROM size." << std::endl;
		return false;
	}

	file.seekg(0, std::ios::beg);
	file.read(reinterpret_cast<char*>(&mMemory[ROM_ADDR]),
	          fileSize);
	file.close();

	std::copy(FONT_SET.begin(),
	          FONT_SET.end(),
	          mMemory.begin());

	return true;
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
	std::cout << "rom: Jumping to: " << addr << "." << std::endl;
	mAddr = addr;
}

void Rom::Call(uint16_t addr)
{
	std::cout << "rom: Calling subroutine at: " << addr << "." << std::endl;
	mCalls.push(mAddr);
	mAddr = addr;
}

void Rom::Return()
{
	std::cout << "rom: Returning from current subroutine." << std::endl;
	assert(!mCalls.empty());

	mAddr = mCalls.top();
	mCalls.pop();
}

void Rom::Dump(uint16_t addr,
               const std::vector<uint8_t>& reg,
               size_t index)
{
	std::cout << "rom: Dumping data at: " << addr << "." << std::endl;
	assert((addr + index) < mMemory.size());
	assert(index < reg.size());

	std::copy(reg.begin(),
	          reg.begin() + index,
	          mMemory.begin() + addr);
}

void Rom::Load(uint16_t addr,
               std::vector<uint8_t>& reg,
               size_t index) const
{
	std::cout << "rom: Loading buffers at: " << addr << "." << std::endl;
	assert((addr + index) < mMemory.size());
	assert(index < reg.size());

	std::copy(mMemory.begin() + addr,
	          mMemory.begin() + addr + index,
	          reg.begin());
}

} // namespace Chip8
