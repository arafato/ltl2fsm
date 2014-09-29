# -----------------------------------------------------------------------------
#                                  LTL -> NBA
#              Copyright (C) Carsten Fritz, Björn Teegen 2002-2003
#               Sponsored by Deutsche Forschungsgemeinschaft (DFG)
#     Distributed under the terms of the GNU Lesser General Public License
#         See the files README.txt and LICENSE.txt for more information
# -----------------------------------------------------------------------------

from debuglib import DEBUG



ParserError = 'Parser error'


class Parser:

    def __init__(self, lexer):
        self.lexer = lexer
        self.unreadBuffer = []


    def readToken(self):
        if self.unreadBuffer:
            return self.unreadBuffer.pop()
        else:
            return self.lexer.readToken()


    def unreadToken(self, token):
        if token != None:
            self.unreadBuffer.append(token)


    def getTokenClass(self, token):
        return self.lexer.getTokenClass(token)


    def readId(self):
        token = self.readToken()
        if self.getTokenClass(token) == 'id':
            return token
        else:
            raise ParserError, 'Identifier expected'


    def expect(self, token):
        if self.readToken() != token:
            raise ParserError, "'" + token + "' expected"


    def readElement(self):
        token = self.readToken()
        if not token:
            return None
        else:
            self.unreadToken(token)
            return self.readElement2()


    def readElement2(self):
        raise NotImplementedError


    def parse(self):
        element = self.readElement()
        if self.readToken():
            raise ParserError, 'Extra input'
        elif element == None:
            raise ParserError, 'No input'
        else:
            return element
