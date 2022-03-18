#include <iostream>
#include <fstream>
#include <bits/stdc++.h>
#include <string>
#include <typeinfo>

using namespace std;

string ALPH = "abcdefghijklmnopqrstuvwxyz ";
int AVGWORDCOUNT = 53;

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

// Find the closet word to target in vocabularies according to LDistance
string findClosetWord(const string& target, vector<string>& vocabularies) {
    string prediction;
    int predDistance = -1;
    for (auto& voc : vocabularies) {
        int currDistance = LDistance(voc, target);
        if (predDistance == -1 || currDistance < predDistance) {
            prediction = voc;
            predDistance = currDistance;
        }
    }
    return prediction;
}

// Find next possible word from start in decryptedText in vocabularies
// Bad performance, abandon
string findNextWord(int start, const string& decryptedText, vector<string>& vocabularies) {
    string prediction;
    int predDistance = -1;
    for (auto& voc : vocabularies) {
        if(start + voc.length() < decryptedText.length()){
            string target = decryptedText.substr(start, voc.length());
            int currDistance = LDistance(voc, target);
            if (predDistance == -1 || currDistance < predDistance) {
                prediction = voc;
                predDistance = currDistance;
            }
        }
    }
    return prediction;
}

int main() {
    // Initialize an unordered map for input
    unordered_map<char, int> inputFreqMap;
    initializeMap(&inputFreqMap);

    
    // // Get the input from stdin
    // string input;
    // cout << "Input string: ";
    // getline(cin, input);

    // Get the input from "test2_encypted"
    ifstream inFile("test2_encrypted");
    string input;
    getline(inFile, input);

    getCharFreq(inputFreqMap, input);
    vector<char>* inputSortedChar = sortFreqMap(inputFreqMap);

    // Get the plaintext vocabularies from file "dictionary_2.txt"
    vector<string> vocabularies;
    ifstream vocabularyFile("dictionary_2.txt");
    string line;
    string aggregratedWords;
    while (getline(vocabularyFile, line)){
        if (line.length() == 0) continue;
        if (line.substr(0, 4) == "Test") continue;
        vocabularies.push_back(line);
        aggregratedWords += line + " ";
    } 
    aggregratedWords = aggregratedWords.substr(0, aggregratedWords.length()-1);
    // cout << aggregratedWords << endl;
    
    // Get the aggregrated character distribution from plaintexts
    unordered_map<char, int> vocabularyMap;
    initializeMap(&vocabularyMap);
    getCharFreq(vocabularyMap, aggregratedWords);
    vector<char>* vocabularySortedChar = sortFreqMap(vocabularyMap);

    // Map characters according to distribution
    unordered_map<char, char> decodeMap;
    for (int j=0; j<inputSortedChar->size(); j++) {
        decodeMap.insert(pair<char, char>((*inputSortedChar)[j], (*vocabularySortedChar)[j]));
    }

    // Given decodeMap, decode ciphertext and find the LDistance decrypted text and corresponding plaintext 
    char decryptedText [input.length()];
    for (int j=0; j<input.length(); j++) {
        decryptedText[j] = decodeMap[input[j]];
    }
    string decryptedTextStr = decryptedText;
    cout << decryptedText << endl;

    // Trial1
    // Given decryptedText, split the decryptedText into average number of words
    vector<string> decryptedWords;
    string ret = "";
    int averageWordLength = input.length() / AVGWORDCOUNT;
    for (int i=0; i<input.length(); i+=averageWordLength) {
        decryptedWords.push_back(decryptedTextStr.substr(i, averageWordLength));
    }
    for (auto& it:decryptedWords){
        ret += findClosetWord(it, vocabularies) + " ";
    }
    ret = ret.substr(0, ret.length()-1);

    // Trial 2: the performance is too bad
    // int startPointer = 0;
    // string ret = "";
    // while (startPointer < decryptedTextStr.length()) {
    //     string nextWord = findNextWord(startPointer, decryptedTextStr, vocabularies);
    //     if (nextWord.length() > 0) {
    //         ret += nextWord + " ";
    //         startPointer = ret.length();
    //     }
    //     else {
    //         break;
    //     }
    // }

    cout << "My guess is:" <<endl;
    cout << ret << endl;
    ofstream myfile;
    myfile.open ("test2_decrypted");
    myfile << ret;
    myfile.close();
}