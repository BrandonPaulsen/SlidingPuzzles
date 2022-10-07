#include "game.hpp"
#include <cmath>
using std::abs;
#include <cstdlib>
using std::rand;

//		INITIALIZE ALL GAMES TO SOLVED STATE
game::game(int size) {
	this->size = size;
	for(int row = 0; row < size; row++) {
		board.push_back({});
		for(int col = 0; col < size; col++) {
			board.back().push_back((size*row)+col+1);
		}
	}
	emptySpace = {size-1, size-1};
	board.at(emptySpace.row).at(emptySpace.col) = 0;
}

game::game(const game& c) {
	size = c.size;
	depth = c.depth;
	heuristicValue = c.heuristicValue;
	priority = c.priority;
	board = c.board;
	emptySpace = c.emptySpace;
	parent = c.parent;
}

game& game::operator=(const game& c) {
	return *this;
}

/*
		UTILITY FUNCTIONS
*/


//	PRINT THE GAME STATE TO TERMINAL
void game::display(void) {
	for(int row = 0; row < board.size(); row++) {
		for(int col = 0; col < board.size(); col++) {
			cout << board.at(row).at(col) << "\t";
		}
		cout << endl << endl;
	}
	cout << endl << endl;
}

//	FIND A TILE IN A GAME STATE, RETURN POSITION
position game::find(int tile) {
	for(int row = 0; row < size; row++) {
		for(int col = 0; col < size; col++) {
			if(board.at(row).at(col) == tile) {
				position found = {row,col};
				return found;
			}
		}
	}
	position notFound = {-1,-1};
	return notFound;
}


//	GETTER FUNCTION
matrix game::getBoard(void) {
	return board;
}


//	TAKE USER GAME STATE INPUT
//		IF THERE IS TIME, CHECK TO SEE IF GAME STATE IS VALID
void game::enterUserState(void) {
	int input;
	for(int row = 0; row < size; row++) {
		for(int col = 0; col < size; col++) {
			cout << "ENTER TILE AT POSITION " << row << ", " << col << endl;
			cin >> input;
			board.at(row).at(col) = input;
		}
	}
	emptySpace = find(0);
}


//	RETURN STRING REPRESENTING BOARD STATE FOR USE IN UNORDERED SET TO TRACK VISITED STATES
string game::getID(void) {
	string ID = "";
	for(int row = 0; row < size; row++) {
		for(int col = 0; col < size; col++) {
			ID+= to_string(board.at(row).at(col));
			ID+= "*";
		}
	}
	return ID;
}


//	SET PARENT POINTER VALUE
void game::setParent(game* p) {
	parent = p;
}


//	RETURN GAME POINTER TO PARENT
game* game::getParent(void) {
	return parent;
}

int game::getDepth(void) {
	return depth;
}

int game::getPriority(void) {
	return priority;
}

/*
	MOVEMENT FUNCTIONS	
*/


//	CHECK BOARD POSITION TO GENERATE VALID MOVES AND RETURN VECTOR OF VALID MOVES
vector<position> game::getValidMoves(void) {
	vector<position> moves = {};
	if(-1 < emptySpace+UP && emptySpace+UP < size) {
		moves.push_back(UP);
	}
	if(-1 < emptySpace+RIGHT && emptySpace+RIGHT < size) {
		moves.push_back(RIGHT);
	}
	if(-1 < emptySpace+DOWN && emptySpace+DOWN < size) {
		moves.push_back(DOWN);
	}
	if(-1 < emptySpace+LEFT && emptySpace+LEFT < size) {
		moves.push_back(LEFT);
	}
	return moves;
};

//	MUTATE CURRENT GAME BY APPLYING CURRENT MOVE
void game::applyMove(position& move) {
	position newEmptySpace = emptySpace+move;
	board.at(emptySpace.row).at(emptySpace.col) = board.at(newEmptySpace.row).at(newEmptySpace.col);
	emptySpace = newEmptySpace;
	board.at(emptySpace.row).at(emptySpace.col) = 0;
}


//	RETURN NEW GAME THAT CORRESPONDS TO NEW STATE AFTER MOVE
game* game::getChild(position& move) {
	game* child = new game(size);
	child->size = size;
	child->depth = depth+1;
	child->board = board;
	child->emptySpace = emptySpace;
	child->parent = this;
	child->applyMove(move);
	return child;
}

//	RETURN A VECTOR OF ALL VALID CHILDREN ACCESSIBLE FROM CURRENT STATE
vector<game*> game::getChildren(void) {
	vector<game*> children = {};
	vector<position> moves = getValidMoves();
	for(position move:moves) {
		children.push_back(getChild(move));
	}
	return children;
}

//	RANDOMIZE BOARD BY MAKING RANDOM MOVES
//		USEFUL FOR TESTING HEURISTIC FUNCTIONS
void game::randomize(void) {
	for(int r = 0; r < 20*size*size; r++) {
		vector<position> moves = getValidMoves();
		applyMove(moves.at(rand()%moves.size()));
	}
}

/*
		HEURISTIC FUNCTIONS
*/

//	UNIFORM COST HEURISTIC RETURNS 0 AND CORRESPONDS TO DFS
int game::uniformCostHeuristic(game* goalState) {
	return 0;
}

//	MISPLACED TILE HEURISTIC COUNTS THE NUMBER OF TILES IN THE WRONG PLACE
//		DOES NOT COUNT 0 TO BE OUT OF PLACE NO MATTER WHAT
int game::misplacedTileHeuristic(game* goalState) {
	int misplacedTileCount = 0;
	for(int row = 0; row < size; row++) {
		for(int col = 0; col < size; col++) {
			if(board.at(row).at(col) != 0) {
				if(board.at(row).at(col) != goalState->board.at(row).at(col)) {
					misplacedTileCount++;
				}
			}
		}
	}
	return misplacedTileCount;
}

//	MANHATTAN DISTANCE HEURISTIC CALCULATES THE MANHATTAN (TAXICAB) DISTANCE BETWEEN EACH CHILD IN BOTH GAMES
//		ALWAYS AT LEAST AS GOOD IF NOT BETTER THAN THE MISPLACED TILE HEURISTIC
//		MANHATTAN DISTANCE = ABS(A.TILE.X - B.TILE.X) + ABS(A.TILE.Y - B.TILE.Y)
int game::manhattanDistanceHeuristic(game* goalState) {
	int totalManhattanDistance = 0;
	for(int row = 0; row < size; row++) {
		for(int col = 0; col < size; col++) {
			if(board.at(row).at(col) != 0) {
				position found = goalState->find(board.at(row).at(col));
				int drow = abs(row-found.row);
				int dcol = abs(col-found.col);
				totalManhattanDistance+= drow+dcol;
			}
		}
	}
	return totalManhattanDistance;
}

//	UPDATE THE HEURISTIC VALUE AND PRIORITY BASED ON DESIRED HEURISTIC
void game::updatePriority(game* goalState, string heuristic) {
	if(heuristic == "uniformCost") {
		heuristicValue = uniformCostHeuristic(goalState);
	} else if(heuristic == "misplacedTile") {
		heuristicValue = misplacedTileHeuristic(goalState);
	} else if(heuristic == "manhattanDistance") {
		heuristicValue = manhattanDistanceHeuristic(goalState);
	}
	priority = depth+heuristicValue;
}