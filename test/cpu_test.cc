#include <chip8_test_lib/test_cpu.h>
#include <chip8_test_lib/mocks.h>

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

TEST_F(CpuTest, Comparisons_0x3XNN)
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

TEST_F(CpuTest, Comparisons_0x4XNN)
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

TEST_F(CpuTest, Comparisons_0x5XYN)
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

TEST_F(CpuTest, Arithmetic_0x6XNN)
{
	EXPECT_CALL(mRom, ReadOp())
		.WillOnce(Return(0x65EE));

	mCpu.Execute();

	auto& vBuffers = mCpu.GetV();
	EXPECT_EQ(vBuffers[0x5], 0xEE);
}

TEST_F(CpuTest, Arithmetic_0x7XNN)
{
	auto& vBuffers = mCpu.GetV();
	vBuffers[0x3] = 0x1A;

	EXPECT_CALL(mRom, ReadOp())
		.WillOnce(Return(0x7302));

	mCpu.Execute();

	EXPECT_EQ(vBuffers[0x3], 0x1C);
}

} // namespace Test
} // namespace Chip8
