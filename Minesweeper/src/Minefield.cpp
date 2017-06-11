#include "Minefield.h"

#include "ResourceManager.h"
#include "Difficulty.h"
#include "MathUtils.h"

#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/VertexArray.hpp>

#include <algorithm>
#include <queue>
#include <random>
#include <map>

namespace minesweeper {

	namespace {
		const unsigned squareSize = 25;
	}

	Minefield::Minefield()
		:
		Minefield({})
	{ }

	Minefield::Minefield(const Difficulty & difficulty)
		:
		rows(difficulty.getRows()),
		columns(difficulty.getColumns()),
		grid(rows * columns),
		gridCenter(static_cast<sf::Vector2f>(getSize()) * 0.5f),
		frozen(false)
	{
		generateMines(difficulty.getMines());
	}

	bool Minefield::reveal(const sf::Vector2i & position)
	{
		if (frozen) 
			return false;

		const sf::Vector2f mousePos(static_cast<sf::Vector2f>(position));
		const sf::FloatRect gridBounds = getBounds();

		if (gridBounds.contains(mousePos)) {
			const sf::Vector2f relativeToOrigin = sf::Vector2f{mousePos.x - gridBounds.left, mousePos.y - gridBounds.top};
			const sf::Vector2u squareIndexes = {static_cast<unsigned>(relativeToOrigin.y / squareSize), static_cast<unsigned>(relativeToOrigin.x / squareSize)};
			const Square square = getSquare(squareIndexes.x, squareIndexes.y);

			if (!square.is(Square::Revealed) && !square.is(Square::Flagged)) {
				revealArea(squareIndexes.x, squareIndexes.y);

				if (finished()) {
					revealMines();
					return (frozen = true);
				}
			}
		}

		return false;
	}

	void Minefield::flag(const sf::Vector2i & position)
	{
		if (frozen) return;

		const sf::Vector2f mousePos(static_cast<sf::Vector2f>(position));
		const sf::FloatRect gridBounds = getBounds();

		if (gridBounds.contains(mousePos)) {
			const sf::Vector2f relativeToOrigin = sf::Vector2f{mousePos.x - gridBounds.left, mousePos.y - gridBounds.top};
			const sf::Vector2u squareIndexes = {static_cast<unsigned>(relativeToOrigin.y / squareSize), static_cast<unsigned>(relativeToOrigin.x / squareSize)};
			Square & square = getSquare(squareIndexes.x, squareIndexes.y);

			if (!square.is(Square::Revealed))
				square.toggle(Square::Flagged);
		}
	}

	void Minefield::revealArea(unsigned row, unsigned col)
	{
		if (getSquare(row, col).is(Square::Mined)) {
			getSquare(row, col).toggle(Square::Detonated);
			return;
		}

		getSquare(row, col).toggle(Square::Revealed);
		std::queue<sf::Vector2u> indexes;
		indexes.push({row, col});

		while (!indexes.empty()) {
			const auto first = static_cast<sf::Vector2i>(indexes.front());
			indexes.pop();

			if (getSquare(first.x, first.y).isNumber())
				continue;

			for (const auto & dir : math::directions) {
				if (first.x + dir.x < 0 || first.y + dir.y < 0 || first.x + dir.x >= static_cast<int>(rows) || first.y + dir.y >= static_cast<int>(columns)) 
					continue;

				const sf::Vector2u nextSquareIndexes = {static_cast<unsigned>(first.x + dir.x), static_cast<unsigned>(first.y + dir.y)};
				Square & nextSquare = getSquare(nextSquareIndexes.x, nextSquareIndexes.y);

				if (!nextSquare.is(Square::Mined) && !nextSquare.is(Square::Revealed)) {
					nextSquare.toggle(Square::Revealed);
					indexes.push(nextSquareIndexes);
				}
			}
		}
	}

	int Minefield::flagCount() const
	{
		return std::count_if(grid.begin(), grid.end(), [] (const Square & square) { return square.is(Square::Mined); })
		- std::count_if(grid.begin(), grid.end(), [] (const Square & square) { return square.is(Square::Flagged); });
	}

	bool Minefield::mineDetonated() const
	{
		return std::find_if(grid.begin(), grid.end(), [] (const Square & square) { return square.is(Square::Detonated); }) != grid.end();
	}

	bool Minefield::finished() const
	{
		if (mineDetonated()) return true;
		
		return grid.size() == 
			std::count_if(grid.begin(), grid.end(), [] (const Square & square) { return square.is(Square::Revealed) || square.is(Square::Mined); });
	}

	sf::Vector2u Minefield::getSize() const
	{
		return {squareSize * columns, squareSize * rows};
	}

	void Minefield::revealMines()
	{
		for (Square & square : grid)
			if (!square.is(Square::Flagged) && square.is(Square::Mined)) 
				square.toggle(Square::Revealed);
	}

	void Minefield::generateMines(unsigned minesCount)
	{
		unsigned randomRow = 0;
		unsigned randomCol = 0;

		while (minesCount) {
			do {
				randomRow = math::genRandom(0, rows - 1);
				randomCol = math::genRandom(0, columns - 1);
			} while (getSquare(randomRow, randomCol).is(Square::Mined));

			placeMine(randomRow, randomCol);
			--minesCount;
		}
	}

	void Minefield::placeMine(unsigned row, unsigned col)
	{
		getSquare(row, col).toggle(Square::Mined);

		const int initialRow = static_cast<int>(row);
		const int initialCol = static_cast<int>(col);

		for (const auto & dir : math::directions) {
			if (initialCol + dir.x < 0 || initialRow + dir.y < 0 || initialCol + dir.x >= static_cast<int>(columns) || initialRow + dir.y >= static_cast<int>(rows)) 
				continue;

			Square & square = getSquare(row + dir.y, col + dir.x);
			if (!square.is(Square::Mined))
				square.increment();
		}
	}

	Square & Minefield::getSquare(unsigned row, unsigned col)
	{
		return grid[row * columns + col];
	}

	const Square & Minefield::getSquare(unsigned row, unsigned col) const
	{
		return grid[row * columns + col];
	}

	void Minefield::center(const sf::Vector2f & position)
	{
		gridCenter = position;
	}

	sf::FloatRect Minefield::getBounds() const
	{
		const sf::Vector2f size(static_cast<float>(columns), static_cast<float>(rows));
		return {gridCenter - 0.5f * static_cast<float>(squareSize) * size, static_cast<float>(squareSize) * size};
	}

	void Minefield::draw(sf::RenderTarget & target, sf::RenderStates states) const
	{
		states.texture = &ResourceManager::getInstance().getTexture("Squares");
		const float texSquareSize = static_cast<float>(states.texture->getSize().y);
		sf::VertexArray vertices(sf::Quads, rows * columns * 4);

		for (unsigned index = 0; index < vertices.getVertexCount(); ++index) {
			const unsigned row = index / 4 / columns;
			const unsigned col = index / 4 % columns;
			const int firstQuad = index % 4 == 2 || index % 4 == 3;
			const int secondQuad = index % 4 == 1 || index % 4 == 2;
			const Square & square = getSquare(row, col);

			vertices[index].position = sf::Vector2f(std::ceil(getBounds().left), std::ceil(getBounds().top)) + 
				sf::Vector2f(static_cast<float>(squareSize * (col + firstQuad)), static_cast<float>(squareSize * (row + secondQuad)));

			if (!square.is(Square::Revealed)) {
				if (square.is(Square::Flagged))
					vertices[index].texCoords = sf::Vector2f(texSquareSize * 12 + (texSquareSize * firstQuad), (texSquareSize * secondQuad));
				else
					vertices[index].texCoords = sf::Vector2f(texSquareSize * 11 + (texSquareSize * firstQuad), (texSquareSize * secondQuad));
			}
			else {
				if (square.is(Square::Detonated))
					vertices[index].texCoords = sf::Vector2f(texSquareSize * 10 + (texSquareSize * firstQuad), (texSquareSize * secondQuad));
				else if (square.is(Square::Mined))
					vertices[index].texCoords = sf::Vector2f(texSquareSize * 9 + (texSquareSize * firstQuad), (texSquareSize * secondQuad));
				else
					vertices[index].texCoords = sf::Vector2f(texSquareSize * square.isNumber() + (texSquareSize * firstQuad), (texSquareSize * secondQuad));
			}
		}

		target.draw(vertices, states);
	}
}