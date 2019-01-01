import os
import sys
import argparse
import subprocess

#Const globals
EXIT_CODE_SUCCESS = 0
EXIT_CODE_FAILURE_BUILD_APP = 1
EXIT_CODE_FAILURE_BUILD_LOCATION = 2

QT_LOC_VAR_NAME = "QT_LOC"
VS_LOC_VAR_NAME = "VS_LOC"
JOM_LOC_VAR_NAME = "JOM_LOC"
ZIP_LOC_VAR_NAME = "ZIP_LOC"
NSIS_LOC_VAR_NAME = "NSIS_LOC"
LLVM_LOC_VAR_NAME = "LLVM_LOC"

QT_LOC_DEFAULT = r"C:\Qt\5.11.1\msvc2017_64\bin\""
VS_LOC_DEFAULT = r"C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\VC\Auxiliary\Build\vcvarsall.bat"
JOM_LOC_DEFAULT = r"C:\Qt\Tools\QtCreator\bin\jom.exe"
ZIP_LOC_DEFAULT = r"C:\Program Files\7-Zip\7z.exe"
NSIS_LOC_DEFAULT = r"C:\Program Files (x86)\NSIS\""
LLVM_LOC_DEFAULT = r"C:\Program Files\LLVM\bin\""

ORIGINAL_DIR = os.getcwd()
PROJECT_DIR = os.path.abspath(os.path.join(ORIGINAL_DIR , "..\.."))

PLATFORM_TARGET = "AMD64"

#Mutable globals
current_activity = ""

bat_file_contents = ""


###### Utility funcs

def say(message: str):
    print(current_activity + ": " + message)

def say_error(message: str):
    print(current_activity + ": " + message)

def exit_on_error(error_number):
    os.chdir(ORIGINAL_DIR)
    sys.exit(error_number)

def get_env_var_and_test_validity(env_var_name: str, default_env_var_value: str):
    if not is_env_var_set(env_var_name):
        say(f"{env_var_name} not defined. Using default value.")
        path = default_env_var_value
    else:
        path = os.getenv(env_var_name)
    say(f"{env_var_name} set to '{path}'")
    if not os.path.exists(path):
        say_error(f"{env_var_name} does not exist. Exiting")
        exit_on_error(EXIT_CODE_FAILURE_BUILD_LOCATION)
    say(f"{env_var_name} exists.")
    return path

def is_env_var_set(env_var_name: str):
    var = os.getenv(env_var_name)
    if var is None:
        return False
    return True

def run_process(process: list):
    proc = subprocess.run(process,
                       stdout=subprocess.PIPE,
                       stderr=subprocess.PIPE)
    print("STDOUT:")
    print(proc.stdout.decode("utf-8"))
    print("STDERR:")
    print(proc.stderr.decode("utf-8"))
    if proc.returncode != 0:
        exit_on_error(proc.returncode)

def add_line_to_run_bat_file(line: str):
    global bat_file_contents
    bat_file_contents += line + "\n"

def add_error_handling_line_to_bat_file():
    global bat_file_contents
    bat_file_contents += "IF ERRORLEVEL 1 EXIT /B " + str(EXIT_CODE_FAILURE_BUILD_APP) + "\n"

def create_batch_file():
    file = open("test.bat", "w+")
    file.write(bat_file_contents)
    file.close()

###### End of utility funcs

def build():
    #Mutable global variables are bad, mmkay
    global current_activity
    current_activity = "BUILD"
    COMPILE_MODE = ""
    COMPILER = ""
    
    say("Testing if all required build environment variables are set:")
    QT_LOC = get_env_var_and_test_validity(QT_LOC_VAR_NAME, QT_LOC_DEFAULT)
    VS_LOC = get_env_var_and_test_validity(VS_LOC_VAR_NAME, VS_LOC_DEFAULT)
    
    if is_env_var_set("BUILD_CLANG"):
        say("BUILD_CLANG defined. Building for win32-clang-msvc.")
        get_env_var_and_test_validity(LLVM_LOC_VAR_NAME, LLVM_LOC_DEFAULT)
        COMPILER = "win32-clang-msvc"
    else:
        say("BUILD_CLANG not defined. Building for msvc.")
        COMPILER = "win32-msvc"
        
    say("All required build environment values are set.")

    say("Testing optional build environment variables:")

    add_line_to_run_bat_file("cd " + PROJECT_DIR)

    #SET UP VS
    say("Adding 'vcvarsall.bat' to batch file.")
    add_line_to_run_bat_file("@ECHO Setting up environment with Visual Studio 'vcvarsall.bat':")
    add_line_to_run_bat_file("call " + '"' + VS_LOC + '"' + " " + PLATFORM_TARGET)
    add_error_handling_line_to_bat_file()
    add_line_to_run_bat_file("@ECHO Environment set up.")
    say("'vcvarsall.bat' added to batch file.")

    QMAKE_LOC = QT_LOC + r"\qmake.exe"

    if is_env_var_set("BUILD_VSPROJ"):
        say("BUILD_VSPROJ defined. Building Visual Studio project files.")
        add_line_to_run_bat_file("@ECHO Running Visual Studio project file building:")
        add_line_to_run_bat_file('"' + QMAKE_LOC + '"' +  " -tp vc")
        add_error_handling_line_to_bat_file()
        add_line_to_run_bat_file("@ECHO Done building Visual Studio project files.")
    else:
        say("BUILD_VSPROJ not defined. Not building Visual Studio project files.")

    if is_env_var_set("BUILD_DEBUG"):
        COMPILE_MODE = "debug"
        say("BUILD_DEBUG defined. Building '{COMPILE_MODE}' version.")
    else:
        COMPILE_MODE = "release"
        say("BUILD_DEBUG not defined. Building '{COMPILE_MODE}' version.")

    add_line_to_run_bat_file("@ECHO Running qmake:")
    add_line_to_run_bat_file('"' + QMAKE_LOC + '"' +  " -spec " + COMPILER + " CONFIG+=X86_64 " + "CONFIG+=" + COMPILE_MODE)
    add_error_handling_line_to_bat_file()
    add_line_to_run_bat_file("@ECHO qmake done.")    
        
    if is_env_var_set(JOM_LOC_VAR_NAME):
        JOM_LOC = os.getenv(JOM_LOC_VAR_NAME)
    else:
        say(f"{JOM_LOC_VAR_NAME} not defined. Using default value")
        JOM_LOC = JOM_LOC_DEFAULT
    say(f"{JOM_LOC_VAR_NAME} set to '{JOM_LOC}")

    if os.path.exists(JOM_LOC):
        say(f"{JOM_LOC_VAR_NAME} exists. Using jom.")
        add_line_to_run_bat_file("@ECHO Running jom:")
        add_line_to_run_bat_file(JOM_LOC)
        add_error_handling_line_to_bat_file()
        add_line_to_run_bat_file("@ECHO jom done.")
        say(f"{JOM_LOC_VAR_NAME} added to batch file.")
    else:
        say(f"{JOM_LOC_VAR_NAME} does not exists. Using nmake.")
        add_line_to_run_bat_file("@ECHO Running nmake:")
        #nmake is in the path because of 'vcvarsall.bat' from VS_LOC
        add_line_to_run_bat_file("nmake")
        add_error_handling_line_to_bat_file()
        add_line_to_run_bat_file("@ECHO nmake done.")
        say("nmake added to batch file.")

    create_and_run_batch_file()
    
if __name__ == "__main__":
    build()
    #parser = argparse.ArgumentParser(description='Builds OpenVR Advanced Settings on Windows.')
    #parser.add_argument('action', type=str, help='action to perform. Can be build, deploy, package or format')
    #args = parser.parse_args()
    #if args.action == "build":
    #    build()
