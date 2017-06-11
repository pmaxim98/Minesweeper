#include "Game.h"

namespace minesweeper {

	Game::Game()
		:
		difficulty(Difficulty::Type::Medium),
		minefield(difficulty),
		gui(minefield, difficulty)
	{
		minefield.center(gui.getCenter());
	}

	void Game::run()
	{
		while (gui.window.isOpen())
		{
			sf::Event event;
			while (gui.window.pollEvent(event) && gui.window.hasFocus())
			{
				if (event.type == sf::Event::Closed)
					gui.window.close();

				if (event.type == sf::Event::MouseButtonPressed && !gui.get("difficultyWindow")->isVisible()) {
					if (event.mouseButton.button == sf::Mouse::Left) {
						if (minefield.reveal({event.mouseButton.x, event.mouseButton.y}))
							if (!minefield.mineDetonated())
								gui.showWinImage();
					}
					else if (event.mouseButton.button == sf::Mouse::Right)
						minefield.flag({event.mouseButton.x, event.mouseButton.y});
				}
				
				gui.handleEvent(event);
			}

			if (!minefield.finished()) {
				gui.updateClock();
				gui.updateFlagsCounter(minefield.flagCount());
			}

			gui.window.clear(sf::Color::White);
			gui.window.draw(minefield);
			gui.draw();
			gui.window.display();
		}
	}

}