#ifndef GAME_HPP
#define GAME_HPP

#include <vector>
using std::vector;

#define matrix vector<vector<int>>

struct position {
	int row = 0;
	int col = 0;
	friend bool operator==(const position& a, const position& b) {
		return (a.row == b.row) && (a.col == b.col);
	}
	position operator+(const position& d) {
		position p;
		p.row = this->row + d.row;
		p.col = this->col + d.col;
		return p;
	}
};

const position UP = {-1,0};
const position RIGHT = {0,1};
const position DOWN = {1,0};
const position LEFT = {0,-1};

class game {
	private:
		int size = 0;
		matrix board = {{}};
		position emptySpace = {0,0};
	public:
		game(int size);
		friend bool operator==(const game& a, const game& b) {
			return (a.size == b.size) && (a.board == b.board);
		}
};

#endif