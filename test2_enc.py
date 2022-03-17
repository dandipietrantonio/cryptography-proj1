from hashlib import new
import random

ALPH_LIST = ["a", "b", "c", "d", "e", "f", "g", "h", "i", "j", "k", "l", "m", "n", "o", "p", "q", "r", "s", "t", "u", "v", "w", "x", "y", "z", " "]

def getVocabularies():
    vocabularies = []
    with open('dictionary_2.txt') as f:
        lines = f.readlines()
        for line in lines:
            line = line.strip()
            if len(line) == 0:
                continue
            if line[:4] == "Test":
                continue
            vocabularies.append(line)
    return vocabularies

def generateLine(vocabularies, N):
    ret = ""
    while len(ret) < N:
        new_word = vocabularies[random.randint(0, len(vocabularies)-1)]
        if len(ret) + len(new_word) + 1 <= N:
            ret += new_word + " "
        else:
            break
    ret = ret[:-1]
    return ret
    

# Returns 1 with prob 1, else 0
def flipCoin(prob1):
    return random.random() < prob1

# Returns a random key. For the sake of this project, a key is just a dict
# mapping the input character to its ciphertext counterpart
def getKey():
    key = dict()
    
    alphCopy = [c for c in ALPH_LIST]
    for char in ALPH_LIST:
        i = random.randrange(0, len(alphCopy))
        val = alphCopy.pop(i)
        key[char] = val
    return key

# Encrypts a message following the scheme in the project description.
def encrypt(message, key, probRandom):
    ret = ""
    msgIdx = 0

    while msgIdx < len(message):
        if flipCoin(probRandom):
            ret += random.choice(ALPH_LIST)
        else:
            ret += key[message[msgIdx]]
            msgIdx += 1
    return ret

if __name__=="__main__":
    vocabularies = getVocabularies()
    plaintext = generateLine(vocabularies, 500)
    print("The generated plaintext is:")
    print(plaintext)
    with open('test2_plaintext', 'w') as f:
        f.write(plaintext)
    p = float(input("Enter the prob of a random char (0-1): "))
    with open('test2_encrypted', 'w') as f:
        f.write(encrypt(plaintext, getKey(), p))
