#include "game.hpp"

vector<vector<game*>> generateSearchTree(game* root) {
	unordered_set<string> visited;
	visited.insert(root->getID());

	vector<vector<game*>> searchTree = {};
	searchTree.push_back({root});

	while(searchTree.back().size() != 0) {
		vector<game*> currLevel = searchTree.back();
		vector<game*> nextLevel = {};
		for(game* parent:currLevel) {
			parent->setChildren();
			for(game* child:parent->getChildren()) {
				if(visited.find(child->getID()) == visited.end()) {
					nextLevel.push_back(child);
					visited.insert(child->getID());
				}
			}
		}
		searchTree.push_back(nextLevel);
	}
	searchTree.pop_back();
	return searchTree;
}

game* bruteForce(game* initialState, game* goalState) {
	game* path = new game(initialState->getSize());

	vector<vector<game*>> levels = generateSearchTree(goalState);

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
	// cout << "VISITED " << visited.size() << " NODES" << endl;
	return path;
}

vector<game*> getAllMismatch() {
	vector<game*> mismatchingInitialStates;
	game* root = new game(3);
	vector<vector<game*>> searchTree = generateSearchTree(root);

	for(vector<game*> level:searchTree) {
		cout << "DEPTH: " << level.front()->getDepth() << endl;
		for(game* state:level) {
			game* misplacedTileInitialState = new game(3);
			misplacedTileInitialState->setBoard(state->getBoard());
			game* misplacedTileGoalState = new game(3);
			misplacedTileGoalState->setBoard(root->getBoard());
			string misplacedTileHeuristic = "misplacedTile";
			game* misplacedTilePath = heuristicSearch(misplacedTileInitialState, misplacedTileGoalState, misplacedTileHeuristic);

			game* manhattanDistanceInitialState = new game(3);
			manhattanDistanceInitialState->setBoard(state->getBoard());
			game* manhattanDistanceGoalState = new game(3);
			manhattanDistanceGoalState->setBoard(root->getBoard());
			string manhattanDistanceHeuristic = "manhattanDistance";
			game* manhattanDistancePath = heuristicSearch(manhattanDistanceInitialState, manhattanDistanceGoalState, manhattanDistanceHeuristic);

			if(manhattanDistancePath->getDepth() != misplacedTilePath->getDepth()) {
				mismatchingInitialStates.push_back(state);
			}
			delete misplacedTileGoalState;
			delete manhattanDistanceGoalState;
		}
	}
	return mismatchingInitialStates;
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
		displayPath(bruteForce(initialState, goalState));
		return 0;
	}

	game* path = heuristicSearch(initialState, goalState, heuristic);
	displayPath(path);
	cout << "SOLUTION DEPTH: " << path->getDepth() << endl;
}


//072461358
