#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <stdlib.h>
#include <time.h>
#include <math.h>
using namespace std;

string stob(string str);
double directMapped(int bBytes, int cBytes, int lines, string filename);
double fullAssociative(int cacheSize, int lineSize, bool LRU, string filename);
double setAssociative(int blockSize, int cacheSize, int associativity, bool LRU, string filename);

int main()
{
    int cacheSize, blockSize, associativity, design;
    string filename = "gcc.trace";
    bool LRU = true;
    
    cout << "How many bytes in cache?" << endl;
    cin >> cacheSize;
    cout << "How many bytes in a block/line?" << endl;
    cin >> blockSize;
    cout << "How many lines in a set?" << endl;
    cin >> associativity;  // number of lines in a set = associativity

    // Additional input to select replacement scheme
    cout << "Select a replacement scheme:" << endl;
    cout << "1. Least Recently Used" << endl;
    cout << "2. First In First Out" << endl;
    cin >> design;

    if (design == 1)
        LRU = true;

    cout << endl << "Direct Mapped Cache" << endl;
    cout << "Hit rate: " << directMapped(blockSize, cacheSize, 1, filename) << endl;

    cout << endl << associativity << "-way Associative Cache" << endl;
    cout << "Hit rate: " << setAssociative(blockSize, cacheSize, associativity, LRU, filename) << endl;

    cout << endl << "Fully Associative Cache" << endl;
    cout << "Hit rate: " << fullAssociative(cacheSize, blockSize, LRU, filename) << endl;    
}

double fullAssociative(int cacheSize, int lineSize, bool LRU, string filename) {
    int hit = 0, total = 0, fill = 0, counter = 0;
    string address;
    int iTag;

    int tagBytes = 32 - (int)log2(lineSize);     // number of bytes in the tag field
    int numLines = cacheSize / lineSize;         // number of lines in the cache

    // Cache structure - is a vector of int pairs. First is the tag, second is the counter
    vector<pair<int, int>> cache;
    for (int i = 0; i < (numLines); i++) {
        cache.push_back(make_pair(-1, -1));
    }

    fstream inFile(filename);
    while (!inFile.eof()) {
        bool miss = true;
        getline(inFile, address, ' ');

        if (address.length() != 10) {
            continue;
        }

        // Get the tag as a decimal number
        address = stob(address);
        iTag = stoi(address.substr(0, tagBytes), 0, 2);

        // search through the cache
        for (int i = 0; i < numLines; i++) {
            if (cache.at(i).first == iTag) {
                cache.at(i).second = counter;
                hit++;
                miss = false;
                break;
            }
        }
        if (miss) {
            // If the cache is not full, insert new element at first empty spot
            if (fill < numLines) {
                cache.at(fill).first = iTag;
                cache.at(fill).second = counter;
                fill++;
            }
            else {
                // If using LRU replacement, erase the element with smallest counter.
                // Items in the cache have counter updated each iteration so smallest is LRU.

                if (LRU) {
                    int minCounter = cache.at(0).second;
                    int minIndex = 0;
                    // get index of smallest element in the cache
                    for (int i = 0; i < numLines; i++) {
                        if (cache.at(i).second < minCounter && cache.at(i).second != -1) {
                            minCounter = cache.at(i).second;
                            minIndex = i;
                        }
                    }
                    // Update the least recently used index
                    cache.at(minIndex).first = iTag;
                    cache.at(minIndex).second = counter;
                }

                // For FIFO, the earliest item in the cache must be the first one inserted, ie, 
                // the item at index 0. Delete the first element by shifting the rest of the cache
                // contents forward. Then, insert the new item at the end of the cache vector.

                else {
                    for (int i = 0; i + 1 < numLines; i++) {
                        cache.at(i).first = cache.at(i + 1).first;
                    }
                    cache.at(numLines - 1).first = iTag;
                }
            }
        }

        counter++;
        total++;
    }
    return ((double)hit / total);
}

double directMapped(int blockSize, int cacheSize, int lines, string filename) {
    int hit = 0, total = 0, s = 0, tag;
    string foo, address, set;

    int tagSize = (int)(32 - log2(cacheSize/blockSize) - log2(blockSize));  // number of bytes in the tag field
    int setSize = (int)(log2(cacheSize / blockSize));                       // number of bytes in the set field

    // Cache structure is a vector of ints. The index is the set, counter is not needed.
    vector<int> cache;
    for (int i = 0; i < (cacheSize/blockSize); i++) {
        cache.push_back(-1);
    }

    fstream inFile(filename);
    while (!inFile.eof()) {
        getline(inFile, address, ' ');

        if (address.length() != 10) {
            continue;
        }

        // Get the tag as a decimal number
        address = stob(address);
        tag = stoi(address.substr(0, tagSize), 0, 2);
        s = stoi(address.substr(tagSize, setSize), 0, 2);
        s = s % cache.size();   

        // hit or miss
        if (cache.at(s) == tag) {
            hit++;
        }
        else {
            cache.at(s) = tag;
        }
        total++;
    }

    return ((double)hit/total);
}

double setAssociative(int blockSize, int cacheSize, int associativity, bool LRU, string filename) {
    int hit = 0, total = 0, set, index, minIndex, tag;
    string foo, address;// , tag;

    int linesInCache = cacheSize / blockSize;              // number of lines in the cache
    int s = linesInCache / associativity;                  // number of sets in the cache
    int setSize = (int)(log2(s));                          // bytes in the set field
    int tagSize = 32 - setSize - (int)(log2(blockSize));   // bytes in the tag field
    int counter = 0, mincounter = 0;
    bool miss = true;

    // Cache structure is a vector of int pairs. The first is the tag, the second is counter.
    vector<pair<int,int>> cache;
    for (int i = 0; i < linesInCache; i++) {
        cache.push_back(make_pair(-1,-1));
    }

    fstream inFile(filename);
    while (!inFile.eof()) {
        getline(inFile, foo, ' ');
        getline(inFile, address, ' ');
        getline(inFile, foo);

        if (address.length() != 10) {
            break;
        }

        address = stob(address);
        tag = stoi(address.substr(0, tagSize), 0, 2);
        set = stoi(address.substr(tagSize, setSize),0,2);
        set = set % s;
        index = associativity * set;

        // If the set and tags match, then the access is a hit
        for (int i = 0; i < associativity; i++) {
            if (cache.at(index).first == tag) {
                hit++;
                if (LRU) {
                    cache.at(index).second = counter;
                }
                miss = false;
                break;
            }
            index++;
        }

        if (miss) {

            // If access is a miss and the cahce has an empty position, insert the tag
            index = associativity * set;
            for (int i = 0; i < associativity; i++) {
                if (cache.at(index).first == -1) {
                    cache.at(index).first = tag;
                    cache.at(index).second = counter;
                    miss = false;
                    break;
                }
                index++;
            }

            /* If access is a miss and the cache is full get the index of the element with
            *  smallest count in the set and replace the tag and counter.
            * 
            * For LRU replacement, the counter is updated every iteration, so the smallest
            * counter is least recently used.
            * 
            * For FIFO replacement, the counter is only set when an item is first added to
            * the cache, so the item with smallest counter is first in
            */

            if (miss) {
                index = associativity * set;
                minIndex = index;
                mincounter = cache.at(index).second;
                for (int i = 0; i < associativity; i++) {
                    if (cache.at(index).second < mincounter && cache.at(index).second != -1) {
                        minIndex = index;
                        mincounter = cache.at(index).second;
                    }
                    index++;
                }
                cache.at(minIndex).first = tag;
                cache.at(minIndex).second = counter;
            }

            /*
            * If access is a miss, the cache is full, and using random replacement
            * get a random index in the set and replace the tag. Counter is not 
            * used, so it does not need to be updated.
            */
            /*
            if (miss && !LRU) {
                srand(time(NULL));
                int r = rand() % associativity + (associativity * set); // random index in set
                cache.at(r).tag = tag;
            }
            */
        }
        counter++;
        total++;
        miss = true;
    }

    return ((double)hit / total);
}

// Helper Function
string stob(string str) {
    // convert string to binary
    string result = "";
    // remove the 0x prefix

    str = str.substr(2, str.length());

    for (unsigned int i = 0; i < str.length(); i++) {
        char c = tolower(str[i]);
        switch (c) {
        case '0':
            result = result + "0000";
            break;
        case '1':
            result = result + "0001";
            break;
        case '2':
            result = result + "0010";
            break;
        case '3':
            result = result + "0011";
            break;
        case '4':
            result = result + "0100";
            break;
        case '5':
            result = result + "0101";
            break;
        case '6':
            result = result + "0110";
            break;
        case '7':
            result = result + "0111";
            break;
        case '8':
            result = result + "1000";
            break;
        case '9':
            result = result + "1001";
            break;
        case 'a':
            result = result + "1010";
            break;
        case 'b':
            result = result + "1011";
            break;
        case 'c':
            result = result + "1100";
            break;
        case 'd':
            result = result + "1101";
            break;
        case 'e':
            result = result + "1110";
            break;
        case 'f':
            result = result + "1111";
            break;
        }
    }
    return result;
}
