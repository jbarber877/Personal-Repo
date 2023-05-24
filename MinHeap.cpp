#include <vector>
#include <algorithm>
#include <iostream>
#include <fstream>
#include <sstream>
#include <chrono>

using namespace std;

class minHeap {
	int tail = 0;
public:
	vector<int> minHeap;

	void Insert(int value);
	void Delete(int value);
	int Search(int value);
	void heapify(int position);
	void levelOrder();
};

void minHeap::Insert(int value) {
	// Add a value to the heap at the next available position
	minHeap.push_back(value);
	int p = tail++;

	// As long as the new element is not at the root and is less than its parent
	while (p != 0 && minHeap.at(p) < minHeap.at((p - 1) / 2)) {
		swap(minHeap.at(p), minHeap.at((p - 1) / 2));
		p = (p - 1) / 2;
	}
}

void minHeap::Delete(int value) {
	int position = Search(value);

	if (position != -1) {
		// replace with last item in heap
		minHeap.at(position) = minHeap.at(tail - 1);
		// clear the last element
		minHeap.erase(minHeap.begin() + tail - 1);
		tail--;

		heapify(position);
	}
}

int minHeap::Search(int value) {
	//Searches the array for a value and returns its index

	vector<int>::iterator it = find(minHeap.begin(), minHeap.end(), value);
	if (it == minHeap.end()) {
		// the value is not in the heap
		return -1;
	}
	else {
		int index = it - minHeap.begin();
		return index;
	}
}

void minHeap::heapify(int position) {
	// Preserves minheap property (the root is the smallest element in the tree) by swapping element 
	// at position with the smaller of its children if it is greater than either.

	int root = position;
	if (((2 * position + 1) < tail) && minHeap.at(2 * position + 1) < minHeap.at(root)) {
		root = 2 * position + 1;
	}
	if (((2 * position + 2) < tail) && minHeap.at(2 * position + 2) < minHeap.at(root)) {
		root = 2 * position + 2;
	}

	//swap node at index with largest node
	if (root != position) {
		swap(minHeap.at(position), minHeap.at(root));
		heapify(root);
	}
}

void minHeap::levelOrder() {
	// Prints comma separated level order traversal of the heap
	for (int i = 0; i < tail; i++) {
		cout << minHeap.at(i) << ",";
	}
}

int main() {
	cout << "Choose a file to run:" << endl;
	cout << "1. Large_ASC.txt" << endl;
	cout << "2. Large_DESC.txt" << endl;
	cout << "3. Large_RAND.txt" << endl;

	int option;
	string choice;
	cin >> option;

	switch (option) {
	case 1:
		choice = "Large_ASC.txt";
		break;
	case 2:
		choice = "Large_DESC.txt";
		break;
	case 3:
		choice = "Large_RAND.txt";
		break;
	}

	minHeap mH;
	ifstream file;
	file.open(choice);
	chrono::high_resolution_clock::time_point start;
	chrono::high_resolution_clock::time_point end;
	chrono::duration<double> elapsed;

	string temp;
	int i = 0;

	// Insertion
	if (file.is_open()) {
		// Start the clock
		start = chrono::high_resolution_clock::now();
		while (!file.eof()) {
			// Get a number from the file, parse as int, and insert to heap
			getline(file, temp, ',');
			mH.Insert(stoi(temp));
			i++;
		}
		//Stop the clock
		end = chrono::high_resolution_clock::now();
		elapsed = end - start;
	}
	cout << "Inserting " << i << " elements into heap took " << elapsed.count() << " seconds." << endl;
	file.close();

	// Traversal
	start = chrono::high_resolution_clock::now();
	cout << "Level Order Traversal :" << endl;
	mH.levelOrder();
	end = chrono::high_resolution_clock::now();
	elapsed = end - start;
	cout << endl << "Level Order Traversal took " << elapsed.count() << " seconds for " << i << " elements." << endl;

	// Deletion
	file.open(choice);
	i = 0;

	if (file.is_open()) {
		start = chrono::high_resolution_clock::now();
		while (!file.eof()) {
			// Get a number from the file, parse as int, and delete from heap
			getline(file, temp, ',');
			mH.Delete(stoi(temp));
			i++;
		}
		end = chrono::high_resolution_clock::now();
		elapsed = end - start;
	}
	cout << "Deleting " << i << " elements from heap took " << elapsed.count() << " seconds." << endl;
	file.close();

	return 0;
}
