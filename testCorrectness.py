"""
Tests the correctness of our approach. 

For test 1, we encrypt each plaintext with random probability 0 and test that
our approach correctly guesses all plaintexts. We then repeat this process with
a random probability increased by 0.05 repeatedly until we get to 0.75. Any
errors are reported.
"""
import sys
import os
from subprocess import run
from test1_enc import *


plaintexts = []
plaintexts.append("underwaists wayfarings fluty analgia refuels transcribing nibbled okra buttonholer venalness hamlet praus apprisers presifted cubital walloper dissembler bunting wizardries squirrel preselect befitted licensee encumbrances proliferations tinkerer egrets recourse churl kolinskies ionospheric docents unnatural scuffler muches petulant acorns subconscious xyster tunelessly boners slag amazement intercapillary manse unsay embezzle stuccoer dissembles batwing valediction iceboxes ketchups phonily con")
plaintexts.append("rhomb subrents brasiers render avg tote lesbian dibbers jeopardy struggling urogram furrowed hydrargyrum advertizing cheroots goons congratulation assaulters ictuses indurates wingovers relishes briskly livelihoods inflatable serialized lockboxes cowers holster conciliating parentage yowing restores conformities marted barrettes graphically overdevelop sublimely chokey chinches abstracts rights hockshops bourgeoisie coalition translucent fiascoes panzer mucus capacitated stereotyper omahas produ")
plaintexts.append("yorkers peccaries agenda beshrews outboxing biding herons liturgies nonconciliatory elliptical confidants concealable teacups chairmanning proems ecclesiastically shafting nonpossessively doughboy inclusion linden zebroid parabolic misadventures fanciers grovelers requiters catmints hyped necklace rootstock rigorously indissolubility universally burrowers underproduced disillusionment wrestling yellowbellied sherpa unburnt jewelry grange dicker overheats daphnia arteriosclerotic landsat jongleur")
plaintexts.append("cygnets chatterers pauline passive expounders cordwains caravel antidisestablishmentarianism syllabubs purled hangdogs clonic murmurers admirable subdialects lockjaws unpatentable jagging negotiated impersonates mammons chumminess semi pinner comprised managership conus turned netherlands temporariness languishers aerate sadists chemistry migraine froggiest sounding rapidly shelving maligning shriek faeries misogynist clarities oversight doylies remodeler tauruses prostrated frugging comestible ")
plaintexts.append("ovulatory geriatric hijack nonintoxicants prophylactic nonprotective skyhook warehouser paganized brigading european sassier antipasti tallyho warmer portables selling scheming amirate flanker photosensitizer multistage utile paralyzes indexer backrests tarmac doles siphoned casavas mudslinging nonverbal weevil arbitral painted vespertine plexiglass tanker seaworthiness uninterested anathematizing conduces terbiums wheelbarrow kabalas stagnation briskets counterclockwise hearthsides spuriously s")

def runExecutable(executableName, input):
    programOutput = ""
    p = run([str(os.getcwd()) + "/" + executableName, input], text=True, capture_output=True)
    return(p.stdout)

if __name__=="__main__":
    # Get the executable name, a.out by default
    argv = sys.argv
    executableName = None
    if len(sys.argv) > 1:
        executableName = str(argv[1])
    else:
        executableName = "a.out"
    print("Testing with excecutable: " + executableName)

    # First, make sure that the test1 solution works.
    numErrors = 0
    errorsFound = [] # (probRand, input text, what it guessed)
    numTestsRun = 0
    probRand = 0
    while probRand <= 0.75:
        for text in plaintexts:
            enc = encrypt(text, getKey(), probRand)
            result = runExecutable(executableName, enc)[23:] # strips the "My plaintext guess is: "
            result = result[:len(result) - 1] # strips the trailing \n
            if result != text:
                numErrors += 1
                errorsFound.append((probRand, text, result))
                print("Error found with probRand="+str(probRand))
                # print(errorsFound[-1])
            numTestsRun += 1
        probRand += 0.05
    print("Overall I found " + str(numErrors) + " errors in " + str(numTestsRun) + " tests.")
                
            



