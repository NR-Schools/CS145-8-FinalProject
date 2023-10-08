import re, json

def refine(input_list):
    input_list = sorted(list(set(input_list)), key=lambda x: len(x), reverse=True)
    return input_list

def get_RES_SYM_TXT(source_code, reserved_words_filter, symbols_filter):
    reserved_words_filter = refine(reserved_words_filter)
    symbols_filter = refine(symbols_filter)
    reserved_words = []
    symbols = []
    library = refine(reserved_words_filter+[re.escape(symbol) for symbol in symbols_filter])
    library_pattern = r"(?:{})".format("|".join(library))
    library_matches = re.findall(library_pattern, source_code)
    reserved_words = [lexical for lexical in library_matches if lexical in reserved_words_filter]
    symbols = [lexical for lexical in library_matches if lexical in symbols_filter]
    RES_SYM = {
        "reserved_words": reserved_words,
        "symbols":symbols
    }
    with open("RES_SYM.TXT", "w") as f:
        json.dump(RES_SYM, f, indent=4)
    return source_code

def get_NOSPACES_TXT(source_code):
    source_code = source_code.replace(" ", "")
    with open("NOSPACES.TXT", "w") as f:
        f.write(source_code)
    return source_code