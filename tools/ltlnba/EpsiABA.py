# -----------------------------------------------------------------------------
#                                  LTL -> NBA
#              Copyright (C) Carsten Fritz, Björn Teegen 2002-2003
#               Sponsored by Deutsche Forschungsgemeinschaft (DFG)
#     Distributed under the terms of the GNU Lesser General Public License
#         See the files README.txt and LICENSE.txt for more information
# -----------------------------------------------------------------------------

import os
import string
from BuechiAutomaton import BuechiAutomaton, AlternatingBuechiAutomaton
import LTLUtils
import Utilities
from Utilities import Queue, Stack, removeDuplicates, minimalElements, tighten, adjustUnivStates, termOrSimplify
from Constants import DEBUG
from SimulationRelation import SimulationRelation
from random import randint
from time import time
from SCC import scc2

#
# CLASS EpsiABA
#

class EpsiABA(AlternatingBuechiAutomaton):
    "Class representation of alternating Büchi automata with \
    \epsi-transitions"


    def __init__(self, parseTree, accStateMode=0):
        # parseTree: Tuple
        # accStateMode: 0 oder 1. 0 bedeutet, daß oder- und und-Zustände keine
        ## Akzeptierzustände sind, bei 1 hängt das von den Teilformeln ab.
        "Constructor. The parse tree comes in positive normal form"

        AlternatingBuechiAutomaton.__init__(self)
        self.name = "Epsi_ABA"
        self.parseTree = parseTree
        
        self.propSet = self.__findPropositions(parseTree)
        self.propSet.sort()
        removeDuplicates(self.propSet)
        # propSet: List.
        # The list of propositions appearing in parseTree
	if self.propSet == []:
	    self.propSet = ['dummy']

        self.propSetDict = {}
        for i in range(len(self.propSet)):
            self.propSetDict[self.propSet[i]] = i
        # propSetDict: Dictionary. Die Umkehrung von propSet -- den Buchstaben
        # wird eine Zahl zugeordnet.

        self.initialState = self.parseTree   # Brauchen wir das?
        self.transitionRelation = self.__makeTrans(Queue(parseTree))
	# transitionRelation: standardmäßig Dictionary of (state, dictionary
	# of (state, list of term))

        self.accFunctionList = [self.isAccStateMode0,  self.isAccStateMode1, \
                        self.isAccStateMH, self.isAccStateMode3]
        self.isFinalState = self.accFunctionList[accStateMode]
        self.isExistentialState = self.isExistentialState0
        self.isUniversalState = self.isUniversalState0

	self.transitionRelation = tighten(self.transitionRelation, self.initialState, \
					  self.isExistentialState, [])

        self.transitionRelation = adjustUnivStates(self.transitionRelation, \
						   self.isExistentialState)
	
	for p in self.transitionRelation.keys():
	    for q in self.transitionRelation[p].keys():
		self.transitionRelation[p][q] = termOrSimplify(self.transitionRelation[p][q])

        ### ERST MAL RAUSGENOMMEN
        ### self.states2Terms = self.__makeStates2Terms
        ### # states2Terms: Dictionary of (State, List of Terms).
        ### # Ordnet jedem
        ### # Zustand die Terme (einschl. \epsi) zu, mit denen die ausgehenden
        ### # Kanten beschriftet sind.
        ### END: ERST MAL RAUSGENOMMEN
   
        self.possibleTerms = {}
        self.__makeStates2Letters(self.initialState, self.possibleTerms)
        # possibleTerms: Dictionary of (State, List of Term).
        # possibleTerms[q] ist die Liste der Terme, die Spoiler im Zustand q
        # ansagen darf.

        self.mainStates = self.__makeMainStates([self.initialState])
        # mainStates: List of Tuples. The states in which a round of the
        # simulation game can start and end.

        self.powerSet = {}
        # powerSet: Dictionary of (State, Dictionary of (tuple(Term), List 
        # of List of State)).
        # Für einen Zustand p und einen Term t ist powerSet[p][tuple(t)] die
        # Liste der Mengen M von Zuständen so, daß ({p}, A) --t--> (M, B) eine
        # Transition im MH-Automaten für gewisse A, B ist.
        ## Dies dient nur als Hilfsvariable zur Berechnung des MH-Automaten.

        self.transitionRelation = Utilities.reduce2reachable(self.transitionRelation, self.initialState)
        
	self.stateSet = self.transitionRelation.keys()

	self.topologicalSorting = scc2(self.stateSet, lambda q: \
				       self.transitionRelation[q].keys())
	# Topological sorting of the states: List of list of states
	self.topSortState2Number = {}
	# Dictionary of (state, integer)
	for i in range(len(self.topologicalSorting)):
	    for state in self.topologicalSorting[i]:
		if state in self.mainStates:
		    self.topSortState2Number[state] = i

	#NEU für optimiertes MH
        self.zInversed = []
        self.zNormal = {}
        

    def next(self, M, i):
        # M: list of states
        # i: int
        ## Returns the value of the next-function as defined in the description
        ## of the optimized MH-algo for very weak ABA.
        ## Different from that description, '-1' plays the role of '0' and the
        ## z-values of the P-elements range from 0 to k-1
        erg = len(self.zInversed)
        for q in M:
            if self.zNormal.has_key(q):
                zq = self.zNormal[q]
                if ((zq > i) and (zq < erg)):
                    erg = zq
        if (erg == len(self.zInversed)):
            return -1
        else:
            return erg
        

    def topCompare(self, p, q):
	# p, q: mainStates
	"Sortieren nach umgekehrter top. Ordnung"
	if self.topSortState2Number[p] < self.topSortState2Number[q]:
	    return 1
	elif self.topSortState2Number[p] > self.topSortState2Number[q]:
	    return -1
	else:
	    return 0
	

    def getStateSet(self):
        return self.mainStates


    def __makeTrans(self, queue):
        # queue: Queue
        "Puts the states of the epsi-ABA into the dictionary transDict which\
         is then returned.\
         The return type is Dictionary of (Tuple, Dictionary of (Tuple, List))\
         so the keys are states and the entries are dictionaries, the latter \
         also having states as keys and terms over propositions (in \
         disjunctive normal form) as entries (see memo below).\
         Example: {('X', 'a'): {('a',): [[0, 0]]}} denotes the transition \
         relation only containing a transition from 'Xa' to 'a' with an \
         arbitrary subset of the two propositions.\
         WE ALSO USE STATE 'tt' NOW."
        transDict = {}

        while(not queue.isEmpty()):
            if DEBUG: print queue.q
            parseTree = queue.dequeue()
            if(not transDict.has_key(parseTree)):
                transDict[parseTree] = {}
                if (type(parseTree) == type("")):
                    if (parseTree in self.propSet):
                        # parseTree ist Proposition
                        transDict[parseTree]['tt'] = [self.__vectorize(parseTree)]
                        queue.enqueue('tt')
                    elif (parseTree == 'tt'):
                        transDict['tt']['tt'] = [[0]*len(self.propSet)]
                # parseTree ist Tupel
                elif (parseTree[0] == 'X'):
                    transDict[parseTree][parseTree[1]] = [[0]*len(self.propSet)]
                    queue.enqueue(parseTree[1])
                    
                elif (parseTree[0] == 'U'):
                    ##orSubForm = ('|', parseTree[2], ('&', parseTree[1],\
                    ##                                  ('X', parseTree)))
                    ##transDict[parseTree][orSubForm] = [[]] # Also epsilon
                    ##queue.enqueue(orSubForm)
                    andPart = ('&', parseTree[1], ('X', parseTree))
                    transDict[parseTree][parseTree[2]] = [[]] # epsilon
                    transDict[parseTree][andPart] = [[]] # epsilon
                    queue.enqueue(parseTree[2])
                    queue.enqueue(andPart)
                    
                elif (parseTree[0] == 'V'):
                    ##andSubForm = ('&', parseTree[2], ('|', parseTree[1],\
                    ##                                   ('X', parseTree)))
                    ##transDict[parseTree][andSubForm] = [[]] 
                    ##queue.enqueue(andSubForm)
                    orPart = ('|', parseTree[1], ('X', parseTree))
                    transDict[parseTree][parseTree[2]] = [[]] # epsilon
                    transDict[parseTree][orPart] = [[]] # epsilon
                    queue.enqueue(parseTree[2])
                    queue.enqueue(orPart)
                    
                elif (parseTree[0] == '!'):
                    transDict[parseTree]['tt'] = [self.__vectorize(parseTree)]
                    queue.enqueue('tt')
                elif (parseTree[0] in ['&', '|']):
                    transDict[parseTree][parseTree[1]] = [[]]
                    transDict[parseTree][parseTree[2]] = [[]]
                    queue.enqueue(parseTree[1])
                    queue.enqueue(parseTree[2])
                    
                elif (parseTree[0] in ['F', 'G']):
                    # Analog zu &, |
                    transDict[parseTree][parseTree] = [[0]*len(self.propSet)]
                    transDict[parseTree][parseTree[1]] = [[]]
                    queue.enqueue(parseTree[1])

        return transDict


    def toBuechiAutomaton(self, sim=None):
        "Constructs an equivalent NBA using the method of Miyano-Hayashi"
        if self.powerSet == {}:
            self.makePowerSet()

        if not (sim == None):
	    sim.adjustEquivClasses(self.topCompare)
            #instead of just sim.adjustGetRep(self.isFinalState)
            self.simplifyPowerSet(sim)

        #NEU: zNormal und zInversed topologisch sortiert herstellen
        cand2ndComp = []
        if not (sim == None):
            cand2ndComp = map(sim.getRepresentative, self.mainStates)
            removeDuplicates(cand2ndComp)
        else:
            cand2ndComp = self.mainStates
        cand2ndComp = filter(lambda q: not self.isFinalState(q),
                                cand2ndComp)
        for anSCC in self.topologicalSorting:
            for aState in anSCC:
                if (aState in cand2ndComp):
                    self.zInversed.append(aState)
        for i in range(len(self.zInversed)):
            self.zNormal[self.zInversed[i]] = i
        print "z:", self.zNormal
        print "z invers:", self.zInversed
        #ENDE: NEU: zNormal und zInversed herstellen
                
        aDict = {}
        if not (sim == None):
            asInitial = sim.getRepresentative(self.initialState)
        else:
            asInitial = self.initialState
        if self.isFinalState(asInitial):
            initialMHState = ((asInitial, ), ())
        else:            
            initialMHState = ((asInitial, ), (asInitial, ))
	    
        self.__makeBuechiAutomaton(initialMHState, aDict, {}, sim)

        ### Stattdessen jetzt ...
        # LTLUtils.simplifyMHAuto(aDict, initialMHState, sim)
        ### ... LTLUtils.simplifyMHTransition in __makeBuechiAutomaton
        
        LTLUtils.adjustMHTransFormat(aDict, initialMHState)
        ## Es gehört nicht unbedingt nach LTLUtils, aber ich möchte jetzt
        ## dafür auch keine neue Klasse aufmachen.
        
        erg = LTLUtils.newEpsiABA(aDict, initialMHState, self)
        ## Auch newEpsiABA ist nicht ideal in LTLUtils aufgehoben ... mal sehen

	return LTLUtils.sccBasedSimplifications(erg)

    

    def makePowerSet(self):
        aDict = {}
        for state in self.mainStates:
            self.__doPSet(state, aDict)
        self.powerSet = aDict



    def __doPSet(self, state, dict): # state: State,
        # dict: Dictionary of (State, Dictionary of (tuple(term), List of List
        # of States))
        ## 'Term' ist dabei wirklich nur ein Vektor über {0, 1, -1}
        if dict.has_key(state):
            return

        dict[state] = {}

        ## Rekursion: Alle \epsi-Nachfolger bearbeiten
        for q in self.transitionRelation[state].keys():
            if [] in self.transitionRelation[state][q]:
                self.__doPSet(q, dict)
        ## Ab jetzt sind alle \epsi-Nachfolger in dict als quasi-existentielle
        ## Zustände OHNE \EPSI-TRANSITIONEN eingetragen.

        ## state im o.g. Format in dict eintragen
        for q in self.transitionRelation[state].keys():
            for t in self.transitionRelation[state][q]:
                term = tuple(t)
                if not dict[state].has_key(term):
                    dict[state][term] = [[q]]
                else:
                    dict[state][term] += [[q]]
        # Jetzt ist dict[state] im o.g. Format

        if self.isExistentialState(state):
            ## Der einfache Fall
            if not dict[state].has_key(()):
                return # keine \epsi-Transitionen -> fertig
            for stateList in dict[state][()]:
                for q in stateList:
                    for t in dict[q].keys():
                        if not dict[state].has_key(t):
                            dict[state][t] = []
                        dict[state][t] += dict[q][t]
                        # einfach rüberkopieren
            del dict[state][()]
            # und hübsch machen
            for t in dict[state].keys():
                dict[state][t].sort()
                removeDuplicates(dict[state][t])                    
        elif self.isUniversalState(state):
            ## Der schwierige Fall
            reachOfTerms = {}
            # reachOfTerms ist die neue Transitionsliste, die dict[state]
            # ersetzen wird.
            for posT in self.possibleTerms[state]:
                posT = tuple(posT)
                reachOfTerms[posT] = [[]]
                for t in dict[state].keys():
                    if LTLUtils.oneTermImplies(posT, t):
                        # Beachte, daß oneTermImplies(posT, ()) == 0 gilt.
                        for q in dict[state][t]:
                            reachOfTerms[posT][0] += q
                ## Jetzt haben wir reachOfTerms[posT] = [[q0, ..., qn]], wobei
                ## die qi die Zustände sind, die von state über eine
                ## Termtransition (mit posT) erreichbar sind

                if dict[state].has_key(()):
                    # Wenn es \epsi-Transitionen von state gibt:                
                    for stateList in dict[state][()]:
                        for q in stateList:
                            # Für jeden \epsi-Nachfolger q:
                            ergQ = []                            
                            for tQ in dict[q].keys():
                                if LTLUtils.oneTermImplies(posT, tQ):
                                    # Kommt man mit tState auch über die
                                    # tQ-Transition von q?
                                    ergQ += dict[q][tQ]
                                    # Dann trage tQ-Nachf. von q in ergQ ein.
                            reachOfTerms[posT] = Utilities.listMerge(reachOfTerms[posT], ergQ)
                            ## Zu den Zuständen in reachOfTerms[posT] kommt je
                            ## ein Zustand aus ergQ
                            
            dict[state] = reachOfTerms

        ## Und noch mal Rekursion: Jetzt die Termnachfolger
        for q in self.transitionRelation[state].keys():
            if not ([] in self.transitionRelation[state][q]):
                self.__doPSet(q, dict)


        

    def simplifyPowerSet(self, sim): # sim: SimulationRelation
        "Der besseren Übersichtlichkeit wegen machen wir diesen Teil hier und \
         nicht in __doPSet, auch wenn die Effizienz etwas leidet. Wir \
         vereinfachen self.powerSet auf folgende Arten:\
         1. Wir ersetzen in den Nachfolgerlisten jeden Zustand durch einen \
         Repräsentanten seiner Äqu.klasse.\
         2. Wir werfen nichtminimale Zustände raus.\
         3. Äquiv. Zustände bekommen gleiche Nachfolgerlisten."
        # Wir ersetzen durch Repräsentanten der Äqu.klasse und werfen
        # nichtminimale Zustände raus
        for state in self.powerSet.keys():
            if state in self.mainStates:
                for t in self.powerSet[state].keys():
                    for i in range(len(self.powerSet[state][t])):
                        self.powerSet[state][t][i] = \
                         minimalElements(self.powerSet[state][t][i], \
                                         sim.isLess)
                        for j in range(len(self.powerSet[state][t][i])):
                            self.powerSet[state][t][i][j] = \
                             sim.getRepresentative(self.powerSet[state][t][i][j])
                    if DEBUG:
                        print "PowerSet: (",state,", ",t,") -> ", \
                              self.powerSet[state][t]
############### Ich probiere, ob man nicht besser einen geschickt gewählten
############### Repräsentanten nimmt (der nämlich top. maximal im ABA ist).
############### Scheint zu klappen, aber das muß ich mir noch mal klar machen.
##        # Zusammenfassen äquivalenter Zustände.
##        # also: Äquiv. Zustände bekommen gleiche Nachfolgerlisten
##        for state in self.powerSet.keys():
##            if state in self.mainStates:
##                statesClass = sim.equivalenceClasses[sim.state2equivClass[state]]
##                # statesClass: List of States
##                for q in statesClass:
##                    if not (state == q):
##                        for t in self.powerSet[q].keys():
##                            if self.powerSet[state].has_key(t):
##                                self.powerSet[state][t] += self.powerSet[q][t]
##                            else:
##                                self.powerSet[state][t] = self.powerSet[q][t]
##                # Und wieder ordentlich machen
##                for t in self.powerSet[state].keys():
##                    self.powerSet[state][t].sort()
##                    removeDuplicates(self.powerSet[state][t])
        


    def __makeBuechiAutomaton(self, mhState, transDict, weDict, sim):
        # mhState: Pair of Tuple of State, transDict: Dictionary
        # sim: SimulationRelation
        "Constructs an equivalent NBA using an adapted optimization of the \
         algorithm of Miyano-Hayashi.\
         Diese Methode wird aus toBuechiAutomaton aufgerufen.\
         Wir nehmen als Zustände des NBA Paare von Tupeln von Zuständen: \
         MHstate = pair of tuple of state. \
         Und transDict ist Dictionary of (MHstate, Dictionary of (tuple(Term),\
         List of MHstate))."

        if transDict.has_key(mhState) or weDict.has_key(mhState):
            return ## Rekursionsabbruch
        
        transDict[mhState] = {}

        #Berechnen der möglichen Terme von mhState
        auxList = [0] * len(mhState[0])
        for i in range(len(mhState[0])):
            auxList[i] = self.possibleTerms[mhState[0][i]]
        posTerms = Utilities.termAndForLists(auxList)

        STATIC = 'static'
        PROGRESS = 'progress'
        for posT in posTerms:
            posT = tuple(posT)
            transDict[mhState][posT] = []
            succ4posT = {}
            # 1. Zu jedem p in mhState[0] finde Liste von Listen von
            # Nachfolgezuständen succ4posT[p]
            for p in mhState[0]:
                succ4posT[p] = []
                for t in self.powerSet[p].keys():
                    if LTLUtils.oneTermImplies(posT, t):
                        for aSet in self.powerSet[p][t]:
                            succ4posT[p].append([])
                            for q in aSet:
                                succ4posT[p][-1].append(q)
                        # Und nicht succ4posT[p] += self.powerSet[p][t]
                        
            # 2. Mache removeDuplicates
            for p in mhState[0]:
                removeDuplicates(succ4posT[p])

            # 3. Wenn p in mhState[1], dann erweitere jede Liste aSet in
            # succ4posT[p], für die p in aSet gilt, um einen Sonderwert
            # 'static'; erweitere die, für die p \notin aSet gilt, um einen
            # Sonderwert 'progress'.
            if (mhState[1] != ()):
                p = mhState[1][0]
                for aSet in succ4posT[p]:
                    if (p in aSet):
                        aSet.append(STATIC)
                    else:
                        aSet.append(PROGRESS)
           
            # 4. Iteriere listMerge über succ4posT[q] für alle q in mhState[0].
            # Das Ergebnis sei erg1stCposT, eine Liste von Listen von
            # Zuständen.
            erg1stCposT = [[]]
            for q in mhState[0]:
                erg1stCposT = Utilities.listMerge(erg1stCposT, succ4posT[q])

            # 5. Fallunterscheidung in Schleife:
            erg2ndCposT = []
            for succSet in erg1stCposT:
                if (mhState[1] == ()):
                    nextValue = self.next(succSet, -1)
                    if (nextValue == -1):
                        erg2ndCposT.append([])
                    else:
                        erg2ndCposT.append([self.zInversed[nextValue]])
                elif (STATIC in succSet):
                    erg2ndCposT.append([mhState[1][0]])
                    succSet.remove(STATIC)
                else: # PROGRESS in succSet
                    nextValue = self.next(succSet, self.zNormal[mhState[1][0]])
                    if (nextValue == -1):
                        erg2ndCposT.append([])
                    else:
                        erg2ndCposT.append([self.zInversed[nextValue]])
                    succSet.remove(PROGRESS)

            # Geeignetes Entfernen nichtminimaler Zustände aus den Mengen in
            # erg1stCposT
            if (sim != None):
                for i in range(len(erg1stCposT)):
                    erg1stCposT[i] = minimalElements(erg1stCposT[i], \
                                                     sim.isLess)
                    if ((erg2ndCposT[i] != []) and \
                        not (erg2ndCposT[i][0] in erg1stCposT[i])):
                        erg1stCposT[i].append(erg2ndCposT[i][0])

            # Kopieren nach transDict
            for i in range(len(erg1stCposT)):
                transDict[mhState][posT] += [(tuple(erg1stCposT[i]), \
                                              tuple(erg2ndCposT[i]))]

	    # Neu für schwache Äquivalenz
	    for i in range(len(transDict[mhState][posT])):
		if weDict.has_key(transDict[mhState][posT][i]):
		    transDict[mhState][posT][i] = weDict[transDict[mhState][posT][i]]
            transDict[mhState][posT].sort()
            removeDuplicates(transDict[mhState][posT])

        # Vernünftigerweise macht man hier schon 'richtiges' Simplifizieren.
        LTLUtils.simplifyMHtransition(mhState, transDict, weDict, \
				      (lambda x: x[1] == tuple([])), sim)
   
        # Rekursion einleiten
	toBeDone = []
	if transDict.has_key(mhState):
	    toBeDone = transDict[mhState].values() # List of list of MH-State
        auxList = []
        for listOfMHState in toBeDone:
            auxList += listOfMHState
        toBeDone = auxList # List of MH-State
        toBeDone.sort()
        removeDuplicates(toBeDone)
	
        for nextMHState in toBeDone:
            self.__makeBuechiAutomaton(nextMHState, transDict, weDict, sim)
            
                        


    def __makeStates2Letters(self, state, map): # state: tuple, map: dictionary
        "This method uses the transition relation to compute the mapping map.\
         For every state q reachable from 'state', map[q] is the list of\
         letters (i.e., a list of vectors, see MEMO) that Spoiler may choose\
         when he is in state q."
        if (map.has_key(state)):
            return

	if (type(state) == type("")):
	    if (state == 'tt'):
                map[state] = self.transitionRelation[state]['tt']
                if DEBUG:
                    print str(state) + " " + str(map[state])
	    elif (state == 'ff'):
		map[state] = []
		if DEBUG:
                    print str(state) + " " + str(map[state])
            else:
                self.__makeStates2Letters('tt', map)
                map[state] = self.transitionRelation[state]['tt']
                if DEBUG:
                    print str(state) + " " + str(map[state])
        elif (state[0] == '!'):
	    self.__makeStates2Letters('tt', map)
            map[state] = self.transitionRelation[state]['tt']
            if DEBUG:
                print str(state) + " " + str(map[state])
            
        elif (state[0] == 'X'):
            self.__makeStates2Letters(state[1], map)
            map[state] = [[0]*len(self.propSet)]
            if DEBUG:
                print str(state) + " " + str(map[state])
            
        elif (state[0] in ['|', 'U']):
            self.__makeStates2Letters(state[1], map)
            self.__makeStates2Letters(state[2], map)
            map[state] = Utilities.union(map[state[1]], map[state[2]])
            if DEBUG:
                print str(state) + " " + str(map[state])
            ## Die folgenden 4 Zeilen waren vorher auskommentiert.
            ## Ich habe sie wieder reingenommen, weil ich sie für
            ## makePowerSet brauche.
            if (state[0] == 'U'):
                #map[('|', state[2], ('&', state[1], ('X', state)))] =\
                #    map[state]
                self.__makeStates2Letters(('&', state[1], ('X', state)), map)
                                                      
        elif (state[0] == '&'):
            self.__makeStates2Letters(state[1], map)
            self.__makeStates2Letters(state[2], map)
            map[state] = Utilities.termAnd(map[state[1]], map[state[2]])
            if DEBUG:
                print str(state) + " " + str(map[state])

        elif(state[0] == 'V'):
            self.__makeStates2Letters(state[1], map)
            self.__makeStates2Letters(state[2], map)
            map[state] = Utilities.union(Utilities.termAnd(map[state[1]],\
                                                           map[state[2]]),\
                                         map[state[2]]) 
            if DEBUG:
                print str(state) + " " + str(map[state])
            ## Die folgenden beiden Zeilen waren vorher auskommentiert.
            ## Ich habe sie wieder reingenommen, weil ich sie für
            ## makePowerSet brauche.
            # map[('&', state[2], ('|', state[1], ('X', state)))] = map[state]
            self.__makeStates2Letters(('|', state[1], ('X', state)), map)
                
        elif (state[0] == 'G'):
            self.__makeStates2Letters(state[1], map)
            map[state] = map[state[1]]
            if DEBUG:
                print str(state) + " " + str(map[state])
            
        elif (state[0] == 'F'):
            self.__makeStates2Letters(state[1], map)
            map[state] = Utilities.union(map[state[1]], [[0]*len(self.propSet)])
            if DEBUG:
                print str(state) + " " + str(map[state])


    ### ERST MAL RAUSGENOMMEN
    ### def __makeStates2Terms(self):
    ###     aDict = {}
    ###     for p in self.stateSet:
    ###         aDict[p] = self.transitionRelation[p].values()
    ###         aList = []
    ###         for l in aDict[p]:
    ###             for t in l:
    ###                 aList += [t]
    ###         aDict[p] = aList
    ###         aDict[p].sort()
    ###         removeDuplicates(aDict[p])
    ###     return aDict
    ### END: ERST MAL RAUSGENOMMEN



    def __vectorize(self, propExpr): # propExpr: String or Tuple
        "Parameter: A proposition or a negated proposition. Returns a vector\
         of length len(propSet) with the parameter position == 1 or -1,\
         and 0 at the other positions"
        if not (type(propExpr) == type(())):
            if (type(propExpr) == type("")):
                # Es ist ein String, also eine Proposition
                erg = [0] * len(self.propSet)
                erg[self.propSetDict[propExpr]] = 1
                return erg
        # Ab hier ist es ein Tupel
        elif (propExpr[0] == '!'): 
            erg = [0] * len(self.propSet)
            erg[self.propSetDict[propExpr[1]]] = -1
            return erg


    def vectorToString(self, vector):
        if vector == []:
            return 'ë' # iso-8859-1, Zeichen Nr. 235
        else:
            literals = []
            for i in range(0, len(vector)):
                if vector[i] == 1:
                    literals.append(self.propSet[i])
                elif vector[i] == -1:
                    literals.append('!' + self.propSet[i])

            if literals == []:
                return 'tt'
            else:
                return string.join(literals, ' & ')



    def __makeMainStates(self, stateList): # stateList: List
        "Computes the set of main states, i.e., states in which a round can\
         start and end."
        transPairs = self.transitionRelation.items()
        for p2qa in transPairs:
            targets = p2qa[1].items()
            for q2a in targets:
                if not (q2a[1] == [[]]):
                    stateList.append(q2a[0])
        stateList.sort()
        removeDuplicates(stateList)
        return stateList
            
        

    def __findPropositions(self, parseTree): # parseTree: Tuple OR List
        "Returns the list of propositions in parseTree"
        if (len(parseTree) == 0):
            return []

        if (type(parseTree) == type("")):
	    if parseTree in ['tt', 'ff']:
		return []
	    else:
		return [parseTree]
	elif (parseTree[0] in LTLUtils.prefixOp):
	    return self.__findPropositions(parseTree[1])
	elif (parseTree[0] in LTLUtils.infixOp):
	    return self.__findPropositions(parseTree[1]) + \
		   self.__findPropositions(parseTree[2])
	else:
	    return []


    def isExistentialState0(self, state):
        ## Setzt positive NF von state voraus
        return (type(state) == type('')) or \
               (type(state) == type((None, )) \
                and (state[0] in ['!', '|', 'X', 'U', 'F']))

    def isUniversalState0(self, state):
        ## Setzt positive NF von state voraus
        return (type(state) == type((None, ))) and \
               (state[0] in ['&', 'V', 'G'])

    def true(self, state):
        return 1

    def false(self, state):
        return 0

    def isAccStateMode0(self, state):
	return ((type(state) == type((None, ))) and (state[0] in ['V', 'G'])) \
               or ((type(state) == type("")) and (state == 'tt'))

    def isAccStateMode1(self, state):
        if self.isAccStateMode0(state):
            return 1
        if not (type(state) == type(())):
            return 0
        return (state[0] in ['|', '&']) and self.isAccStateMode1(state[1]) \
               and self.isAccStateMode1(state[2])


    def isAccStateMH(self, state):
        # state: MHstate
        return state[1] == ()
                

    def isAccStateMode3(self, state):
        return not ((type(state) == type(())) and (state[0] in ['U', 'F']))


    def isNotFinalState(self, state):
        return (not self.isFinalState(state))


    def str(self):
        "Returns the transition relation in a readable String format"
        erg = ""
        for p in self.transitionRelation.keys():
            for q in self.transitionRelation[p].keys():
                erg += "(" + LTLUtils.toString(p) + "; " +\
                       str(self.transitionRelation[p][q]) + "; " +\
                       LTLUtils.toString(q) + ")" + (u'\n')
        return erg


    # def tighten(self, TR, start, toBeDone = []): # Zum Debuggen
    #     self.__tighten(TR, start, toBeDone)
    

    #
    # überschriebene neue Methoden aus [Alternating]BuechiAutomaton
    #
    
    def getStates(self):
        """A.getStates() -> list of object

Return a list of all states."""

        return self.transitionRelation.keys()


    def getTransitions(self):
        """A.getTransitions() -> list of (object, object, object)

Return a list of all transitions.  Each transition is a tuple of the
form (fromState, symbol, toState)."""

        transitions = []
        for fromState in self.transitionRelation:
            byTarget = self.transitionRelation[fromState]
            for toState in byTarget.keys():
                for symbol in byTarget[toState]:
                    transitions.append((fromState, symbol, toState))

        return transitions


    def getSuccessors(self, fromState):
        """A.getSuccessors(object) -> list of object

Return a list of all the successor states."""

        return self.transitionRelation[fromState].keys()


    def getAllSymbols(self, fromState, toState):
        """A.getAllSymbols(object, object) -> list of string

Return a list of all symbol representations from fromState to toState."""

        symbols = self.transitionRelation[fromState][toState]
        return map(self.vectorToString, symbols)


    def display(self, userOptions={}, autoOptions={}, tempDir='/tmp/', \
		viewer='gv', dotPath=''):
	"Show an image of the automaton"
        ## tempDir: The output directory.
        ## viewer: The name (and path, if necessary) of the displaying program.
        ## dotPath: The path to the dot program (empty if the path is included in the
        ## $PATH environment variable.
	filename = "".join([tempDir, str(time()), str(randint(100, 999)), \
				'.ps'])
        ## The name of the temporary file contains a timestamp and a random number.
        if DEBUG:
            print filename
	self.saveAs('ps', filename, userOptions, autoOptions, dotPath)
	os.system(viewer + ' ' + filename + ' &')
    

    def writeDot(self, f, userOptions = {}, autoOptions = {}):
        'A.writeDot(file, [hash])'

        options = {'size': '7.27, 10.69', 'ratio': 'compress'}
        options.update(userOptions)
        drawOptions = {'exShape': 'diamond', 'state2string': LTLUtils.toString,
		       'wrapFunc': LTLUtils.wrap}
        drawOptions.update(autoOptions)

        f.write('digraph "' + self.getName() + '" {\n')
        for option in options.keys():
            f.write('    ' + option + '="' + options[option] + '";\n')
        f.write('\n')

        initialState = self.getInitialState()
        if initialState != None:
            f.write('    init [label="", shape=plaintext];\n')
        states = self.getStates()
        for state in states:
            line = '    ' + str(states.index(state))
            label = drawOptions['wrapFunc'](drawOptions['state2string'](state), 24)
            attributes = ['label="' + label + '"',
                          'fontname="TimesNewRoman"']
            if self.isExistentialState(state):
                attributes.append('shape='+drawOptions['exShape'])
            elif self.isUniversalState(state):
                attributes.append('shape=box')
            if self.isFinalState(state):
                attributes.append('peripheries=2')
            line += ' [' + string.join(attributes, ', ') + '];\n'
            f.write(line)
        f.write('\n')

        if initialState != None:
            f.write('    init -> ' + str(states.index(initialState)) + ';\n')
        for fromState in states:
            for toState in self.getSuccessors(fromState):
                symbols = self.getAllSymbols(fromState, toState)
                symbols = map(lambda symbol: str(symbol), symbols)
                f.write('    ' + str(states.index(fromState)) +
                        ' -> ' + str(states.index(toState)) +
                        ' [label="' + string.join(symbols, ', ') +
                        '", fontname="TimesNewRoman"];\n')

        f.write('}\n')



    def writeAutomatonlbtt(self, f):
	"""A.writeAutomatonlbtt(f)

Write this Büchi automaton in the format required by lbtt. \
Use only for non-alternating automata (NBA)!"""

	f.write(str(len(self.stateSet)) + ' 1\n')
	i = 0; stateNum = {}
	for state in self.stateSet:
	    stateNum[state] = i
	    i += 1
	for state in self.stateSet:
	    f.write(str(stateNum[state]))
            if (state == self.initialState):
                f.write(' 1')
            else:
                f.write(' 0')
	    if self.isFinalState(state):
		f.write(' 0')
	    f.write(' -1\n')
	    for succ in self.transitionRelation[state].keys():
		f.write(str(stateNum[succ]) + ' ' + \
			LTLUtils.term2lbtt(self.transitionRelation[state][succ], self.propSet) + '\n')
	    f.write('-1\n')


    def saveAutomatonlbtt(self, fileName):
	"""A.writeAutomatonlbtt(f)

Save this Büchi automaton in the format required by lbtt. \
Use only for non-alternating automata (NBA)!"""

	f = open(fileName, 'w')
	self.writeAutomatonlbtt(f)
	f.close()



"MEMO: Terme über Propositions als Kantenbeschriftung.\
 Eine Kantenbeschriftung ist eine Liste von Vektoren der Länge len(propSet).\
 Ist etwa propSet = ['a', 'b', 'c'], so steht [[0, 1, 1], [-1, 1, -1]] für\
 die Terme {b && c, !a && b && !c}, d.h. jeder Vektor zeigt an, ob in einem\
 konjunktiven Term die Proposition positiv ('1') oder negiert ('-1') vorkommen\
 muß oder ob das egal ist ('0').\
 Eine epsilon-Transition wird durch [[]] dargestellt.\
 \
 Nun hat man a priori ja nur vier Arten von Kantenbeschriftungen: positive \
 Propositions, negative Propositions, epsilon und 2^Prop.\
 Frage: Macht es Sinn, diese Automaten zu 'straffen', so daß man weniger \
 Zustände, aber kompliziertere Kantenbeschriftungen erhält?\
 Falls nicht, braucht man das Theater mit den disj. NF ja gar nicht \
 veranstalten."
 
    
