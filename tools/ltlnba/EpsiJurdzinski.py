# -----------------------------------------------------------------------------
#                                  LTL -> NBA
#              Copyright (C) Carsten Fritz, Björn Teegen 2002-2003
#               Sponsored by Deutsche Forschungsgemeinschaft (DFG)
#     Distributed under the terms of the GNU Lesser General Public License
#         See the files README.txt and LICENSE.txt for more information
# -----------------------------------------------------------------------------

## import jurd (no C-code included in this release)

from Constants import DEBUG
from Utilities import getZeroMatrix
##from EpsiDelayedSimGame import POS_PRIORITY, MOVE_WHO, S, D, DUP, SPO, HAS_TERM
from Jurdzinski import Jurdzinski


#
# Given a parity game graph with neither self loops nor dead ends and
# each vertex with a priority of 0,1 or 2, this Jurdzinski-Algorithm 
# computes the states (p,q) from which Duplicator has a winning 
# Strategy, represented as a Dictionary of (State, List of State)
#

POS_PRIORITY = 0
SPO = 1
DUP = 2
MOVE_WHO = 5
HAS_TERM = 8
## The length of a position p is either HAS_TERM or MOVE_WHO; in the second
## case, we have p[MOVE_WHO] = S.
## Remember: p[POS_PRIORITY] in [0, 1, 2], p[SPO] and p[DUP] are states,
## p[MOVE_WHO] in [S, D] and only there if len(p) == HAS_TERM.

S = 0
D = 1


class EpsiJurdzinski(Jurdzinski):
    "This class defines the Jurdzinski-Algorithm adapted to the needs of class\
     EpsiDelayedSimGame"

    def __init__(self, epsiGameGraph): # epsiGameGraph: EpsiDelayedSimGame
        self.V = epsiGameGraph.getV() # the V sets for the algorithm
                                      ## (list of positions, ie, tuples)
	self.E = epsiGameGraph.getE() # the E sets for the algorithm with E := E[v]
                                      ## (Liste von Listen von Zahlen)
	self.P = epsiGameGraph.getP() # the P sets for the algorithm with P := P[v]
                                      ## (Liste von Listen von Zahlen)
	self.infinity = 1         # infinity is n_1 + 1, but n_1 is
                                  ## calculated later
	self.B = [0] * len(self.V) # the B and C array for the algorithm
	self.C = [0] * len(self.V)           
	self.r = [0] * len(self.V) # the rho function for the algorithm
	self.L = []                # the L set for the algorithm
        self.epsiGameGraph = epsiGameGraph

        #
        # This is the initialisation of the Jurdzinski-Algorithm described
        # on page 14
        #
        for v in self.V:                # foreach v in V do
            v_num = epsiGameGraph.pos2numberDict[v]
	    priority = v[POS_PRIORITY]
	    if (priority % 2) == 1:
	        self.infinity += 1
	        self.L.append(v_num)                # L:={v in V|p(v) is odd};
	    self.B[v_num] = 0                       #   B(v):=0
	    self.C[v_num] = len(self.E[v_num])      #   C(v):=|{w|(v,w) in E}|;
	    self.r[v_num] = 0                       #   r(v):=0;


    
    def compute(self):
        while len(self.L) != 0:                     # while |L|<>0 do 	    
	    v = self.L.pop()                        #   let v in L;
	    t = self.r[v]                           #   t:=r(v);
	    i = self.V[v][POS_PRIORITY]
	    self.B[v] = self.val(v)                 #   B(v):=val(r,v);
	    self.C[v] = self.cnt(v)                 #   C(c):=cnt(r,v);
	    self.r[v] = self.incr(self.B[v], i)     #   r(v):=incr_v(B(v));
            #   P:={w in V|(w,v) in E};
            for w in self.P[v]:
                # foreach w in P such that w not in L do
	        if w not in self.L:
		    i_w = self.V[w][POS_PRIORITY]
		    b_w = self.B[w]
	            r_v = self.r[v]
	            if (len(self.V[w]) == HAS_TERM) and \
                       (self.V[w][MOVE_WHO] == D):
                        ## If len(self.V[w]) < HAS_TERM then Spoiler moves
                        #     if w in V0
	                if self.xBracket(t, i_w) == b_w:
                            #    and <t>_w=B(w)
			    if self.xBracket(r_v, i_w) > b_w:
                                #   and <r>_w > B(w)         
	                        if self.C[w] > 1:   #  and C(w)>1 then  
	                            self.C[w] -= 1  #   C(w):=C(w)-1;      
	                        if self.C[w] == 1:  #  and C(w)=1 then  
	                            self.L.append(w) #  L:=L UNION {w}; 
	            else:  #  if w in V1
	                if self.xBracket(r_v, i_w) == b_w:
                            #  and <r>_w = B(w)
			    if self.xBracket(t, i_w) < b_w:
                                #  and <t>_w < B(w) then
	                        self.C[w] += 1
                                #   C(w):=C(w)+1;
	                if self.xBracket(r_v, i_w) > b_w:
                            #  and <r>_w > B(w) then
	                    self.L.append(w)
                            #   L:=L UNION {w};



    #
    # Given the size n of the state set, this method returns the Relation
    # as a Dictionary of (State, List of State), ie, if q in aquivStates[p]
    # then p <= q
    #
    def getRelation(self, mainStates=[]): # mainStates: List of States
        aquivStates = {}
        if mainStates == []:
            for i in range(len(self.r)):
                if (len(self.V[i]) < HAS_TERM) and (self.r[i] < self.infinity):
                    # Remember that p, q are mainStates.
                    p = self.V[i][SPO]
                    q = self.V[i][DUP]
                    if not aquivStates.has_key(p):
                        aquivStates[p] = [q]
                    else:
                        aquivStates[p].append(q)
        else:
            for p in mainStates:
                aquivStates[p] = []
            for p in mainStates:
                for q in mainStates:
                    pqDeBit = self.epsiGameGraph.de_bit(p, q)
                    pqPos = (pqDeBit, p, q, pqDeBit)
                    pqNumber = self.epsiGameGraph.pos2numberDict[pqPos]
                    if self.r[pqNumber] < self.infinity:
                        aquivStates[p] += [q]                    
            
	return aquivStates

    

    #
    # This is the val(r,v) function, it returns
    #
    # / <min({r(w) | (v,w) in E})>_v  if v in V_0,
    # \ <max({r(w) | (v,w) in E})>_v  if v in V_1.
    #
    def val(self, v):
	if (len(self.V[v]) == HAS_TERM) and \
           (self.V[v][MOVE_WHO] == D):
   	    val = self.infinity
	    for w in self.E[v]:
	        val = min((self.r[w], val))
        else:
	    val = 0
	    for w in self.E[v]:
	        val = max((self.r[w], val))
        return self.xBracket(val, self.V[v][POS_PRIORITY])

### No C-code included in this release.
## def doEpsiJurdzinski_C(game, mainStates, jurdMode=jurd.BUECHI):
##     # game: EpsiDelatedSimGame
##     # mainStates: List of States
##     (rho, infinity) = jurd.computeRho(game, jurdMode)

##     aquivStates = {}
##     for p in mainStates:
##         aquivStates[p] = []
##     for p in mainStates:
##         for q in mainStates:
##             pqDeBit = game.de_bit(p, q)
##             pqPos = (pqDeBit, p, q, pqDeBit)
##             pqNumber = game.pos2numberDict[pqPos]
##             if rho[pqNumber] < infinity: 
##                         aquivStates[p] += [q]

##     return aquivStates
