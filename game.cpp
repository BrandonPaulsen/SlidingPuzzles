#include "game.hpp"


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

void game::display(void) { 
	for(int row = 0; row < board.size(); row++) {
		for(int col = 0; col < board.size(); col++) {
			cout << board.at(row).at(col) << "\t";
		}
		cout << endl << endl;
	}
	cout << endl << endl;
};

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

void game::applyMove(position move) {

}

vector<game> game::getChildren(void) {
	vector<game> children = {};
	return children;
}
