# -----------------------------------------------------------------------------
#                                  LTL -> NBA
#              Copyright (C) Carsten Fritz, Björn Teegen 2002-2003
#               Sponsored by Deutsche Forschungsgemeinschaft (DFG)
#     Distributed under the terms of the GNU Lesser General Public License
#         See the files README.txt and LICENSE.txt for more information
# -----------------------------------------------------------------------------

from Utilities import removeDuplicates
from EpsiDelayedSimGame import S, D, MOVE_WHO, DE_BIT, NO_TERM, HAS_TERM
from SCC import scc, computeTransposedGraph

def makeNode2SCCList(sccList, numberOfNodes):
    # sccList: List of List of Node (result of function 'scc')
    "Returns a List list of Number s.t. list[node] = i iff \
     node in sccList[i]"
    list = [-1] * numberOfNodes
    for i in range(len(sccList)):
        for node in sccList[i]:
            list[node] = i
    return list


def makeSCCsuccessors(G, node2sccList):
    # G: List of List of Node (adjacency list)
    # node2sccList: List of Number; Result of 'makeNode2SCCList'
    "Returns a List erg of List of number s.t. i in erg[node] \
     iff there is an edge from node to a v s.t. i == node2sccList[v]."
    erg = [-1] * len(G)
    for node in range(len(G)):
        erg[node] = []
        for succNode in G[node]:
            erg[node].append(node2sccList[succNode])
        erg[node].sort()
        removeDuplicates(erg[node])
    return erg


def makeSCCsuddenDeath(sccList, node2sccSuccsList, V):
    # sccList: List of List of Node (result of function 'scc')
    # node2sccSuccsList: List of List of number; result of 'makeSCCsuccessors'
    # V: List of Position
    "Returns a list erg of {S, D, -1} s.t. erg[i] == S iff Spoiler wins \
     immediately in SCC sccList[i] and erg[i] == D iff Duplicator wins \
     immediately in SCC sccList[i], else -1."
    erg = [-1] * len(sccList)
    for sccNumber in range(len(sccList)):
        someoneWinsImm = 1
        nodeNumber = 0
        while someoneWinsImm and (nodeNumber < len(sccList[sccNumber])):
            node = sccList[sccNumber][nodeNumber]
            someoneWinsImm = (node2sccSuccsList[node] == [sccNumber])
            # If node2sccSuccsList[node] != [sccNumber], you can leave the
            # SCC at node. Hence nobody has won instantly.
            nodeNumber += 1
        if someoneWinsImm:
            if V[node][DE_BIT] == 0:
                erg[sccNumber] = D
            elif V[node][DE_BIT] == 1:
                erg[sccNumber] = S
    return erg
            


def whoWins(node, r, V):
    # node: Node
    # r: Dictionary of (Node, number); computed in algoAnd94
    # V: List of Position
    "The interpretation of the r-values computed by algoAnd94."
    if (V[node][DE_BIT] == 0 and r[node] == 0) or \
       (V[node][DE_BIT] == 1 and r[node] > 0):
        return S
    elif (V[node][DE_BIT] == 0 and r[node] > 0) or \
         (V[node][DE_BIT] == 1 and r[node] == 0):
        return D


def algoAnd94(V, sccList, sccSuddenDeath, node2sccList, sccSuccs, posList, \
              P = None):
    ## Yes, I know: A function with 6 or 7 parameters must be crap.
    # V: List of List of Number; an adjacency list
    # sccList: List of List of Number; result of 'scc'
    # sccSuddenDeath: List of {S, D, -1}; result of 'makeSCCsuddenDeath'
    # node2sccList: List of Number; Result of 'makeNode2SCCList'
    # sccSuccs: List of List of Number; result of 'makeSCCsuccessors'
    # posList: List of Positions
    "Adaptation of the AND-/OR-graph reachability algorithm of [And94]. \
     Returns a List of Number that can be interpreted by 'whoWins'."
    if P == None:
        P = computeTransposedGraph(V)
    
    def spoilerWinsImmediately(node, r):
        ## FRAGE: Braucht man diesen 'suddenDeath'-Kram eigentlich?
        ## Ist der Algorithmus nicht inzwischen so clever, daß er das selbst
        ## merkt?
        # Does Spoiler win "suddenly" in 'node'?
        if sccSuddenDeath[node2sccList[node]] == S:
            # Yes, if 'sccSuddenDeath' says so.
            return 1
        elif sccSuccs[node] == [node2sccList[node]]:
            # No, if 'node' only has successors in its own SCC.
            return 0
        if spoilerMoves(posList[node]):
            # Spoiler wins suddenly if it is his move and ...
            for succ in V[node]:
                if (node2sccList[succ] != node2sccList[node]) and \
                   (whoWins(succ, r, posList) == S):
                    # ... there is a successor 'succ' in another SCC
                    # such that 'succ' is already known to be winning
                    # for Spoiler.
                    return 1
        elif duplicatorMoves(posList[node]):
            # Also, Spoiler wins suddenly if it is Duplicator's move, but
            # the SCC of 'node' is trivial and all successive states are
            # winning for Spoiler.
            if not (node2sccList[node] in sccSuccs[node]):
                for succ in V[node]:
                    if whoWins(succ, r, posList) == D:
                        return 0
                return 1
        # Else Spoiler doesn't win suddenly.
        return 0

    def duplicatorWinsImmediately(node, r):
        # Comments in analogy to 'spoilerWinsImmediately'.
        if sccSuddenDeath[node2sccList[v]] == D:
            return 1
        elif sccSuccs[node] == [node2sccList[node]]:
            return 0
        if duplicatorMoves(posList[node]):
            for succ in V[node]:
                if (node2sccList[succ] != node2sccList[node]) and \
                   (whoWins(succ, r, posList) == D):
                    return 1
        elif spoilerMoves(posList[node]):
            if not (node2sccList[node] in sccSuccs[node]):
                for succ in V[node]:
                    if whoWins(succ, r, posList) == S:
                        return 0
                return 1
        return 0


    def onStack(node, L, M):
	sccNumber = node2sccList[node]
	L[sccNumber].append(node)
	if M[sccNumber].has_key(node):
	    del M[sccNumber][node]


    def largeCaseDistinction(w, L, M, whoWinsIn_v):
	# You could do it shorter, but it's easier to read in this way.
	if posList[w][DE_BIT] == 0:                    
	    if (whoWinsIn_v == S) and spoilerMoves(posList[w]):
		r[w] = 0
		onStack(w, L, M)
	    elif (whoWinsIn_v == S) and duplicatorMoves(posList[w]):
		r[w] -= 1
		if r[w] == 0:
		    onStack(w, L, M)
		    # w gets on the stack only if r[w] == 0
		    # or == len(V[w])+1.
	    elif (whoWinsIn_v == D) and spoilerMoves(posList[w]):
		r[w] += 1
		if r[w] == len(V[w]) + 1:
		    onStack(w, L, M)
	    elif (whoWinsIn_v == D) and duplicatorMoves(posList[w]):
		r[w] = len(V[w]) + 1
		onStack(w, L, M)
	elif posList[w][DE_BIT] == 1:
	    if (whoWinsIn_v == S) and spoilerMoves(posList[w]):
		r[w] = len(V[w]) + 1
		onStack(w, L, M)
	    elif (whoWinsIn_v == S) and duplicatorMoves(posList[w]):
		r[w] += 1
		if r[w] == len(V[w]) + 1:
		    onStack(w, L, M)
	    elif (whoWinsIn_v == D) and spoilerMoves(posList[w]):
		r[w] -= 1
		if r[w] == 0:
		    onStack(w, L, M)
	    elif (whoWinsIn_v == D) and duplicatorMoves(posList[w]):
		r[w] = 0
		onStack(w, L, M)
	
    ## Now we really start
    
    L = ['dummy'] * len(sccList)
    for i in range(len(sccList)):
        L[i] = []
    # one stack per SCC
    M = ['dummy'] * len(sccList)
    for i in range(len(sccList)):
	M[i] = {}
	for pos in sccList[i]:
	    j = 0
	    while j < len(P[pos]):
		if node2sccList[P[pos][j]] != node2sccList[pos]:
		    M[i][pos] = pos
		    j = len(P[pos])
		else:
		    j += 1

    # For every SCC i, M[i] is the set of positionens in i having predecessors
    # in another SCC.
    r = [-1] * len(V)
    # The list of results.

    # Initialization. Idea: In an SCC with DE_BIT == 0, Spoiler must try to
    # reach another SCC in which he can win, while for Duplicator, it's
    # sufficient to stay in this SCC (the other way around for DE_BIT == 1).
    # Hence Spoiler wins if, for DE_BIT 0, he reaches a valuation 0 or, for
    # DE_BIT 1, a valuation > 0; else Duplicator wins.
    # In some sense, the valuation is the number of successors of a node that
    # are winning for Spoiler (if DE_BIT == 1) or Duplicator (DE_BIT == 0).
    for v in range(len(V)):
        if posList[v][DE_BIT] == 0:
            if spoilerMoves(posList[v]):
                rho = 1
            else:
                rho = len(V[v])
        elif posList[v][DE_BIT] == 1:
            if spoilerMoves(posList[v]):
                rho = len(V[v])
            else:
                rho = 1

        r[v] = rho


    # We work on the SCCs
    workList = range(len(sccList))
    workList.reverse()
    # We work on the SCCs in reversed top. order [FW01]
    for sccNum in workList:

        for v in sccList[sccNum]:
            if not (v in L[node2sccList[v]]):
                if spoilerWinsImmediately(v, r):
                    if posList[v][DE_BIT] == 0:
                        r[v] = 0
                    else:
                        r[v] = len(V[v]) + 1
                    onStack(v, L, M)
                elif duplicatorWinsImmediately(v, r):
                    if posList[v][DE_BIT] == 0:
                        r[v] = len(V[v]) + 1
                    else:
                        r[v] = 0
		    onStack(v, L, M)
        
        while len(L[sccNum]) > 0:
            v = L[sccNum].pop()
            whoWinsIn_v = whoWins(v, r, posList)
            for w in P[v]:
                if 0 < r[w] <= len(V[w]):
                    # Only if the winner in w is not computed so far.
		    largeCaseDistinction(w, L, M, whoWinsIn_v)

        # And now: Almost the same once again. We now have to work on the nodes
        # in M[sccNum]: They have never been on the stack L[sccNum], but they
        # have predecessors in other SCCs, so they matter here.
	for v in M[sccNum].keys():
	    whoWinsIn_v = whoWins(v, r, posList)
            for w in P[v]:
		if (node2sccList[w] != node2sccList[v]) and (0 < r[w] <= len(V[w])):
                    # If w is in another SCC a the winner in w is
                    # not computed so far.
		    largeCaseDistinction(w, L, M, whoWinsIn_v)

    return r


def spoilerMoves(node): # node: Position
    return (len(node) < HAS_TERM) or (node[MOVE_WHO] == S)

def duplicatorMoves(node): # node: Position
    return not spoilerMoves(node)


def computeSimRel(gameGraph, mainStates):
    # gameGraph: EpsiDelayedSimGame
    # mainStates: List of States
    "We connect the above functions to a working whole and compute the result \
    as a Dictionary of (State, List of States), \
    where q in ergDict[p] iff p <=_{de} q."

    Jurd_V = gameGraph.Jurd_V
    # List of positions
    Jurd_E = gameGraph.Jurd_E
    # List of List of Number s.t. i in Jurd_E[j] iff there is an edge from
    # Jurd_V[j] to Jurd_V[i]
    Jurd_P = gameGraph.Jurd_P
    # Jurd_E transponiert
    pos2number = gameGraph.pos2numberDict
    
    sccList = scc(Jurd_E, Jurd_P)
    node2scc = makeNode2SCCList(sccList, len(Jurd_V))
    sccSuccs = makeSCCsuccessors(Jurd_E, node2scc)
    sccSuddenDeath = makeSCCsuddenDeath(sccList, sccSuccs, Jurd_V)
    rList = algoAnd94(Jurd_E, sccList, sccSuddenDeath, node2scc, sccSuccs, \
                      Jurd_V, Jurd_P)

    ergDict = {}
    for p in mainStates:
        ergDict[p] = []
    for p in mainStates:
        for q in mainStates:
            pqDeBit = gameGraph.de_bit(p, q)
            pqPos = (pqDeBit, p, q, pqDeBit)
            if whoWins(pos2number[pqPos], rList, Jurd_V) == D:
                ergDict[p].append(q)                    
            
    return ergDict
        
