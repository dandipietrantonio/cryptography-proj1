#include <iostream>
#include <bits/stdc++.h>
#include <string>

using namespace std;

string ALPH = "abcdefghijklmnopqrstuvwxyz ";

void initializeMap(unordered_map<char, int> *m) {
    for (int i = 0; i < ALPH.length(); i++) {
        (*m).insert(pair<char, int>(ALPH[i], 0));
    }
}

int main() {
    // Initialize an unordered map
    unordered_map<char, int> freqMap;
    initializeMap(&freqMap);

    // Get the input from stdin
    string input;
    cout << "Input string: ";
    getline(cin, input);

    // Count the character frequencies
    for (int i=0; i < input.length(); i++) {
        freqMap[input[i]]++;
    }

    // For now, print to stdout
    for (int i = 0; i < ALPH.length(); i++) {
        char c = ALPH[i];
        cout << c << ": " << freqMap[c] << " | ";
    }
}
