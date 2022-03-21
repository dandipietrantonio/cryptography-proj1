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

    
    // // Get the input from stdin
    // string input;
    // cout << "Input string: ";
    // getline(cin, input);

    // Get the input from "encypted"
    ifstream inFile("encrypted");
    string input;
    getline(inFile, input);
    // cout << input << endl;


    getCharFreq(inputFreqMap, input);
    vector<char>* inputSortedChar = sortFreqMap(inputFreqMap);

    // Initialize an unordered map for plaintexts
    vector<string> pTextMap;

    // Get the plaintext from file "dictionary_1.txt"
    ifstream plainTFile("dictionary_1.txt");
    string line;
    string aggregratedPlainText;
    while (getline(plainTFile, line)){
        if (line.length() == 0) continue;
        if (line.substr(0, 4) == "Test") continue;
        if (line.substr(0, 21) == "Candidate Plaintext #") continue;
        pTextMap.push_back(line);
        aggregratedPlainText += line;
    } 

    // The accuracy of using aggregrated character distribution is not very high, delete before submission
    // // Get the aggregrated character distribution from plaintexts
    // getCharFreq(pTextFreqMap, aggregratedPlainText);
    // vector<char>* pTextSortedChar = sortFreqMap(pTextFreqMap);

    // Get the character distribution of all plaintexts separately
    vector<vector<char>*> pTextSortedChars;
    for (string item : pTextMap) {
        unordered_map<char, int> pTextFreqMap;
        initializeMap(&pTextFreqMap);
        getCharFreq(pTextFreqMap, item);
        vector<char>* pTextSortedChar = sortFreqMap(pTextFreqMap);
        pTextSortedChars.push_back(pTextSortedChar);
    }

    // Create a vector storing the distance between decrypted text and each plaintext
    vector<int> decryptDistances;
    for (int i=0; i<pTextSortedChars.size(); i++) {
        // Map characters according to distribution
        unordered_map<char, char> decodeMap;
        for (int j=0; j<inputSortedChar->size(); j++) {
            decodeMap.insert(pair<char, char>((*inputSortedChar)[j], (*pTextSortedChars[i])[j]));
        }

        // Given decodeMap, decode ciphertext and find the LDistance decrypted text and corresponding plaintext 
        char decryptedText [input.length()];
        for (int j=0; j<input.length(); j++) {
            decryptedText[j] = decodeMap[input[j]];
        }
        int currDistance = LDistance(pTextMap[i], decryptedText);

        // Store the result
        decryptDistances.push_back(currDistance);

        // cout << string(decryptedText).length() << endl;
        
        // Store the corresponding distance
        cout << LDistance(pTextMap[i], decryptedText) << endl;
    }

    // Find the plaintext id with minimum distance to corresponding decrypted text
    int guessedPlainTextId = distance(begin(decryptDistances), min_element(begin(decryptDistances), end(decryptDistances)));
    cout << "My plaintext guess is Candidate Plaintext #"<< guessedPlainTextId + 1 << endl;
    cout << pTextMap[guessedPlainTextId] << endl;
}
