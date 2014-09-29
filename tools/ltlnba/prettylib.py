# -----------------------------------------------------------------------------
#                                  LTL -> NBA
#              Copyright (C) Carsten Fritz, Björn Teegen 2002-2003
#               Sponsored by Deutsche Forschungsgemeinschaft (DFG)
#     Distributed under the terms of the GNU Lesser General Public License
#         See the files README.txt and LICENSE.txt for more information
# -----------------------------------------------------------------------------

class Pretty:

    def __init__(self):
        self.SYMBOLS = {}
        self.PRIORITIES = {}
        self.BRACKET_RULES = {}


    def getPriority(self, expr):
        if type(expr) == type(()):
            op = expr[0]
            return self.PRIORITIES[op]
        else:
            return 0

    def withBrackets(self, op, expr, i):
        priority = self.getPriority(expr)
        return priority in self.BRACKET_RULES[op][i]


    def format(self, expr):
        return str(expr)
