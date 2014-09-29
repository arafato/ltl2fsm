# -----------------------------------------------------------------------------
#                                  LTL -> NBA
#              Copyright (C) Carsten Fritz, Björn Teegen 2002-2003
#               Sponsored by Deutsche Forschungsgemeinschaft (DFG)
#     Distributed under the terms of the GNU Lesser General Public License
#         See the files README.txt and LICENSE.txt for more information
# -----------------------------------------------------------------------------

from Constants import S, D, DEBUG, POS_PLAYER, POS_PRIORITY, POS_P, POS_Q
from Utilities import getZeroMatrix

#
# Given a parity game graph with neither self loops nor dead ends and
# each vertex with a priority of 0,1 or 2, this Jurdzinski-Algorithm 
# computes the states (p,q) from which Duplicator has a winning 
# Strategy, represented by 1 in the relation matrix.
#

class Jurdzinski:
    "This class defines the Jurdzinski-Algorithm"

    def __init__(self, gameGraph):
        self.V = gameGraph.getV() # the V sets for the algorithm
	self.E = gameGraph.getE() # the E sets for the algorithm with E := E[v]
	self.P = gameGraph.getP() # the P sets for the algorithm with P := P[v]
	self.infinity = 1         # infinity is n_1 + 1, but n_1 is
                                  ## calculated later
	self.B = []               # the B and C array for the algorithm
	self.C = []            
	self.r = []               # the rho function for the algorithm
	self.L = []               # the L set for the algorithm

        #
        # This is the initialisation of the Jurdzinski-Algorithm described
        # on page 14
        #
        for i in range(len(self.V)):                # foreach v in V do
	    v = self.V[i]
	    priority = v[POS_PRIORITY]
	    if (priority % 2) == 1:
	        self.infinity += 1
	        self.L.append(i)                    # L:={v in V|p(v) is odd};
	    self.B.append(0);                       #   B(v):=0
	    self.C.append(len(self.E[i]))           #   C(v):=|{w|(v,w) in E}|;
	    self.r.append(0);                       #   r(v):=0;

    #
    # This is the main part of the Jurdzinski-Algorithm described on page 14
    #

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
	            if self.V[w][POS_PLAYER] == D:
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
    # as a n x n matrix.	
    #
    def getRelation(self, n):
        aquivStates = getZeroMatrix(n, n)
        for i in range(0,len(self.r)):
	    v = self.V[i]
	    if len(v) == (2+4):
	        #if not v[0]: FIX-ME
		    p = v[POS_P]
		    q = v[POS_Q]
		    if self.r[i]<self.infinity:
	                aquivStates[p][q] = 1
	return aquivStates 

    #
    # This is the cnt(r,v) function, it returns 
    #
    # |{u | u is successor of v and <r(u)>_v = val(r,v)|
    #
    def cnt(self, v):
	succ = 0
	val = self.B[v]
	i = self.V[v][POS_PRIORITY]
	for u in self.E[v]:
	    if self.xBracket(self.r[u],i) == val:
		succ += 1
        return succ
		   
    #
    # This is the <x>_i, it returns
    #
    # / x       if i<>0,
    # | 0       if i=0 and x<infinty,
    # \ infinty otherwise.
    #
    def xBracket(self, x, i):
        if i != 0:
	    return x
	if x < self.infinity:
	    return 0
        return self.infinity

    #
    # This is the incr_i(x) function, it returns
    #
    # / <x>_i             if i=0 or i=2,
    # \ min(x+1, infinty) otherwise.
    #
    def incr(self, x, i):
        if i == 1:
            return min((x+1, self.infinity))
        return self.xBracket(x, i)

    #
    # This is the val(r,v) function, it returns
    #
    # / <min({r(w) | (v,w) in E})>_v  if v in V_0,
    # \ <max({r(w) | (v,w) in E})>_v  if v in V_1.
    #
    def val(self, v):
	if self.V[v][POS_PLAYER] == D:
   	    val = self.infinity
	    for w in self.E[v]:
	        val = min((self.r[w], val))
        else:
	    val = 0
	    for w in self.E[v]:
	        val = max((self.r[w], val))
        return self.xBracket(val, self.V[v][POS_PRIORITY])

    # alternative implementation of the Jurdzinski-algorithm

    def update(self, v):
        return self.incr(self.val(v), self.V[v][POS_PRIORITY])

    def compute2(self):
        found = 1
	while found:
            if DEBUG: print self.r
	    found = 0
            for i in range(len(self.V)):                # foreach v in V do
		if self.update(i) != self.r[i]:
		    found = 1
		    self.r[i] = self.update(i)
		    












