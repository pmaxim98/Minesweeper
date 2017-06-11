#ifndef MATHUTILS_H
#define MATHUTILS_H

#include <random>
#include <algorithm>

#include <SFML/System/Vector2.hpp>

namespace math {

	template<typename T>
	T clamp(const T & value, const T & lowerBound, const T & upperBound)
	{
		T v = std::max(std::min(value, upperBound), lowerBound);
		return v;
	}

	void initRandomGenerator();

	int genRandom(int ls, int rs);

	static const sf::Vector2i directions[8] = {{-1, 0}, {-1, -1}, {0, -1}, {-1, 1}, {1, 0}, {1, 1}, {0, 1}, {1, -1}};

	static std::mt19937 mt_rand;

}

#endif // !MATH_H

