#include "game.hpp"


int main() {
	srand(time(NULL));
	priority_queue<game> Q;
	game solved = game(3);
	solved.display();
	vector<game> children = solved.getChildren();
	for(game child:children) {
		child.display();
	}
	// game random = game(3);
	// random.randomize();
	// random.display();
	// cout << "UNIFORM COST:\t\t" << random.uniformCost(solved) << endl;
	// cout << "MISPLACED TILE:\t\t" << random.misplacedTile(solved) << endl;
	// cout << "MANHATTAN DISTANCE:\t" << random.manhattanDistance(solved) << endl;
}