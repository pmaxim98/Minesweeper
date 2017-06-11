#include "Game.h"
#include "MathUtils.h"
#include "ResourceManager.h"

int main()
{
	math::initRandomGenerator();
	if (!ResourceManager::getInstance().init())
		std::exit(-1);

	minesweeper::Game game;
	game.run();
}