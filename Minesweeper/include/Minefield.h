#ifndef MINEFIELD_H
#define MINEFIELD_H

#include "Square.h"

#include <SFML/Graphics/Drawable.hpp>

#include <vector>

namespace minesweeper {

	class Difficulty;

	class Minefield : public sf::Drawable
	{
		public:
			Minefield();
			explicit Minefield(const Difficulty & difficulty);

			bool reveal(const sf::Vector2i & mousePosition);
			void flag(const sf::Vector2i & mousePosition);
			void center(const sf::Vector2f & position);

			int flagCount() const;
			bool mineDetonated() const;
			bool finished() const;
			sf::Vector2u getSize() const;

		private:
			bool frozen;
			unsigned rows, columns;
			std::vector<Square> grid;
			sf::Vector2f gridCenter;
			
			void revealMines();
			void revealArea(unsigned row, unsigned col);
			void generateMines(unsigned minesCount);
			void placeMine(unsigned row, unsigned col);
			Square & getSquare(unsigned row, unsigned col);
			const Square & getSquare(unsigned row, unsigned col) const;
			sf::FloatRect getBounds() const;

			virtual void draw(sf::RenderTarget & target, sf::RenderStates states) const override;
	};

}

#endif // !MINEFIELD_H
