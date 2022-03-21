"""
Tests the correctness of our approach. 

For test 2, we generate a sentence with vocabularies in dictionary and 
encrypt the sentence with random probability 0 and test the accuracy of
our approach by comparing the outpu with the original sentence. We then repeat
 this process with a random probability increased by 0.05 repeatedly until we get to 0.2. 
"""
import sys
import os
from subprocess import run
from test2_enc import *

ROUNDS = 200

def LDistance(lhs, rhs):
    if lhs == "":
        return len(rhs)
    if rhs == "":
        return len(lhs)
    grid = [[0 for _ in range(len(rhs)+1)] for _ in range(len(lhs)+1)]
    for i in range(1, len(lhs)+1):
        grid[i][0] = i
    for j in range(1, len(rhs)+1):
        grid[0][j] = j
    for i in range(1, len(lhs)+1):
        for j in range(1, len(rhs)+1):
            sub_cost = 0 if lhs[i-1] == rhs[j-1] else 1
            grid[i][j] = min([grid[i][j-1]+1, grid[i][j-1]+1, grid[i-1][j-1]+sub_cost])
    return grid[len(lhs)][len(rhs)]

def runExecutable(executableName, input):
    p = run([str(os.getcwd()) + "/" + executableName, input], text=True, capture_output=True)
    return(p.stdout)

if __name__ == '__main__':
    # Get the executable name, a.out by default
    argv = sys.argv
    executableName = None
    if len(sys.argv) > 1:
        executableName = str(argv[1])
    else:
        executableName = "a.out"
    print("Testing with excecutable: " + executableName)

    # Generate plaintext with vocabularies from dictionary_2.txt
    vocabularies = getVocabularies()
    plaintext = generateLine(vocabularies, 500)
    print("The generated plaintext is:")
    print(plaintext)
    plaintextwords = plaintext.split(' ')
    print("The number of words is:", len(plaintextwords))

    print("========")
    p = 0
    while p <= 0.5:
        currTotalLDistance = 0
        currMinLDistance = float('+inf')
        currTotalAccuracy = 0
        for _ in range(ROUNDS):
            encrypted_input = encrypt(plaintext, getKey(), p)
            result = runExecutable(executableName, encrypted_input).strip().split('\n')[1]
            # print(result)
            decryptedwords = result.split(' ')
            currLDistance = LDistance(decryptedwords, plaintextwords)
            currTotalLDistance += currLDistance
            currMinLDistance = min(currMinLDistance, currLDistance) 
        print("At randomness = {:.2f}, the average LDistance of {} rounds is {:.2f}, min LDistance is {:.2f}".format(p, ROUNDS, currTotalLDistance/ROUNDS, currMinLDistance))

        p += 0.05



