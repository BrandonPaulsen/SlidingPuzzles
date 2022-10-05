#include "game.hpp"

int main() {
	priority_queue<game> Q;
	game test = game(3);
	Q.push(test);
	vector<game> children = test.getChildren();
	for(game child:children) {
		Q.push(child);
	}
	while(!Q.empty()) {
		game curr = Q.top();
		curr.display();
		Q.pop();
	}
}