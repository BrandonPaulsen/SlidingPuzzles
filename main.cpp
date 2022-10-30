#include "game.hpp"
#include <chrono>
using namespace std::chrono;

struct searchResult {
	game* path;
	int depth;
	int visitedStates;
	int frontierSize;
	int searchTime;
	searchResult(int d, int v, int f, int t, game* p) {
		depth = d;
		visitedStates = v;
		frontierSize = f;
		searchTime = t;
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
	milliseconds start = duration_cast<milliseconds>(system_clock::now().time_since_epoch());

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

	milliseconds end = duration_cast<milliseconds>(system_clock::now().time_since_epoch());
	int duration = end.count()-start.count();

	searchResult res(path->getDepth(), visited.size(), Q.size(), duration, path);
	return res;
}

void displayPath(game* path) {
	while(path->getParent() != nullptr) {
		path->display();
		path = path->getParent();
	}
	path->display();
}

void monteCarlo(int size, int simulations, string& heuristic) {
	vector<int> numDepth = {};
	vector<int> numVisited = {};
	vector<int> numFrontier = {};
	vector<int> numTime = {};
	while(simulations > 0) {
		game* initialState = new game(size);
		initialState->randomize();
		game* goalState = new game(size);
		searchResult res = heuristicSearch(initialState, goalState, heuristic);
		while(numDepth.size() <= res.depth) {
			numDepth.push_back(0);
			numVisited.push_back(0);
			numFrontier.push_back(0);
			numTime.push_back(0);
		}
		delete goalState;
		numDepth.at(res.depth)++;
		numVisited.at(res.depth) += res.visitedStates;
		numFrontier.at(res.depth) += res.frontierSize;
		numTime.at(res.depth) += res.searchTime;
		simulations--;
	}
	vector<double> averageVisited = {};
	vector<double> averageFrontier = {};
	vector<double> averageTime = {};
	for(int d = 0; d < numDepth.size(); d++) {
		while(averageVisited.size() <= d) {
			averageVisited.push_back(0);
			averageFrontier.push_back(0);
			averageTime.push_back(0);
		}
		averageVisited.at(d) = (double)numVisited.at(d)/(double)numDepth.at(d);
		averageFrontier.at(d) = (double)numFrontier.at(d)/(double)numDepth.at(d);
		averageTime.at(d) = (double)numTime.at(d)/(double)numDepth.at(d);
	}
	cout << "DEPTH:" << endl;
	cout << "[";
	for(int d = 0; d < numDepth.size(); d++) {
		if(numDepth.at(d) != 0) {
			cout << d;
			if(d != numDepth.size()-1) {
				cout << ", ";
			}
		}
	}
	cout << "]" << endl;
	cout << "VISITED:" << endl;
	cout << "[";
	for(int d = 0; d < numDepth.size(); d++) {
		if(numDepth.at(d) != 0) {
			cout  << averageVisited.at(d);
			if(d != numDepth.size()-1) {
				cout << ", ";
			}
		}
	}
	cout << "]" << endl;
	cout << "FRONTIER:" << endl;
	cout << "[";
	for(int d = 0; d < numDepth.size(); d++) {
		if(numDepth.at(d) != 0) {
			cout << averageFrontier.at(d);
			if(d != numDepth.size()-1) {
				cout << ", ";
			}
		}
	}
	cout << "]" << endl;
	cout << "TIME:" << endl;
	cout << "[";
	for(int d = 0; d < numDepth.size(); d++) {
		if(numDepth.at(d) != 0) {
			cout << averageTime.at(d);
			if(d != numDepth.size()-1) {
				cout << ", ";
			}
		}
	}
	cout << "]" << endl;
}

void testCases(string& heuristic) {
	vector<int> depth = {0,2,4,8,12,16,20,24};
	vector<int> numVisited = {};
	vector<int> numFrontier = {};
	vector<int> numTime = {};
	vector<game*> testCases = {};
	game* test0 = new game(3);
	testCases.push_back(test0);
	game* test1 = new game(3);
	test1->setBoard({{1,2,3},{4,5,6},{0,7,8}});
	testCases.push_back(test1);
	game* test2 = new game(3);
	test2->setBoard({{1,2,3},{5,0,6},{4,7,8}});
	testCases.push_back(test2);
	game* test3 = new game(3);
	test3->setBoard({{1,3,6},{5,0,2},{4,7,8}});
	testCases.push_back(test3);
	game* test4 = new game(3);
	test4->setBoard({{1,3,6},{5,0,7},{4,8,2}});
	testCases.push_back(test4);
	game* test5 = new game(3);
	test5->setBoard({{1,6,7},{5,0,3},{4,8,2}});
	testCases.push_back(test5);
	game* test6 = new game(3);
	test6->setBoard({{7,1,2},{4,8,5},{6,3,0}});
	testCases.push_back(test6);
	game* test7 = new game(3);
	test7->setBoard({{0,7,2},{4,6,1},{3,5,8}});
	testCases.push_back(test7);
	int currTest = 0;
	for(game* test:testCases) {
		cout << "RUNNING TEST " << currTest << endl;
		game* solved = new game(3);
		searchResult res = heuristicSearch(test,solved,heuristic);
		numVisited.push_back(res.visitedStates);
		numFrontier.push_back(res.frontierSize);
		numTime.push_back(res.searchTime);
		cout << "\tFINISHED TEST" << endl;
		currTest++;
	}
	cout << "FINSISHED RUNNING ALL TESTS" << endl;
	cout << "DEPTH:" << endl;
	cout << "[";
	for(int d:depth) {
		cout << d;
		if(d != depth.back()) {
			cout << ", ";
		}
	}
	cout << "]" << endl;
	cout << "VISITED:" << endl;
	cout << "[";
	for(int visited:numVisited) {
		cout << visited;
		if(visited != numVisited.back()) {
			cout << ", ";
		}
	}
	cout << "]" << endl;
	cout << "FRONTIER:" << endl;
	cout << "[";
	for(int frontier:numFrontier) {
		cout << frontier;
		if(frontier != numFrontier.back()) {
			cout << ", ";
		}
	}
	cout << "]" << endl;
	cout << "TIME:" << endl;
	cout << "[";
	for(int t:numTime) {
		cout << t;
		if(t != numTime.back()) {
			cout << ", ";
		}
	}
	cout << "]"<< endl;
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

	int action = 0;
	string heuristic = "";
	cin >> action;

	if(action == 1) {
		displayPath(bruteForce(initialState, goalState));
		return 0;
	} else if(action == 2)  {
		heuristic = "uniformCost";
	} else if(action == 3) {
		heuristic = "misplacedTile";
	} else if(action == 4) {
		heuristic = "manhattanDistance";
	}

	searchResult result = heuristicSearch(initialState, goalState, heuristic);
	displayPath(result.path);
	cout << "SOLUTION DEPTH: " << result.depth << endl;
	cout << "NUMBER OF VISITED STATES: " << result.visitedStates << endl;
	cout << "NUMBER OF FRONTIER NODES: " << result.frontierSize << endl;
	cout << "SEARCH TIME IN MILLISECONDS: " << result.searchTime << endl;
}