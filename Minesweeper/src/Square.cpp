#include "Square.h"

namespace minesweeper {

	Square::Square()
		:
		Square(Empty)
	{ }

	Square::Square(Square::Type type)
		:
		value(type)
	{ }

	bool Square::is(Square::Type type) const
	{
		return (value & type) == type;
	}

	int Square::isNumber() const
	{
		return value & ((1 << 4) - 1);
	}

	void Square::toggle(Square::Type type)
	{
		value ^= type;
	}

	void Square::increment()
	{
		if ((value & ((1 << 4) - 1)) == ((1 << 4) - 1)) 
			value = 0;
		else value++;
	}
}