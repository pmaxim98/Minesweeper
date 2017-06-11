#ifndef DIFFICULTY_H
#define DIFFICULTY_H

namespace minesweeper {

		class Difficulty final
		{
			public:
				enum class Type : unsigned short {
					Custom,
					Easy,
					Medium,
					Hard
				};

				Difficulty();
				explicit Difficulty(Type type);
				Difficulty(unsigned rows, unsigned columns, unsigned mines);

				Type getType() const;
				unsigned getRows() const;
				unsigned getColumns() const;
				unsigned getMines() const;

			private:
				Type type;
				unsigned rows;
				unsigned columns;
				unsigned mines;
		};
}

#endif // !DIFFICULTY_H
