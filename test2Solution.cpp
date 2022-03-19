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
// clusters are determined by the character frequency chart in the test2 analysis
// notebook.
vector<char> T1 = {'e', ' '};
vector<char> T2 = {'a', 'r', 's', 'i', 'l', 't'};
// vector<char> T3 = {};
vector<vector<char>> ALL_T = {T1, T2};
// vector<char> T3 = {};
// vector<char> T4 = {'o', 'n'}

// Vector of all words in dict2
vector<string> DICT2 = {"lacrosses", "protectional", "blistered", "leaseback", "assurers",
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

int factorial(int n)
{
    if (n == 1)
        return 1;
    else
        return n * factorial(n - 1);
}

/*
 * Uses a sliding window to compare substring to all windows of the same size
 * in text. Returns the minimum Levenshtein distance value among all possible
 * windows.
 *
 */
float findMinLDistanceForSubstringInText(string text, string substring)
{
    int curMinLDistance = 2147483647; // make val for int
    int startIdx = 0;
    int endIdx = substring.length() - 1;

    while (endIdx < text.length())
    {
        int curLDistance = LDistance(substring, text.substr(startIdx, endIdx));
        if (curLDistance < curMinLDistance)
        {
            curMinLDistance = curLDistance;
        }
        startIdx++;
        endIdx++;
    }
    return curMinLDistance;
}

int findIdxOfCharInVector(char c, vector<char> v)
{
    for (int i = 0; i < v.size(); i++)
    {
        if (v[i] == c)
        {
            return i;
        }
    }
    return -1;
}

int main()
{
    // Initialize an unordered map for input
    unordered_map<char, int> inputFreqMap;
    initializeMap(&inputFreqMap);

    // // Get the input from stdin
    // string input;
    cout << "Input string: ";
    // getline(cin, input);

    // Get the input from "encypted"
    ifstream inFile("test2_encrypted");
    string input;
    getline(inFile, input);
    cout << input << endl;

    // Get char freq of the input
    getCharFreq(inputFreqMap, input);
    vector<char> *inputSortedChar = sortFreqMap(inputFreqMap);
    // cout << "Input char freq: ";
    for (char c : *inputSortedChar)
    {
        // cout << c << " ";
    }
    // cout << endl;

    // Create a map from encChar to decChar, based on char freq
    unordered_map<char, char> decodingMap;
    unordered_map<char, char> reverseDecodingMap; // map from decChar to encChar
    for (int i = 0; i < (*inputSortedChar).size(); i++)
    {
        decodingMap[(*inputSortedChar)[i]] = SORTED_CHAR_DIST[i];

        reverseDecodingMap[SORTED_CHAR_DIST[i]] = (*inputSortedChar)[i];
    }

    string bestDecryptedSoFar;
    float minAvgMinLDistanceFound = 9999999; // initialize to a high value
    /*
     * curPermutation holds the current permutation swap attempt. Let's say
     * that our current tier is [t1, t2, t3] and our current permutation is
     * [x1, x2, x3]. This implies that whatever used to be decoded to t1 should
     * now decode to x1, etc.
     *
     */
    bool firstDecryptionAttempt = true;
    int curBestNumWordsFound = 0;
    for (vector<char> tier : ALL_T)
    {

        sort(tier.begin(), tier.end()); // get the lexicographic order

        vector<char> curPermutation = tier;  // start at the first lexicographic order
        vector<char> bestPermutation = tier; // the best permutation found so far
        string curDecryptedAttempt;

        for (int i = 0; i < factorial(tier.size()); i++) // we have to check all n! permutations
        {
            // Attempt to decrypt
            if (firstDecryptionAttempt)
            {
                for (char c : input)
                {
                    curDecryptedAttempt += decodingMap[c];
                }
                firstDecryptionAttempt = false;
            }
            else
            {
                string temp;
                for (char c : bestDecryptedSoFar)
                {
                    // Update the decrypted attempt, adjusting for the values in this permutation
                    int foundIdx = findIdxOfCharInVector(c, tier);
                    if (foundIdx != -1) // the char needs to be substituted
                    {
                        temp += curPermutation[foundIdx];
                    }
                    else
                    {
                        temp += c;
                    }
                }
                curDecryptedAttempt = temp;
            }

            // Get the average L Distance for each word's min LDistance in the decrypted text
            int sumOfLDistances = 0; // used for avg
            for (string word : DICT2)
            {
                int minLDistance = findMinLDistanceForSubstringInText(curDecryptedAttempt, word);
                sumOfLDistances += minLDistance;
            }
            float avgMinLDistance = (float)sumOfLDistances / DICT2.size();

            // Update min avg LDistance found if necessary
            // cout << "Avg LDistance: " << avgMinLDistance << endl;
            int numWordsFound = 0;
            for (string word : DICT2)
            {
                if (curDecryptedAttempt.find(word) != string::npos)
                    numWordsFound++;
            }
            if (numWordsFound == curBestNumWordsFound)
            {
                if (avgMinLDistance < minAvgMinLDistanceFound)
                {
                    // cout << "Found a new best guess: " << curDecryptedAttempt << endl;
                    // cout << "The previous guess was: " << bestDecryptedSoFar << endl;
                    bestDecryptedSoFar = curDecryptedAttempt;
                    minAvgMinLDistanceFound = avgMinLDistance;
                    bestPermutation = curPermutation;
                }
            }
            else if (numWordsFound > curBestNumWordsFound)
            {
                cout << "WE ACTUALY FOUND WORDS!!!!! We found this many: " << numWordsFound << endl;
                bestDecryptedSoFar = curDecryptedAttempt;
                minAvgMinLDistanceFound = avgMinLDistance;
                bestPermutation = curPermutation;
                curBestNumWordsFound = numWordsFound;
            }

            // Adjust the cur permutation
            next_permutation(curPermutation.begin(), curPermutation.end());
        }
    }

    cout << "My best guess is: " << endl
         << bestDecryptedSoFar << endl;
    cout << "with an average min LDistance of " << minAvgMinLDistanceFound << endl;
}
