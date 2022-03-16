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
    plaintexts = []
    plaintexts.append(
    "underwaists wayfarings fluty analgia refuels transcribing nibbled okra buttonholer venalness hamlet praus apprisers presifted cubital walloper dissembler bunting wizardries squirrel preselect befitted licensee encumbrances proliferations tinkerer egrets recourse churl kolinskies ionospheric docents unnatural scuffler muches petulant acorns subconscious xyster tunelessly boners slag amazement intercapillary manse unsay embezzle stuccoer dissembles batwing valediction iceboxes ketchups phonily con"
    )
    plaintexts.append(
    "rhomb subrents brasiers render avg tote lesbian dibbers jeopardy struggling urogram furrowed hydrargyrum advertizing cheroots goons congratulation assaulters ictuses indurates wingovers relishes briskly livelihoods inflatable serialized lockboxes cowers holster conciliating parentage yowing restores conformities marted barrettes graphically overdevelop sublimely chokey chinches abstracts rights hockshops bourgeoisie coalition translucent fiascoes panzer mucus capacitated stereotyper omahas produ"
    )
    plaintexts.append(
    "yorkers peccaries agenda beshrews outboxing biding herons liturgies nonconciliatory elliptical confidants concealable teacups chairmanning proems ecclesiastically shafting nonpossessively doughboy inclusion linden zebroid parabolic misadventures fanciers grovelers requiters catmints hyped necklace rootstock rigorously indissolubility universally burrowers underproduced disillusionment wrestling yellowbellied sherpa unburnt jewelry grange dicker overheats daphnia arteriosclerotic landsat jongleur"
    )
    plaintexts.append(
    "cygnets chatterers pauline passive expounders cordwains caravel antidisestablishmentarianism syllabubs purled hangdogs clonic murmurers admirable subdialects lockjaws unpatentable jagging negotiated impersonates mammons chumminess semi pinner comprised managership conus turned netherlands temporariness languishers aerate sadists chemistry migraine froggiest sounding rapidly shelving maligning shriek faeries misogynist clarities oversight doylies remodeler tauruses prostrated frugging comestible "
    )
    plaintexts.append(
    "ovulatory geriatric hijack nonintoxicants prophylactic nonprotective skyhook warehouser paganized brigading european sassier antipasti tallyho warmer portables selling scheming amirate flanker photosensitizer multistage utile paralyzes indexer backrests tarmac doles siphoned casavas mudslinging nonverbal weevil arbitral painted vespertine plexiglass tanker seaworthiness uninterested anathematizing conduces terbiums wheelbarrow kabalas stagnation briskets counterclockwise hearthsides spuriously s"
    )
    m = int(input("Enter which plaintext you would like to encrypt: "))
    p = float(input("Enter the prob of a random char (0-1): "))
    with open('encrypted', 'w') as f:
        f.write(encrypt(plaintexts[m], getKey(), p))
