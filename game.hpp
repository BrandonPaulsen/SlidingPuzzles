#include <vector>
using std::vector;
#include <string>
using std::string;
#include <math.h>
#include <iostream>
using std::cout;
using std::endl;
#include <queue>
using std::priority_queue;





#ifndef GAME_HPP
#define GAME_HPP

#define matrix vector<vector<int>>

struct position {
	int row = 0;
	int col = 0;
	friend bool operator==(const position& a, const position& b) {
		return (a.row == b.row) && (a.col == b.col);
	}
	friend bool operator<(int r, const position& p) {
		return (r < p.row) && (r < p.col);
	}
	friend bool operator<(const position& p, int r) {
		return (p.row < r) && (p.col < r);
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
		int depth = 0;
		int heuristic = 0;
		int priority = 0;
		matrix board = {};
		position emptySpace = {0,0};
	public:
		friend bool operator==(const game& a, const game& b) {
			return (a.size == b.size) && (a.board == b.board);
		}

		friend bool operator<(const game& a, const game& b) {
			return a.priority < b.priority;
		}

		game(int size);
		void display(void);
		vector<position> getValidMoves(void);
		void applyMove(position& move);
		game getChild(position& move);
		vector<game> getChildren(void);
		position find(int tile);
		string getHash(void);
		void randomize(void);
		int uniformCost(game& compGame);
		int misplacedTile(game& compGame);
		int manhattanDistance(game& compGame);
};

#endif