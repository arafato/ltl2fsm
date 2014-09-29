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



def pgz(formula, accMode=3):
    parser = LTLParser(LTLLexer(formula))
    f = posNF(removeTrueFalse(parser.parse()))
    A = EpsiABA(f, accMode)
    ##A.writeDot(sys.stdout)

    G = EpsiDelayedSimGame(A)

    t1 = time()
    rel1 = computeSimRel(G, A.mainStates)
    t1 -= time()
    ##print "Computation according to [And94, FW01] was successful."
    ##print "Computation of sim.rel needed ", t1*(-1), " seconds with And94."

    t2 = time()
    rel2 = G.computeSimulationRelation()
    t2 -= time()
    ##print "Computation according to Jurdzinski in Python was successful."
    ##print "Computation of sim.rel needed ", t2*(-1), " seconds with Jurdzinski."

    if (rel1 != rel2):
        ##print "MAJOR ERROR!! rel1 != rel2"
	return

    rel = rel1
    ##for p in rel.keys():
	##for q in rel[p]:
	    ##print toString(p), " <= ", toString(q)
	    ##print
            ##print

    sim = SimulationRelation(rel)
    NBA = A.toBuechiAutomaton(sim)
    ##NBA.writeAutomatonPromela(stdout)
    ##NBA.display({}, {'exShape': 'ellipse', 'state2string': \
    ##    		     mhStateToString, 'wrapFunc': wrapMH})
    NBA.writeDot(sys.stdout, {}, {'exShape': 'ellipse', 'state2string': \
    		     mhStateToString, 'wrapFunc': wrapMH})
