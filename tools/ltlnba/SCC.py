# -----------------------------------------------------------------------------
#                                  LTL -> NBA
#              Copyright (C) Carsten Fritz, Björn Teegen 2002-2003
#               Sponsored by Deutsche Forschungsgemeinschaft (DFG)
#     Distributed under the terms of the GNU Lesser General Public License
#         See the files README.txt and LICENSE.txt for more information
# -----------------------------------------------------------------------------

def computeTransposedGraph(G):
    if (type(G) == type({})):
        GT = {}
        bereich = G.keys()
    elif (type(G) == type([])):
        GT = []
        bereich = range(len(G))
        
    for i in bereich:
        GT[i] = []
    for i in bereich:
        for j in G[i]:
           GT[j].append(i)
    return GT


def cTG2(nodeSet, succFunc):
    # nodelist: list of node
    # succFunc: function nodeset -> set of list of node
    G = {}
    for node in nodeSet:
	G[node] = succFunc(node)
    return computeTransposedGraph(G)

#
# SCC-algorithm
# 

def scc(G, GT = None):
    # G: Dictionary of (Node, List of node)
    # OR G: List of List of Number; both is an adjacency list
    "Returns the topologically sorted list of SCCs of G \
     (List of List of Node)."
    if GT == None:
        GT = computeTransposedGraph(G)
    dfs(G)    
    dfs(GT, R)
    return SCC


def scc2(nodelist, succFunc, GT = None):
    # nodelist: list of node
    # succFunc: function nodeset -> set of list of node
    G = {}
    for node in nodelist:
	G[node] = succFunc(node)
    return scc(G, GT)

	
#
# DFS-algorithm
#
# G is an adjacency list
# V (optional) is a list describing an ordering of nodes 

BLACK = 0
GRAY  = 1
WHITE = 2

#
# Computes in R all nodes as a list in <F order.
#

def dfs(G, V = None):
    global t,c,p,D,F,R,SCC #mMn: BÖSE
    if (type(G) == type({})):
        c = {}
        p = {}
        D = {}
        F = {}
        if V == None:
            V = G.keys()
        for v in V:
            c[v] = WHITE
            p[v] = -1
    elif (type(G) == type([])):
        c = [WHITE] * len(G)
        p = [-1] * len(G)
        D = [-1] * len(G)
        F = [-1] * len(G)
        if V == None:
            V = range(len(G))

    R = []
    SCC = []
    t = 0
    for v in V:
	if c[v] == WHITE:
	    scc = [v]
	    __dfs(G, v, scc)
	    SCC.append(scc)
            

def __dfs(G, u, scc):
    global t,c,p,D,F,R
    c[u] = GRAY
    t += 1
    D[u] = t
    for v in G[u]:
	if c[v] == WHITE:
	    scc.append(v)
	    p[v] = u
	    __dfs(G,v,scc)
    c[u] = BLACK
    t += 1
    F[u] = t
    R[0:0] = [u]
