#ifndef GUI_H
#define GUI_H

#include <TGUI/Gui.hpp>
#include <SFML/Graphics/RenderWindow.hpp>

namespace minesweeper {

	class Minefield;
	class Difficulty;

	class Gui : public tgui::Gui
	{
		public:
			Gui() = delete;
			Gui(Minefield & minefield, Difficulty & difficulty);

			void updateFlagsCounter(int flags);
			void updateClock();
			void showWinImage();
			sf::Vector2f getCenter() const;

			sf::RenderWindow window;
		private:
			sf::Clock clock;
	};

}

#endif // !GUI_H
