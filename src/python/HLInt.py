import sys
import NOSPACES
import RES_SYM
import LEXER as lexer
import PARSER as parser

with open(sys.argv[1], "r") as f:
    source_code = f.read()
# print(source_code)

source_code = NOSPACES.get_NOSPACES_TXT(source_code)
    
reserved_words_filter = ["integer", "double", "output", "if"]
symbols_filter = [":", ":=", "<<", "+", "-", "==", "!=", "<", ">", "(", ")", ";"]

source_code = RES_SYM.get_RES_SYM_TXT(source_code, reserved_words_filter, symbols_filter)

# lexer.input(source_code)
# while True:
#     tok = lexer.token()
#     if not tok: 
#         break  
#     print(tok)

parser.parse(source_code)
if not parser.error_reported:
    print("NO ERROR(S) FOUND")
