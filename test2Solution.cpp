#include <iostream>
#include <fstream>
#include <bits/stdc++.h>
#include <string>
#include <typeinfo>

using namespace std;

string ALPH = "abcdefghijklmnopqrstuvwxyz ";

// Frequencies of characters, from least frequent to most frequent
vector<char> SORTED_CHAR_DIST = {'j', 'q', 'x', 'w', 'z', 'y', 'v', 'h', 'k', 'b', 'm', 'p', 'f', 'd', 'g', 'u', 'c', 'n', 'o', 't', 'i', 'l', 's', 'a', 'r', 'e', ' '};

// The characters are divided into frequency clusters. These clusters are used
// to swap characters around and slowly lead us toward the correct answer. The
// clusters are determined by the character frequency chart in the test2 analysis notebook.
vector<char> T1 = {'e', ' '};
vector<char> T2 = {'r', 'a', 's', 'l', 'i', 't'};
// vector<char> T3 = {};
// vector<char> T4 = {'o', 'n'}

// Vector of all words in dict2
vector<string> DICT2 = {
    "lacrosses", "protectional", "blistered", "leaseback", "assurers",
    "frizzlers", "submerse", "rankness", "moonset", "farcer",
    "brickyard", "stolonic", "trimmings", "glottic", "harshens",
    "tortoni", "unlikely", "alefs", "gladding", "favourig",
    "particulate", "baldpates", "changeover", "lingua", "proctologica",
    "freaking", "outflanked", "amulets", "imagist", "hyped",
    "pilfers", "overachiever", "clarence", "outdates", "smeltery"};

void initializeMap(unordered_map<char, int> *m)
{
    for (int i = 0; i < ALPH.length(); i++)
    {
        (*m).insert(pair<char, int>(ALPH[i], 0));
    }
}

// Fill in character frequency into an unorderd map
void getCharFreq(unordered_map<char, int> &m, const string &text)
{
    // Count the character frequencies
    for (int i = 0; i < text.length(); i++)
    {
        m[text[i]]++;
    }
}

// Sort the unordered map according to value (character frequency)
// return a vector of sorted character
vector<char> *sortFreqMap(unordered_map<char, int> &m)
{
    // Declare vector of pairs
    vector<pair<char, int>> mapItems;

    // Copy key-value pair from m to mapItems vector
    for (pair<const char, int> &item : m)
    {
        mapItems.push_back(item);
    }

    // Sort mapItems vector
    sort(mapItems.begin(), mapItems.end(), [](auto &left, auto &right)
         { return left.second < right.second; });

    // Return sorted vector of character by frequency
    vector<char> *ret = new vector<char>;
    for (pair<char, int> &item : mapItems)
    {
        ret->push_back(char(item.first));
    }
    return ret;
}

// Find the minimum amount of character changes between two strings
int LDistance(const string &lhs, const string &rhs)
{
    int ll = lhs.length();
    int lr = rhs.length();
    vector<vector<int>> dist(ll + 1, vector<int>(lr + 1));

    for (int i = 1; i <= ll; i++)
    {
        dist[i][0] = i;
    }
    for (int j = 1; j <= lr; j++)
    {
        dist[0][j] = j;
    }
    for (int i = 1; i <= ll; i++)
    {
        for (int j = 1; j <= lr; j++)
        {
            int subCost = lhs[i - 1] == rhs[j - 1] ? 0 : 1;
            dist[i][j] = min(min(1 + dist[i][j - 1], subCost + dist[i - 1][j - 1]), 1 + dist[i - 1][j]);
        }
    }
    return dist[ll][lr];
}

int main()
{
    // Initialize an unordered map for input
    unordered_map<char, int> inputFreqMap;
    initializeMap(&inputFreqMap);

    // // Get the input from stdin
    // string input;
    // cout << "Input string: ";
    // getline(cin, input);

    // Get the input from "encypted"
    ifstream inFile("test2_encrypted");
    string input;
    getline(inFile, input);
    // cout << input << endl;

    // Get char freq of the input
    getCharFreq(inputFreqMap, input);
    vector<char> *inputSortedChar = sortFreqMap(inputFreqMap);
    cout << "Input char freq: ";
    for (char c : *inputSortedChar)
    {
        cout << c << " ";
    }
    cout << endl;

    // Create a map from encChar to decChar, based on char freq
    unordered_map<char, char> decodingMap;
    for (int i = 0; i < (*inputSortedChar).size(); i++)
    {
        decodingMap.insert(pair<char, char>((*inputSortedChar)[i], SORTED_CHAR_DIST[i]));
    }

    // Attempt to decrypt the message by directly matching character frequencies
    string decrypted;
    for (char c : input)
    {
        decrypted += decodingMap[c];
    }

    // Print the decrypted text
    cout << "My guess is: " << endl
         << decrypted << endl;
}
