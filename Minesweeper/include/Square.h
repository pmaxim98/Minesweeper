#ifndef SQUARE_H
#define SQUARE_H

namespace minesweeper {

	class Square
	{
		public:
			enum Type : unsigned char {
				Empty,
				Revealed = 1 << 4,
				Flagged = 1 << 5,
				Mined = 1 << 6,
				Detonated = 1 << 7
			};

			Square();
			explicit Square(Square::Type type);

			void toggle(Square::Type type);
			void increment();

			bool is(Square::Type type) const;
			int isNumber() const;

		private:
			unsigned char value;
	};

}

#endif // !SQUARE_H
