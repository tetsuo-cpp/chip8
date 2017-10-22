#include <chip8_test_lib/mocks.h>
#include <chip8_test_lib/test_cpu.h>

#include <gtest/gtest.h>
#include <gmock/gmock.h>

using namespace ::testing;

namespace Chip8 {
namespace Test {

class CpuTest : public ::testing::Test
{
public:
	CpuTest()
		: mCpu(mRom,
		       mDisplay,
		       mController,
		       mRandom,
		       mClock)
	{}

	virtual void SetUp() override
	{
		auto& vBuffers = mCpu.GetV();
		std::for_each(vBuffers.begin(), vBuffers.end(),
		              [](uint8_t buf)
		              {
			              EXPECT_EQ(buf, 0x0);
		              });

		EXPECT_EQ(mCpu.GetI(), 0x0);
		EXPECT_EQ(mCpu.GetDelayTimer(), 0x0);
	}

	StrictMock<MockRom>        mRom;
	StrictMock<MockDisplay>    mDisplay;
	StrictMock<MockController> mController;
	StrictMock<MockRandom>     mRandom;
	StrictMock<MockClock>      mClock;
	TestCpu                    mCpu;
};

TEST_F(CpuTest, ClearDisplay_0x00E0)
{
	EXPECT_CALL(mRom, ReadOp())
		.WillOnce(Return(0x00E0));
	EXPECT_CALL(mDisplay, Clear());
	EXPECT_CALL(mDisplay, Render());

	mCpu.Execute();
}

TEST_F(CpuTest, ReturnFromSubroutine_0x00EE)
{
	EXPECT_CALL(mRom, ReadOp())
		.WillOnce(Return(0x00EE));
	EXPECT_CALL(mRom, Return());

	mCpu.Execute();
}

TEST_F(CpuTest, GoToAddress_0x1NNN)
{
	EXPECT_CALL(mRom, ReadOp())
		.WillOnce(Return(0x1123));
	EXPECT_CALL(mRom, GoTo(0x123));

	mCpu.Execute();
}

TEST_F(CpuTest, CallSubroutine_0x2NNN)
{
	EXPECT_CALL(mRom, ReadOp())
		.WillOnce(Return(0x2123));
	EXPECT_CALL(mRom, Call(0x123));

	mCpu.Execute();
}

TEST_F(CpuTest, Conditional_0x3XNN)
{
	auto& vBuffers = mCpu.GetV();
	vBuffers[0x3] = 0xFF;

	EXPECT_CALL(mRom, ReadOp())
		.WillOnce(Return(0x33FF));
	EXPECT_CALL(mRom, SkipOp())
		.Times(1);

	mCpu.Execute();

	EXPECT_CALL(mRom, ReadOp())
		.WillOnce(Return(0x3300));
	EXPECT_CALL(mRom, SkipOp())
		.Times(0);

	mCpu.Execute();
}

TEST_F(CpuTest, Conditional_0x4XNN)
{
	auto& vBuffers = mCpu.GetV();
	vBuffers[0x3] = 0xFF;

	EXPECT_CALL(mRom, ReadOp())
		.WillOnce(Return(0x43FF));
	EXPECT_CALL(mRom, SkipOp())
		.Times(0);

	mCpu.Execute();

	EXPECT_CALL(mRom, ReadOp())
		.WillOnce(Return(0x4300));
	EXPECT_CALL(mRom, SkipOp())
		.Times(1);

	mCpu.Execute();
}

TEST_F(CpuTest, Conditional_0x5XYN)
{
	auto& vBuffers = mCpu.GetV();
	vBuffers[0x3] = 0xFF;
	vBuffers[0x4] = 0xFF;

	EXPECT_CALL(mRom, ReadOp())
		.WillOnce(Return(0x5340));
	EXPECT_CALL(mRom, SkipOp())
		.Times(1);

	mCpu.Execute();

	vBuffers[0x4] = 0x00;

	EXPECT_CALL(mRom, ReadOp())
		.WillOnce(Return(0x5340));
	EXPECT_CALL(mRom, SkipOp())
		.Times(0);

	mCpu.Execute();
}

TEST_F(CpuTest, Assignment_0x6XNN)
{
	EXPECT_CALL(mRom, ReadOp())
		.WillOnce(Return(0x65EE));

	mCpu.Execute();

	auto& vBuffers = mCpu.GetV();
	EXPECT_EQ(vBuffers[0x5], 0xEE);
}

TEST_F(CpuTest, Addition_0x7XNN)
{
	auto& vBuffers = mCpu.GetV();
	vBuffers[0x3] = 0x1A;

	EXPECT_CALL(mRom, ReadOp())
		.WillOnce(Return(0x7302));

	mCpu.Execute();

	EXPECT_EQ(vBuffers[0x3], 0x1C);
}

TEST_F(CpuTest, Assignment_0x8XY0)
{
	auto& vBuffers = mCpu.GetV();
	vBuffers[0x3] = 0xAA;
	vBuffers[0x4] = 0xBB;

	EXPECT_CALL(mRom, ReadOp())
		.WillOnce(Return(0x8340));

	mCpu.Execute();

	EXPECT_EQ(vBuffers[0x3], 0xBB);
	EXPECT_EQ(vBuffers[0x4], 0xBB);
}

TEST_F(CpuTest, BitwiseOr_0x8XY1)
{
	auto& vBuffers = mCpu.GetV();
	vBuffers[0x3] = 0x00;
	vBuffers[0x4] = 0x1A;

	EXPECT_CALL(mRom, ReadOp())
		.WillOnce(Return(0x8341));

	mCpu.Execute();

	EXPECT_EQ(vBuffers[0x3], 0x1A);
	EXPECT_EQ(vBuffers[0x4], 0x1A);
}

TEST_F(CpuTest, BitwiseAnd_8XY2)
{
	auto& vBuffers = mCpu.GetV();
	vBuffers[0x3] = 0x3A;
	vBuffers[0x4] = 0x30;

	EXPECT_CALL(mRom, ReadOp())
		.WillOnce(Return(0x8342));

	mCpu.Execute();

	EXPECT_EQ(vBuffers[0x3], 0x30);
	EXPECT_EQ(vBuffers[0x4], 0x30);
}

TEST_F(CpuTest, BitwiseXor_0x8XY3)
{
	auto& vBuffers = mCpu.GetV();
	vBuffers[0x3] = 0x3A;
	vBuffers[0x4] = 0x30;

	EXPECT_CALL(mRom, ReadOp())
		.WillOnce(Return(0x8343));

	mCpu.Execute();

	EXPECT_EQ(vBuffers[0x3], 0x0A);
	EXPECT_EQ(vBuffers[0x4], 0x30);
}

TEST_F(CpuTest, Addition_0x8XY4_NoCarry)
{
	auto& vBuffers = mCpu.GetV();
	vBuffers[0x3] = 0x01;
	vBuffers[0x4] = 0x01;

	EXPECT_CALL(mRom, ReadOp())
		.WillOnce(Return(0x8344));

	mCpu.Execute();

	EXPECT_EQ(vBuffers[0x3], 0x02);
	EXPECT_EQ(vBuffers[0x4], 0x01);
	EXPECT_EQ(vBuffers[0xF], 0x00);
}

TEST_F(CpuTest, Addition_0x8XY4_Carry)
{
	auto& vBuffers = mCpu.GetV();
	vBuffers[0x3] = 0xFF;
	vBuffers[0x4] = 0x01;

	EXPECT_CALL(mRom, ReadOp())
		.WillOnce(Return(0x8344));

	mCpu.Execute();

	EXPECT_EQ(vBuffers[0x3], 0x00);
	EXPECT_EQ(vBuffers[0x4], 0x01);
	EXPECT_EQ(vBuffers[0xF], 0x01);
}

TEST_F(CpuTest, Subtraction_0x8XY5_NoBorrow)
{
	auto& vBuffers = mCpu.GetV();
	vBuffers[0x3] = 0xFF;
	vBuffers[0x4] = 0x01;

	EXPECT_CALL(mRom, ReadOp())
		.WillOnce(Return(0x8345));

	mCpu.Execute();

	EXPECT_EQ(vBuffers[0x3], 0xFE);
	EXPECT_EQ(vBuffers[0x4], 0x01);
	EXPECT_EQ(vBuffers[0xF], 0x01);
}

TEST_F(CpuTest, Subtraction_0x8XY5_Borrow)
{
	auto& vBuffers = mCpu.GetV();
	vBuffers[0x3] = 0x01;
	vBuffers[0x4] = 0x02;

	EXPECT_CALL(mRom, ReadOp())
		.WillOnce(Return(0x8345));

	mCpu.Execute();

	EXPECT_EQ(vBuffers[0x3], 0xFF);
	EXPECT_EQ(vBuffers[0x4], 0x02);
	EXPECT_EQ(vBuffers[0xF], 0x00);
}

TEST_F(CpuTest, BitShiftRight_0x8XY6_Lsb0)
{
	auto& vBuffers = mCpu.GetV();
	vBuffers[0x3] = 0xF0;

	EXPECT_CALL(mRom, ReadOp())
		.WillOnce(Return(0x8236));

	mCpu.Execute();

	EXPECT_EQ(vBuffers[0x2], 0x78);
	EXPECT_EQ(vBuffers[0x3], 0x78);
	EXPECT_EQ(vBuffers[0xF], 0x0);
}

TEST_F(CpuTest, BitShiftRight_0x8XY6_Lsb1)
{
	auto& vBuffers = mCpu.GetV();
	vBuffers[0x3] = 0xF1;

	EXPECT_CALL(mRom, ReadOp())
		.WillOnce(Return(0x8236));

	mCpu.Execute();

	EXPECT_EQ(vBuffers[0x2], 0x78);
	EXPECT_EQ(vBuffers[0x3], 0x78);
	EXPECT_EQ(vBuffers[0xF], 0x1);
}

TEST_F(CpuTest, Subtraction_0x8XY7_NoBorrow)
{
	auto& vBuffers = mCpu.GetV();
	vBuffers[0x2] = 0x02;
	vBuffers[0x3] = 0x08;

	EXPECT_CALL(mRom, ReadOp())
		.WillOnce(Return(0x8237));

	mCpu.Execute();

	EXPECT_EQ(vBuffers[0x2], 0x06);
	EXPECT_EQ(vBuffers[0x3], 0x08);
	EXPECT_EQ(vBuffers[0xF], 0x1);
}

TEST_F(CpuTest, Subtraction_0x8XY7_Borrow)
{
	auto& vBuffers = mCpu.GetV();
	vBuffers[0x2] = 0x04;
	vBuffers[0x3] = 0x02;

	EXPECT_CALL(mRom, ReadOp())
		.WillOnce(Return(0x8237));

	mCpu.Execute();

	EXPECT_EQ(vBuffers[0x2], 0xFE);
	EXPECT_EQ(vBuffers[0x3], 0x02);
	EXPECT_EQ(vBuffers[0xF], 0x0);
}

TEST_F(CpuTest, BitShiftLeft_0x8XYE_Msb0)
{
	auto& vBuffers = mCpu.GetV();
	vBuffers[0x3] = 0x11;

	EXPECT_CALL(mRom, ReadOp())
		.WillOnce(Return(0x823E));

	mCpu.Execute();

	EXPECT_EQ(vBuffers[0x2], 0x22);
	EXPECT_EQ(vBuffers[0x3], 0x22);
	EXPECT_EQ(vBuffers[0xF], 0x0);
}

TEST_F(CpuTest, BitShiftLeft_0x8XYE_Msb1)
{
	auto& vBuffers = mCpu.GetV();
	vBuffers[0x3] = 0x81;

	EXPECT_CALL(mRom, ReadOp())
		.WillOnce(Return(0x823E));

	mCpu.Execute();

	EXPECT_EQ(vBuffers[0x2], 0x02);
	EXPECT_EQ(vBuffers[0x3], 0x02);
	EXPECT_EQ(vBuffers[0xF], 0x1);
}

TEST_F(CpuTest, Conditional_0x9XYN)
{
	auto& vBuffers = mCpu.GetV();
	vBuffers[0x3] = 0xFF;
	vBuffers[0x4] = 0xFF;

	EXPECT_CALL(mRom, ReadOp())
		.WillOnce(Return(0x9340));
	EXPECT_CALL(mRom, SkipOp())
		.Times(0);

	mCpu.Execute();

	vBuffers[0x4] = 0x0;

	EXPECT_CALL(mRom, ReadOp())
		.WillOnce(Return(0x9340));
	EXPECT_CALL(mRom, SkipOp())
		.Times(1);

	mCpu.Execute();
}

TEST_F(CpuTest, SetIndex_0xANNN)
{
	EXPECT_CALL(mRom, ReadOp())
		.WillOnce(Return(0xA222));

	mCpu.Execute();

	EXPECT_EQ(mCpu.GetI(), 0x222);
}

TEST_F(CpuTest, GoTo_0xBNNN)
{
	auto& vBuffers = mCpu.GetV();
	vBuffers[0x0] = 0x11;

	EXPECT_CALL(mRom, ReadOp())
		.WillOnce(Return(0xB022));
	EXPECT_CALL(mRom, GoTo(0x033))
		.Times(1);

	mCpu.Execute();
}

TEST_F(CpuTest, Random_0xCXNN)
{
	auto& vBuffers = mCpu.GetV();

	EXPECT_CALL(mRom, ReadOp())
		.WillOnce(Return(0xC20F));
	EXPECT_CALL(mRandom, Generate())
		.WillOnce(Return(0x22));

	mCpu.Execute();

	EXPECT_EQ(vBuffers[0x2], 0x02);
}

TEST_F(CpuTest, DrawSprite_0xDXYN_NoCollision)
{
	mCpu.GetI() = 0x4444;
	auto& vBuffers = mCpu.GetV();
	vBuffers[0x2] = 0x4;
	vBuffers[0x3] = 0x6;

	std::vector<uint8_t> sprite {0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1};
	EXPECT_EQ(sprite.size(), 8u);

	EXPECT_CALL(mRom, ReadOp())
		.WillOnce(Return(0xD238));
	EXPECT_CALL(mRom, Load(0x4444, _, 8))
		.WillOnce(WithArgs<1>(
			          Invoke([&sprite](std::vector<uint8_t>& buf)
			                 {
				                 // Populate the sprite buffer to simulate loading from a rom.
				                 buf = sprite;
			                 })
			          ));
	EXPECT_CALL(mDisplay, DrawSprite(0x4, 0x6, sprite))
		.WillOnce(Return(0x0));
	EXPECT_CALL(mDisplay, Render())
		.Times(1);

	mCpu.Execute();

	EXPECT_EQ(vBuffers[0xF], 0x0);
}

TEST_F(CpuTest, DrawSprite_0xDXYN_Collision)
{
	mCpu.GetI() = 0x4444;

	auto& vBuffers = mCpu.GetV();
	vBuffers[0x2] = 0x4;
	vBuffers[0x3] = 0x6;

	std::vector<uint8_t> sprite {0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1};
	EXPECT_EQ(sprite.size(), 8u);

	EXPECT_CALL(mRom, ReadOp())
		.WillOnce(Return(0xD238));
	EXPECT_CALL(mRom, Load(0x4444, _, 8))
		.WillOnce(WithArgs<1>(
			          Invoke([&sprite](std::vector<uint8_t>& buf)
			                 {
				                 // Populate the sprite buffer to simulate loading from a rom.
				                 buf = sprite;
			                 })
			          ));
	EXPECT_CALL(mDisplay, DrawSprite(0x4, 0x6, sprite))
		.WillOnce(Return(0x1));
	EXPECT_CALL(mDisplay, Render())
		.Times(1);

	mCpu.Execute();

	EXPECT_EQ(vBuffers[0xF], 0x1);
}

TEST_F(CpuTest, KeyPress_0xEX9E)
{
	auto& vBuffers = mCpu.GetV();
	vBuffers[0x2] = 0xA;

	EXPECT_CALL(mRom, ReadOp())
		.WillOnce(Return(0xE29E));
	EXPECT_CALL(mController, IsKeyPressed(0xA))
		.WillOnce(Return(true));
	EXPECT_CALL(mRom, SkipOp())
		.Times(1);

	mCpu.Execute();

	EXPECT_EQ(vBuffers[0x2], 0xA);

	EXPECT_CALL(mRom, ReadOp())
		.WillOnce(Return(0xE29E));
	EXPECT_CALL(mController, IsKeyPressed(0xA))
		.WillOnce(Return(false));
	EXPECT_CALL(mRom, SkipOp())
		.Times(0);

	mCpu.Execute();
}

TEST_F(CpuTest, KeyPress_0xEXA1)
{
	auto& vBuffers = mCpu.GetV();
	vBuffers[0x2] = 0xA;

	EXPECT_CALL(mRom, ReadOp())
		.WillOnce(Return(0xE2A1));
	EXPECT_CALL(mController, IsKeyPressed(0xA))
		.WillOnce(Return(true));
	EXPECT_CALL(mRom, SkipOp())
		.Times(0);

	mCpu.Execute();

	EXPECT_EQ(vBuffers[0x2], 0xA);

	EXPECT_CALL(mRom, ReadOp())
		.WillOnce(Return(0xE2A1));
	EXPECT_CALL(mController, IsKeyPressed(0xA))
		.WillOnce(Return(false));
	EXPECT_CALL(mRom, SkipOp())
		.Times(1);

	mCpu.Execute();
}

TEST_F(CpuTest, GetDelayTimer_0xFX07)
{
	EXPECT_CALL(mRom, ReadOp())
		.WillOnce(Return(0xF207));
	EXPECT_CALL(mClock, GetDelayTimer())
		.WillOnce(Return(0x34));

	mCpu.Execute();

	auto& vBuffers = mCpu.GetV();
	EXPECT_EQ(vBuffers[0x2], 0x0034);
}

TEST_F(CpuTest, WaitForKeyPress_0xFX0A)
{
	EXPECT_CALL(mRom, ReadOp())
		.WillOnce(Return(0xF20A));
	EXPECT_CALL(mController, WaitForKeyPress())
		.WillOnce(Return(0x1));

	mCpu.Execute();

	auto& vBuffers = mCpu.GetV();
	EXPECT_EQ(vBuffers[0x2], 0x0001);
}

TEST_F(CpuTest, SetDelayTimer_0xFX15)
{
	auto& vBuffers = mCpu.GetV();
	vBuffers[0x2] = 0xA;

	EXPECT_CALL(mRom, ReadOp())
		.WillOnce(Return(0xF215));
	EXPECT_CALL(mClock, SetDelayTimer(0xA))
		.Times(1);

	mCpu.Execute();
}

TEST_F(CpuTest, SoundTimer_0xFX18)
{
	// Not implemented.
	EXPECT_CALL(mRom, ReadOp())
		.WillOnce(Return(0xF018));

	mCpu.Execute();
}

TEST_F(CpuTest, IncrementIndex_0xFX1E)
{
	mCpu.GetI() = 0x1;

	auto& vBuffers = mCpu.GetV();
	vBuffers[0x2] = 0x3;

	EXPECT_CALL(mRom, ReadOp())
		.WillOnce(Return(0xF21E));

	mCpu.Execute();

	EXPECT_EQ(mCpu.GetI(), 0x4);
}

TEST_F(CpuTest, GetIndexOfCharacter_0xFX29)
{
	auto& vBuffers = mCpu.GetV();
	vBuffers[0x0] = 0x3;

	EXPECT_CALL(mRom, ReadOp())
		.WillOnce(Return(0xF029));

	mCpu.Execute();

	EXPECT_EQ(mCpu.GetI(), 15);
}

TEST_F(CpuTest, Dump_0xFX55)
{
	mCpu.GetI() = 5;

	EXPECT_CALL(mRom, ReadOp())
		.WillOnce(Return(0xF255));
	EXPECT_CALL(mRom, Dump(5, mCpu.GetV(), 3))
		.Times(1);

	mCpu.Execute();

	EXPECT_EQ(mCpu.GetI(), 8);
}

TEST_F(CpuTest, Load_0xFX65)
{
	mCpu.GetI() = 3;

	EXPECT_CALL(mRom, ReadOp())
		.WillOnce(Return(0xF265));
	EXPECT_CALL(mRom, Load(3, mCpu.GetV(), 3))
		.Times(1);

	mCpu.Execute();

	EXPECT_EQ(mCpu.GetI(), 6);
}

} // namespace Test
} // namespace Chip8
