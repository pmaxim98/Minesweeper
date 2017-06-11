#include "MathUtils.h"

#include <chrono>

namespace math {

	

	void initRandomGenerator()
	{
		const auto seed = std::chrono::high_resolution_clock::now().time_since_epoch().count();
		mt_rand.seed(static_cast<unsigned>(seed));
	}

	int genRandom(int ls, int rs)
	{
		std::uniform_int_distribution<int> distribution(ls, rs);
		return distribution(mt_rand);
	}

}