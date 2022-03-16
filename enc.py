import random

ALPH_LIST = ["a", "b", "c", "d", "e", "f", "g", "h", "i", "j", "k", "l", "m", "n", "o", "p", "q", "r", "s", "t", "u", "v", "w", "x", "y", "z", " "]

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
    print("Key: ", key)
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
    # Get input text
    m = input("Enter the text you would like to encrypt: ")
    p = float(input("Enter the prob of a random char (0-1): "))

    print("Encrypted message:", encrypt(m, getKey(), p))
