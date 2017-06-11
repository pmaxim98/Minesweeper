#ifndef GAME_H
#define GAME_H

#include "Gui.h"
#include "Minefield.h"
#include "Difficulty.h"

namespace minesweeper {

	class Game final
	{
		public:
			Game();
			
			void run();

		private:
			Difficulty difficulty;
			Minefield minefield;
			Gui gui;
	};

}

#endif // ! GAME_H

