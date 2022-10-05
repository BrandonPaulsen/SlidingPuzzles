#include "game.hpp"
#include <iostream>
using std::cout;
using std::endl;


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