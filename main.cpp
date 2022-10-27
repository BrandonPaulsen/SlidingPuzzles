#include "game.hpp"

struct searchResult {
	game* path;
	int depth;
	int visitedStates;
	int frontierSize;
	searchResult(int d, int v, int f, game* p) {
		depth = d;
		visitedStates = v;
		frontierSize = f;
		path = p;
	}
};

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

searchResult heuristicSearch(game* initialState, game* goalState, string& heuristic) {
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
			visited.insert(currState->getID());
			for(game* child:children) {
				if(visited.find(child->getID()) == visited.end()) {
					child->updatePriority(initialState, heuristic);
					Q.push(child);
				}
			}
		}
	}

	searchResult res(path->getDepth(), visited.size(), Q.size(), path);
	return res;
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

	cout << "What would you like to do?" << endl;
	cout << "\t1: Solve with Brute Force" << endl;
	cout << "\t2: Solve with Uniform Cost Heuristic" << endl;
	cout << "\t3: Solve with Misplaced Tile Heuristic" << endl;
	cout << "\t4: Solve with Manhattan Tile Heuristic" << endl;
	cout << "\t5: Compare Misplaced Tile Heuristic solution and Manhattan Distance Heuristic Solution" << endl;
	cout << "\t6: Approximate solution statistics per depth" << endl;

	int action = 0;
	string heuristic = "";
	cin >> action;

	if(action == 1) {
		displayPath(bruteForce(initialState, goalState));
	} else if(action == 2)  {
		heuristic = "uniformCost";
	} else if(action == 3) {
		heuristic = "misplacedTile";
	} else if(action == 4) {
		heuristic = "manhattanDistance";
		return 0;
	} else if(action == 5) {
		string manhattanDistanceHeuristic = "manhattanDistance";
		searchResult manhattanDistanceResult = heuristicSearch(initialState, goalState, manhattanDistanceHeuristic);

		string misplacedTileHeuristic = "misplacedTile";
		searchResult misplacedTileResult = heuristicSearch(initialState, goalState, misplacedTileHeuristic);

		if(manhattanDistanceResult.depth != misplacedTileResult.depth) {
			cout << "MISPLACED TILE PATH:" << endl;
			displayPath(misplacedTileResult.path);
			cout << "MANHATTAN DISTANCE PATH:" << endl;
			displayPath(manhattanDistanceResult.path);
			cout << "MISPLACED TILE SOLUTION DEPTH AT: " << misplacedTileResult.depth << endl;
			cout << "MISPLACED TILE VISITED STATES: " << misplacedTileResult.visitedStates << endl;
			cout << "MISPLACED TILE FRONTIER SIZE: " << misplacedTileResult.frontierSize << endl;
			cout << "MANHATTAN DISTANCE SOLUTION DEPTH AT: " << manhattanDistanceResult.depth << endl;
			cout << "MANHATTAN DISTANCE VISITED STATES: " << manhattanDistanceResult.visitedStates << endl;
			cout << "MANHATTAN DISTANCE FRONTIER SIZE: " << manhattanDistanceResult.frontierSize << endl;
		} else {
			cout << "SOLUTIONS AT THE SAME DEPTH (" << manhattanDistanceResult.depth << ")" << endl;
		}
		return 0;
	} else if(action == 6) {
		cout << "What heuristic would you like to use?" << endl;
		cout << "\t1: Uniform Cost" << endl;
		cout << "\t2: Misplaced Tile" << endl;
		cout << "\t3: Manhattan Distance" << endl;
		int heuristicInt = 0;
		cin >> heuristicInt;

		if(heuristicInt == 1) {
			heuristic = "uniformCost";
		} else if(heuristicInt == 2) {
			heuristic = "misplacedTile";
		} else if(heuristicInt == 3) {
			heuristic = "manhattanDistance";
		} else {
			return 0;
		}

		cout << "How many randomized trials would you like to run?" << endl;
		int trials = 0;
		cin >> trials;

		vector<int> totalVisitedStates = {};
		vector<int> totalFrontierSize = {};
		vector<int> depthCount = {};

		for(int i = 0; i < trials; i++) {
			cout << "RUNNING TRIAL " << i+1 << endl;
			game* initialState = new game(size);
			initialState->randomize();

			game* goalState = new game(size);
			goalState->setParent(nullptr);

			searchResult result = heuristicSearch(initialState, goalState, heuristic);

			while(result.depth >= depthCount.size()) {
				totalVisitedStates.push_back(0);
				totalFrontierSize.push_back(0);
				depthCount.push_back(0);
			}

			totalVisitedStates.at(result.depth) += result.visitedStates;
			totalFrontierSize.at(result.depth) += result.frontierSize;
			depthCount.at(result.depth)++;
		}

		for(int depth = 0; depth < depthCount.size(); depth++) {
			cout << "Depth " << depth << endl;
			if(depthCount.at(depth) == 0) {
				cout << "\tNo Solutions" << endl;
			} else {
				cout << "\tNumber of Solutions at Depth: " << depthCount.at(depth) << endl;
				cout << "\tAverage Visited States: " << totalVisitedStates.at(depth) / depthCount.at(depth) << endl;
				cout << "\tAverage Frontier Size: " << totalFrontierSize.at(depth) / depthCount.at(depth) << endl;
			}
		}


		return 0;
	}

	searchResult result = heuristicSearch(initialState, goalState, heuristic);
	displayPath(result.path);
	cout << "SOLUTION DEPTH: " << result.depth << endl;
	cout << "NUMBER OF VISITED STATES: " << result.visitedStates << endl;
	cout << "NUMBER OF FRONTIER NODES: " << result.frontierSize << endl;
}


//072461358
