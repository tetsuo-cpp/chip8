#pragma once

#include <chip8_lib/interfaces.h>

#include <gtest/gtest.h>
#include <gmock/gmock.h>

namespace Chip8 {
namespace Test {

class MockRom : public IRom
{
public:
	MOCK_METHOD0(ReadOp, uint16_t());
	MOCK_METHOD0(SkipOp, void());
	MOCK_METHOD1(GoTo, void(uint16_t addr));
	MOCK_METHOD1(Call, void(uint16_t addr));
	MOCK_METHOD0(Return, void());
	MOCK_METHOD3(Dump, void(uint16_t addr,
	                        const std::vector<uint8_t>& reg,
	                        size_t length));
	MOCK_CONST_METHOD3(Load, void(uint16_t addr,
	                              std::vector<uint8_t>& reg,
	                              size_t length));
};

class MockDisplay : public IDisplay
{
public:
	MOCK_METHOD0(Clear, void());
	MOCK_METHOD3(DrawSprite, bool(uint16_t x,
	                              uint16_t y,
	                              const std::vector<uint8_t>& sprite));
	MOCK_CONST_METHOD0(Render, void());
};

class MockController : public IController
{
public:
	MOCK_METHOD0(ProcessEvents, bool());
	MOCK_METHOD0(WaitForKeyPress, uint8_t());
	MOCK_CONST_METHOD1(IsKeyPressed, bool(uint8_t key));
};

class MockRandom : public IRandom
{
public:
	MOCK_METHOD0(Generate, uint8_t());
};

class MockClock : public IClock
{
public:
	MOCK_METHOD0(WaitForNextCycle, void());
	MOCK_CONST_METHOD0(GetDelayTimer, uint8_t());
	MOCK_METHOD1(SetDelayTimer, void(uint8_t timer));
};

} // namespace Test
} // namespace Chip8
