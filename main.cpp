#include "game.hpp"

int main() {
	srand(time(NULL));
	cout << "What size game would you like to solve?" << endl;
	int size = 0;
	cin >> size;
	game* initialState = new game(size);
	game* goalState = new game(size);
	goalState->setParent(nullptr);


	cout << "Would you like to pick a puzzle state (1) or use a random one (2)?" << endl;
	int stateInt = 0;
	cin >> stateInt;
	if(stateInt == 1) {
		initialState->enterUserState();
	} else if(stateInt == 2) {
		initialState->randomize();
	}

	cout << "INITIAL STATE:" << endl;
	initialState->display();
	cout << "GOAL STATE:" << endl;
	goalState->display();

	cout << "Would you like to use Uniform Cost Heuristic (1), Misplaced Tile Heuristic (2), or Manhattan Distance Heuristic (3)?" << endl;
	int heuristicInt = 0;
	string heuristic = "";
	cin>> heuristicInt;
	if(heuristicInt == 0) {
		heuristic = "uniformCost";
	} else if(heuristicInt == 1)  {
		heuristic = "misplacedTile";
	} else if(heuristicInt == 2) {
		heuristic = "manhattanDistance";
	}

	unordered_set<string> visited;
	visited.insert(goalState->getID());

	vector<vector<game*>> levels = {};
	levels.push_back({goalState});

	while(levels.back().size() != 0) {
		vector<game*> currLevel = levels.back();
		vector<game*> nextLevel = {};
		for(game* parent:currLevel) {
			vector<game*> children = parent->getChildren();
			for(game* child:children) {
				if(visited.find(child->getID()) == visited.end()) {
					nextLevel.push_back(child);
					visited.insert(child->getID());
				}
			}
		}
		levels.push_back(nextLevel);
	}
	levels.pop_back();

	for(vector<game*> level:levels) {
		for(game* state:level) {
			if(*state == *initialState) {
				cout << "PATH FOUND AT DEPTH " << state->getDepth() << endl;
				while(state != nullptr) {
					state->display();
					state = state->getParent();
				}
			}
		}
	}
}