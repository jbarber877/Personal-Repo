#include <iostream>
#include <map>
#include <list>
#include <vector>
#include <cstring>
#include <iomanip>
using namespace std;

class Graph {
public:
	struct Node {
		double rank = 0.0, temp = 0.0, outdegree = 0.0;
		vector<string> links;
		Node() {
			outdegree = 0;
		}
		Node(string s, float w) {
			outdegree = 0;
			links.push_back(s);
		}
	};

	// Adjacency List
	map<string, Node*> l;

	void addEdge(string from, string to) {

		// Create the from and to pages if they do not exist
		if (l.find(from) == l.end())
			l.insert(make_pair(from, new Node("none", 0)));

		if (l.find(to) == l.end())
			l.insert(make_pair(to, new Node("none", 0)));

		// Create a new edge
		l[to]->links.push_back(from);
		l[from]->outdegree += 1;

		// Remove the dummy node from the list
		if (l[to]->links.size() > 1 && l[to]->links.at(0) == "none") {
			l[to]->links.erase(l[to]->links.begin());
		}
	}

	void printAdjList() {
		// Iterate over all keys in the map, print the page and its rank
		for (auto p : l) {
			string page_name = p.first;
			cout << page_name << " " << setprecision(2) << fixed << p.second->rank << endl;
		}
	}

	void newRank(int numIterations) {
		// Initialize rank assuming all pages are equally important
		for (auto p : l) {
			p.second->rank = 1.0 / l.size();
		}

		for (int i = 0; i < numIterations - 1; i++) {
			for (auto page : l) {
				double newTemp = 0.0;
				for (auto p : page.second->links) {
					// set the function temp to the rank of incoming pages / number of outgoing links
					newTemp += l[p]->rank / l[p]->outdegree;
				}
				// store the result in the node's temporary variable
				page.second->temp = newTemp;
			}
			for (auto page : l) {
				// assign the result to the page's rank
				page.second->rank = page.second->temp;
			}
		}
	}

}; // end of class

int main() {
	Graph g;
	int numLines, numIterations;
	string from, to;

	cin >> numLines;
	cin >> numIterations;

	for (int i = 0; i < numLines; i++) {
		cin >> from;
		cin >> to;
		g.addEdge(from, to);
	}

	// Perform the power rotations
	g.newRank(numIterations);

	// Print the list
	g.printAdjList();
	return 0;
}
