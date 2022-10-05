#include "game.hpp"

int main() {
	game test = game(3);
	test.display();
	vector<position> moves = test.getValidMoves();
	cout << "VALID MOVES:" << endl;
	for(position move:moves) {
		if(move == UP) {
			cout << "\tUP" << endl;
		} else if(move == RIGHT) {
			cout << "\tRIGHT" << endl;
		} else if(move == DOWN) {
			cout << "\tDOWN" << endl;
		} else if(move == LEFT) {
			cout << "\tLEFT" << endl;
		} else {
			cout << "INVALID MOVE" << endl;
		}
	}
}