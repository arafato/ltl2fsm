# -----------------------------------------------------------------------------
#                                  LTL -> NBA
#              Copyright (C) Carsten Fritz, Björn Teegen 2002-2003
#               Sponsored by Deutsche Forschungsgemeinschaft (DFG)
#     Distributed under the terms of the GNU Lesser General Public License
#         See the files README.txt and LICENSE.txt for more information
# -----------------------------------------------------------------------------

from LTLUtils import parse, posNF, mhStateToString, toString, wrapMH, lbttParser, removeTrueFalse
from EpsiABA import EpsiABA
from EpsiDelayedSimGame import EpsiDelayedSimGame
from SimulationRelation import SimulationRelation
from WAADeSimAlgo_opt import computeSimRel
from EpsiJurdzinski import EpsiJurdzinski #, doEpsiJurdzinski_C (no C-code included)
from time import time
from sys import stdout

from ltllib import *
from neverlib import *


def showAuto(formula, accMode=3):
    # formula: String
    parser = LTLParser(LTLLexer(formula))
    f = posNF(removeTrueFalse(parser.parse()))
    A = EpsiABA(f, accMode)

    A.display()
    
    G = EpsiDelayedSimGame(A)

    t1 = time()
    rel1 = computeSimRel(G, A.mainStates)
    t1 -= time()
    print "Computation according to [And94, FW01] was successful."
    print "Computation of sim.rel needed ", t1*(-1), " seconds with And94."

    t2 = time()
    rel2 = G.computeSimulationRelation()
    t2 -= time()
    print "Computation according to Jurdzinski in Python was successful."
    print "Computation of sim.rel needed ", t2*(-1), " seconds with Jurdzinski."

    if (rel1 != rel2):
        print "MAJOR ERROR!! rel1 != rel2"
	return

    rel = rel1
    for p in rel.keys():
	for q in rel[p]:
	    print toString(p), " <= ", toString(q)
	    print
            print

    sim = SimulationRelation(rel)
    NBA = A.toBuechiAutomaton(sim)

    NBA.writeAutomatonPromela(stdout)
    NBA.writeAutomatonlbtt(stdout)

    print "Initial state: ", mhStateToString(NBA.initialState)
    print
    transCount = 0
    for p in NBA.stateSet:
	for q in NBA.transitionRelation[p]:
	    for t in NBA.transitionRelation[p][q]:
		print "From ", mhStateToString(p), " with ", t, " to ", \
		      mhStateToString(q)
		if NBA.isFinalState(p):
		    print mhStateToString(p), "is accepting."
		print 
		transCount += 1

    print "Number of states: ", len(NBA.stateSet)
    print "number of transitions: ", transCount

    NBA.display({}, {'exShape': 'ellipse', 'state2string': \
    		     mhStateToString, 'wrapFunc': wrapMH})

## NO C-CODE INCLUDED IN THIS RELEASE
## ## evtl. in der ersten Klammer: 'size': '4,4'

## ##    print "Berechnung der Sim.rel benötigte ", t1*(-1), " Sekunden mit And94."
## ##    print "Berechnung der Sim.rel benötigte ", t2*(-1), " Sekunden mit Jurdzinski."

##     t3 = time()
##     rel3 = doEpsiJurdzinski_C(G, A.mainStates)
##     t3 -= time()
##     print "Berechnung nach Jurdzinski in C erfolgreich."
##     ##
##     if (rel != rel3):
##         print "MAJOR ERROR!! rel != rel3"
##         return

##     print "Berechnung der Sim.rel benötigte ", t3*(-1), " Sekunden mit Jurdzinski in C."

