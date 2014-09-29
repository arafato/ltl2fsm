# -----------------------------------------------------------------------------
#                                  LTL -> NBA
#              Copyright (C) Carsten Fritz, Björn Teegen 2002-2003
#               Sponsored by Deutsche Forschungsgemeinschaft (DFG)
#     Distributed under the terms of the GNU Lesser General Public License
#         See the files README.txt and LICENSE.txt for more information
# -----------------------------------------------------------------------------

from Constants      import DEBUG, D, S, X, Y, EXISTENTIAL, UNIVERSAL, NOT_FINAL, FINAL, POS_S, POS_PLAYER, POS_P, POS_Q
from Utilities      import getMatrixOfEmptyLists
from BuechiAutomaton import AlternatingBuechiAutomaton
from time           import time

#
# CLASS SimulationGame
#
# SimulationGame computes, for an input Büchi automaton, a game graph (V, E).
# The graph is defined by the method computeEdges which is to be implemented
# in a subclass.

class SimulationGame:
    "This 'abstract' class defines the different simulation games"

##    D           = 0
##    S           = 1
##    X           = 0
##    Y           = 1
##    EXISTENTIAL = 0
##    UNIVERSAL   = 1
##    NOT_FINAL   = 0
##    FINAL       = 1
##    POS_S	= 5
##    POS_PLAYER  = POS_S
##    POS_P       = 2
##    POS_Q	= 3


    def __init__(self, automaton):
        # V is a list of nodes. Nodes are tuples of the form
	# (priority, b, p, q, -1, S), (priority, b, p, q, a, d, y)
	# or (priority, b, p, q, a, s, x, d, y).
        self.V        = []

	# E is a list of successor lists. The i-th successor list
	# contains the successors of the i-th node. 
	self.E        = []

	# P is like E, but for the transposed graph formed by V and E.
	self.P        = []

	# automaton is the automaton given as an argument.
	self.automaton = automaton

	# Vc is a matrix where the elements are lists of tuples.
        # Every node v added to V is also added to the list at matrix
        # position (p,q) together with a "hash value" (the position of that
        # node in V).
	size = len(self.automaton.getStateSet())
	self.Vc = getMatrixOfEmptyLists(size, size)

	# Measure the time.
        self.timeOfAddV = 0

    def getV(self):
        return self.V

    def getE(self):
        return self.E

    def getP(self):
	return self.P

    def getAutomaton(self):
        return self.automaton

    #
    # Adds a vertex v to V iff v is not in V and returns its index number. 
    # Otherwise it will return the index number of the existing equal vertex.
    #
    def addV(self, v, priority = None):
        timer = time()
	if priority == None:
	    priority = self.p((0,v[0])+v[1:])
	vertex = (priority, v[0]) + (v[1:])
	if len(v) == 3:
	    vertex = (priority, v[0]) + (v[1], v[2], -1, S)
	else:
	    vertex = (priority, v[0]) + v[1:]
        w = self.isInV(vertex)
	if w == -1: 
	    w = len(self.V)
	    self.E.append([])
	    self.P.append([])
	    self.V.append(vertex)
	    if vertex[POS_P] >= 0:
	        if vertex[POS_Q] >= 0:
	            self.Vc[vertex[POS_P]][vertex[POS_Q]].append((vertex,w))
        self.timeOfAddV += time()-timer
	return w
    	    
    # 
    # Iff a vertex w in V exists which is equal to v it returns the
    # number of w else -1.
    #
    def isInV(self, v):
	p = v[POS_P]
	q = v[POS_Q]
	if p >= 0:
	    if q >= 0:
  	        for w in self.Vc[p][q]:
	            if v == w[0]:
                        return w[1]
	return -1

    #
    # Adds a new edge to this graph.
    #
    def addEdge(self, v ,w):
	if w not in self.E[v]:
	    self.E[v].append(w) 
	    self.P[w].append(v)

    #
    # Computes all edges of the game graph, without the quasi-selfloops
    #
    def computeGameEdges(self):
	if DEBUG:
	    print "  Calculating game edges ... Time: %f" % time()
        for p in range(len(self.automaton.getStateSet())):
            for q in range(len(self.automaton.getStateSet())):
	        if self.automaton.isExistentialState(p):
	            if self.automaton.isExistentialState(q):
		        self.computeEdges(p, q, S, X, D, Y)
		    else:
		        self.computeEdges(p, q, S, X, S, Y)
	        else:
	            if self.automaton.isExistentialState(q):
		        self.computeEdges(p, q, D, X, D, Y)
		    else:
		        self.computeEdges(p, q, S, Y, D, X)
	if DEBUG:
	    print "  ... game edges calculated. Time: %f" % time()
            print "|V|=",len(self.V),", |E|=",len(self.E)
            print "Time for addV: %f" % self.timeOfAddV
        self.timeOfAddV = 0

    #
    # Each dead end will be connected to a corresponding quasi-selfloop
    #
    def removeDeadEnds(self):
        # Self-loops are not allowed, so we create two nodes with the same
        # priority and connect them. 
	winningPositionAForDuplicator = self.addV((0,-1,0,0,0,1),0)
	winningPositionBForDuplicator = self.addV((0,0,-2,0,0,1),0)
	winningPositionAForSpoiler    = self.addV((0,-3,0,0,1,1),1)
	winningPositionBForSpoiler    = self.addV((0,0,-4,0,1,1),1)
	self.addEdge(winningPositionAForDuplicator, \
		     winningPositionBForDuplicator)
	self.addEdge(winningPositionBForDuplicator, \
		     winningPositionAForDuplicator)
	self.addEdge(winningPositionAForSpoiler, \
		     winningPositionBForSpoiler)
	self.addEdge(winningPositionBForSpoiler, \
		     winningPositionAForSpoiler)

        for number in range(len(self.V)):
	    v = self.V[number]

	    # No outgoing edges of v?
	    if len(self.E[number]) == 0:

	        # Depending on who of the players has to move, 
		# add an edge from v to the respective self-loop.
		if v[POS_PLAYER] == S:
		    self.addEdge(number,winningPositionAForDuplicator)
		else:
		    self.addEdge(number,winningPositionAForSpoiler)

    #
    # Returns TRUE if the given state number belongs to an accepting state.
    #
    def isFinalState(self, stateNumber):
        return self.automaton.isFinalState(stateNumber)

    #
    # {abstract} p
    #
    # def p(self, v):

    #
    # {abstract} computeEdges
    #
    # def computeEdges(self, p, q, s, x, d, y):
              
    #
    # {abstract} computeSimulationRelation
    #
    # def computeSimulationRelation(self):
		
    #
    # Not all kinds of simulation games are useful to build
    # quotient automata.
    #
    def addTransitionsToQuotientAutomaton(self, qABA, sLD, R):
	print "Not implemented"
	





