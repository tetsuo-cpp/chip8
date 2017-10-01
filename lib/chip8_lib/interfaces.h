#pragma once

#include <string>
#include <vector>

namespace Chip8 {

class ICpu
{
public:
	virtual ~ICpu() {}

	virtual void Execute() = 0;
};

class IRom
{
public:
	virtual ~IRom() {}

	virtual uint16_t ReadOp() = 0;
	virtual void SkipOp() = 0;
	virtual void GoTo(uint16_t addr) = 0;
	virtual void Call(uint16_t addr) = 0;
	virtual void Return() = 0;
	virtual void Dump(uint16_t addr,
	                  const std::vector<uint8_t>& reg,
	                  size_t length) = 0;
	virtual void Load(uint16_t addr,
	                  std::vector<uint8_t>& reg,
	                  size_t length) const = 0;
};

class IDisplay
{
public:
	virtual ~IDisplay() {}

	virtual void Clear() = 0;
	virtual bool DrawSprite(uint16_t x,
	                        uint16_t y,
	                        const std::vector<uint8_t>& sprite) = 0;
	virtual void Render() const = 0;
};

class IController
{
public:
	virtual ~IController() {}

	virtual bool ProcessEvents() = 0;
	virtual uint8_t WaitForKeyPress() = 0;
	virtual bool IsKeyPressed(uint8_t key) const = 0;
};

class IRandom
{
public:
	virtual ~IRandom() {}

	uint8_t operator()() { return Generate(); }
	virtual uint8_t Generate() = 0;
};

} // namespace Chip8
