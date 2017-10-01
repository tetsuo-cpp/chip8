#include "random.h"

namespace Chip8 {

uint8_t Random::Generate()
{
	return mDist(mEngine);
}

} // namespace Chip8
