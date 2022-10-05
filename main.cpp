#include "game.hpp"

int main() {
	game test = game(3);
	test.display();
	vector<game> children = test.getChildren();
	for(game child:children) {
		child.display();
	}
}