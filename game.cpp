#include "game.hpp"


//		INITIALIZE ALL GAMES TO SOLVED STATE
game::game(int size) {
	this->size = size;
	for(int row = 0; row < size; row++) {
		board.push_back({});
		for(int col = 0; col < size; col++) {
			board.back().push_back((size*row)+col+1);
		}
		emptySpace = {size-1, size-1};
		board.at(emptySpace.row).at(emptySpace.col) = 0;
	}
}