# -----------------------------------------------------------------------------
#                                  LTL -> NBA
#              Copyright (C) Carsten Fritz, Björn Teegen 2002-2003
#               Sponsored by Deutsche Forschungsgemeinschaft (DFG)
#     Distributed under the terms of the GNU Lesser General Public License
#         See the files README.txt and LICENSE.txt for more information
# -----------------------------------------------------------------------------

LexerError = 'Lexer error'


class Lexer:

    def __init__(self):
        self.WHITESPACES = '\t\n\f\r '
        self.UPPER_CASE = 'ABCDEFGHIJKLMNOPQRSTUVWXYZ'
        self.LOWER_CASE = 'abcdefghijklmnopqrstuvwxyz'
        self.DIGITS = '0123456789'

        self.ID_FIRST = self.UPPER_CASE + self.LOWER_CASE
        self.ID_REST = self.ID_FIRST + self.DIGITS + '_'

        self.NUM_FIRST = self.DIGITS
        self.NUM_REST = self.NUM_FIRST

        self.STR_FIRST = ''

        self.COMMENT_STARTERS = []

        self.KEYWORDS = []
        self.TERMINALS = []


    def getTokenClass(self, token):
        if not token:
            return None
        elif token in self.KEYWORDS:
            return 'keyword'
        elif token in self.TERMINALS:
            return token
        elif token[0] in self.ID_FIRST:
            return 'id'
        elif token[0] in self.NUM_FIRST:
            return 'num'
        elif token[0] in self.STR_FIRST:
            return 'str'
        else:
            raise LexerError, 'Illegal token: ' + repr(token)


    def skipComment(self, commentStarter):
        pass


    def skipWhitespacesAndComments(self):
        pass


    def readStr(self):
        starter = self.s[0]
        token = starter
        self.s = self.s[1:]
        while self.s and (self.s[0] != starter):
            token += self.s[0]
            self.s = self.s[1:]
        token += self.s[0]
        self.s = self.s[1:]
        return token


    def readRest(self, rest):
        token = self.s[0]
        self.s = self.s[1:]
        while self.s and self.s[0] in rest:
            token = token + self.s[0]
            self.s = self.s[1:]
        return token


    def readToken(self):
        self.skipWhitespacesAndComments()
        if not self.s:
            return None

        for terminal in self.TERMINALS:
            if self.s[:len(terminal)] == terminal:
                token = terminal
                self.s = self.s[len(terminal):]
                return token

        if self.s[0] in self.ID_FIRST:
            return self.readRest(self.ID_REST)
        elif self.s[0] in self.NUM_FIRST:
            return self.readRest(self.NUM_REST)
        elif self.s[0] in self.STR_FIRST:
            return self.readStr()

        raise LexerError, 'Illegal character: ' + repr(self.s[0])


class StringLexer(Lexer):

    def __init__(self, s):
        Lexer.__init__(self)
        self.s = s


    def skipWhitespacesAndComments(self):
        ready = 0
        while not ready:
            while self.s and self.s[0] in self.WHITESPACES:
                self.s = self.s[1:]

            ready = 1

            for commentStarter in self.COMMENT_STARTERS:
                if self.s[:len(commentStarter)] == commentStarter:
                    ready = 0
                    self.s = self.s[len(commentStarter):]
                    self.skipComment(commentStarter)
                    break


class FileLexer(Lexer):

    def __init__(self, f):
        Lexer.__init__(self)
        self.f = f
        self.s = f.readline()


    def skipWhitespacesAndComments(self):
        while self.s:
            ready =  0
            while not ready:
                while self.s and self.s[0] in self.WHITESPACES:
                    self.s = self.s[1:]

                ready = 1

                for commentStarter in self.COMMENT_STARTERS:
                    if self.s[:len(commentStarter)] == commentStarter:
                        ready = 0
                        self.s = self.s[len(commentStarter):]
                        self.skipComment(commentStarter)
                        break

            if self.s: break

            self.s = self.f.readline()
