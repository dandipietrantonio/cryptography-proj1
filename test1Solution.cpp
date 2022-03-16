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

// Find the minimum amount of character changes between two strings
int LDistance(const string& lhs, const string& rhs) {
    int ll = lhs.length();
    int lr = rhs.length();
    vector<vector<int>> dist(ll+1, vector<int>(lr+1));
    
    for(int i=1; i<=ll; i++){
        dist[i][0] = i;
    }
    for(int j=1; j<=lr; j++){
        dist[0][j] = j;
    }
    for(int i=1; i<=ll; i++){
        for(int j=1; j<=lr; j++){
            int subCost = lhs[i - 1] == rhs[j - 1] ? 0 : 1;
            dist[i][j] = min(min(1 + dist[i][j - 1], subCost + dist[i - 1][j - 1]), 1 + dist[i - 1][j]);
        }
    }
    return dist[ll][lr];
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
    ifstream plainTFile("dictionary_1.txt");
    string line;
    string aggregratedPlainText;
    int currInd = 0;
    while (getline(plainTFile, line)){
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

    // // Get the input from stdin
    // string input;
    // cout << "Input string: ";
    // getline(cin, input);

    // Get the input from "encypted"
    ifstream inFile("encrypted");
    string input;
    getline(inFile, input);
    cout << input << endl;


    getCharFreq(inputFreqMap, input);
    vector<char>* inputSortedChar = sortFreqMap(inputFreqMap);

    // Map characters according to distribution
    unordered_map<char, char> decodeMap;
    for (int i=0; i<inputSortedChar->size(); i++) {
        decodeMap.insert(pair<char, char>((*inputSortedChar)[i], (*pTextSortedChar)[i]));
    }

    // Given decodeMap, decode ciphertext and find the plaintext with lowest LDistance
    char decryptedText [input.length()];
    for (int i=0; i<input.length(); i++) {
        decryptedText[i] = decodeMap[input[i]];
    }

    // cout << decryptedText << endl;
    
    // For now, print distance of each plaintext, the lowest one would be the predicted original text
    for (auto& it : pTextMap){
        cout << LDistance(it.second, decryptedText) << endl;
    }
}
