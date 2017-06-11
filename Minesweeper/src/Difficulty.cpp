#include "Difficulty.h"

#include "MathUtils.h"

#include <SFML/Window/VideoMode.hpp>

namespace minesweeper {

	namespace {
		const unsigned squareSize = 25;
		const unsigned maxRows = sf::VideoMode::getDesktopMode().height / squareSize - 6;
		const unsigned maxColumns = sf::VideoMode::getDesktopMode().width / squareSize - 4;
		const unsigned minMines = 0;
		const unsigned minRows = 1;
		const unsigned minColumns = 1;
	}

	Difficulty::Difficulty()
		:
		Difficulty(Type::Easy)
	{ }

	Difficulty::Difficulty(Type type)
		:
		type(type),
		rows(type == Type::Easy ? 8 : (type == Type::Medium ? 16 : (type == Type::Hard ? 20 : 8))),
		columns(type == Type::Easy ? 8 : (type == Type::Medium ? 16 : (type == Type::Hard ? 32 : 8))),
		mines(type == Type::Easy ? 10 : (type == Type::Medium ? 40 : (type == Type::Hard ? 99 : 10)))
	{ }

	Difficulty::Difficulty(unsigned rows, unsigned columns, unsigned mines)
		:
		type(Type::Custom),
		rows(math::clamp(rows, minRows, maxRows)),
		columns(math::clamp(columns, minColumns, maxColumns)),
		mines(math::clamp(mines, minMines, this->rows * this->columns - 1u))
	{ }

	Difficulty::Type Difficulty::getType() const
	{
		return type;
	}

	unsigned Difficulty::getRows() const
	{
		return rows;
	}

	unsigned Difficulty::getColumns() const
	{
		return columns;
	}

	unsigned Difficulty::getMines() const
	{
		return mines;
	}

}