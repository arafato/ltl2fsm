# -----------------------------------------------------------------------------
#                                  LTL -> NBA
#              Copyright (C) Carsten Fritz, Björn Teegen 2002-2003
#               Sponsored by Deutsche Forschungsgemeinschaft (DFG)
#     Distributed under the terms of the GNU Lesser General Public License
#         See the files README.txt and LICENSE.txt for more information
# -----------------------------------------------------------------------------


#
# Some helpful utilities ...
#

#
# Returns the contents of the specified filename as a String (including NEWLINEs)
#
def readAllLinesFromFile(fileName):
    file = open(fileName, 'r')
    allLines = file.readlines()
    file.close()
    return allLines

#
# Writes all lines of the given 'allLines' list to the specified filename.
#
def writeAllLinesToFile(fileName, allLines):
    file = open(fileName, 'w')
    for line in allLines:
        file.write(line)
    file.close()

#
# Returns the first element of the given list and removes it.
#
def getNextToken(tokenList):
    if len(tokenList) != 0:
        token = tokenList[0]
        tokenList[0:1] = []
    else:
        token = ""
    return token

#
# SCC-algorithm
# 

def scc(G):
    dfs(G)    
    GT = __computeTransposedGraph(G)
    dfs(GT,R)
    return SCC

def __computeTransposedGraph(G):
    GT = []
    for i in range(len(G)):
    	GT.append([])
    for i in range(len(G)):
	for j in range(len(G[i])):
	   GT[G[i][j]].append(i)
    return GT
	
#
# DFS-algorithm
#
# G is an adjacency list
# V is an optional list of node ordering 

BLACK = 0
GRAY  = 1
WHITE = 2

#
# Computes in R all nodes as a list in <F order.
#

def dfs(G, V = None):
    global t,c,p,D,F,R,SCC
    c = {}
    p = {}
    D = {}
    F = {}
    R = []
    SCC = []
    if V == None:
        V = range(len(G))
    for v in V:
	c[v] = WHITE
	p[v] = -1
    t = 0
    for v in V:
	if c[v] == WHITE:
	    scc = [v]
	    __dfs(G, v, scc)
	    SCC.append(scc)

def __dfs(G, u, scc):
    global t,c,p,D,F,R
    c[u] = GRAY
    t += 1
    D[u] = t
    for v in G[u]:
	if c[v] == WHITE:
	    scc.append(v)
	    p[v] = u
	    __dfs(G,v,scc)
    c[u] = BLACK
    t += 1
    F[u] = t
    R[0:0] = [u]

#
# Returns an m x n matrix of lists.
#

def getMatrixOfEmptyLists(m, n):
    matrix = []
    for i in range(m):
        matrix.append([])
        for j in range(n):
	    matrix[i].append([])
    return matrix

#
# Returns an m x n matrix initialized with 0.
#

def getZeroMatrix(m, n):
    matrix = []
    for i in range(m):
        matrix.append([])
        for j in range(n):
	    matrix[i].append(0)
    return matrix

#
# Removes duplicates from the sorted list l
#

def removeDuplicates(l): # l: List
    if (l == []): return
    lastSeen = l[0]
    i = 1
    while(i < len(l)):
        if (l[i] == lastSeen):
            del l[i]
        else:
            lastSeen = l[i]
            i += 1
        
#
# A recursive version of tuple(list)
#

def tuplify(l): # l: List
    m = [0] * len(l)
    for i in range(len(l)):
        if (type(l[i]) == type([])):
            m[i] = tuplify(l[i])
        else:
            m[i] = l[i]
    return tuple(m)

            
#
# A recursive version of list(tuple)
#

def listify(t): # t: Tuple
    l = list(t)
    for i in range(len(l)):
        if (type(l[i]) == type((None,))):
            l[i] = listify(l[i])
    return l


#
# The union of two lists interpreted as sets
#

def union(l1, l2): # l1, l2: Lists or Tuples
    l = list(l1 + l2)
    l.sort()
    removeDuplicates(l)
    return l	    


def setminus(l1, l2): # l1, l2: Lists or Tuples of comparable elements
    l1 = list(l1); l2 = list(l2)
    l1.sort(); l2.sort()

    i = j = 0
    while i < len(l1) and j < len(l2):
        if l1[i] < l2[j]:
            i += 1 # Wer cool sein will, schreibt natürlich 'i += l1[i]<l2[j]'
        elif l1[i] > l2[j]:
            j += 1
        else: # l1[i] == l2[j]
            del l1[i]

    return l1

#
# ...
#

def termAndForLists(llt): # llt: List of List of Terms
    "The result is the termAnd of all lists of terms in llt, ie, \
     a list of terms.\
     The result is computed by a divide-and-conquer approach."
    length = len(llt)
    if length == 0:
        return []
    elif length == 1:
        return llt[0]
    else: # length >= 2
        return termAnd(termAndForLists(llt[0:length/2]), \
                       termAndForLists(llt[length/2:]))


#
# Pairwise intersection of the terms in l1 and l2
#

def termAnd(l1, l2): # l1, l2: Lists of vectors (terms)
    if (l1 == []) or (l2 == []):
	return []
    "All vectors must be of equal length"
    length = len(l1[0])    
    erg = []
    for a in l1:
        for b in l2:
            copyIt = 1
            vec = [0] * length
            for i in range(length):
                if ((a[i] == 0) or (a[i] == b[i])):
                    vec[i] = b[i]
                elif (b[i] == 0):
                    vec[i] = a[i]
                else: # Die Einträge sind verschieden und beide ungleich 0
                    copyIt = 0
                    break
            if (copyIt):
                erg.append(vec)
    erg.sort()
    removeDuplicates(erg)
    return erg


#
# Simlify terms like (a & b) | (!a & b) to b
#

def termOrSimplify(termList):
    # termList: List of terms
    "We do this in a very naive way."
    i = 0; j = 1
    while i < len(termList):
	while j < len(termList):
	    diff = listDiff(termList[i], termList[j])
	    if (type(diff) == type([])) and (len(diff) == 1) and \
	       (termList[i][diff[0]] + termList[j][diff[0]] == 0):
		termList[i] = termList[i][:diff[0]] + [0] + termList[i][diff[0]+1:]
		termList =  termList[:j] + termList[j+1:]
		j = 0
	    else:
		j += 1
	i += 1

    termList.sort()
    removeDuplicates(termList)
    return termList
		    

def listDiff(list0, list1):
    # list0, list1: Lists
    "Differences between list0 and list1"
    l = len(list0) - len(list1)
    if l != 0:
	return l
    # Lists have same length
    erg = []
    for i in range(len(list0)):
	if list0[i] != list1[i]:
	    erg.append(i)
    return erg


def listMerge(list1, list2, sortIt=1): # list1, list2: List of List
                                       # sortIt: Boolean (0 or 1)
    "Ex.: list1 = [[1, 2], [3, 4]], list2 = [[5, 6], [7, 8]], so the result \
     is [[1, 2, 5, 6], [1, 2, 7, 8], [3, 4, 5, 6], [3, 4, 7, 8]]. For \
     sortIt=0, the result is [[1, 2, 5, 6], [3, 4, 5, 6], [1, 2, 7, 8], \
     [3, 4, 7, 8]]. Since there is no removeDuplicates for sortIt=0, \
     the length of the result is len(list)*len(list2)."
    erg = []
    for l2 in list2:
        for l1 in list1:
            erg += [l1 + l2]
    for l in erg:
        l.sort()
        removeDuplicates(l)
    if sortIt:
        erg.sort()
        removeDuplicates(erg)
    return erg


def isSubList(list1, list2): # list1, list2: Sorted sequence types
    "Returns 1 iff the sorted list1 is sublist of the sorted list2, else 0. \
     Sublist in the sense of a subset relation, \
     ie, isSubList([1, 1], [1]) == 1."
    if len(list2) == 0:
        return len(list1) == 0
    i, j = 0, 0
    while (i < len(list1)) and (j < len(list2)):
        if list1[i] == list2[j]:
            i += 1
        else:
            j += 1
    return (i == len(list1)) and (j < len(list2))


def deleteWithPointers(aList, pointer, listOfPointers):
    # aList: List
    # pointer: integer
    # listOfPointers: List of Integer
    ## Returns the list of adjusted pointers
    "Delete aList[pointer] but keep the elements of listOf Pointers pointing \
    at their respective elements in aList (or on the succeeding element if the\
    old element was deleted now)."
    del aList[pointer]
    for i in range(len(listOfPointers)):
	if listOfPointers[i] > pointer:
	    listOfPointers[i] -= 1
    return listOfPointers


def maximalElements(lOS, isLess):
    # lOS: List, isLess: Function Listelements X Listelements --> {0, 1}
    "Returns subset of the maximal (wrt. isLess) list elements"
    aList = [0] * len(lOS)
    for i in range(len(aList)):
        aList[i] = lOS[i]
        
    i = 0
    while i < (len(aList) - 1):
        j = i + 1
        while j < len(aList):
            if isLess(aList[i], aList[j]):
                aList[i] = aList[j]
                del aList[j]
                j = i + 1
            elif isLess(aList[j], aList[i]):
                del aList[j]
            else:
                j += 1
        i += 1

    aList.sort()
    return aList


def minimalElements(lOS, isLess):
    # lOS: List, isLess: Function List elements X List elements --> {0, 1}
    "Returns subset of the  minimal (wrt. isLess) list elements"
    aList = [0] * len(lOS)
    for i in range(len(aList)):
        aList[i] = lOS[i]
        
    i = 0
    while i < (len(aList) - 1):
        j = i + 1
        while j < len(aList):
            if isLess(aList[j], aList[i]):
                aList[i] = aList[j]
                del aList[j]
                j = i + 1
            elif isLess(aList[i], aList[j]):
                del aList[j]
            else:
                j += 1
        i += 1

    aList.sort()
    return aList


def reduce2reachable(TR, initial):
    # TR: Dictionary of (state, Dictionary of (state, list of term))
    # initial: State
    erg = {}
    q = Queue(initial)
    while not q.isEmpty():
        p = q.dequeue()
        if not erg.has_key(p):
### Das Auskommentierte ist hübscher, wirft aber a.a.O. Probleme auf.
##	    for succ in TR[p].keys():
##		if TR[p][succ] == []:
##		    del TR[p][succ]
##		else:
##                  q.append(succ)
	    q.append(TR[p].keys())
            erg[p] = TR[p]
    return erg
    

#
# A queue
#

class Queue:
    "A standard queue"

    def __init__(self, obj=None):
        "Constructor with optional initial parameter"
        if(obj == None):
            self.q = []
        else:
            self.q = [obj]

    def enqueue(self, obj):
        self.q = self.q + [obj]

    def append(self, aList):
        self.q = self.q + aList

    def dequeue(self):
        o = self.q[0]
        self.q = self.q[1:]
        return o

    def isEmpty(self):
        return self.q == []


#
# A stack
#

class Stack:
    "A standard stack"

    def __init__(self, obj=None):
        "Constructor with optional initial parameter"
        if(obj == None):
            self.s = []
        else:
            self.s = [obj]

    def push(self, obj):
        self.s = [obj] + self.s

    def pop(self):
        o = self.s[0]
        self.s = self.s[1:]
        return o

    def isEmpty(self):
        return self.s == []


#
# A simple set implementation
#

class Set:
    "A set"

    immutables = [type(0), type((None,)), type(""), type(type(0)), type(0+0j)]

    def __init__(self, obj=None):
        "Constructor with optional initial parameter"
        self.set = {}
        self.size = 0
        if not (obj == None):
            self.add(obj)

    def add(obj):
        self.set[self.hashcode(obj)] = obj
        self.size += 1

    def remove(obj):
        del self.set[self.hashcode(obj)]
        self.size -= 1

    def union(aSet):
        if not (type(aSet) == type(self)): return
        erg = {}
        erg.update(self.set)
        erg.update(aSet.set)
        ausgabe = Set()
        ausgabe.set = erg
        ausgabe.size = len(erg)
        return ausgabe

    def intersec(aSet):
        if not (type(aSet) == type(self)):
            return
        erg = {}
        schluessel = self.set.keys()
        for s in schluessel:
            if(aSet.set.has_key(s)):
               erg[s] = self.set[s]
        ausgabe = Set()
        ausgabe.set = erg
        ausgabe.size = len(erg)
        return ausgabe
              

    def hashcode(self, obj):
        t = type(obj)
        if (t == type([])):
            return tuplify(obj)
        if (t in immutables):
            return obj
        

def tighten(TR, start, isExistentialState, done = []):        
    # TR: Dictionary like transitionRelation
    # start: State
    # done: List
    "If there is an edge (p, \epsi, q) such that p and q have the same mode \
     or q has only one outgoing edge, then we can replace the p-edge by the \
     edges (p, t, q'), for every edge (q, t, q'). \
     Remember: We use a special definition of universal states: If p is a \
     universal state, then there are only \epsi-transitions at p before the \
     application of tighten. After tighten, p may have term transitions, \
     which then means: The opponent chooses a transition for the pebble, \
     and if the pebble cannot take that transition with the current round's \
     term, the opponent wins."
    
    if start in done:
	# print start 
	# print done
	return TR 
    
    done.append(start)
    
    for q in TR[start].keys():
	## Perhaps: Only the \epsi-successors here
	if [] in TR[start][q]:
	    tighten(TR, q, isExistentialState, done)

    if (type(start) != type(())) or (start[0] == '!'):
	return TR

    succList = TR[start].keys()

    for q in succList:
	if [] in TR[start][q]: # there is \epsi-transition
	    q_items = TR[q].items() # List of pair
	    if (isExistentialState(start) == \
		isExistentialState(q)) or \
		(len(q_items) == 1 and len(q_items[0][1]) == 1):
                    ## q has the same mode than start or just one
                    ## outgoing edge
		for qPair in q_items: # copy transitions
		    if not TR[start].has_key(qPair[0]):
			TR[start][qPair[0]] = []
		    TR[start][qPair[0]] += qPair[1]
                    # delete \epsi-transitions
		TR[start][q].remove([])
		if TR[start][q] == []:
		    del TR[start][q]

        ## ... and work on the term successors recursively
    for q in TR[start].keys():
	if TR[start][q] != [[]]:
	    tighten(TR, q, isExistentialState, done)

    return TR


def adjustUnivStates(TR, isExistential):
    "Simplifies the transition relation of universal states after 'tighten'."
    for q in TR.keys():
	if (not isExistential(q)):
	    q_items = TR[q].items()
	    termList = []
	    for qPair in q_items:
		TR[q][qPair[0]].sort()
		removeDuplicates(TR[q][qPair[0]])
		termList += qPair[1]
		while [] in termList:
		    # We can have [] in the list multiple times.
		    termList.remove([])
	    while len(termList) > 1:
		tA = termAnd([termList[0]], [termList[1]])
		if tA == []:
		    termList = []
		    break
		else:
		    termList = tA + termList[2:]
	    for qPair in q_items:
		if TR[q][qPair[0]] != [[]]:
		    # For this if-clause, the removeDuplicates of above
                    # is crucial!
		    if [] in TR[q][qPair[0]]:
			TR[q][qPair[0]] = [[]]
		    else:
			TR[q][qPair[0]] = []
		    for term in termList:
			TR[q][qPair[0]].append(term)
	    
    return TR
