#include <iostream>
#include <fstream>
#include <bits/stdc++.h>
#include <string>
#include <typeinfo>

using namespace std;

string ALPH = "abcdefghijklmnopqrstuvwxyz ";

void initializeMap(unordered_map<char, int> *m) {
    for (int i = 0; i < ALPH.length(); i++) {
        (*m).insert(pair<char, int>(ALPH[i], 0));
    }
}

// Fill in character frequency into an unorderd map
void getCharFreq(unordered_map<char, int>& m, const string& text) {
    // Count the character frequencies
    for (int i=0; i < text.length(); i++) {
        m[text[i]]++;
    }
}

// Sort the unordered map according to value (character frequency)
// return a vector of sorted character
vector<char>* sortFreqMap(unordered_map<char, int>& m) {
    // Declare vector of pairs
    vector<pair<char, int>> mapItems;
  
    // Copy key-value pair from m to mapItems vector
    for (pair<const char, int>& item : m) {
        mapItems.push_back(item);
    }
  
    // Sort mapItems vector
    sort(mapItems.begin(), mapItems.end(), [](auto &left, auto &right) {
        return left.second < right.second;
    });
  
    // Return sorted vector of character by frequency
    vector<char>* ret = new vector<char>;
    for (pair<char, int>& item : mapItems) {
        ret->push_back(char(item.first));
    }
    return ret;
}

int main() {
    // Initialize an unordered map for input
    unordered_map<char, int> inputFreqMap;
    initializeMap(&inputFreqMap);

    // Initialize an unordered map for plaintexts
    unordered_map<int, string> pTextMap;
    unordered_map<char, int> pTextFreqMap;
    initializeMap(&pTextFreqMap);

    // Get the plaintext from files
    ifstream infile("dictionary_1.txt");
    string line;
    string aggregratedPlainText;
    int currInd = 0;
    while (getline(infile, line)){
        if (line.length() == 0) continue;
        if (line.substr(0, 4) == "Test") continue;
        if (line.substr(0, 21) == "Candidate Plaintext #") {
            currInd = atoi(line.substr(21, 1).c_str()); 
            continue;
        }
        pTextMap[currInd] = line;
        aggregratedPlainText += line;
    } 

    // Get the aggregrated character distribution from plaintexts
    getCharFreq(pTextFreqMap, aggregratedPlainText);
    vector<char>* pTextSortedChar = sortFreqMap(pTextFreqMap);

    // Get the input from stdin
    string input;
    cout << "Input string: ";
    getline(cin, input);

    getCharFreq(inputFreqMap, input);
    vector<char>* inputSortedChar = sortFreqMap(pTextFreqMap);

    // Map characters according to distribution
    unordered_map<char, char> decodeMap;
    for (int i=0; i<inputSortedChar->size(); i++) {
        decodeMap.insert(pair<char, char>((*inputSortedChar)[i], (*pTextSortedChar)[i]));
    }

    // TODO: given decodeMap, decode ciphertext and find the plaintext with lowest LDistance
}
