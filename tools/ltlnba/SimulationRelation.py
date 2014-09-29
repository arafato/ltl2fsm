# -----------------------------------------------------------------------------
#                                  LTL -> NBA
#              Copyright (C) Carsten Fritz, Björn Teegen 2002-2003
#               Sponsored by Deutsche Forschungsgemeinschaft (DFG)
#     Distributed under the terms of the GNU Lesser General Public License
#         See the files README.txt and LICENSE.txt for more information
# -----------------------------------------------------------------------------

from Utilities import removeDuplicates, minimalElements, maximalElements, \
     union, setminus
import LTLUtils

class SimulationRelation:

    def __init__(self, relDict, simMode=""):
        # relDict: Dictionary of (State, List of State)

        self.relDict = relDict

        if simMode == "":
            self.simMode = "unspecified"
        else:
            self.simMode = simMode

        self.stateList = relDict.keys()

        self.size = len(self.stateList)

        self.state2numberDict = {}
        for i in range(len(self.stateList)):
            self.state2numberDict[self.stateList[i]] = i

        self.matrix = [0] * self.size
        for i in range(self.size):
            aList = [0] * self.size
            self.matrix[i] = aList
        for p in self.stateList:
            for q in self.relDict[p]:
                self.matrix[self.state2numberDict[p]][self.state2numberDict[q]] = 1

        self.equivalenceClasses = self.equivClasses(self.stateList)
        # List of List of State
        # The list of equivalnece classes of the state set.
        # Also is the reverse of self.state2equivClass.

        self.state2equivClass = self.__makeState2EquivClass()
        # Dictionary of (State, Number).
        # state2equivClass[p] == state2equivClass[q] iff isEquiv(p, q)

        


    def isLeq(self, p, q): # p, q: States
        return self.matrix[self.state2numberDict[p]][self.state2numberDict[q]]

    def isLeqMH(self, p, q): # p, q: MH-States
        "It has to hold: \forall m \in M1 \exists n \in M0): n <= m and \
	 (m \in N1 implies n \in N0). \
         NOTA BENE: It is relatively expensive to call this method!"

        ##
        ## Does this really fit? What about F((F a) U b)?
        ##
        
        (M0, N0) = p; (M1, N1) = q

	for m in N1:
	    success = 0; i = 0
	    while (not success) and (i < len(N0)):
		success = self.isLeq(N0[i], m)
		if (not success):
		    i += 1
	    if (i == len(N0)):
		return 0

	M1_minus_N1 = setminus(M1, N1)
	for m in M1_minus_N1:
	    success = 0; i = 0
	    while (not success) and (i < len(M0)):
		success = self.isLeq(M0[i], m)
		if (not success):
		    i += 1
	    if (i == len(M0)):
		return 0

	return 1
	

	            
    def isLess(self, p, q): # p, q: States
        return self.isLeq(p, q) and not self.isLeq(q, p)

    def isLessMH(self, p, q): # p, q: MH-States
        return self.isLeqMH(p, q) and not self.isLeqMH(q, p)

    def isEquiv(self, p, q): # p, q: States
        return self.isLeq(p, q) and self.isLeq(q, p)

    def isIncomparable(self, p, q): # p, q: States
        return (not self.isLeq(p, q)) and (not self.isLeq(q, p))


    def equivClasses(self, lOS): #lOS: List of States
        "Returns a partition (ie, a list of disjoint lists) of the states in \
         lOS in their equivalence classes wrt the simulation relation"
        erg = []
        for state in lOS:
            finished = 0
            i = 0
            while (not finished) and (i < len(erg)):
                if self.isEquiv(state, erg[i][0]):
                    erg[i] += [state]
                    finished = 1
                i += 1
            if (not finished):
                erg += [[state]]

        return erg
                

    def __makeState2EquivClass(self):
        erg = {}
        for i in range(len(self.equivalenceClasses)):
            for state in self.equivalenceClasses[i]:
                erg[state] = i
        return erg


    def getRepresentative(self, state): # state: State
        "Returns a standard representative of the equivalence class of \
         'state', ie, a state q s.t. state \equiv q and \forall states p: if \
         p \equiv state then getRepresentative(state) == getRepresentative(p).\
         If you also want the rep. to be an accepting state if its class \
         contains an accepting state, call adjustGetRep first."
        if state in self.stateList:
            return self.equivalenceClasses[self.state2equivClass[state]][0]
        else:
            return None
         

    def adjustGetRep(self, isAcc): # isAcc: Function
        "An auxiliary method to force getRepresentative to return an \
         accepting state, if possible."
        for aClass in self.equivalenceClasses:
            for i in range(len(aClass)):
                if isAcc(aClass[i]):
                    (aClass[0], aClass[i]) = (aClass[i], aClass[0])
                    break

    def adjustEquivClasses(self, sorter): # sorter: Function
	"To sort the equivalence classes as defined by the function sorter"
	for aClass in self.equivalenceClasses:
	    aClass.sort(sorter)


    def adjust2getShortRep(self, isAcc): # isAcc: Function
	"An auxiliary method that not only urges getRepresentative to \
	 return an accepting state, if possible, but also a short one."
	def sorter(p, q): # p, q: States
	    pIsAcc = isAcc(p)
	    qIsAcc = isAcc(q)
	    if (not pIsAcc) and qIsAcc:
		return 1
	    elif pIsAcc and (not qIsAcc):
		return -1
	    # pIsAcc == qIsAcc
	    pLen = LTLUtils.LTLlen(p)
	    qLen = LTLUtils.LTLlen(q)
	    if pLen < qLen:
		return -1
	    elif pLen > qLen:
		return 1
	    else:
		return 0

	for aClass in self.equivalenceClasses:
	    aClass.sort(sorter)
		


    def minimizeMHstate(self, mhstate, isAccepting):
        # mhstate: List of length 2 of list of states
        # isAccepting: Function States --> {0, 1}
        "Non-minimal states are removed from both(!) components. \
         A non-minimal state remains only if it also appears in the second \
         component and cannot be substituted there. \
         Accepting states are not removed from the second component."

        (firstComp, secondComp) = mhstate
        newFirstComp = minimalElements(firstComp, self.isLess)
        newSecondComp = minimalElements(secondComp, self.isLess)

	newFirstComp = union(newFirstComp, newSecondComp)

        return (newFirstComp, newSecondComp) 
            
        
        
                
        
