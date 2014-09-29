# -----------------------------------------------------------------------------
#                                  LTL -> NBA
#              Copyright (C) Carsten Fritz, Björn Teegen 2002-2003
#               Sponsored by Deutsche Forschungsgemeinschaft (DFG)
#     Distributed under the terms of the GNU Lesser General Public License
#         See the files README.txt and LICENSE.txt for more information
# -----------------------------------------------------------------------------

import os
import string

from lexlib import *
from ltllib import *
from parselib import *


class NeverLexer(FileLexer):

    def __init__(self, f):
        FileLexer.__init__(self, f)
        self.NUM_FIRST = ''
        self.COMMENT_STARTERS = ['/*']
        self.TERMINALS = ['&&', '->', '::', '||',
                          '!', '(', ')', '1', ':', ';', '{', '}']


    def skipComment(self, commentStarter):
        while self.s:
            while self.s and self.s[:2] != '*/':
                self.s = self.s[1:]

            if self.s[:2] == '*/':
                self.s = self.s[2:]
                break

            self.s = self.f.readline()


class NeverParser(Parser):

    def readId(self):
        token = self.readToken()
        if self.getTokenClass(token) == 'id':
            return token
        else:
            raise ParserError, 'Identifier expected'

    def readPrimary(self):
        token = self.readToken()
        if token == '(':
            a = self.readAnd()
            self.expect(')')
            return a
        elif token == '1':
            return TT
        elif self.getTokenClass(token) == 'id':
            return token
        else:
            raise ParserError, 'Operand expected'

    def readPrefix(self):
        token = self.readToken()
        if token == '!':
            a = self.readPrefix()
            return (NOT, a)
        else:
            self.unreadToken(token)
            return self.readPrimary()

    def readAnd(self):
        a = self.readPrefix()
        token = self.readToken()
        while token == '&&':
            b = self.readPrefix()
            a = (AND, a, b)
            token = self.readToken()
        self.unreadToken(token)
        return a

    def readOr(self, terms):
        terms.append(self.readAnd())
        token = self.readToken()
        while token == '||':
            terms.append(self.readAnd())
            token = self.readToken()
        self.unreadToken(token)

    def readTerms(self):
        terms = []
        self.readOr(terms)
        return terms

    def readStmnt(self):
        token = self.readToken()
        if self.getTokenClass(token) == 'id':
            fromId = token
            self.expect(':')
            token = self.readToken()
            if token == 'false':
                pass
            elif token == 'if':
                self.automaton.addState(fromId)
                token = self.readToken()
                while token == '::':
                    terms = self.readTerms()
                    self.expect('->')
                    self.expect('goto')
                    toId = self.readId()
                    self.automaton.addTransition(fromId, terms, toId)
                    token = self.readToken()
                self.unreadToken(token)
                self.expect('fi')
            elif token == 'skip':
                self.automaton.addState(fromId)
                self.automaton.addTransition(fromId, [TT], fromId)
            else:
                raise ParserError, "'false', 'if' or 'skip' expected"
        else:
            self.unreadToken(token)

    def readSequence(self):
        self.readStmnt()
        token = self.readToken()
        while token == ';':
            self.readStmnt()
            token = self.readToken()
        self.unreadToken(token)

    def readElement(self):
        token = self.readToken()
        if not token:
            return None
        else:
            self.automaton = NeverAutomaton()
            self.unreadToken(token)
            self.expect('never')
            self.expect('{')
            self.readSequence()
            self.expect('}')
            return self.automaton


class NeverAutomaton:

    def __init__(self):
        self.states = {}
        self.initialState = None
        self.transitions = {}
        self.pretty = LTLPretty()


    def addState(self, id):
        init = id[-5:] == '_init'
        accept = id[:7] == 'accept_'

        name = id
        if init: name = name[:-5]
        if accept: name = name[7:]
        if name == 'all': name = 'tt'

        state = (name, accept)
        self.states[id] = state
        self.transitions[id] = {}
        if init:
            self.initialState = id

    def addTransition(self, fromId, terms, toId):
        self.transitions[fromId][toId] = terms

    def getInitialState(self):
        return self.initialState

    def getStateName(self, id):
        return self.states[id][0]

    def isFinalState(self, id):
        return self.states[id][1]

    def getStates(self):
        return self.states.keys()

    def getSuccessors(self, id):
        return self.transitions[id].keys()

    def getAllTerms(self, fromId, toId):
        terms = self.transitions[fromId][toId]
        result = []
        for term in terms:
            result.append(self.pretty.format(term))
        return result


    def writeDot(self, f, userOptions = {}):
        'A.writeDot(file, [hash])'

        options = {'size': '7.27, 10.69', 'ratio': 'compress'}
        options.update(userOptions)

        f.write('digraph G {\n')
        for option in options.keys():
            f.write('    ' + option + '="' + options[option] + '";\n')
        f.write('\n')

        initialState = self.getInitialState()
        if initialState != None:
            f.write('    init [label="", shape=plaintext];\n')
        for state in self.getStates():
            line = '    ' + state
            attributes = ['label="' + self.getStateName(state) + '"',
                          'fontname="TimesNewRoman"']
            attributes.append('shape=ellipse')
            if self.isFinalState(state):
                attributes.append('peripheries=2')
            line = line + ' [' + string.join(attributes, ', ') + '];\n'
            f.write(line)
        f.write('\n')

        if initialState != None:
            f.write('    init -> ' + initialState + ';\n')
        for fromState in self.getStates():
            for toState in self.getSuccessors(fromState):
                terms = self.getAllTerms(fromState, toState)
                f.write('    ' + fromState + ' -> ' + toState +
                        ' [label="' + string.join(terms, ', ') + '",' +
                        ' fontname="TimesNewRoman"];\n')

        f.write('}\n')


    def saveAs(self, format, fileName, userOptions = {}):
        'A.saveAs(string, string, [hash])'

        f = os.popen('dot -T' + format + ' -o "' + fileName + '"', 'w')
        self.writeDot(f, userOptions)
        return f.close()
