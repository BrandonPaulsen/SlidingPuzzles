#include "game.hpp"

int main() {
	srand(time(NULL));
	priority_queue<game> Q;
	unordered_set<string> states;
	game solved = game(3);
	states.insert(solved.getID());

	vector<game> currentLevel = {};
	vector<game> nextLevel = {solved};
	int depth = -1;

	while(nextLevel.size() != 0) {
		currentLevel = nextLevel;
		nextLevel = {};
		depth++;
		cout << "NUMBER OF STATES AT DEPTH " << depth << ": " << currentLevel.size() << endl;
		for(game parent:currentLevel) {
			vector<game> children = parent.getChildren();
			for(game child:children) {
				if(states.find(child.getID()) == states.end()) {
					nextLevel.push_back(child);
					states.insert(child.getID());
				}
			}
		}
	}
}