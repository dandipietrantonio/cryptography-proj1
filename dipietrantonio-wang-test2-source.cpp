#include<iostream>
#include <fstream>
#include <bits/stdc++.h>
#include <string>
#include <typeinfo>

    using namespace std;

string ALPH = "abcdefghijklmnopqrstuvwxyz ";
size_t SHORTESTVOCLEN = 5;      // The length of shortest word in vocabularies
size_t SPACECANDIDATERANGE = 4; // The range of finding characters mapped to space
int GROUPSIZE = 3;

void initializeMap(unordered_map<char, int> *m)
{
    for (size_t i = 0; i < ALPH.length(); i++)
    {
        (*m).insert(pair<char, int>(ALPH[i], 0));
    }
}

// Fill in character frequency into an unorderd map
void getCharFreq(unordered_map<char, int> &m, const string &text)
{
    // Count the character frequencies
    for (size_t i = 0; i < text.length(); i++)
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
    size_t ll = lhs.length();
    size_t lr = rhs.length();
    vector<vector<int>> dist(ll + 1, vector<int>(lr + 1));

    for (size_t i = 1; i <= ll; i++)
    {
        dist[i][0] = i;
    }
    for (size_t j = 1; j <= lr; j++)
    {
        dist[0][j] = j;
    }
    for (size_t i = 1; i <= ll; i++)
    {
        for (size_t j = 1; j <= lr; j++)
        {
            int subCost = lhs[i - 1] == rhs[j - 1] ? 0 : 1;
            dist[i][j] = min(min(1 + dist[i][j - 1], subCost + dist[i - 1][j - 1]), 1 + dist[i - 1][j]);
        }
    }
    return dist[ll][lr];
}

// Find the closet word to target in labeledVocabularies according to LDistance
pair<string, int> findClosetLabeledWord(const string &target, vector<pair<string, string>> &labeledVocabularies)
{
    string prediction;
    int predDistance = -1;
    for (auto &voc : labeledVocabularies)
    {
        int currDistance = LDistance(voc.first, target);
        if (predDistance == -1 || currDistance < predDistance)
        {
            prediction = voc.second;
            predDistance = currDistance;
        }
    }
    return pair<string, int>(prediction, predDistance);
}

// Tokenize the target by splitting on spaces
void tokenize(const string &target, vector<string> &output)
{
    stringstream ss(target);
    string token;
    while (getline(ss, token, ' '))
    {
        output.push_back(token);
    }
}

int main(int argc, char *argv[])
{
    // Initialize an unordered map for input
    unordered_map<char, int> inputFreqMap;
    initializeMap(&inputFreqMap);

    // Get the input from stdin
    string input;
    if (argc == 1) // user wants to input enc from stdin
    {
        cout << "Input string: ";
        getline(cin, input);
    }
    else // user input enc as cli parameter
    {
        input = argv[1];
    }

    // Get the input from "test2_encypted"
    ifstream inFile("test2_encrypted");
    getline(inFile, input);

    getCharFreq(inputFreqMap, input);
    vector<char> *inputSortedChar = sortFreqMap(inputFreqMap); // Characters sorted from least to most frequent

    // Get the plaintext vocabularies and aggregratedWords from file "dictionary_2.txt"
    vector<string> vocabularies = {"lacrosses", "protectional", "blistered", "leaseback", "assurers", "frizzlers", "submerse", "rankness", "moonset", "farcer", "brickyard", "stolonic", "trimmings", "glottic", "rotates", "twirlier", "stuffer", "publishable", "invalided", "harshens", "tortoni", "unlikely", "alefs", "gladding", "favouring", "particulate", "baldpates", "changeover", "lingua", "proctological", "freaking", "outflanked", "amulets", "imagist", "hyped", "pilfers", "overachiever", "clarence", "outdates", "smeltery"};
    string aggregratedWords = "lacrosses protectional blistered leaseback assurers frizzlers submerse rankness moonset farcer brickyard stolonic trimmings glottic rotates twirlier stuffer publishable invalided harshens tortoni unlikely alefs gladding favouring particulate baldpates changeover lingua proctological freaking outflanked amulets imagist hyped pilfers overachiever clarence outdates smeltery";

    // Get the aggregrated character distribution from aggregratedWords
    unordered_map<char, int> vocabularyMap;
    initializeMap(&vocabularyMap);
    getCharFreq(vocabularyMap, aggregratedWords);
    vector<char> *vocabularySortedChar = sortFreqMap(vocabularyMap);

    // Remove space from vocabularySortedChar
    vocabularySortedChar->erase(find(vocabularySortedChar->begin(), vocabularySortedChar->end(), ' '));

    // Map characters in vocabularySortedChar according to distribution by group
    unordered_map<char, char> vocabularyCharLabelMap;
    for (size_t j = 0; j < vocabularySortedChar->size(); j++)
    {
        int currGroupInd = j / GROUPSIZE;
        vocabularyCharLabelMap.insert(pair<char, char>((*vocabularySortedChar)[j], char('a' + currGroupInd)));
    }

    // Given vocabularyCharLabelMap, label vocabularies
    vector<pair<string, string>> labeledVocabularies;
    for (size_t j = 0; j < vocabularies.size(); j++)
    {
        string currLabeledVoc;
        for (size_t k = 0; k < vocabularies[j].length(); k++)
        {
            currLabeledVoc += vocabularyCharLabelMap[vocabularies[j][k]];
        }
        labeledVocabularies.push_back(pair<string, string>(currLabeledVoc, vocabularies[j]));
    }

    // Iteratively select a character in ciphertext as space and calculate total distance between decrypted words and best-match vocabularies
    string retDecryptText = "";
    int minDecryptDistance = -1;
    for (size_t spaceInd = inputSortedChar->size() - 1; spaceInd > inputSortedChar->size() - SPACECANDIDATERANGE - 1; spaceInd--)
    {
        string currMapDecrypt = "";
        int currMapDistance = 0;
        // Remove the selected character mapped to space in a copy of inputSortedChar
        vector<char> inputSortedCharCP = *inputSortedChar;
        char spaceMap = inputSortedCharCP[spaceInd];
        inputSortedCharCP.erase(inputSortedCharCP.begin() + spaceInd);

        // Map characters according to distribution in groups after removing space-mapped character
        unordered_map<char, char> inputLabelMap;
        for (size_t j = 0; j < inputSortedCharCP.size(); j++)
        {
            int currGroupInd = j / GROUPSIZE;
            inputLabelMap.insert(pair<char, char>(inputSortedCharCP[j], char('a' + currGroupInd)));
        }
        inputLabelMap.insert(pair<char, char>(spaceMap, ' '));

        // Given inputLabelMap, decode characters in ciphertext
        char decryptedText[input.length()];
        for (size_t j = 0; j < input.length(); j++)
        {
            decryptedText[j] = inputLabelMap[input[j]];
        }
        string decryptedTextStr = decryptedText;

        // Split the decryptedText by space
        vector<string> tokenizedDecrypted;
        tokenize(decryptedTextStr, tokenizedDecrypted);

        // Find the best-matching word in tokenizedDecrypted and record distance
        for (size_t i = 0; i < tokenizedDecrypted.size(); i++)
        {
            string currToken = tokenizedDecrypted[i];
            // If the tokenized decrypted word is too short, combine the next word to create a longer candidate
            if (currToken.length() < SHORTESTVOCLEN && i < tokenizedDecrypted.size() - 1)
            {
                i++;
                currToken += ' ' + tokenizedDecrypted[i];
            }
            pair<string, int> predictionPair = findClosetLabeledWord(currToken, labeledVocabularies);
            currMapDecrypt += predictionPair.first + ' ';
            currMapDistance += predictionPair.second;
        }
        currMapDecrypt = currMapDecrypt.substr(0, currMapDecrypt.length() - 1);

        // Compare the current total distance with shortest total distance
        // Select the current decrypted text if the total distance is shorter than the previous best-matching text
        if (minDecryptDistance == -1 || currMapDistance < minDecryptDistance)
        {
            minDecryptDistance = currMapDistance;
            retDecryptText = currMapDecrypt;
        }
    }

    cout << "My guess is:" << endl;
    cout << retDecryptText << endl;
}
