# -----------------------------------------------------------------------------
#                                  LTL -> NBA
#              Copyright (C) Carsten Fritz, Björn Teegen 2002-2003
#               Sponsored by Deutsche Forschungsgemeinschaft (DFG)
#     Distributed under the terms of the GNU Lesser General Public License
#         See the files README.txt and LICENSE.txt for more information
# -----------------------------------------------------------------------------

from DelayedSimulationGame   import DelayedSimulationGame
from SimulationGame          import SimulationGame
from Constants               import DEBUG
from QuotientAutomaton       import getMinimumSuccessorList
from EpsiJurdzinski          import EpsiJurdzinski
from time                    import time
from Utilities               import Queue, tuplify
from LTLUtils                import termImplies, toString, gamePos2String
from SimulationRelation      import SimulationRelation
	
#
# CLASS EpsiDelayedSimGame
#

POS_PRIORITY = 0
SPO = 1
DUP = 2
DE_BIT = 3
TERM = 4
MOVE_WHO = 5
MOVE_WHERE = 6
FINISHED = 7

NO_TERM = 4
HAS_TERM = 8

S = 0
D = 1

class EpsiDelayedSimGame(DelayedSimulationGame):
    "This class represents delayed simulation games for Epsi-ABA"

    def __init__(self, automaton): # automaton: EpsiABA
        if DEBUG:
	    print "Constructing delayed simulation game ..."
        SimulationGame.__init__(self, automaton)
        self.V = {} # V: Dictionary of (Position, List of Position).
                    # Positions are Tuples (see below)
        if DEBUG:
            print "Starting to compute game edges."
        time0 = time()
	self.computeGameEdges()
        time0 = time() - time0
        if DEBUG:
            print "Finished computing game edges. Time: %f seconds" % time0
	self.removeDeadEnds()
	if DEBUG:
            print "... delayed simulation game constructed. Time: %f" % time()

        self.Jurd_V = self.V.keys()
        self.Jurd_V.sort()
        self.pos2numberDict = self.__makePos2Number()
##        if DEBUG:
##            print "Positionen <-> Nummern"
##            for pairs in self.pos2numberDict.items():
##                print pairs[1], " : ", pairs[0]
        self.Jurd_E = self.__makeJurd_E()
        self.Jurd_P = self.__makeJurd_P()
##        if DEBUG:
##            print "Das Spielbrett"
##            for i in range(len(self.Jurd_E)):
##                print i, " : ", self.Jurd_E[i]
        "Looks good so far."


    def getV(self):
        return self.Jurd_V

    def getE(self):
        return self.Jurd_E

    def getP(self):
        return self.Jurd_P

    def __makePos2Number(self):
        "Creates a Dictionary (Position as tuple -> position as number)"
        out = {}
        for i in range(len(self.Jurd_V)):
            out[self.Jurd_V[i]] = i
        return out

    def __makeJurd_E(self):
        "Creates a list of lists of positions as numbers.\
         out[i] is the list of successors of position no. i"
        out = [0] * len(self.Jurd_V)
        for i in range(len(self.Jurd_V)):
            out[i] = self.__numberize(self.V[self.Jurd_V[i]])
        return out

    def __numberize(self, tupleList):
        "Given a list of positions as tuples, a list of these positions as \
         numbers is returned"
        out = []
        for t in tupleList:
            out.append(self.pos2numberDict[t])
        return out

    def __makeJurd_P(self):
        "Computes E transposed"
        out = [[]] * len(self.Jurd_E)
        for i in range(len(self.Jurd_E)):
            for j in self.Jurd_E[i]:
                if not (i in out[j]):
                    out[j] = out[j] + [i]
        return out


    def computeGameEdges(self):
	for p in self.automaton.mainStates:
	    for q in self.automaton.mainStates: 
		if DEBUG:
                    print "Current pair: " + toString(p) + " and " + \
                          toString(q)
                b = self.de_bit(p, q)
                self.__makeGame(Queue((b, p, q, b)))
        if DEBUG:
            print "Number of positions:" + str(len(self.V.keys()))
	    for fromPos in self.V.keys():
		for toPos in self.V[fromPos]:
		    print gamePos2String(fromPos) + " -> " + gamePos2String(toPos)


    def __makeGame(self, q): # q: Queue
       "To compute the positions reachable from the positions in q"
       # A position consists of
       # (priority, state Sp., state D., de-bit) or
       # (pri., stateSp, stateDup, de-bit {0, 1}, term, Who moves? {S, D},
       # In which pos.? {0, 1, 2}, Where are we done? {-1, 0, 1, 2} where
       # -1: nowhere, 2: for both pebbles (an auxiliary value)
       ## The MOVE_WHERE-value 2 indicates that the active player can choose
       ## the pebble that he wants to move.
       while not q.isEmpty():
           pos = q.dequeue()
           if not self.V.has_key(pos):
               # pos is new position
	       if DEBUG:
                   print gamePos2String(pos)
               self.V[pos] = []
               if (len(pos) == NO_TERM): # no term chosen so far
                   whatMove = self.determineMove(pos) # Tuple of ({S,D}, {0,1,2})
                   for t in self.automaton.possibleTerms[pos[SPO]]:
                       nextPos = (2, ) + pos[1:] + (tuplify(t), ) + \
                                 whatMove + (-1, )
                       q.enqueue(nextPos)
                       self.V[pos].append(nextPos)
               else: # there is a term
                   ## Added for MOVE_WHERE == 2
                   if (pos[MOVE_WHERE] == 2):
                       whereList = [0, 1]
                   else:
                       whereList = [pos[MOVE_WHERE]]
                   for where in whereList:
                   ## END Added for MOVE_WHERE == 2
                   ## (and 'where' instead of 'pos[MOVE_WHERE]' in the lower
                   ## block)
                       edges = self.automaton.transitionRelation[pos[where+1]].items()
                       for qa in edges: # qa: Tuple (State, List of Term)
                           if ([] in qa[1]): # there is epsi-transition 
                               if (where == 0):
                                   nextPos = (pos[POS_PRIORITY], qa[0]) + pos[DUP:]
                               elif (where == 1):
                                   nextPos = pos[:DUP] + (qa[0], ) + pos[DE_BIT:]
                               whatMove = self.determineMove(nextPos)
                               nextPos = nextPos[:MOVE_WHO] + whatMove + \
                                         nextPos[FINISHED:]
                               q.enqueue(nextPos)
                               self.V[pos].append(nextPos)
                           if termImplies(pos[TERM], qa[1]):
                           # There is usable term transition
                           # This does NOT exclude the possibility that there
                           # also is an epsi-transition.
                               finished = pos[FINISHED]
                               if (finished == -1):
                                   finished = where
                               else:
                                   finished = 2
                               if (where == 0):
                                   nextPos = (pos[POS_PRIORITY], qa[0]) + \
                                             pos[DUP:FINISHED] + (finished, )
                               elif (where == 1):
                                   nextPos = pos[:DUP] + (qa[0], ) + \
                                             pos[DE_BIT:FINISHED] + (finished, )
                               if (nextPos[FINISHED] == 2):
                                   bit = self.de_bit_WithHistory(nextPos[SPO], \
                                                                 nextPos[DUP], \
                                                                 nextPos[DE_BIT])
                                   nextPos = (bit, ) + nextPos[SPO:DE_BIT] + \
                                             (bit, )
                               else:
                                   whatMove = self.determineMove(nextPos)
                                   nextPos = nextPos[:MOVE_WHO] + whatMove + \
                                             nextPos[FINISHED:]
                               ## Note that pos[MOVE_WHERE] is not copied
                               ## to nextPos.
                               q.enqueue(nextPos)
                               self.V[pos].append(nextPos)
                           else:
                           ## If the tighten-method (called in the
                           ## constructor of EpsiABA) causes a term transition
                           ## at a universal state, but the chosen term does
                           ## not imply that transition's term, the player
                           ## with the pebble on that state has lost.
                           ## This part is only to be used in connection with
                           ## a former call of the tighten-method.
                               if self.automaton.isUniversalState(pos[where+1]) and not (qa[1] == [[]]):
				   if where == 0:
				       # Spoiler cannot continue
				       self.V[pos].append((0, 0, "winningPositionAForDuplicator", 0))
                                   elif where == 1:
				       # Duplicator cannot continue
                                       self.V[pos].append((1, "winningPositionAForSpoiler", 0, 1))
				   else:
				       print "NO WAY!"
				       return 1/0
                                   
                                   
                                   
                               
                           
                       
    def de_bit_WithHistory(self, p, q, b): # p, q: States (i.e., tuples), 
                                           # b: 0 or 1
        "Return the b' of position (p, q, b') where the previous position has\
         de-bit b"
        return self.automaton.isNotFinalState(q) and \
               (b or self.automaton.isFinalState(p))


    def de_bit(self, p, q): # p, q: states (i.e., tuples)
        "Returns 1 if position (p, q) always has de-bit 1, else 0"
        return self.automaton.isFinalState(p) and \
               self.automaton.isNotFinalState(q)


    def determineMove(self, pos):
        # who: S Spoiler, D Duplicator
        # where: 0 1st component, 1 2nd component, 2 choose freely
        isExSpo = self.automaton.isExistentialState(pos[SPO])
        isExDup = self.automaton.isExistentialState(pos[DUP])
        hasTerm = (len(pos) == HAS_TERM)
        if (hasTerm):
            finished = pos[FINISHED]
        else:
            finished = None
        
        if (hasTerm):
            if (finished == -1):
                if (isExSpo or (not isExDup)):
                    who = S
                    where = (not isExSpo)
                else:
                    who = D
                    where = 2 
            elif (finished == 0):
                if isExDup:
                    who = D
                else:
                    who = S
                where = 1
            else: # finished == 1
                if isExSpo:
                    who = S
                else:
                    who = D
                where = 0
        else: # No term chosen.
            if (isExSpo or (not isExDup)):
                who = S
                where = (not isExSpo)
            else:
                who = D
                where = 2 

        # You can do it shorter, but this is clearer.
        return (who, where)



    def removeDeadEnds(self):
        # Self-loops are not allowed, hence create two nodes A, B of same
        # priority and connect them.
	winningPositionAForDuplicator = (0, 0, "winningPositionAForDuplicator", 0)
	winningPositionBForDuplicator = (0, 0, "winningPositionBForDuplicator", 0)
	winningPositionAForSpoiler    = (1, "winningPositionAForSpoiler", 0, 1)
	winningPositionBForSpoiler    = (1, "winningPositionBForSpoiler", 0, 1)
	self.V[winningPositionAForDuplicator] = [winningPositionBForDuplicator]
	self.V[winningPositionBForDuplicator] = [winningPositionAForDuplicator]
	self.V[winningPositionAForSpoiler] = [winningPositionBForSpoiler]
	self.V[winningPositionBForSpoiler] = [winningPositionAForSpoiler]

        positions = self.V.keys()
        for v in positions:
	    # No outgoing edges from v?
	    if len(self.V[v]) == 0:
	        # Depending on who can move, and an edge from v to the
                # respective self-loop.
		if (len(v) == NO_TERM) or (v[MOVE_WHO] == S):
		    self.V[v] = [winningPositionAForDuplicator]
		else:
		    self.V[v] = [winningPositionAForSpoiler]


    #
    # This method computes the simulation relation
    #
    def computeSimulationRelation(self):
        jurdzinski = EpsiJurdzinski(self)
	jurdzinski.compute()
        rel = jurdzinski.getRelation(self.automaton.mainStates)
        relObject = SimulationRelation(rel, "delayed_simulation")
        ##del rel[0] Nicht mehr nötig.
        if DEBUG:
            for k in rel.items():
                formula = toString(k[0])
                geq_formulas = []
                for l in k[1]:
                    geq_formulas.append(toString(l))
                print formula," <=_de ",geq_formulas
        return rel

            
                     
