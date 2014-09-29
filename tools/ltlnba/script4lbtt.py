#!/usr/bin/python

# -----------------------------------------------------------------------------
#                                  LTL -> NBA
#              Copyright (C) Carsten Fritz, Björn Teegen 2002-2003
#               Sponsored by Deutsche Forschungsgemeinschaft (DFG)
#     Distributed under the terms of the GNU Lesser General Public License
#         See the files README.txt and LICENSE.txt for more information
# -----------------------------------------------------------------------------


"""Format: \
path-to-this-program parameters-with-dash input-file-name output-file-name"""

import sys
from LTLUtils import posNF, lbttParser, removeTrueFalse
from EpsiABA import EpsiABA
from EpsiDelayedSimGame import EpsiDelayedSimGame
from SimulationRelation import SimulationRelation
from WAADeSimAlgo_opt import computeSimRel

LONGOPTIONS = ['-A', '-a'] # options that require another parameter

inFilename = ''; outFilename = ''
i = 1; optDict = {'-A': '3', '-a': 'And'}
while sys.argv[i][0] == '-':
    if sys.argv[i] in LONGOPTIONS:
	optDict[sys.argv[i]] = sys.argv[i+1]
	i += 2
    else:
	optDict[sys.argv[i]] = ''
	i += 1

if len(sys.argv[i:]) != 2:
    print 'USAGE: $ script4lbtt.py options input-ltl-file output-nba-file'
    sys.exit()

inFilename = sys.argv[i]
outFilename = sys.argv[i+1]

f = open(inFilename, 'r')
formula = f.readline()
f.close()

if formula[-1] == '\n':
    formula = formula[:-1]

A = EpsiABA(posNF(removeTrueFalse(lbttParser(formula)[0])), int(optDict['-A']))
G = EpsiDelayedSimGame(A)

rel = None
if optDict['-a'] == 'And':
    rel = computeSimRel(G, A.mainStates)
elif optDict['-a'] == 'Jurd':
    rel = G.computeSimulationRelation()
elif optDict['-a'] == 'Xcheck':
    rel1 = computeSimRel(G, A.mainStates)
    rel2 = G.computeSimulationRelation()
    if rel1 != rel2:
	print "MAJOR ERROR!!"
	print "The two game solvers have different results!"
	sys.exit(666)
    else:
	rel = rel1

sim = SimulationRelation(rel)
NBA = A.toBuechiAutomaton(sim)

NBA.saveAutomatonlbtt(outFilename)


