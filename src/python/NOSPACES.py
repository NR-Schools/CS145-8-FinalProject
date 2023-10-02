def get_NOSPACES_TXT(source_code):
    source_code = source_code.replace(" ", "")
    with open("NOSPACES.TXT", "w") as f:
        f.write(source_code)
    return source_code


