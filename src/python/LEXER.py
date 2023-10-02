import ply.lex as lex

tokens = ['TYPE', 'IF',  'INTEGER', 'DOUBLE', 'ASSIGNMENT', 'PLUS', 'MINUS', 'OUTPUT', 'SEMI_COLON', 'OPEN_PAREN', 'CLOSE_PAREN', 'LT', 'GT', 'EQ', 'NEQ', 'COLON', 'STRING', 'IDENTIFIER']

t_TYPE = r'\b(integer|double)\b'  # Add more types as needed
t_IF = r'\b(if|for|integer)\b' # i dont understand
t_ASSIGNMENT = r':='
t_PLUS = r'\+'
t_MINUS = r'-'
t_OUTPUT = r'(?i)\s*output\s*<<\s*'
t_INTEGER = r'-?[0-9]+'
t_DOUBLE = r'-?[0-9]+\.[0-9]+'
t_SEMI_COLON = r';'
t_OPEN_PAREN = r'\('
t_CLOSE_PAREN = r'\)'
t_EQ = r'=='
t_NEQ = r'!='
t_LT = r'<'
t_GT = r'>'
t_COLON = r':'
t_STRING = r'\".*?\"'
t_IDENTIFIER = r'[a-zA-Z][a-zA-Z0-9]*' 
t_ignore = ' \t\n'

def t_error(t):
    print("Illegal character '%s'" % t.value[0])
    t.lexer.skip(1)

lexer = lex.lex()