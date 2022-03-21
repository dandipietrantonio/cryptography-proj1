#include <iostream>
#include <fstream>
#include <bits/stdc++.h>
#include <string>
#include <typeinfo>

using namespace std;

string ALPH = "abcdefghijklmnopqrstuvwxyz ";
int L = 500;

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
    int ll = lhs.length();
    int lr = rhs.length();
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

int main(int argc, char *argv[])
{
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

    // Initialize a map to get the character frequency of the input
    unordered_map<char, int> inputFreqMap;
    initializeMap(&inputFreqMap);
    getCharFreq(inputFreqMap, input);
    vector<char> *inputSortedChar = sortFreqMap(inputFreqMap); // Characters sorted from least to most frequent

    // Map characters according to distribution by group
    unordered_map<char, char> inputLabelMap;
    for (size_t j = 0; j < inputSortedChar->size(); j++)
    {
        int currGroupInd = j / 2;
        inputLabelMap.insert(pair<char, char>((*inputSortedChar)[j], char('a' + currGroupInd)));
    }

    // Given labelMap, label ciphertext
    char labeledInputText[input.length()];
    for (size_t j = 0; j < input.length(); j++)
    {
        labeledInputText[j] = inputLabelMap[input[j]];
    }

    // Get the plaintext from file "dictionary_1.txt"
    vector<string> plaintexts = {
        "underwaists wayfarings fluty analgia refuels transcribing nibbled okra buttonholer venalness hamlet praus apprisers presifted cubital walloper dissembler bunting wizardries squirrel preselect befitted licensee encumbrances proliferations tinkerer egrets recourse churl kolinskies ionospheric docents unnatural scuffler muches petulant acorns subconscious xyster tunelessly boners slag amazement intercapillary manse unsay embezzle stuccoer dissembles batwing valediction iceboxes ketchups phonily con",
        "rhomb subrents brasiers render avg tote lesbian dibbers jeopardy struggling urogram furrowed hydrargyrum advertizing cheroots goons congratulation assaulters ictuses indurates wingovers relishes briskly livelihoods inflatable serialized lockboxes cowers holster conciliating parentage yowing restores conformities marted barrettes graphically overdevelop sublimely chokey chinches abstracts rights hockshops bourgeoisie coalition translucent fiascoes panzer mucus capacitated stereotyper omahas produ",
        "yorkers peccaries agenda beshrews outboxing biding herons liturgies nonconciliatory elliptical confidants concealable teacups chairmanning proems ecclesiastically shafting nonpossessively doughboy inclusion linden zebroid parabolic misadventures fanciers grovelers requiters catmints hyped necklace rootstock rigorously indissolubility universally burrowers underproduced disillusionment wrestling yellowbellied sherpa unburnt jewelry grange dicker overheats daphnia arteriosclerotic landsat jongleur",
        "cygnets chatterers pauline passive expounders cordwains caravel antidisestablishmentarianism syllabubs purled hangdogs clonic murmurers admirable subdialects lockjaws unpatentable jagging negotiated impersonates mammons chumminess semi pinner comprised managership conus turned netherlands temporariness languishers aerate sadists chemistry migraine froggiest sounding rapidly shelving maligning shriek faeries misogynist clarities oversight doylies remodeler tauruses prostrated frugging comestible ",
        "ovulatory geriatric hijack nonintoxicants prophylactic nonprotective skyhook warehouser paganized brigading european sassier antipasti tallyho warmer portables selling scheming amirate flanker photosensitizer multistage utile paralyzes indexer backrests tarmac doles siphoned casavas mudslinging nonverbal weevil arbitral painted vespertine plexiglass tanker seaworthiness uninterested anathematizing conduces terbiums wheelbarrow kabalas stagnation briskets counterclockwise hearthsides spuriously s",
    };

    // Get the character distribution of all plaintexts separately
    vector<vector<char> *> pTextCharsSortedByFreq; // index i holds a list of chars sorted by frequency in increasing order for plaintext
    for (string text : plaintexts)
    {
        unordered_map<char, int> pTextFreqMap;
        initializeMap(&pTextFreqMap);
        getCharFreq(pTextFreqMap, text);
        vector<char> *pTextSortedChar = sortFreqMap(pTextFreqMap);
        pTextCharsSortedByFreq.push_back(pTextSortedChar);
    }

    // Create a vector storing the distance between decrypted text and each plaintext
    vector<int> decryptDistances;
    for (size_t i = 0; i < pTextCharsSortedByFreq.size(); i++)
    {
        // Map characters according to distribution by group
        unordered_map<char, char> labelMap;
        vector<char> curPlaintextCharsSortedByFreq = *pTextCharsSortedByFreq[i];
        for (size_t j = 0; j < curPlaintextCharsSortedByFreq.size(); j++)
        {
            int currGroupInd = j / 2;
            labelMap.insert(pair<char, char>(curPlaintextCharsSortedByFreq[j], char('a' + currGroupInd)));
        }

        // Given decodeMap, decode ciphertext and find the LDistance decrypted text and corresponding plaintext
        char labeledPText[input.length()];
        for (size_t j = 0; j < input.length(); j++)
        {
            labeledPText[j] = labelMap[plaintexts[i][j]];
        }
        int currDistance = LDistance(labeledPText, labeledInputText);

        // Store the result
        // cout << "Here's one " << i << ": " << labeledPText << endl;
        decryptDistances.push_back(currDistance);
    }

    // Find the plaintext id with minimum distance to corresponding decrypted text
    int guessedPlainTextId = distance(begin(decryptDistances), min_element(begin(decryptDistances), end(decryptDistances)));
    cout << "My plaintext guess is: " << plaintexts[guessedPlainTextId] << endl;
}
