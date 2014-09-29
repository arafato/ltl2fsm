# -----------------------------------------------------------------------------
#                                  LTL -> NBA
#              Copyright (C) Carsten Fritz, Björn Teegen 2002-2003
#               Sponsored by Deutsche Forschungsgemeinschaft (DFG)
#     Distributed under the terms of the GNU Lesser General Public License
#         See the files README.txt and LICENSE.txt for more information
# -----------------------------------------------------------------------------

from BuechiAutomaton import AlternatingBuechiAutomaton
from Constants      import EXISTENTIAL, DEBUG

#
# QuotientAutomaton
#

MAX = 0
MIN = 1

def __getSuccessorList(successorList, relation, MinMax):
    sL = successorList[0:len(successorList)]
    change = 1
    while change == 1:
	change = 0
	for p in sL:
	    for q in sL:
	        if (p != q) & (change == 0):
                    if relation[p][q] == 1:
		        sL.remove([p,q][MinMax])
		        change = 1
		    else:
			if relation[q][p] == 1:
		            sL.remove([p,q][1-MinMax])
			    change = 1
    return sL

def getMaximumSuccessorList(successorList, relation):
    return __getSuccessorList(successorList, relation, MAX)

def getMinimumSuccessorList(successorList, relation):
    return __getSuccessorList(successorList, relation, MIN)

def __getEquivalentStateList(automaton, relation):
    sL  = []
    sLD = {}
    for p in range(len(automaton.getStateSet())):
	if not sLD.has_key(p):
	    sLD[p] = len(sL)	    
	    sL.append([p])
	    for q in range(p+1,len(automaton.getStateSet())):
	        if not sLD.has_key(q):
	            if (relation[p][q] == 1) & (relation[q][p] ==1):
		        sLD[q] = sLD[p]
			sL[sLD[q]].append(q)
    return (sL, sLD)

#
# Adds the states in the given successorList to the given quotient 
# automaton. Mixed classes of states becomes existential.
#
def __addStatesToQuotientAutomaton(automaton, qAutomaton, successorList):
    if DEBUG:
        print successorList

    for stateList in successorList:
        stateMode = automaton.getStateMode(stateList[0])
	finalMode = automaton.getFinalModeFromList(stateList)
	for state in stateList[1:len(stateList)]:
            if automaton.isExistentialState(state):
	        stateMode = EXISTENTIAL
		break

	qAutomaton.addState(stateMode, finalMode)

#
# Returns the quotient automaton of the given automaton with the
# given relation and simulation type as a new alternating buechi
# automaton.
#
def getQuotientAutomaton(simulationGame):
    automaton = simulationGame.getAutomaton()
    relation = simulationGame.computeSimulationRelation()

    if DEBUG:
        printRelation(relation)

    sL = __getEquivalentStateList(automaton, relation)
    qAutomaton = AlternatingBuechiAutomaton()
    qAutomaton.setInitialState(sL[1][automaton.getInitialState()])
    qAutomaton.setAlphabetSize(automaton.getAlphabetSize())
    
    __addStatesToQuotientAutomaton(automaton, qAutomaton, sL[0])
    simulationGame.addTransitionsToQuotientAutomaton(qAutomaton, sL[1], relation)
    qAutomaton.sL = sL[0]
    qAutomaton.removeNotReachableStates()    

    return qAutomaton


#
# Prints the nXn matrix rel as a matrix
#
##def printRelation(rel):
##    for line in rel:
##        print line


def printRelation(relation):
    print "  |",
    for i in range(len(relation)):
        print i,
    print
    print "--+----------------------------"
    for i in range(len(relation)):
        print i,"|",
        for j in range(len(relation)):
            print relation[i][j],
        print
