#include "game.hpp"

int main() {
	cout << "What size game would you like to solve?" << endl;
	int size = 0;
	cin >> size;
	game initialState = game(size);
	game finalState = game(size);


	cout << "Would you like to pick a puzzle state (1) or use a random one (2)?" << endl;
	int stateInt = 0;
	cin >> stateInt;
	if(stateInt == 1) {
		initialState.enterUserState();
	} else if(stateInt == 2) {
		initialState.randomize();
	}

	cout << "Would you like to use Uniform Cost Heuristic (1), Misplaced Tile Heuristic (2), or Manhattan Distance Heuristic (3)?" << endl;





	// srand(time(NULL));
	// priority_queue<game> Q;
	// unordered_set<string> states;
	// game solved = game(3);
	// states.insert(solved.getID());

	// vector<game> currentLevel = {};
	// vector<game> nextLevel = {solved};
	// int depth = -1;

	// while(nextLevel.size() != 0) {
	// 	currentLevel = nextLevel;
	// 	nextLevel = {};
	// 	depth++;
	// 	cout << "NUMBER OF STATES AT DEPTH " << depth << ": " << currentLevel.size() << endl;
	// 	for(game parent:currentLevel) {
	// 		vector<game> children = parent.getChildren();
	// 		for(game child:children) {
	// 			if(states.find(child.getID()) == states.end()) {
	// 				nextLevel.push_back(child);
	// 				states.insert(child.getID());
	// 			}
	// 		}
	// 	}
	// }
}