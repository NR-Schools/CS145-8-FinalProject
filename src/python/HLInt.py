import sys
import json

#data defining
def refine(input_list):
  input_list = sorted(list(set(input_list)), key=lambda x: len(x), reverse=True)
  return input_list

def get_NOSPACES_TXT(code):
  no_spaces = code.replace(" ", "")
  with open("NOSPACES.TXT", "w") as f:
    f.write(no_spaces)

import re

def get_RES_SYM_TXT(code,reserved_words_filter,symbols_filter):
  reserved_words = []
  symbols = []
  library = refine(reserved_words_filter+[re.escape(symbol) for symbol in symbols_filter])

  # Create a regex pattern for reserved words and symbols
  library_pattern = r"(?:{})".format("|".join(library))

  # Find all matches in the code
  library_matches = re.findall(library_pattern, code)

  # Append matches to the respective lists
  reserved_words = [lexical for lexical in library_matches if lexical in reserved_words_filter]
  symbols = [lexical for lexical in library_matches if lexical in symbols_filter]

  print(reserved_words)
  print(symbols)
  RES_SYM = {
    "reserved_words": reserved_words,
    "symbols":symbols
  }
  with open("RES_SYM.TXT", "w") as f:
    json.dump(RES_SYM, f, indent=4)
    
def main():
  reserved_words_filter = refine(["integer", "double", "output", "if"])
  symbols_filter = refine([":", ":=", "<<", "+", "-", "<", ">", "(", ")", ";"])
  
  if len(sys.argv) != 2:
    print("Usage: python3 HLInt.py <filename>")
    return
    
  with open(sys.argv[1], "r") as f:
    code = f.read()
  
  get_NOSPACES_TXT(code)
  get_RES_SYM_TXT(code,reserved_words_filter,symbols_filter)

if __name__ == "__main__":
  main()