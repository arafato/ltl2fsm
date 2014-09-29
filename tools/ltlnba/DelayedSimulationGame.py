# -----------------------------------------------------------------------------
#                                  LTL -> NBA
#              Copyright (C) Carsten Fritz, Björn Teegen 2002-2003
#               Sponsored by Deutsche Forschungsgemeinschaft (DFG)
#     Distributed under the terms of the GNU Lesser General Public License
#         See the files README.txt and LICENSE.txt for more information
# -----------------------------------------------------------------------------

from SimulationGame   import SimulationGame
from Constants        import DEBUG, D, S, X, Y, EXISTENTIAL, UNIVERSAL, NOT_FINAL, FINAL, POS_S, POS_PLAYER, POS_P, POS_Q, POS_B
from QuotientAutomaton import getMinimumSuccessorList
from Jurdzinski       import Jurdzinski
from time import time
	
#
# CLASS DelayedSimulationGame
#

class DelayedSimulationGame(SimulationGame):
    "This class represents delayed simulation games"

##    POS_B        = 1

    def __init__(self, automaton):
	if DEBUG:
	    print "Constructing delayed simulation game ..."
	SimulationGame.__init__(self, automaton)
        if DEBUG:
            print "Starting to compute game edges. Time: %f" % time()
	self.computeGameEdges()
        if DEBUG:
            print "Finished computing game edges. Time: %f" % time()
	self.removeDeadEnds()
	if DEBUG:
            print "... delayed simulation game constructed. Time: %f" % time()

    def p(self, v):
	if len(v) >= (5+2):
             return 2
	return v[POS_B]

    def computeEdges(self, p, q, s, x, d, y):
	for a in range(self.automaton.getAlphabetSize()):
	    if self.isFinalState(q):
	        self.computeEdgeV0V1([p,q], a, s, x, d, y, 0)
	    else:
		self.computeEdgeV0V1([p,q], a, s, x, d, y, 1)
	        if not self.isFinalState(p):
	            self.computeEdgeV0V1([p,q], a, s, x, d, y, 0)	
	    
    def computeEdgeV0V1(self, pq, a, s, x, d, y, b):
	v0 = self.addV((b, pq[0], pq[1]))
	v1 = self.addV((b, pq[0], pq[1], a, s, x, d, y))
	self.addEdge(v0, v1)
	successorList = self.automaton.getSuccessorList(pq[x], a)
	for successor in successorList:  
	    pq[x] = successor
            self.computeEdgeV1V2(v1, pq, a, x, d, y, b)

    def computeEdgeV1V2(self, v1, pq, a, x, d, y, b):
	if self.isFinalState(pq[x]):
	    v2 = self.addV((y, pq[0], pq[1], a, d, y))
	else:
	    v2 = self.addV((b, pq[0], pq[1], a, d, y))
 	self.addEdge(v1, v2)
	b = self.V[v2][POS_B]
	successorList = self.automaton.getSuccessorList(pq[y], a)
	t = pq[y]
	for j in successorList: 
	    pq[y] = j
	    self.computeEdgeV2V3(v2, pq, x, y, b)
	pq[y] = t

    def computeEdgeV2V3(self, v2, pq, x, y, b):
	if self.isFinalState(pq[y]):
	    v3 = self.addV((x, pq[0], pq[1]))
	else:
	    v3 = self.addV((b, pq[0], pq[1]))
	self.addEdge(v2, v3)

    #
    # This is a method for computing the simulation relation
    #
    def computeSimulationRelation(self):
        jurdzinski = Jurdzinski(self)
	jurdzinski.compute()
        return jurdzinski.getRelation(len(self.automaton.getStateSet()))


    def addTransitionsToQuotientAutomaton(self, qABA, sLD, R):
	for p in range(len(self.automaton.getStateSet())):
            if self.automaton.isExistentialState(p):	    
	        for a in range(self.automaton.alphabet):
	            successorList = self.automaton.getSuccessorList(p, a)
	            for q in successorList:
			qABA.addTransition((sLD[p],a,sLD[q]))

	for p in range(len(self.automaton.getStateSet())):
            if qABA.isUniversalState(sLD[p]):
	        for a in range(self.automaton.alphabet):
	            successorList = self.automaton.getSuccessorList(p, a)
		    minSuccessorList = getMinimumSuccessorList(successorList, R)
	            for q in minSuccessorList:
			qABA.addTransition((sLD[p],a,sLD[q]))
    
    def toString(self):
        if DEBUG:
            print "# Delayed simulation game:"
	self.printAttributes()


















