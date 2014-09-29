# -----------------------------------------------------------------------------
#                                  LTL -> NBA
#              Copyright (C) Carsten Fritz, Björn Teegen 2002-2003
#               Sponsored by Deutsche Forschungsgemeinschaft (DFG)
#     Distributed under the terms of the GNU Lesser General Public License
#         See the files README.txt and LICENSE.txt for more information
# -----------------------------------------------------------------------------

import os
import re
import string
import sys

from Constants import EXISTENTIAL, UNIVERSAL, FINAL, NOT_FINAL, TRUE, FALSE, DEBUG
from Utilities import readAllLinesFromFile, writeAllLinesToFile, \
		      getNextToken, scc


ParseError = 'Parse Error'


#		    
# CLASS BuechiAutomaton
#

class BuechiAutomaton:
    "This class represents Buechi automata"


    def __init__(self, fileName = None):
        "Buechi automata are defined by their transitionRelation"

        self.name = None

        self.alphabet = []

	self.stateSet = {}
        # stateSet: Dict of (label, tupel of (mode, acceptance))
	
	self.initialState = None

        self.transitionRelation = {}
	# transitionRelation: Dict of (state, Dict of (letter, List of state))

	self.sL = []
	self.sL2 = []
        # sL, sL2: List of list of int.
        # The elements of sL and sL2 (lists of int) can be regarded as sets
        # containing the 1st and 2nd argument, respectively, of the
        # Miyano-Hayashi construktion for the states of the NBA.

        if fileName != None:
            f = open(fileName, 'r')
            s = f.read()
            f.close()
            self.parse(s)


    def getStateSet(self):
       return self.stateSet.keys()

    def __lex(self, s):
        'lex(string) -> list of string'

        tokens = re.split('\\s', s)
        tokens = filter(lambda token: token != '', tokens)
        return tokens


    def __expect(self, token, s):
        if token != s:
            raise ParseError, "'" + s + "' expected"


    def parse(self, s):
        """parse(string)

Sets the automaton's fields.

ABA descriptions are defined by the following grammar:

description:
  'name:' from-name

from-name:
  'alphabet:' from-alphabet

from-alphabet:
  'state-set:' from-state-set
  string 'state-set:' from-state-set

from-state-set:
  'initial-state:' from-initial-state
  string string 'initial-state:' from-initial-state

from-initial-state:
  string 'transition-relation:' from-transition-relation

from-transition-relation:
  end-of-input
  string string string from-transition-relation"""

        self.name = None
        self.alphabet = []
        self.stateSet = {}
        self.initialState = None
        self.transitionRelation = {}

        tokens = self.__lex(s)
        try:
            self.__expect(tokens[0], 'name:')
            self.parseFromName(tokens[1:])
        except IndexError:
            raise ParseError, 'Unexpected end of input'


    def parseFromName(self, tokens):
        self.name = tokens[0]
        self.__expect(tokens[1], 'alphabet:')
        self.parseFromAlphabet(tokens[2:])


    def parseFromAlphabet(self, tokens):
        symbol = tokens[0]
        while symbol != 'state-set:':
            tokens = tokens[1:]
            self.addSymbol(symbol)
            symbol = tokens[0]
        self.parseFromStateSet(tokens[1:])


    def parseFromStateSet(self, tokens):
        label = tokens[0]
        while label != 'initial-state:':
            attrs = tokens[1]
            tokens = tokens[2:]
            self.addState(label, 'u' in attrs, 'f' in attrs)
            label = tokens[0]
        self.parseFromInitialState(tokens[1:])


    def parseFromInitialState(self, tokens):
        try:
            self.setInitialState(tokens[0])
        except ValueError, s:
            raise ParseError, s
        self.__expect(tokens[1], 'transition-relation:')
        self.parseFromTransitionRelation(tokens[2:])


    def parseFromTransitionRelation(self, tokens):
        while tokens != []:
            try:
                (fromState, symbol, toState) = tokens[:3]
                self.addTransition(fromState, symbol, toState)
            except ValueError, s:
                raise ParseError, s
            tokens = tokens[3:]


    def loadAutomatonAbaOld(self, fileName):
        f = open(fileName, 'r')
        lines = f.readlines()
        f.close()

	state = 0
        fromState = 0
	for line in lines:
            tokens = line.split()
            token = getNextToken(tokens)
	    if token == "<name>":
	        self.name = getNextToken(tokens)
	    elif token == "<alphabet>":
                size = int(getNextToken(tokens))
                self.alphabet = map(lambda c: chr(ord('a') + c), range(size))
	    elif token == "<initial>":
		self.initialState = getNextToken(tokens)
	    elif token == "<s>":
                universal = FALSE
                final = FALSE
                while tokens != []:
                    token = getNextToken(tokens)
                    if token == "u":
                        universal = TRUE
                    if token == "f":
                        final = TRUE
                self.addState(str(state), universal, final)
                state += 1
	    elif token == "<t>":
                a = 0
                while tokens != []:
                    token = getNextToken(tokens)
                    if token == "/":
                        a += 1
                    else:
                        toState = int(token)
                        self.addTransition(str(fromState), chr(ord('a') + a),
                                           str(toState))
                fromState += 1


    def writeAba(self, f):
        'A.writeAba(file)'

        f.write('name: ' + self.getName() + '\n')

        f.write('alphabet:')
        for symbol in self.getAlphabet():
            f.write(' ' + symbol)
        f.write('\n')
        f.write('\n')

        f.write('state-set:\n')
        for state in self.getStates():
            f.write(state + ' ')
            if self.isUniversalState(state): f.write('u')
            else: f.write('e')
            if self.isFinalState(state): f.write('f')
            f.write('\n')
        f.write('\n')

        initialState = self.getInitialState()
        f.write('initial-state: ' + initialState + '\n')
        f.write('\n')

        f.write('transition-relation:\n')
        for transition in self.getTransitions():
            (fromState, symbol, toState) = transition
            f.write(fromState + ' ' + symbol + ' ' + toState + '\n')


    def writeDot(self, f, userOptions = {}, autoOptions = {}):
        'A.writeDot(file, [hash])'

        options = {'rankdir': 'LR', 'orientation': 'landscape'}
        options.update(userOptions)

        f.write('digraph "' + self.getName() + '" {\n')
        for option in options.keys():
            f.write('    ' + option + '="' + options[option] + '";\n')
        f.write('\n')

        initialState = self.getInitialState()
        if initialState != None:
            f.write('    init [label="", shape=plaintext];\n')
        for state in self.getStates():
            line = '    ' + state
            attributes = ['label="' + state + '"',
                          'fontname="TimesNewRoman"']
            if self.isExistentialState(state):
                attributes.append('shape=diamond')
            elif self.isUniversalState(state):
                attributes.append('shape=box')
            if self.isFinalState(state):
                attributes.append('peripheries=2')
            line += ' [' + string.join(attributes, ', ') + '];\n'
            f.write(line)
        f.write('\n')

        if initialState != None:
            f.write('    init -> ' + initialState + ';\n')
        for fromState in self.getStates():
            for toState in self.getSuccessors(fromState):
                symbols = self.getAllSymbols(fromState, toState)
                f.write('    ' + fromState + ' -> ' + toState +
                        ' [label="' + string.join(symbols, ', ') +
                        '", fontname="TimesNewRoman"];\n')

        f.write('}\n')


    def saveAutomatonAba(self, fileName):
        """A.saveAutomatonAba(string)

Save this Buechi automaton in .aba format."""

        f = open(fileName, 'w')
        self.writeAba(f)
        f.close()


    def saveAs(self, format, fileName, userOptions = {}, autoOptions = {}, \
	       dotPath = "./"):
        # format: String ("ps", "gif" etc.)
        """A.saveAs(string, string, [hash])

Save this Buechi automaton in the given format."""
        f = os.popen(dotPath + 'dot -T' + format + ' -o "' + fileName + '"', \
	    'w')
        self.writeDot(f, userOptions, autoOptions)
        self.writeDot(sys.stdout, userOptions, autoOptions)
        return f.close()


#
# Save as a PROMELA never-claim
#
    def saveAutomatonPromela(self, fileName, options={}):
	"""A.saveAutomatonPromela(string, [directory])

Save the automaton in the format of a Promela never-claim."""
	if options.has_key("appendTo"):
	    g = open(options["appendTo"], 'r')
	    f = open(fileName, 'w')
	    allLines = g.readlines()
	    f.writelines(allLines)
	    f.close()
	    g.close()
	    f = open(fileName, 'a')
	else:
	    f = open(fileName, 'w')

	if (not options.has_key("letters")):
	    options["letters"] = string.lowercase
    
	self.writeAutomatonPromela(f, options["letters"])
	


    def writeAutomatonPromela(self, f, letterList=string.lowercase):
	"""A.writeAutomatonPromela(file)

Write the automaton in the format of a Promela never-claim.\
We assume that the transitionRelation is in the format \
Dict of (state, Dict of (state, list of term))."""
	termLen = 0
	if len(self.transitionRelation[self.initialState].keys()) != 0:
	    termLen = len((self.transitionRelation[self.initialState].keys())[0])

	f.write('never {\t/* ' + self.name + ' */\n')

	# make a copy first
	isInit = 0; acceptAll = -1
	stateList = [0] * len(self.stateSet)
	for i in range(len(self.stateSet)):
	    stateList[i] = self.stateSet[i]
	    if stateList[i] == self.initialState:
		isInit = i
	    if stateList[i][0][0] == 'tt':
		acceptAll = i
	if acceptAll == 0: # Holzauge, sei wachsam!
	    acceptAll = isInit
		
	# initial state first, tt last
	dummy = stateList[isInit]
	stateList[isInit] = stateList[0]
	stateList[0] = dummy
	if acceptAll != -1:
	    dummy = stateList[acceptAll]
	    stateList[acceptAll] = stateList[-1]
	    stateList[-1] = dummy
	
	# make a dict (state, number)
	stateIDdict = {}
	i = 0
	for q in stateList:
	    stateIDdict[q] = i
	    i += 1

	# compute list of state names
	stateNameList = [0] * len(stateList)
	for i in range(len(stateList)):
	    q = stateList[i]
	    qName = ""
	    if self.isFinalState(q):
		qName += 'accept_'
	    if q[0][0] == 'tt':
##	       (self.isFinalState(q)) and \
##	       (self.transitionRelation[q].has_key(q)) and \
##	       (((0,)*termLen) in self.transitionRelation[q][q]):
		qName += 'all'
	    else: 
		qName += ('T'+str(i))
	    if (i == 0):
		qName += '_init'
	    stateNameList[i] = qName

	# transitions
	for i in range(len(stateList)):
	    if (stateNameList[i].find('accept_all') != -1):
		f.write(stateNameList[i] + ':\n\tskip\n')
	    else:
		f.write(stateNameList[i] + ':\n\tif\n') 
		q = stateList[i]
		byState = self.transitionRelation[q]
		for toState in byState.keys():
		    for term in byState[toState]:
			# compute condition
			condition = ""
			for j in range(len(term)):
			    if (term[j] == 1):
				condition += (letterList[j] + ' && ')
			    elif (term[j] == -1):
				condition += ('(! ' + letterList[j] + ') && ')
			if condition == "":
			    condition = "(1)"
			else:
			    condition = '(' + (condition[:-4]) + ')'

			f.write('\t:: ' + condition + ' -> goto ' + \
				stateNameList[stateIDdict[toState]] + '\n')
		f.write('\tfi;\n')
	f.write('}\n')
			



    #
    # Saves this buechi automaton in .daVinci-format 
    #
    def saveAutomatonDaVinci(self, fileName):
        """A.saveAutomatonDaVinci(string)

Save this Buechi automaton in .daVinci format."""

        state = FALSE
	daVinci = "["
	daVinci += "l(\"\",n(\"\",[a(\"OBJECT\",\"init\"),a(\"_GO\",\"text\")]"
        daVinci += ",[l(\"i\",e(\"\",[a(\"\",\"\"),a(\"\",\"\")]"
	daVinci += ",r(\""+str(self.getInitialState())+"\")))])),"
	for s in range(len(self.stateSet)):
	    stateTerm = self.__writeState(s)
	    if state:
	        daVinci += ","
	    if len(self.sL) != 0:
		label = self.sL[s]
		stateLabel = str(label[0])
		for i in range(1,len(label)):
		    stateLabel += ","+str(label[i])

		if len(self.sL2) != 0:
		    label = self.sL2[s]
		    stateLabel += "\\n"+str(label[0])
		    for i in range(1,len(label)):
		        stateLabel += ","+str(label[i])
 	    else:
		stateLabel = str(s)
            daVinci += "l(\""+str(s)+"\",n(\"\",["
	    daVinci += "a(\"OBJECT\",\""+stateLabel+"\")"
	    if self.isExistentialState(s):
		daVinci += ",a(\"_GO\",\"rhombus\")"
	    if self.isFinalState(s):
		daVinci += ",a(\"BORDER\",\"double\")"
	    daVinci += "],["
	    daVinci += stateTerm
	    daVinci += "]))"
	    state = TRUE
	daVinci  += "]"
	writeAllLinesToFile(fileName, [daVinci])


    def __writeState(self, stateI):
	edge = FALSE
	string = ""
	for t in range(len(self.stateSet)):
            edgeTerm = self.__writeEdges(stateI, t)
	    if len(edgeTerm) != 0:
	 	if edge:
		    string += ","
		string += edgeTerm
		edge = TRUE 
	if len(string) == 0:
	    return ""
	else:
	    return string #+"\n"
	

    def __writeEdges(self, stateI, stateJ):
	character = FALSE
	label = ""
	for c in range(self.getAlphabetSize()):
	    sL = self.getSuccessorList(stateI,c)
	    if stateJ in sL:
		if character:
		    label += ","
	        label += str(c)
		character = TRUE
	if len(label) == 0:
	    return ""
	else:
            return "l(\""+str(stateI)+"->"+str(stateJ)+"\",e(\"\",[a(\"OBJECT\",\""+label+"\"),a(\"EDGEPATTERN\",\"single;solid;1;1\")],r(\""+str(stateJ)+"\")))"


    def addSymbol(self, symbol):
        """A.addSymbol(string)

Adds a new symbol to the alphabet."""

        self.alphabet.append(symbol)


    def addState(self, label, universal, final):
        """A.addState(string, boolean, boolean)

Adds a new state."""

        self.stateSet[label] = (universal, final)
        self.transitionRelation[label] = {}


    def addTransition(self, fromState, symbol, toState):
        """A.addTransition(string, string, string)

Adds a new transition."""
        bySymbol = self.transitionRelation[fromState]
        if not bySymbol.has_key(symbol):
            bySymbol[symbol] = [toState]
        else:
            bySymbol[symbol].append(toState)


    def getName(self):
        """A.getName() -> string
        
Return the name."""

        return self.name


    def setName(self, name):
        """A.setName(string)

Set the name."""
        self.name = name


    def getAlphabet(self):
        """A.getAlphabet() -> list of string
        
Return a list of all symbols of the alphabet."""

        return self.alphabet


    def getStates(self):
        """A.getStates() -> list of string

Return a list of all states."""

        return self.stateSet.keys()


    def isUniversalState(self, state):
        """A.isUniversalState(string) -> boolean

Return wheather the given state is universal."""

        return self.stateSet[state][0]


    def isExistentialState(self, state):
        """A.isExistentialState(string) -> boolean

Return wheather the given state is existential."""

        return not self.isUniversalState(state)


    def isFinalState(self, state):
        """A.isFinal(string) -> boolean

Return wheather the given state is final."""

        return self.stateSet[state][1]


    def getInitialState(self):
        """A.getInitialState() -> string

Return the initial state."""

        return self.initialState


    def setInitialState(self, state):
        """A.setInitialState(string)

Set the initial state."""
        self.initialState = state


    def getTransitions(self):
        """A.getTransitions() -> list of (string, string, string)

Return a list of all transitions.  Each transition is a tuple of the
form (fromState, symbol, toState)."""

        transitions = []
        for fromState in self.transitionRelation:
            bySymbol = self.transitionRelation[fromState]
            for symbol in bySymbol.keys():
                for toState in bySymbol[symbol]:
                    transitions.append((fromState, symbol, toState))

        return transitions


    def getSuccessors(self, fromState):
        """A.getSuccessors(string) -> list of string

Return a list of all the successor states."""

        successors = {}
        bySymbol = self.transitionRelation[fromState]
        for symbol in bySymbol.keys():
            for toState in bySymbol[symbol]:
                successors[toState] = ()

        return successors.keys()


    def getAllSymbols(self, fromState, toState):
        """A.getAllSymbols(string, string) -> list of string

Return a list of all symbols from fromState to toState."""

        symbols = []
        bySymbol = self.transitionRelation[fromState]
        for symbol in bySymbol.keys():
            if toState in bySymbol[symbol]:
                symbols.append(symbol)

        return symbols


    def __str__(self):
        return self.name


    def getSuccessorList(self, stateI, character = None):
        characterDict = self.transitionRelation[stateI]
	if character == None:
	    sL = []
	    for character in range(self.alphabet):
	        sLC = self.getSuccessorList(stateI, character)
	        for c in sLC:
		    if c not in sL:
			sL.append(c)
	    return sL
        elif characterDict.has_key(character):
	    return characterDict[character] 
	return []


    def setSuccessorList(self, stateI, character, successorList = None):
        characterDict = self.transitionRelation[stateI]
	if characterDict.has_key(character):
	    characterDict[character] = successorList

 
    def removeNotReachableStates(self, iState = None):
	if iState == None:
	    iState = self.getInitialState() 
        markList = len(self.stateSet)*[FALSE]
        self.__markStates(markList, iState)
        for i in range(len(markList)):
	    position = len(markList)-1-i
            if not markList[position]:
                self.removeState(position)


    def __markStates(self, markList, state):
        markList[state] = TRUE
        sL = []
        for a in range(self.getAlphabetSize()): 
            successorList = self.getSuccessorList(state, a)
	    for successor in successorList:
	        if successor not in sL:
		    sL.append(successor)
        for state in sL:
	    if not markList[state]:
	        self.__markStates(markList, state)	


    def removeState(self, stateNumber):
	iState = self.getInitialState()
	if iState != None:
	    if iState > stateNumber:
		self.setInitialState(iState-1)
	    if iState == stateNumber:
		self.setInitialState(None)

	if self.sL != []:
	    self.sL[stateNumber:stateNumber+1] = []

        self.stateSet[stateNumber:stateNumber+1] = []
	self.transitionRelation[stateNumber:stateNumber+1] = []
	for state in range(len(self.stateSet)):
	    for a in range(self.getAlphabetSize()):
	        newSuccessorList = []
		oldSuccessorList = self.getSuccessorList(state,a)
		for successor in oldSuccessorList:
		    if successor > stateNumber:
		        newSuccessorList.append(successor-1)
		    if successor < stateNumber:
		        newSuccessorList.append(successor)
		self.setSuccessorList(state, a, newSuccessorList)


    def isWeakAutomaton(self):
        G = self.__toGraph()
	sccComponents = scc(G)
	weak = TRUE
	for component in sccComponents:
	    weak &= self.__allInFOrQWithoutF(component)
	return weak


    def __toGraph(self):
	G = []
	for state in range(len(self.stateSet)):
	    G.append(self.getSuccessorList(state))
	return G


    def __allInFOrQWithoutF(self, component):
	mode = self.getFinalMode(component[0])
	for v in component:
            if self.getFinalMode(v) != mode:
	        return 0
	return 1	


    def getFinalModeFromList(self, stateList):
	for state in stateList:
            if self.isFinalState(state):
	        return FINAL
	return NOT_FINAL


    def getAlphabetSize(self):
        return len(self.alphabet)


    def toString(self):
        if DEBUG:
            print "# Buechi Automaton: (",
            if not self.isWeakAutomaton():
                print "not",
                print "weak )"
            self.printAttributes()


    def printAttributes(self):
	print "  Initial state:"
	print "   ", self.initialState
	print "  State set:", \
              len(self.stateSet),"(0=E;1=U , 0=notF;1=F)"
	print "   ", self.stateSet
	print "  Transition relation:"
        print "   ", self.transitionRelation

#
# CLASS AlternatingBuechiAutomaton
#

class AlternatingBuechiAutomaton(BuechiAutomaton):
    "This class represents alternating Buechi automata"


    def __init__(self, fileName = None):
        BuechiAutomaton.__init__(self, fileName)


    def toBuechiAutomaton(self):
	T = len(self.stateSet)*[0]
	T[self.initialState] = 1
	Q = len(self.stateSet)*[0]
	if self.isNotFinalState(self.initialState):
	    Q[self.initialState] = 1
	state = (T,Q)
	V = []
	V.append(state)
	E = []
	go = 1
	c = 0
	while go == 1:
	    for a in range(self.getAlphabetSize()):
	        delta = self.__calculateDelta(V[c][0], a)
		if delta == [[-1]]:
		    T2 = [len(self.stateSet)*[0]]
		elif delta != []:
	            T2 = self.__calculateT2(delta)
		else:
		    T2 = []
	        delta = self.__calculateDelta(V[c][1], a)
		if delta == [[-1]]:
		    Q2 = [len(self.stateSet)*[0]]
		elif delta != []:
	            Q2 = self.__calculateT2(delta)
		else:
		    Q2 = []
		#print "---------------------------"
		#print V[c][0],V[c][1]
	        for t2 in T2:
		    for q2 in Q2:
			#print t2,q2,
			f = FALSE
			if t2 == [0,1,0,0,0,1]:
			    if q2 == [0,1,0,0,0,0]:
				f = 0 #TRUE
			if V[c][0] == [0,1,0,0,0,1]:
			    if V[c][1] == [0,1,0,0,0,0]:
				f = TRUE
			match = 1
			empty = 0
			for i in range(len(t2)):
			    if q2[i] == 1:
				if t2[i] == 0:
				    match = 0
			        if self.isFinalState(i):
				    q2[i] = 0
				empty = 1
			if empty == 0:
			    q2 = []
			    for i in range(len(t2)):
				if self.isFinalState(i):
				    q2.append(0)
				else:
				    q2.append(t2[i])
			#print f,match,a,
			if match == 1:
	                    state = (t2,q2)
			    edge = (a,V[c],state)
			    if edge not in E:
				E.append(edge)
				#print "E",
	                    if state not in V:
		                V.append(state)
				#print "V",
#			print 
	    c += 1
	    if c == len(V):
		go = 0
	ba = BuechiAutomaton()
	ba.setInitialState(self.getInitialState())
	ba.setName(self.getName())
	ba.setAlphabetSize(self.getAlphabetSize())
	VD = {}
	for i in range(len(V)):
	    VD[str(V[i])] = i
	    mode = FINAL
	    Q = V[i][1]
	    for q in Q:
	        if q:
		    mode = NOT_FINAL
		    break
	    ba.addState(mode)
	    ll = []
	    for o in range(len(V[i][0])):
		if V[i][0][o]:
		    ll.append(o)
	    lll = []
	    for o in range(len(V[i][0])):
		if V[i][1][o]:
		    lll.append(o)
	    if ll == []:
		ba.sL.append(["empty"])
	    else:
	        ba.sL.append(ll)
	    if lll == []:
		ba.sL2.append(["empty"])
	    else:
	        ba.sL2.append(lll)
	for i in range(len(E)):
	    character = E[i][0]
	    stateI = VD[str(E[i][1])]
	    stateJ = VD[str(E[i][2])]
	    ba.addTransition((stateI, character, stateJ))
	return ba


    def __calculateDelta(self, T, a):
	T2 = []
	result = 1
	for t in range(len(T)):
	    if T[t]:
	        sL = self.getSuccessorList(t,a)
	        if self.isUniversalState(t):
		    for state in sL:
		        if state not in T2:
		            T2.append([state])
	        else:
		    if sL == []:
		        return []
		    elif sL not in T2:
		        T2.append(sL)
	if (T2 == []) & (result == 1):
	    T2 = [[-1]]
	return T2


    def __calculateT2(self, delta):
	f = FALSE
	if delta == [[1,3],[5]]:
	    f = TRUE
	T = []
	T2 = len(self.stateSet)*[0]
	found = 1
	while found == 1:
	    found = 0
	    for i in delta:
	        if len(i) == 1:
		    found = 1
		    state = i[0]
		    T2[state] = 1
		    for j in range(len(delta)): 
			if state in delta[j]:
			    delta[j] = []
	#if f: print T2,delta
	counter = len(delta)*[0]
	over = 0
	while over == 0:
	    T2b = []
	    for i in T2:
		T2b.append(i)
	    over = 1
	    #if f:print T2b
	    for i in range(len(delta)):
		#if f:print counter
		if len(delta[i]) != 0:
	            counter[i] += over
	            if counter[i] >= len(delta[i]):
		        over = 1
		        counter[i] = 0
		    else:
		        over = 0
		    match = 0
		    for j in delta[i]:
			if T2b[j]:
			    match = 1
			    break
		    if match == 0:
			j = delta[i]
		        T2b[j[counter[i]]] = 1
	    #if f:print "*",T2b
	    T.append(T2b)
	#if f: print T
	return T


    def toString(self):
        if DEBUG:
            print "# Alternating buechi Automaton: (",
            if not self.isWeakAutomaton():
                print "not",
                print "weak )"
            self.printAttributes()


def convert(fileName):
    a = BuechiAutomaton()
    a.loadAutomatonAbaOld(fileName)
    a.saveAutomatonAba(fileName)
