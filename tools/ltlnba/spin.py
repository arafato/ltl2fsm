#!/usr/bin/python

# -----------------------------------------------------------------------------
#                                  LTL -> NBA
#              Copyright (C) Carsten Fritz, Björn Teegen 2002-2003
#               Sponsored by Deutsche Forschungsgemeinschaft (DFG)
#     Distributed under the terms of the GNU Lesser General Public License
#         See the files README.txt and LICENSE.txt for more information
# -----------------------------------------------------------------------------

import sys
import os
from LTLUtils import parse, posNF
from EpsiABA import EpsiABA
from EpsiDelayedSimGame import EpsiDelayedSimGame
from SimulationRelation import SimulationRelation
from WAADeSimAlgo_opt import computeSimRel

if len(sys.argv) < 3:
    print 'USAGE: $ spin.py "LTL formula" "name of a Promela system description'
    print 'file" ["append-to file for the never claim"]'
    sys.exit()

formula = sys.argv[1]
f = posNF(parse(formula))
A = EpsiABA(f, 3)
G = EpsiDelayedSimGame(A)
rel = computeSimRel(G, A.mainStates)
sim = SimulationRelation(rel)
NBA = A.toBuechiAutomaton(sim)
options = {"letters": NBA.propSet}
if len(sys.argv) >= 4:
    options["appendTo"] = sys.argv[3]
NBA.saveAutomatonPromela(sys.argv[2] + ".nba", options)

print "THE OUTPUT USING MY AUTOMATON"
print

f = os.popen('time spin -a -N ' + sys.argv[2] + '.nba ' + sys.argv[2])
data = f.readlines()
f.close()
for output in data:
    print output

f = os.popen('time gcc -o pan pan.c')
data = f.readlines()
f.close()
for output in data:
    print output

f = os.popen('time pan -a')
data1 = f.readlines()
for output in data1:
    print output[:-1]

# And now Spin
print
print "THE OUTPUT USING SPIN'S AUTOMATON"
print

formula = formula.replace("F", "<>")
formula = formula.replace("G", "[]")
formula = formula.replace("|", "||")
formula = formula.replace("&", "&&")

if options.has_key("appendTo"):
    g = open(options["appendTo"], 'r')
    f = open(sys.argv[2] + '.spin.nba', 'w')
    allLines = g.readlines()
    f.writelines(allLines)
    f.close()
    g.close()
    f = open(sys.argv[2] + '.spin.nba', 'a')
else:
    f = open(sys.argv[2] + '.spin.nba', 'w')
g = os.popen('spin -f "' + formula + '"')
data = g.readlines()
g.close()
f.writelines(data)
f.close

f = os.popen('time spin -a -N ' + sys.argv[2] + '.spin.nba ' + sys.argv[2])
data = f.readlines()
f.close()
for output in data:
    print output

f = os.popen('time gcc -o pan pan.c')
data = f.readlines()
f.close()
for output in data:
    print output

f = os.popen('time pan -a')
data2 = f.readlines()
for output in data2:
    print output[:-1]

print
print "DIFFERENCES"
print

mn = min(len(data1), len(data2))
for i in range(mn):
    if data1[i] != data2[i]:
	print "Line ", i, ": "
	print data1[i][:-1]
	print "vs."
	print data2[i][:-1]
if len(data1) != len(data2):
    laenger = ""
    if mn == len(data1):
	data = data2[mn:]
	laenger = "Spin's"
    else:
	data = data1[mn:]
	laenger = "my"
    print "excess in ", laenger, " output:"
    for output in data:
	print output[:-1]
    
    
