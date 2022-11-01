#include <vector>
using std::vector;
#include <string>
using std::string;
using std::to_string;
#include <math.h>
#include <iostream>
using std::cout;
using std::cin;
using std::endl;
#include <fstream>
using std::ofstream;
#include <queue>
using std::priority_queue;
#include <unordered_set>
using std::unordered_set;





#ifndef GAME_HPP
#define GAME_HPP

//vectors of vectors are matrices, and are easier to work with than single vectors
#define matrix vector<vector<int>>

//struct to keep track of poisitions and directions
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

//directions
const position UP = {-1,0};
const position RIGHT = {0,1};
const position DOWN = {1,0};
const position LEFT = {0,-1};

//game class
class game {
	private:
		//keep track of data important to the game class
		int size = 0;
		int depth = 0;
		int heuristicValue = 0;
		int priority = 0;
		bool visited = false;
		matrix board = {};
		position emptySpace = {0,0};
		game* parent;
		vector<game*> children;
	public:
		//comparators to tell when we have found the goal state
		friend bool operator==(const game& a, const game& b) {
			return (a.size == b.size) && (a.board == b.board);
		}
		friend bool operator!=(const game& a, const game& b) {
			return !(a==b);
		}

		//constructors and destructors
		game(int size);
		game(const game& c);
		~game(void);
		game& operator=(const game& c);
		/*
				UTILITY FUNCTIONS
		*/
		void display(void);
		position find(int tile);
		void enterUserState(void);
		/*
			GETTERS
		*/
		int getSize(void) {
			return size;
		}
		int getDepth(void) {
			return depth;
		}
		int getHeuristicValue(void) {
			return heuristicValue;
		}
		int getPriority(void) {
			return priority;
		}
		bool getVisited(void) {
			return visited;
		}
		matrix getBoard(void) {
			return board;
		}
		position getEmptySpace(void) {
			return emptySpace;
		}
		game* getParent(void) {
			return parent;
		}
		//use id because pointers cannot be inserted into unordered_set
		string getID(void) {
				string ID = "";
				for(int row = 0; row < size; row++) {
					for(int col = 0; col < size; col++) {
						ID+= to_string(board.at(row).at(col));
						ID+= "*";
					}
				}
				return ID;
		}
		vector<game*> getChildren(void) {
			return children;
		}
		/*
			SETTERS
		*/
		void setSize(int s) {
			size = s;
		}
		void setDepth(int d) {
			depth = d;
		}
		void setHeuristicValue(int h) {
			heuristicValue = h;
		}
		void setPriority(int p) {
			priority = p;
		}
		void setVisited(bool v) {
			visited = v;
		} 
		void setBoard(matrix b) {
			board = b;
		}
		void setEmptySpace(position es) {
			emptySpace = es;
		}
		void setParent(game* p) {
			parent = p;
		}
		void setChildren(void) {
			vector<position> moves = getValidMoves();
			for(position move:moves) {
				children.push_back(getChild(move));
			}
		}
		/*
			MOVEMENT FUNCTIONS	
		*/
		vector<position> getValidMoves(void);
		void applyMove(position& move);
		game* getChild(position& move);
		void randomize(void);
		/*
				HEURISTIC FUNCTIONS
		*/
		int uniformCostHeuristic(game* goalState);
		int misplacedTileHeuristic(game* goalState);
		int manhattanDistanceHeuristic(game* goalState);
		void updatePriority(game* goalState, string heuristic);
};

#endif