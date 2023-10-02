import os
import subprocess

def execute_file(file_path):
    try:
        # Use subprocess to execute the file
        output = subprocess.check_output(file_path, shell=True)
        return output.decode('utf-8').replace("\n", " ")
    except subprocess.CalledProcessError as e:
        return f"Error: {e}"

def path_resolver(language: str) -> str:
    if language == "c":
        return "cd src/c && HLint"
    if language == "cpp":
        return "cd src/cpp && HLint"
    if language == "python":
        return "cd src/python && HLint.py"
    if language == "haskell":
        return "cd src/haskell && HLint"

if __name__ == "__main__":
    # Ask for language
    print("Make sure that there is a file ready to be executed")
    src_lang = input('Source Language: ')
    print()

    tests_dir = "tests"

    for current_test in next(os.walk(tests_dir))[1]:
        folder_path = os.path.join(tests_dir, current_test)
        print(f"Current Test: {current_test}")
        test_no = 1

        for folder_root, _, files in os.walk(folder_path):
            for file_name in files:
                if file_name.endswith(".HL"):
                    file_path = os.path.join(folder_root, file_name)
                    output = execute_file(f"{path_resolver(src_lang)} ../../{file_path} && cd.. && cd..")
                    print(f" Test for {src_lang} [{test_no}/{len(files)}] --> {output}")
                    test_no += 1

        print("-- END --")
        print()
