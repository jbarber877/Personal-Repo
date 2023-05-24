// This file contains the splay tree class and main method

#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <sstream>
#include <chrono>
#include<queue>

using namespace std;

struct Node {
    int value;
    Node* left;
    Node* right;
    Node();
    Node(int _value);
};

Node::Node() {
    left = nullptr;
    right = nullptr;
}

Node::Node(int _value) {
    value = _value;
    left = nullptr;
    right = nullptr;
}

class SplayTree {
public:
    Node* Insert(int value, Node* root);
    Node* Delete(int value, Node* root);
    Node* Splay(int value, Node* root);
    Node* rightRotate(Node* oldRoot);
    Node* leftRotate(Node* oldRoot);
    Node* Search(int value, Node* root);
    void levelOrder(Node* root);
    void traverseInorder(Node* root);
    void traversePreorder(Node* root);
    void traversePostorder(Node* root);
};

Node* SplayTree::Insert(int value, Node* root) {
    //Insert a node int the tree and return pointer to new root

    Node* curr = root;

    if (root == nullptr) {
        // If there is no root, a newly created node becomes the root
        root = new Node(value);
        return root;
    }

    while (true) {
        // If the key is equal to a node in the list
        if (value == curr->value) {
            return root;
        }
        if (value < curr->value) {
            //If there is no left, insert a new node
            if (curr->left == nullptr) {
                curr->left = new Node(value);
                root = Splay(value, root);
                return root;
            }
            // Otherwise, continue
            curr = curr->left;
            continue;
        }
        else {// key must be > curr->val                      
            //If there is no right, insert a new node
            if (curr->right == nullptr) {
                curr->right = new Node(value);
                root = Splay(value, root);
                return root;
            }
            // Otherwise, continue
            curr = curr->right;
            continue;
        }
    }
}

Node* SplayTree::Delete(int value, Node* root) {
    // Deletes the node with specified value (if it exists) and retruns new root of the tree

    Node* temp;

    if (root == nullptr) {
        // If there is no root, the value is not in the tree
        return nullptr;
    }

    // If there is a root, the tree must be splayed. The node with the specified value
    // will rise to the top (if it exists)
    root = Splay(value, root);

    if (value != root->value) {
        // The value is not in the tree
        return root;
    }
    else {
        temp = root;
        if (root->left == nullptr) {
            // If there is no L subtree, the local root of R subtree becomes new root.
            root = root->right;
        }
        else {
            // local root of L subtree becomes new root
            root = Splay(value, root->left);
            root->right = temp->right;
        }
        free(temp);
        return root;
    }
}

Node* SplayTree::Splay(int value, Node* root) {
    // Rotates the tree to bring the node with specified value to the top.
    // Returns new root of the tree

    if (root == nullptr) {
        // If there is no root, there is nothing to splay
        return nullptr;
    }

    Node h;
    Node* newLeft = &h;
    Node* newRight = &h;

    while (true) {
        // If value is in L subtree, use zig rotations
        if (value < root->value) {
            if (root->left == nullptr) {
                // If there is not left subtree, the value is not in tree
                break;
            }
            if (value < root->left->value) {
                // Zig rotation
                root = rightRotate(root);
                if (root->left == nullptr) {
                    // If the value is not in tree, break
                    break;
                }
            }
            newRight->left = root;
            newRight = newRight->left;
            root = root->left;
            newRight->left = nullptr;
        }
        //If value is in R subtree, use zag rotations
        else if (value > root->value) {
            if (root->right == nullptr) {
                // if there is no right subtree, the value is not in tree
                break;
            }
            if (value > root->right->value) {
                // Zag rotation
                root = leftRotate(root);
                if (root->right == nullptr) {
                    // If the value is not in tree, break
                    break;
                }
            }
            newLeft->right = root;
            newLeft = newLeft->right;
            root = root->right;
            newLeft->right = nullptr;
        }
        // If value is equal to the root's value, it is already at the top
        else {
            // There is no need to splay tree
            break;
        }
    }

    newLeft->right = root->left;
    newRight->left = root->right;
    root->left = h.right;
    root->right = h.left;
    return root;
}

Node* SplayTree::rightRotate(Node* oldRoot) {
    Node* newRoot = oldRoot->left;
    Node* newLeft = newRoot->right;

    // Perform rotation
    newRoot->right = oldRoot;
    oldRoot->left = newLeft;
    return newRoot;
}

Node* SplayTree::leftRotate(Node* oldRoot) {
    Node* newRoot = oldRoot->right;
    Node* newRight = newRoot->left;

    // Perform rotation
    newRoot->left = oldRoot;
    oldRoot->right = newRight;
    return newRoot;
}

Node* SplayTree::Search(int value, Node* root) {
    // Searches for a node in the tree and retruns the root

    if (root == nullptr) {
        // If there is no root, value is not in the tree
        return nullptr;
    }

    // If there is a root, the tree must be splayed
    root = Splay(value, root);

    // If the value exists in the tree, it will now be at the root
    return root;
}

void SplayTree::levelOrder(Node* root)
{
    // If the tree is empty, do nothing
    if (root == nullptr)
        return;

    queue<Node*> q;
    q.push(root);
    int i = 1;

    while (!q.empty())
    {
        int nodeCount = q.size();

        while (nodeCount > 0) {
            Node* node = q.front();
            cout << node->value << ",";
            q.pop();

            if (node->left)
                q.push(node->left);
            if (node->right)
                q.push(node->right);

            nodeCount--;
        }
        //cout << endl;
    }
}

void SplayTree::traverseInorder(Node* root) {
    // This function cannot be called on trees with more than 30k nodes without adjusting the stack size
  // #pragma comment(linker, "/STACK:4000000") // requests a 4 GB stack frame
    if (root == nullptr) {
        return;
    }

    traverseInorder(root->left);
    std::cout << root->value << ",";
    traverseInorder(root->right);
}

void SplayTree::traversePreorder(Node* root) {
    // This function cannot be called on trees with more than 30k nodes without adjusting the stack size
    // #pragma comment(linker, "/STACK:4000000") // requests a 4 GB stack frame
    if (root == nullptr) {
        return;
    }

    std::cout << root->value;
    traverseInorder(root->left);
    traverseInorder(root->right);
}

void SplayTree::traversePostorder(Node* root) {
    // This function cannot be called on trees with more than 30k nodes without adjusting the stack size
    // #pragma comment(linker, "/STACK:4000000") // requests a 4 GB stack frame
    if (root == nullptr) {
        return;
    }

    traverseInorder(root->left);
    traverseInorder(root->right);
    std::cout << root->value;
}

int main()
{
    SplayTree st;
    Node* root;
    root = nullptr;

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

    ifstream file;
    chrono::high_resolution_clock::time_point start;
    chrono::high_resolution_clock::time_point end;
    chrono::duration<double> elapsed;
    string temp, out1;
    int i = 0;



    // Insertion
    file.open(choice);

    if (file.is_open()) {
        start = chrono::high_resolution_clock::now();
        while (!file.eof()) {
            // Get a number from the file, parse as int, and insert to heap
            getline(file, temp, ',');
            root = st.Insert(stoi(temp), root);
            i++;
        }
        end = chrono::high_resolution_clock::now();
        elapsed = end - start;
    }

    cout << "Inserting " << i << " elements into splay tree took " << elapsed.count() << " seconds." << endl;
    file.close();

    // Traversal
    start = chrono::high_resolution_clock::now();
    cout << "Level Order Traversal :" << endl;
    st.levelOrder(root);
    end = chrono::high_resolution_clock::now();
    elapsed = end - start;
    cout << endl;
    cout << "Level Order Traversal took " << elapsed.count() << " seconds for " << i << " elements." << endl;

    // Deletion
    file.open(choice);
    i = 0;

    if (file.is_open()) {
        start = chrono::high_resolution_clock::now();
        while (!file.eof()) {
            // Get a number from the file, parse as int, and delete from heap
            getline(file, temp, ',');
            root = st.Delete(stoi(temp), root);
            i++;
        }
        end = chrono::high_resolution_clock::now();
        elapsed = end - start;
    }

    cout << "Deleting " << i << " elements from splay tree took " << elapsed.count() << " seconds." << endl;
    file.close();

    return 0;
}
