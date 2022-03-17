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

plaintext = ""
with open('test2_plaintext') as f:
    plaintext = f.readline().strip()
plaintextwords = plaintext.split(' ')

decryptedtext = ""
with open('test2_decrypted') as f:
    decryptedtext = f.readline().strip()
decryptedwords = decryptedtext.split(' ')

print("The LDistance is", LDistance(decryptedwords, plaintextwords))
print("The difference in number of words is", LDistance(decryptedwords, plaintextwords))

