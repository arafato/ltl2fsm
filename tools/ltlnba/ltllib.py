# -----------------------------------------------------------------------------
#                                  LTL -> NBA
#              Copyright (C) Carsten Fritz, Björn Teegen 2002-2003
#               Sponsored by Deutsche Forschungsgemeinschaft (DFG)
#     Distributed under the terms of the GNU Lesser General Public License
#         See the files README.txt and LICENSE.txt for more information
# -----------------------------------------------------------------------------

import sys

from debuglib import DEBUG
from lexlib import *
from parselib import *
from prettylib import *



NOT = '!'
NEXT = 'X'
FINALLY = 'F'
GLOBALLY = 'G'
UNTIL = 'U'
RELEASE = 'V'
AND = '&'
OR = '|'
IMPL = '->'
EQUIV = '<->'
TT = 'tt'
FF = 'ff'

UNARY_OPS = [NOT, NEXT, FINALLY, GLOBALLY]
BINARY_OPS = [UNTIL, RELEASE, AND, OR, IMPL, EQUIV]


def substitute(formula, name, substitution):
    if type(formula) == type(()):
        op = formula[0]
        if op in UNARY_OPS:
            return (op,
                    substitute(formula[1], name, substitution))
        elif op in BINARY_OPS:
            return (op,
                    substitute(formula[1], name, substitution),
                    substitute(formula[2], name, substitution))
    elif formula == name:
        return substitution
    else:
        return formula


class LTLLexer(StringLexer):

    def __init__(self, s):
        StringLexer.__init__(self, s)
        self.ID_FIRST = self.LOWER_CASE
        self.ID_REST = self.ID_FIRST
        self.NUM_FIRST = ''
        self.TERMINALS = ['<->', '->',
                          '!', '&', '(', ')', '0', '1', 'A', 'B', 'C',
                          'F', 'G', 'U', 'V', 'X', '|']


class LTLParser(Parser):

    def readPrimary(self):
        token = self.readToken()
        if token == '(':
            a = self.readImplOrEquiv()
            self.expect(')')
            return a
        elif token == '1':
            return TT
        elif token == '0':
            return FF
        elif token in ['A', 'B', 'C']:
            return token
        elif self.getTokenClass(token) == 'id':
            return token
        else:
            raise ParserError, 'Operand expected'

    def readPrefix(self):
        token = self.readToken()
        if token == '!':
            a = self.readPrefix()
            return (NOT, a)
        elif token == 'X':
            a = self.readPrefix()
            return (NEXT, a)
        elif token == 'F':
            a = self.readPrefix()
            return (FINALLY, a)
        elif token == 'G':
            a = self.readPrefix()
            return (GLOBALLY, a)
        else:
            self.unreadToken(token)
            return self.readPrimary()

    def readUntilOrRelease(self):
        a = self.readPrefix()
        token = self.readToken()
        if token == 'U':
            b = self.readPrefix()
            return (UNTIL, a, b)
        elif token == 'V':
            b = self.readPrefix()
            return (RELEASE, a, b)
        else:
            self.unreadToken(token)
            return a

    def readAnd(self):
        a = self.readUntilOrRelease()
        token = self.readToken()
        while token == '&':
            b = self.readUntilOrRelease()
            a = (AND, a, b)
            token = self.readToken()
        self.unreadToken(token)
        return a

    def readOr(self):
        a = self.readAnd()
        token = self.readToken()
        while token == '|':
            b = self.readAnd()
            a = (OR, a, b)
            token = self.readToken()
        self.unreadToken(token)
        return a

    def readImplOrEquiv(self):
        a = self.readOr()
        token = self.readToken()
        if token == '->':
            b = self.readOr()
            return (IMPL, a, b)
        elif token == '<->':
            b = self.readOr()
            return (EQUIV, a, b)
        else:
            self.unreadToken(token)
            return a

    def readElement(self):
        token = self.readToken()
        if not token:
            return None
        else:
            self.unreadToken(token)
            return self.readImplOrEquiv()


class LTLPretty(Pretty):

    def __init__(self):
        Pretty.__init__(self)
        self.SYMBOLS = {
            NOT: '!', NEXT: 'X', FINALLY: 'F', GLOBALLY: 'G',
            UNTIL: ' U ', RELEASE: ' V ', AND: ' & ', OR: ' | ',
            IMPL: ' -> ', EQUIV: ' <-> ',
            TT: '1', FF: '0'}
        self.PRIORITIES = {
            NOT: 5, NEXT: 5, FINALLY: 5, GLOBALLY: 5,
            UNTIL: 4, RELEASE: 4, AND: 3, OR: 2,
            IMPL: 1, EQUIV: 1}
        self.BRACKET_RULES = {
            NOT: ((5, 4, 3, 2, 1),),
            NEXT: ((5, 4, 3, 2, 1),),
            FINALLY: ((5, 4, 3, 2, 1),),
            GLOBALLY: ((5, 4, 3, 2, 1),),
            UNTIL: ((4, 3, 2, 1), (4, 3, 2, 1)),
            RELEASE: ((4, 3, 2, 1), (4, 3, 2, 1)),
            AND: ((4, 2, 1), (4, 2, 1)),
            OR: ((4, 3, 1), (4, 3, 1)),
            IMPL: ((4, 3, 2, 1), (4, 3, 2, 1)),
            EQUIV: ((4, 3, 2, 1), (4, 3, 2, 1))}


    def formatAtom(self, atom):
        if atom in (TT, FF):
            return self.SYMBOLS[atom]
        else:
            return str(atom)

    def format(self, expr):
        if type(expr) == type(()):
            op = expr[0]
            operands = []
            for i in range(1, len(expr)):
                if self.withBrackets(op, expr[i], i - 1):
                    operands.append('(' + self.format(expr[i]) + ')')
                else:
                    operands.append(self.format(expr[i]))

            if op in UNARY_OPS:
                return self.SYMBOLS[op] + operands[0]
            elif op in BINARY_OPS:
                return operands[0] + self.SYMBOLS[op] + operands[1]
        else:
            return self.formatAtom(expr)


class SpinLTLPretty(LTLPretty):

    def __init__(self):
        LTLPretty.__init__(self)
        self.SYMBOLS = {
            NOT: '!', NEXT: 'X', FINALLY: '<>', GLOBALLY: '[]',
            UNTIL: ' U ', RELEASE: ' V ', AND: ' && ', OR: ' || ',
            IMPL: ' -> ', EQUIV: ' <-> ',
            TT: 'true', FF: 'false'}


class WringLTLPretty(LTLPretty):

    def __init__(self):
        LTLPretty.__init__(self)
        self.SYMBOLS = {
            NOT: '!', NEXT: 'X', FINALLY: 'F', GLOBALLY: 'G',
            UNTIL: ' U ', RELEASE: ' R ', AND: ' * ', OR: ' + ',
            IMPL: ' -> ', EQUIV: ' <-> ',
            TT: '(a=1) + (a=0)', FF: '(a=1) * (a=0)'}
        self.PRIORITIES = {
            NOT: 1, NEXT: 1, FINALLY: 1, GLOBALLY: 1,
            UNTIL: 1, RELEASE: 1, AND: 1, OR: 1, IMPL: 1, EQUIV: 1}
        self.BRACKET_RULES = {
            NOT: ((1, 0),),
            NEXT: ((1, 0),),
            FINALLY: ((1, 0),),
            GLOBALLY: ((1, 0),),
            UNTIL: ((1, 0), (1, 0)),
            RELEASE: ((1, 0), (1, 0)),
            AND: ((1, 0), (1, 0)),
            OR: ((1, 0), (1, 0)),
            IMPL: ((1, 0), (1, 0)),
            EQUIV: ((1, 0), (1, 0))}


    def formatAtom(self, atom):
        if atom in (TT, FF):
            return self.SYMBOLS[atom]
        else:
            return str(atom) + '=1'


if __name__ == '__main__':
    f = sys.argv[1]
    ##f = 'a | 1 -> c U 0 & !Ge'
    DEBUG(f)
    expr = LTLParser(LTLLexer(f)).parse()
    DEBUG(repr(expr))
    DEBUG('ti:    %s' % LTLPretty().format(expr))
    DEBUG('Spin:  %s' % SpinLTLPretty().format(expr))
    DEBUG('Wring: %s' % WringLTLPretty().format(expr))
