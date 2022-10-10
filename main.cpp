#include "game.hpp"

game* bruteForce(game* initialState, game* goalState) {
	game* path = new game(initialState->getSize());

	unordered_set<string> visited;
	visited.insert(goalState->getID());

	vector<vector<game*>> levels = {};
	levels.push_back({goalState});

	while(levels.back().size() != 0) {
		vector<game*> currLevel = levels.back();
		vector<game*> nextLevel = {};
		for(game* parent:currLevel) {
			parent->setChildren();
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

	int depth = 0;
	for(vector<game*> level:levels) {
		for(game* state:level) {
			if(*state == *initialState) {
				path = state;
				break;
			}
		}
	}
	return path;
}

game* heuristicSearch(game* initialState, game* goalState, string& heuristic) {
	game* path = new game(initialState->getSize());

	auto compare = [](game* a, game* b) {
		return a->getPriority() > b->getPriority();
	};

	unordered_set<string> visited;
	visited.insert(goalState->getID());
	priority_queue<game*, vector<game*>, decltype(compare)> Q(compare);
	Q.push(goalState);

	while(!Q.empty()) {
		game* currState = Q.top();
		Q.pop();
		if(*currState == *initialState) {
			path = currState;
			break;
		} else {
			currState->setChildren();
			vector<game*> children = currState->getChildren();
			for(game* child:children) {
				if(visited.find(child->getID()) == visited.end()) {
					child->updatePriority(initialState, heuristic);
					visited.insert(child->getID());
					Q.push(child);
				}
			}
		}
	}
	cout << "VISITED " << visited.size() << " NODES" << endl;
	return path;
}

void displayPath(game* path) {
	while(path->getParent() != nullptr) {
		path->display();
		path = path->getParent();
	}
	path->display();
}


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
	cout << "INITIAL STATE HEURISTICS: " << endl;
	cout << "\tUNIFORM COST: " << 0 << endl;
	cout << "\tMISPLACED TILE: " << initialState->misplacedTileHeuristic(goalState) << endl;
	cout << "\tMANHATTAN DISTANCE: " << initialState->manhattanDistanceHeuristic(goalState) << endl;
	cout << "GOAL STATE:" << endl;
	goalState->display();

	cout << "Would you like to use Uniform Cost Heuristic (1), Misplaced Tile Heuristic (2), Manhattan Distance Heuristic (3), or Brute Force (4)?" << endl;
	int heuristicInt = 0;
	string heuristic = "";
	cin>> heuristicInt;
	if(heuristicInt == 1) {
		heuristic = "uniformCost";
	} else if(heuristicInt == 2)  {
		heuristic = "misplacedTile";
	} else if(heuristicInt == 3) {
		heuristic = "manhattanDistance";
	} else if(heuristicInt == 4) {
		bruteForce(initialState, goalState);
		return 0;
	}

	game* path = heuristicSearch(initialState, goalState, heuristic);
	displayPath(path);
	cout << "SOLUTION DEPTH: " << path->getDepth() << endl;


	// string heuristic = "manhattanDistance";

	// game* bruteForcePath = bruteForce(initialState, goalState);
	// cout << "BRUTE FORCE PATH DEPTH: " << bruteForcePath->getDepth() << endl;
	// cout << "MANHATTAN DISTANCE PATH DEPTH: " << manhattanDistancePath->getDepth() << endl;
	// while(bruteForcePath != nullptr && manhattanDistancePath != nullptr) {
	// 	if(*bruteForcePath == *manhattanDistancePath) {
	// 		bruteForcePath->display();
	// 	} else {
	// 		cout << "PATHS DIVERGE AT DEPTH " << bruteForcePath->getDepth() << endl;
	// 		cout << "BRUTE FORCE PATH: " << endl;
	// 		bruteForcePath->display();
	// 		cout << "MANHATTAN DISTANCE PATH: " << endl;
	// 		manhattanDistancePath->display();
	// 		break;
	// 	}
	// 	bruteForcePath = bruteForcePath->getParent();
	// 	manhattanDistancePath = manhattanDistancePath->getParent();
	// }

}


//072461358

/*
STATES FOR WHICH MISPLACED TILE AND MANHATTAN DISTANCE DIVERGE:
	0	7	2

	4	6	1

	3	5	8

	

	1	8	6

	3	2	5

	4	7	0



	6	7	5	

	1	8	3

	0	2	4



	0	3	7

	5	2	6

	1	8	4
*/