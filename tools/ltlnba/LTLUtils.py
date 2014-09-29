# -----------------------------------------------------------------------------
#                                  LTL -> NBA
#              Copyright (C) Carsten Fritz, Björn Teegen 2002-2003
#               Sponsored by Deutsche Forschungsgemeinschaft (DFG)
#     Distributed under the terms of the GNU Lesser General Public License
#         See the files README.txt and LICENSE.txt for more information
# -----------------------------------------------------------------------------

from Constants import DEBUG
import string
import Utilities
from EpsiABA import EpsiABA
from SCC import scc2, cTG2

sequenceType = [type(()), type([])]

PROPOSITIONS = string.lowercase
ONE = '1'
ZERO = '0'
TT = 'tt'
FF = 'ff'
WHITESPACES = ' \t\n\r\f'

OR = '|'
AND = '&'
UNTIL = 'U'
RELEASE = 'V'
NOT = '!'
NEXT = 'X'
FINALLY = 'F'
GLOBALLY = 'G'

IMPL = '->'
EQUIV = '<->'

OPERATORS = {IMPL: '->', EQUIV: '<->',
	     OR: ' | ', AND: ' & ', UNTIL: ' U ', RELEASE: ' V ',
             NOT: '!', NEXT: 'X', FINALLY: 'F', GLOBALLY: 'G'}

NEGATED = {OR: AND, AND: OR, UNTIL: RELEASE, RELEASE: UNTIL,
	   NEXT: NEXT, FINALLY: GLOBALLY, GLOBALLY: FINALLY, TT: FF, FF: TT,
	   ONE: ZERO, ZERO: ONE}

prefixOp     = [NEXT, GLOBALLY, FINALLY, NOT]
infixOp      = [UNTIL, RELEASE, AND, OR, IMPL, EQUIV]
tempOp       = [UNTIL, RELEASE, NEXT, FINALLY, GLOBALLY]

ParseError = 'Parse Error'

def LTLlen(formula): # formula: String or state (ie, tuple)
    "Returns the number of temporal operators in formula"
    if type(formula) == type(""):
        cnt = 0
        for i in range(len(formula)):
            cnt += (formula[i] in tempOp)
        return cnt
    elif type(formula) in sequenceType:
        if formula[0] in prefixOp:
            return (formula[0] in tempOp) + LTLlen(formula[1])
        elif formula[0] in infixOp:
            return (formula[0] in tempOp) + LTLlen(formula[1]) + \
                   LTLlen(formula[2])
        else:
            return 0

def parseTreeSize(formula): # formula: Parse Tree
    "Returns the size of the formula, ie, the number of nodes \
     of its parse tree"
    erg = 1
    for subtree in formula[1:]:
	erg += parseTreeSize(subtree)
    return erg
    
        
def LTLisLeq(form0, form1): # form0, form1: String or state (ie, tuple)
    return LTLlen(form0) <= LTLlen(form1)

def LTLisLess(form0, form1): # form0, form1: String or state (ie, tuple)
    return LTLlen(form0) < LTLlen(form1)


def __lex(s):
    'lex(string) -> list of strings'

    tokens = []
    for c in s:
        if c not in WHITESPACES:
            tokens.append(c)

    return tokens



def lbttParser(s):
     """parse(string) -> tuple tree

Return a parse tree of an input LTL formula in the format of lbtt."""

     if type(s) == type(""):
	 return lbttParser(s.split())
     elif type(s) == type([]):
	 if s == []:
	     return (tuple([]), [])
	 elif s[0] in prefixOp:
	     (subtree, rest) = lbttParser(s[1:])
	     if subtree == tuple([]):
		 raise ParseError, 'Unexpected end of input'
	     return ((s[0], subtree), rest)
	 elif s[0] in infixOp:
	     (lefttree, rest) = lbttParser(s[1:])
	     if (lefttree == tuple([])) or (rest == []):
		 raise ParseError, 'Unexpected end of input'
	     (righttree, rest) = lbttParser(rest)
	     return ((s[0], lefttree, righttree), rest)
	 elif s[0] in ['f', 't']:
	     if s[0] == 'f':
		 return ('0', s[1:])
	     else:
		 return ('1', s[1:])
	 elif (type(s[0]) == type("")) and (s[0][0] == 'p'):
	     return (s[0], s[1:])



def parse(s):
    """parse(string) -> tuple tree

Return an expression tree.

LTL expressions are defined by the following grammar:

expression:
  or-expression

or-expression:
  and-expression
  or-expression '|' and-expression

and-expression:
  until-or-release-expression
  and-expression '&' until-or-release-expression

until-or-release-expression:
  prefix-expression
  prefix-expression 'U' prefix-expression
  prefix-expression 'V' prefix-expression

prefix-expression:
  primary-expression
  '!' prefix-expression
  'X' prefix-expression
  'F' prefix-expression
  'G' prefix-expression

primary-expression:
  proposition
  '0'
  '1'
  '(' expression ')'"""

    tokens = __lex(s)
    (tree, tokens) = __parseOr(tokens)
    if tokens != []:
        raise ParseError, 'Extra input'
    return tree

def __parseOr(tokens):
    (a, tokens) = __parseAnd(tokens)
    while tokens != [] and tokens[0] == '|':
        (b, tokens) = __parseAnd(tokens[1:])
        a = (OR, a, b)
    return (a, tokens)

def __parseAnd(tokens):
    (a, tokens) = __parseUntilOrRelease(tokens)
    while tokens != [] and tokens[0] == '&':
        (b, tokens) = __parseUntilOrRelease(tokens[1:])
        a = (AND, a, b)
    return (a, tokens)

def __parseUntilOrRelease(tokens):
    (a, tokens) = __parsePrefix(tokens)
    if tokens != [] and tokens[0] == 'U':
        (b, tokens) = __parsePrefix(tokens[1:])
        a = (UNTIL, a, b)
    elif tokens != [] and tokens[0] == 'V':
        (b, tokens) = __parsePrefix(tokens[1:])
        a = (RELEASE, a, b)
    return (a, tokens)

def __parsePrefix(tokens):
    if tokens != [] and tokens[0] == '!':
        (a, tokens) = __parsePrefix(tokens[1:])
        a = (NOT, a)
    elif tokens != [] and tokens[0] == 'X':
        (a, tokens) = __parsePrefix(tokens[1:])
        a = (NEXT, a)
    elif tokens != [] and tokens[0] == 'F':
        (a, tokens) = __parsePrefix(tokens[1:])
        a = (FINALLY, a)
    elif tokens != [] and tokens[0] == 'G':
        (a, tokens) = __parsePrefix(tokens[1:])
        a = (GLOBALLY, a)
    else:
        (a, tokens) = __parsePrimary(tokens)
    return (a, tokens)

def __parsePrimary(tokens):
    if tokens == []:
        raise ParseError, 'Unexpected end of input'
    elif tokens[0] == '(':
        (a, tokens) = __parseOr(tokens[1:])
        if tokens == [] or tokens[0] != ')':
            raise ParseError, "Missing ')'"
        return (a, tokens[1:])
    elif (tokens[0] in PROPOSITIONS):
        return (tokens[0], tokens[1:])
    elif (tokens[0] == ONE):
	return (TT, tokens[1:])
    elif (tokens[0] == ZERO):
	return (FF, tokens[1:])
    else:
        raise ParseError, 'Illegal token: ' + str(tokens[0])


def toString(tree):
    '''toString(tree) -> string

Return a simple string representaion of the tree.'''

    if type(tree) == type(()):
        op = tree[0]
        if op in [OR, AND, UNTIL, RELEASE]:
            (a, b) = tree[1:]
            return '(' + toString(a) + OPERATORS[op] + toString(b) + ')'
        elif op in [NOT, NEXT, FINALLY, GLOBALLY]:
            (a,) = tree[1:]
            return OPERATORS[op] + toString(a)
    else:
        return str(tree)


def gamePos2String(gamePos):
    "To print a position of the simulation game."
    vorne = str(gamePos[:1])[:-1]
    pos1 = toString(gamePos[1])
    pos2 = toString(gamePos[2])
    rest = str(gamePos[3:])[1:]
    return vorne + ' ' + pos1 + ', ' + pos2 + ', ' + rest

        
def mhStateToString(mhState): # mhState: Pair of Tuple of State
    erg = "("
    for set in mhState:
        setString = "{"
        for state in set:
            setString += toString(state) + ", "
        if len(setString) > 1:
            setString = setString[:-2] # remove last comma
        setString += "}"
	if setString == "{}":
	    setString = "ø"
        erg += setString + ", "
    return erg[:-2] + ")" # remove last comma


def wrap(s, maxwidth):
    'wrap(string, integer) -> string.'

    i = maxwidth
    while i < len(s):
        s = s[:i] + '\\n' + s[i:]
        i += maxwidth + 1
    return s


def wrapMH(s, maxwidth):
    'wrap(string, integer) -> string.'
    compList = s.split(",")

    erg = ""; lenCounter = 0
    for aString in compList:
	if (len(aString) + lenCounter + 1 > maxwidth) and not (erg == ""):
	    erg += '\\n' + aString + ','
	    lenCounter = len(aString) + 1
	else:
	    erg += aString + ','
	    lenCounter += len(aString) + 1
    return erg[:-1] # remove last comma
    


def negate(tree):
    '''negate(tree) -> tree

Return the negation of the tree.'''

    if type(tree) == type(()):
        op = tree[0]
        if op == NOT:
            return posNF(tree[1])
        elif op in [OR, AND, UNTIL, RELEASE]:
            (a, b) = tree[1:]
            return (NEGATED[op], negate(a), negate(b))
        elif op in [NEXT, FINALLY, GLOBALLY]:
            (a,) = tree[1:]
            return (NEGATED[op], negate(a))
    else:
	if tree in [TT, FF]:
	    return NEGATED[tree]
	else:
	    return (NOT, tree)


def posNF(tree):
    '''posNF(tree) -> tree

Return the tree in positive normal form.'''

    if type(tree) == type(()):
        op = tree[0]
        if op == NOT:
            return negate(tree[1])
        elif op in [OR, AND, UNTIL, RELEASE]:
            (a, b) = tree[1:]
	    a = posNF(a)
	    b = posNF(b)
	    return (op, a, b)
        elif op in [NEXT, FINALLY, GLOBALLY]:
            (a,) = tree[1:]
	    a = posNF(a)
	    return (op, a)
    else:
	if tree in [ONE, TT]:
	    return TT
	elif tree in [ZERO, FF]:
	    return FF
	else:
	    return tree


def removeTrueFalse(tree):
    "Remove occurrences of '0' and '1' as well as '->' and '<->' from the \
    parse tree 'tree' by applying some simplification rules."

    if type(tree) == type(()):
	op = tree[0]
	if op in prefixOp:
	    a = removeTrueFalse(tree[1])
	    if op in [NEXT, FINALLY, GLOBALLY]:
		if a in [ONE, TT]:
		    return ONE
		elif a in [ZERO, FF]:
		    return ZERO
		else:
		    return (op, a)
	    elif (op == NOT) and (a in [ONE, TT, ZERO, FF]):
		return NEGATED[a]
	    else:
		return (op, a)
	elif op in infixOp:
	    if op == IMPL:
		return removeTrueFalse((OR, (NOT, tree[1]), tree[2]))
	    elif op == EQUIV:
		return removeTrueFalse((OR, (AND, tree[1], tree[2]),
					(AND, (NOT, tree[1]), (NOT, tree[2]))))
	    a = removeTrueFalse(tree[1])
	    b = tree[2]
	    if a in [ONE, TT]:
		if op == OR:
		    return ONE
		if op in [AND, RELEASE]:
		    return removeTrueFalse(b)
		if op == UNTIL:
		    return removeTrueFalse((FINALLY, b))
	    elif a in [ZERO, FF]:
		if op in [OR, UNTIL]:
		    return removeTrueFalse(b)
		if op == AND:
		    return ZERO
		if op == RELEASE:
		    return removeTrueFalse((GLOBALLY, b))
	    b = removeTrueFalse(b)
	    if b in [ONE, TT]:
		if op in [OR, UNTIL, RELEASE]:
		    return ONE
		if op == AND:
		    return a
	    elif b in [ZERO, FF]:
		if op == OR:
		    return a
		if op in [AND, UNTIL, RELEASE]:
		    return ZERO
	    return (op, a, b)
    else:
	if tree in [ONE, TT]:
	    return TT
	elif tree in [ZERO, FF]:
	    return FF
	else:
	    return tree
	    
    

def termImplies(t0, l): # t0: Term, ie, list over {0, 1, -1}
                        # l: List of terms of same length than t0
    "Returns 1 if term t0 implies one of the terms in l, else 0.\
     Epsi, i.e. [], is not implied by any term."
    for t1 in l:
        if oneTermImplies(t0, t1):
            return 1
    return 0

def oneTermImplies(t0, t1):
    if (t1 == []) or (t1 == ()): # epsi is not implied
        return 0
    for i in range(len(t1)):
        if (not (t1[i] == 0)) and (not (t0[i] == t1[i])):
            return 0 # t0 doesn't imply t1
    return 1

## termImplies and oneTermImplies can also be used for terms represented as
## tuples instead of lists.


def simplifyMHtransition(mhState, mhTrans, weDict, isAcc, sim=None):
    # mhState: MH-State
    # mhTrans: Dictionary of (MHstate, Dictionary of (tuple(Term), \
    #                                                 List of MHstate))
    # weDict: Dictionary of (MHstate, MHstate)
    # isAcc: Function MHstate -> {0, 1}
    aDict = {}
    aDict[mhState] = mhTrans[mhState]
    simplifyMHAuto_improved(aDict, sim)
    # Is there an equivalent state? Two states q0, q1 are equivalent in a weak
    # sense if q0 \in F \iff q1 \in F and \Delta(q0)[q0/q1] = \Delta(q1)[q0/q1]
    # If there is a weakly equivalent state, we don't need this one.
    weState = checkWeakEquivalence(mhState, mhTrans, isAcc)
    if weState != None:
	del mhTrans[mhState]
	weDict[mhState] = weState
	# We remember that weState substitutes mhState
	for state in mhTrans.keys():
	    for term in mhTrans[state]:
		flag = 0; i = 0
		while i < len(mhTrans[state][term]):
		    if mhTrans[state][term][i] in [mhState, weState]:
			if flag:
			    del mhTrans[state][term][i]
			    i -= 1
			else:
			    mhTrans[state][term][i] = weState
			    flag = 1
		    i += 1
		    			


def checkWeakEquivalence(state, transRel, isAcc):
    # state: State
    # transRel: Dictionary of (State, Dictionary of (tuple(term), \
    #                                                List of State))
    # isAcc: Function StateSet -> {0, 1}
    for q in transRel.keys():
	if (q != state) and (isAcc(q) == isAcc(state)):
	    if transRel[q] == transRel[state]:
		return q
	    else: # Does it work with q substituted for state?
		qItems = transRel[q].items()
		stateItems = transRel[state].items()
		for aList in [qItems, stateItems]:
		    for i in range(len(aList)):
			if aList[i][1] == state:
			    aList[i] = (aList[i][0], q)
		qItems.sort()
		stateItems.sort()
		if qItems == stateItems:
		    return q
    return None



def simplifyMHAuto(mhDict, sim=None):
    # mhDict: Dictionary of (MHstate, Dictionary of (tuple(Term),
    #  List of MHstate)) mit MHstate: Pair of Tuple of State
    # sim: SimulationRelation
    "If for (M, N) and t in mhDict[(M, N)][t] there are states (M0, N0), \
     (M1, N1) s.t. M0 \subset M1 and N0 \subset N1, then delete (M1, N1) from \
     the successor list."
    for (M, N) in mhDict.keys():
        for t in mhDict[(M, N)].keys():
            i = 0
            while i < (len(mhDict[(M, N)][t]) - 1):
                j = i + 1
                while j < len(mhDict[(M, N)][t]):
                    (M0, N0) = mhDict[(M, N)][t][i]
                    (M1, N1) = mhDict[(M, N)][t][j]
                    if Utilities.isSubList(M0, M1) and \
                       Utilities.isSubList(N0, N1):
                        del mhDict[(M, N)][t][j]
                    elif Utilities.isSubList(M1, M0) and \
                         Utilities.isSubList(N1, N0):
                        del mhDict[(M, N)][t][i]
                    else:
                        j += 1
                i += 1

    "Accepting states only need maximal successors -- at this point, we use \
     the preorder defined by sim.isLessMH. Further, you may delete \
     non-maximal successors from non-accepting states in favor of greater \
     accepting successor states."
    if not (sim == None):
        
        def isLessMHspecial(x, y): # x, y: MHstate
            return (y[1] == ()) and sim.isLessMH(x, y)
            ## And not "(x[1] != () and y[1] == ()) and ...", cf. the examples
            ## FX((a | b) U (a & XGa)) and (a | b | c) U ( a & X((a | b) U (a & XGa))).
                
        for p in mhDict.keys():
            for t in mhDict[p].keys():
                if p[1] == (): # p is accepting
                    mhDict[p][t] = Utilities.maximalElements(mhDict[p][t], \
                                                             sim.isLessMH)
                else: # p is not accepting
                    mhDict[p][t] = Utilities.maximalElements(mhDict[p][t], \
                                                             isLessMHspecial)
                    ## That's why I like Python ...


def simplifyMHAuto_improved(mhDict, sim):
    # An improved (but more complex) version of simplifyMHauto
    isLessMH_weak = (lambda M0, N0, M1, N1: Utilities.isSubList(M1, M0) and \
		     Utilities.isSubList(N1, N0))
    isLessMHspecial = (lambda x, y: (y[1] == ()) and sim.isLessMH(x, y))

    "If, given a state q, there are terms t0, t1 such that t1 \implies t0, \
     then delete those states from mhDict[q][t1] that are less \
     (wrt isLessMH_weak) than elements in mhDict[q][t0]."
    for q in mhDict.keys():
	for t0 in mhDict[q].keys():
	    for t1 in mhDict[q].keys():
		if oneTermImplies(t1, t0):
		    i = 0
		    while i < len(mhDict[q][t0]):
			j = 0
			while j < len(mhDict[q][t1]):
			    if (t0 != t1) or (i != j):
				(M0, N0) = mhDict[q][t0][i]
				(M1, N1) = mhDict[q][t1][j]
				if isLessMH_weak(M1, N1, M0, N0) or \
				   ((q[1] == ()) and sim.isLessMH((M1, N1), \
								  (M0, N0))) \
				   or ((q[1] != ()) and \
				       isLessMHspecial((M1, N1), (M0, N0))):
				    i = Utilities.deleteWithPointers(mhDict[q][t1], j, [i, j])[0]
				    j -= 1
			    j += 1
			i += 1

                    

def adjustMHTransFormat(mhDict, iniState):
    # mhDict: Dictionary of (MHstate, Dictionary of (tuple(Term),
    #  List of MHstate)) mit MHstate: Pair of Tuple of State
    # iniState: MHstate
    "Delete unreachable states and simultaneously transfer to \
     the format Dictionary of (MHstate, Dictionary of (MHstate, List of Term))"
    newDict = {}
    q = Utilities.Queue(iniState)
    while not q.isEmpty():
        state = q.dequeue()
        if not newDict.has_key(state):
            newDict[state] = {}
            for t in mhDict[state].keys():
                for nextState in mhDict[state][t]:
                    if newDict[state].has_key(nextState):
                        newDict[state][nextState] += [t]
                    else:
                        newDict[state][nextState] = [t]
                    q.enqueue(nextState)

    "Simplify the term lists"
    for p in newDict.keys():
        for q in newDict[p].keys():
            newDict[p][q] = Utilities.maximalElements(newDict[p][q], \
                                                      oneTermImplies)

    mhDict.clear()
    mhDict.update(newDict)


def sccBasedSimplifications(automaton):
    # automaton: EpsiABA (result of newEpsiABA)

    def deleteState(state, automaton, transposedGraph):
	# state: State of automaton
	# automaton: EpsiABA
	if state in automaton.stateSet:
	    automaton.topologicalSorting[automaton.topSortState2Number[state]].remove(state)
	    del automaton.topSortState2Number[state]
	if transposedGraph.has_key(state):
	    for pred in transposedGraph[state]:
		if automaton.transitionRelation.has_key(pred):
		    if automaton.transitionRelation[pred].has_key(state):
			del automaton.transitionRelation[pred][state]
		    else:
			print "WHY NOT?:", pred, "->", state
	if automaton.transitionRelation.has_key(state):
	    for succ in automaton.transitionRelation[state].keys():
		if state in transposedGraph[succ]:
		    transposedGraph[succ].remove(state)
	    del automaton.transitionRelation[state]
	if state in automaton.stateSet:
	    automaton.stateSet.remove(state)
	if state in automaton.mainStates:
	    automaton.mainStates.remove(state)
	if transposedGraph.has_key(state):
	    del transposedGraph[state]


    def substituteState(betterState, worseState, automaton, transposedGraph):
	for pred in transposedGraph[worseState]:
	    #if automaton.transitionRelation.has_key(pred): # Häh?
	    if not automaton.transitionRelation[pred].has_key(betterState):
		automaton.transitionRelation[pred][betterState] = []
		if not transposedGraph.has_key(betterState):
		    transposedGraph[betterState] = [pred]
		else:
		    transposedGraph[betterState].append(pred)
	    automaton.transitionRelation[pred][betterState] += \
			       automaton.transitionRelation[pred][worseState]
	    automaton.transitionRelation[pred][betterState].sort()
	    Utilities.removeDuplicates(automaton.transitionRelation[pred][betterState])
	    ##if worseState in automaton.transitionRelation[pred][betterState]:
	    ##   automaton.transitionRelation[pred][betterState].remove(worseState)
	    #else:
	    #    print "AND WHY NOT HERE?:", pred
	if worseState == automaton.initialState:
	    automaton.initialState = betterState
	deleteState(worseState, automaton, transposedGraph)
    

    # ZUM DEBUGGEN
    ##automaton.display({}, {'exShape': 'ellipse', 'state2string': \
    ##		   mhStateToString, 'wrapFunc': wrapMH})
    
    transposedGraph = cTG2(automaton.stateSet, lambda q: \
			   automaton.transitionRelation[q].keys())
    # transposedGraph: The transposed automaton graph without edge labels as
    # a Dictionary of (Node, List of Node)
    automaton.topologicalSorting = scc2(automaton.stateSet, lambda q: \
				      automaton.transitionRelation[q].keys(), \
					transposedGraph)    
    
    # Topological sorting for the states: List of list of states
    automaton.topSortState2Number = {}
    # Dictionary of (state, integer)
    for i in range(len(automaton.topologicalSorting)):
	for state in automaton.topologicalSorting[i]:
	    if state in automaton.mainStates:
		automaton.topSortState2Number[state] = i

    # Delete SCCs from which there is no accepting state reachable
    for i in range(len(automaton.topologicalSorting)):
	sccNum = len(automaton.topologicalSorting) - i - 1
	deleteIt = 1
	for state in automaton.topologicalSorting[sccNum]:
	    if automaton.isFinalState(state) and \
	       automaton.transitionRelation[state] != {}:
		deleteIt = 0
	    else:
		for scs in automaton.transitionRelation[state].keys():
		    if (automaton.topSortState2Number[scs] != sccNum):
			deleteIt = 0
	    if not deleteIt:
		break
	if deleteIt:
	    while automaton.topologicalSorting[sccNum] != []:
		state = automaton.topologicalSorting[sccNum][0]
		deleteState(state, automaton, transposedGraph)

    # ZUM DEBUGGEN
    ##print automaton.stateSet
    ##print automaton.transitionRelation
    ##automaton.display({}, {'exShape': 'ellipse', 'state2string': \
    ##		     mhStateToString, 'wrapFunc': wrapMH})
	

    # And weak equivalence again
    for i in range(len(automaton.topologicalSorting)):
	sccNum = len(automaton.topologicalSorting) - i - 1
	for state in automaton.topologicalSorting[sccNum]:
	    equi = weakLeq(state, automaton)
	    if equi != None:
		# ZUM DEBUGGEN		
		##print "Substitute", mhStateToString(equi), "for", \
		##      mhStateToString(state)
		substituteState(equi, state, automaton, transposedGraph)
		# ZUM DEBUGGEN
		##automaton.display({}, {'exShape': 'ellipse', 'state2string': \
		##		       mhStateToString, 'wrapFunc': wrapMH})
    # ZUM DEBUGGEN
    ##print automaton.stateSet
    ##print automaton.transitionRelation
    ##automaton.display({}, {'exShape': 'ellipse', 'state2string': \
    ##		     mhStateToString, 'wrapFunc': wrapMH})
			
    return automaton
			


def weakLeq(state, automaton):
    # state: State
    # automaton: EpsiABA
    "Returns (if possible) a state of automaton != 'state' that can \
     substitute 'state'."
    def isTrivialSCC(state):
	if len(automaton.topologicalSorting[automaton.topSortState2Number[state]]) == 1:
	    if not hasSelfEdge(state):
		return 1
	return 0

    def hasSelfEdge(state):
	return state in automaton.transitionRelation[state].keys()

    def substituteStates(q, state, aList):
	for i in range(len(aList)):
	    if aList[i][0] == state:
		aList[i] = (q, aList[i][1])
	aList.sort()
	Utilities.removeDuplicates(aList)
	return aList

    def isCandidate(q, state):
	# Is state a candidate for substituting q?
	if q == state:
	    return 0
	isAcc = automaton.isFinalState
	if isTrivialSCC(state) or (isAcc(state) == isAcc(q)):
	    return 1
	if isAcc(q) and (not isAcc(state)):
	    if (not (hasSelfEdge(q) or hasSelfEdge(state))) and \
	       (automaton.topSortState2Number[state] == \
		automaton.topSortState2Number[q]) and \
	       (len(automaton.topologicalSorting[automaton.topSortState2Number[q]]) == 2):
		return 1
	return 0
    
    for q in automaton.stateSet:
	if isCandidate(q, state):
	    if automaton.transitionRelation[q] == automaton.transitionRelation[state]:
		return q
	    else: # Does it work with q substituted for state?
		qItems = substituteStates(q, state, \
					  automaton.transitionRelation[q].items())
		stateItems = substituteStates(q, state, \
					      automaton.transitionRelation[state].items())
		if qItems == stateItems:
		    return q
    return None


    

def newEpsiABA(transRel, initial, origEpsiABA):
    # transRel: Dictionary of (MHstate, Dictionary of (MHstate, List of Term))
    # initial: MHstate
    # origEpsiABA: EpsiABA    
    MHauto = EpsiABA('a') # a dummy automaton

    MHauto.name = toString(initial[0][0])
    MHauto.parseTree = origEpsiABA.parseTree
    MHauto.propSet = origEpsiABA.propSet
    MHauto.propSetDict = origEpsiABA.propSetDict
    MHauto.initialState = initial
    MHauto.transitionRelation = transRel
    MHauto.stateSet = transRel.keys()
    ## MHauto.possibleTerms = da müssen wir mal schauen
    MHauto.mainStates = transRel.keys()
    ## MHauto.powerSet = eigentlich überflüssig
    MHauto.isFinalState = MHauto.accFunctionList[2]
    MHauto.isExistentialState = MHauto.true
    MHauto.isUniversalState = MHauto.false

    return MHauto


def term2lbtt(termList, propList):
    # termList: list of terms (ie, vectors of equal length over {0, 1})
    # propList: list of strings
    ## assert (termList != []) -> len(termList[0]) == len(propList)

    out = '| ' * (len(termList) - 1)
    for term in termList:
	termString = ''
	for i in range(len(term)):
	    if term[i] == 1:
		termString = '& ' + termString + propList[i] + ' '
	    elif term[i] == -1:
		termString = '& ' + termString + '! ' + propList[i] + ' '
	if termString == '':
	    if len(term) == 0:
		out += 'f '
	    else:
		out += 't '
	else:
	    out += termString[2:]
    if out == '':
	return 'f'
    else:
	return out[:-1]
    
