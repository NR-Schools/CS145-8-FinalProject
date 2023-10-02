import ply.yacc as yacc
from LEXER import tokens

def p_program(p):
    """program : program statement SEMI_COLON
               | statement SEMI_COLON"""
    p[0] = [p[1]] if len(p) == 3 else p[1] + [p[2]]

def p_statement(p):
    """statement : conditional_statement
                  | variable_declaration
                  | assignment_statement
                  | print_statement """
    p[0] = p[1]

def p_variable_declaration(p):
    """variable_declaration : IDENTIFIER COLON TYPE"""
    p[0] = (p[1], p[3])

def p_assignment_statement(p):
    """assignment_statement : IDENTIFIER ASSIGNMENT expression"""
    p[0] = (p[1], p[3])
    
def p_conditional_statement(p):
    """conditional_statement : IF OPEN_PAREN comparison CLOSE_PAREN COLON statement"""
    p[0] = (p[3], p[6]) if len(p) == 7 else (p[3], None)

def p_expression(p):
    """expression : term
                  | term PLUS term
                  | term MINUS term"""
    p[0] = p[1] if len(p) == 2 else (p[1], p[2], p[3])

def p_term(p):
    """term : INTEGER
            | DOUBLE
            | IDENTIFIER
            | STRING"""
    p[0] = p[1]

def p_print_statement(p):
    """print_statement : OUTPUT expression"""
    p[0] = p[2]

def p_comparison(p):
    """comparison : expression LT expression
                  | expression GT expression
                  | expression EQ expression
                  | expression NEQ expression"""
    p[0] = (p[1], p[2], p[3])

# def p_error(p):
#     if p:
#         print("Syntax error at '%s' of type %s at line %d position %d" % (p.value, p.type, p.lineno, p.lexpos))
#     else:
#         print("Syntax error at EOF")

error_reported = False

def p_error(p):
    global error_reported
    if not error_reported:
        if p:
            print("ERROR")
            error_reported = True
        else:
            print("ERROR")
            error_reported = True

def parse(source_code):
    parser = yacc.yacc()
    parser.parse(source_code)