#include "game.hpp"
#include <chrono>
using namespace std::chrono;

//struct to hold search results and information
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

//construct entire search tree with a given game pointer at its root
vector<vector<game*>> generateSearchTree(game* root) {
	unordered_set<string> visited;
	visited.insert(root->getID());

	vector<vector<game*>> searchTree = {};
	searchTree.push_back({root});

	//construct tree level by level (depth first search)
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

//brute force the game solution
game* bruteForce(game* initialState, game* goalState) {
	game* path = new game(initialState->getSize());

	//generate entire search tree
	vector<vector<game*>> levels = generateSearchTree(goalState);

	int depth = 0;
	//look for goal node and return pointer to path to traverse upwards
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

//heuristic search algorithm
//note: I have reversed the role of the initial state and goal state to allow bottom up path traversal and avoid keeping track of the path at each node
searchResult heuristicSearch(game* initialState, game* goalState, string& heuristic) {
	//initialize start time
	milliseconds start = duration_cast<milliseconds>(system_clock::now().time_since_epoch());

	//construct new game pointer starting at start state
	game* path = new game(initialState->getSize());

	//comparison function
	auto compare = [](game* a, game* b) {
		return a->getPriority() > b->getPriority();
	};

	//prepare queue and visited set
	goalState->updatePriority(initialState, heuristic);
	unordered_set<string> visited;
	visited.insert(goalState->getID());
	priority_queue<game*, vector<game*>, decltype(compare)> Q(compare);
	Q.push(goalState);

	int maxFrontier = Q.size();

	//loop while there are more nodes to look at
	while(!Q.empty()) {
		//remove top node
		game* currState = Q.top();
		Q.pop();
		visited.insert(currState->getID());
		if(maxFrontier < Q.size()) {
			maxFrontier = Q.size();
		}
		cout << "Best state to expand with depth (g(n)) " << currState->getDepth() << " and heuristic (h(n)) " << currState->getHeuristicValue() << endl;
		currState->display();
		//if we have found the initial state, return it to construct path bottom up
		if(*currState == *initialState) {
			path = currState;
			break;
		} else {
			//get children
			currState->setChildren();
			vector<game*> children = currState->getChildren();
			for(game* child:children) {
				//if child is not vistied yet, add it to the queue
				if(visited.find(child->getID()) == visited.end()) {
					child->updatePriority(initialState, heuristic);
					Q.push(child);
				}
			}
		}
	}

	milliseconds end = duration_cast<milliseconds>(system_clock::now().time_since_epoch());
	int duration = end.count()-start.count();

	//return path and associated metadata
	searchResult res(path->getDepth(), visited.size(), maxFrontier, duration, path);
	return res;
}

//display path
void displayPath(game* path) {
	while(path->getParent() != nullptr) {
		path->display();
		path = path->getParent();
	}
	path->display();
}

//monte carlo simulation
void monteCarlo(int size, int simulations, string& heuristic) {
	//keep track of depth, num visited, max frontier, and search time
	vector<int> numDepth = {};
	vector<int> numVisited = {};
	vector<int> numFrontier = {};
	vector<int> numTime = {};
	//iterate over simulations
	while(simulations > 0) {
		//status indicator
		if(simulations%1000 == 0) {
			int completedSimulations = 100000-simulations;
			cout << "Completed " << completedSimulations << " simulations" << endl;
		}
		//execute search
		game* initialState = new game(size);
		initialState->randomize();
		game* goalState = new game(size);
		searchResult res = heuristicSearch(initialState, goalState, heuristic);
		//expand vectors if need be
		while(numDepth.size() <= res.depth) {
			numDepth.push_back(0);
			numVisited.push_back(0);
			numFrontier.push_back(0);
			numTime.push_back(0);
		}
		//keep memory under control (pointers, ugh)
		delete goalState;
		//add result data to other data
		numDepth.at(res.depth)++;
		numVisited.at(res.depth) += res.visitedStates;
		numFrontier.at(res.depth) += res.frontierSize;
		numTime.at(res.depth) += res.searchTime;
		simulations--;
	}
	//take averages
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
	//print data so that it can be graphed in python (I know I could use a sys call  but I don't want to)
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

//run all test cases from project assignment to plot them
void testCases(string& heuristic) {
	//initialize all test cases
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
	//run all test cases
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
	//print out all data (I know I could write a function since this code is also used in the monte carlo simulation, but I don't really feel like it)
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
	cout << "SOLUTION PATH:" << endl;
	displayPath(result.path);
	cout << "SOLUTION DEPTH: " << result.depth << endl;
	cout << "NUMBER OF VISITED STATES: " << result.visitedStates << endl;
	cout << "NUMBER OF FRONTIER NODES: " << result.frontierSize << endl;
	cout << "SEARCH TIME IN MILLISECONDS: " << result.searchTime << endl;
}