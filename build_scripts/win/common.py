"""
Utility functions for building OpenVR Advanced Settings.
"""
import os
import sys
import argparse
import subprocess
import string

def get_version_string():
    dir_path = os.path.dirname(os.path.realpath(__file__))
    with open(dir_path + "/../compile_version_string.txt", "r") as file:
        contents = file.readline().strip()
        return contents


#Const globals
VERSION_STRING = get_version_string()

EXIT_CODE_SUCCESS = 0
EXIT_CODE_FAILURE_BUILD_APP = 1
EXIT_CODE_FAILURE_BUILD_LOCATION = 2

QT_LOC_VAR_NAME = "QT_LOC"
VS_LOC_VAR_NAME = "VS_LOC"
JOM_LOC_VAR_NAME = "JOM_LOC"
ZIP_LOC_VAR_NAME = "ZIP_LOC"
NSIS_LOC_VAR_NAME = "NSIS_LOC"
LLVM_LOC_VAR_NAME = "LLVM_LOC"

QT_LOC_DEFAULT = r"C:\Qt\5.12.2\msvc2017_64\bin\""
VS_LOC_DEFAULT = r"C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\VC\Auxiliary\Build\vcvarsall.bat"
JOM_LOC_DEFAULT = r"C:\Qt\Tools\QtCreator\bin\jom.exe"
ZIP_LOC_DEFAULT = r"C:\Program Files\7-Zip\7z.exe"
NSIS_LOC_DEFAULT = r"C:\Program Files (x86)\NSIS\""
LLVM_LOC_DEFAULT = r"C:\Program Files\LLVM\bin\""

BUILD_CLANG_VAR_NAME = "BUILD_CLANG"
BUILD_VSPROJ_VAR_NAME = "BUILD_VSPROJ"
BUILD_DEBUG_VAR_NAME = "BUILD_DEBUG"

PLATFORM_TARGET = "AMD64"

#Mutable globals
ORIGINAL_DIR = ""
PROJECT_DIR = ""
OUTPUT_DIR = r"\\bin\\win64\\AdvancedSettings"
DEPLOY_DIR = ""

current_activity = ""

bat_file_contents = ""

def set_current_activity(activity: str):
    """
    Wrapper around the current_activity global var to make it usable in other files.
    """
    global current_activity
    current_activity = activity

def say(message: str):
    """
    Prints a message to the console prepended by the current activity, like:
    BUILD: Building things.
    DEPLOY: Deploying things.
    """
    global current_activity
    print(current_activity + ": " + message)

def say_error(message: str):
    """
    Prints an error message to the console.

    Not currently different from say, but put in incase somebody wanted to
    do something fancy with outputting to stderr or something instead.
    """
    global current_activity
    print(current_activity + ": " + message)

def is_env_var_set(env_var_name: str):
    """
    Returns whether or not an environment variable is set.
    """
    var = os.getenv(env_var_name)
    if var is None:
        return False
    return True

def add_line_to_run_bat_file(line: str):
    """
    Adds a line ended by a newline to the batch file.
    """
    global bat_file_contents
    bat_file_contents += line + "\n"

def add_error_handling_line_to_bat_file():
    """
    Adds an error handling line to the batch file.

    This is because some applications don't cause the script to error out
    even if they exit with an error.
    """
    global bat_file_contents
    bat_file_contents += "IF ERRORLEVEL 1 EXIT /B " + str(EXIT_CODE_FAILURE_BUILD_APP) + "\n"

def get_required_env_var_path(env_var_name: str, default_env_var_value: str):
    """
    Easy function for getting *_LOC values, complete with testing for path validity and
    outputting status to console.
    """
    if not is_env_var_set(env_var_name):
        say(f"{env_var_name} not defined. Using default value.")
        #Sanitize quotes because we'll add them later and they might mess up
        #how we put together strings.
        path = default_env_var_value.replace('"', '')
    else:
        path = os.getenv(env_var_name).replace('"', '')
    say(f"{env_var_name} set to '{path}'")
    if not os.path.exists(path):
        say_error(f"{env_var_name} does not exist. Exiting")
        exit_on_error(EXIT_CODE_FAILURE_BUILD_LOCATION)
    say(f"{env_var_name} exists.")
    return path

def find_qt_path():
    """
    Returns the location of the Qt bin directory.

    Precedence goes to the QT_LOC_VAR_NAME environment variable.
    If that is set and exists it will be used, if it's set and doesn't exist
    likely locations will be searched for an install.
    """
    if is_env_var_set(QT_LOC_VAR_NAME):
        say(f"{QT_LOC_VAR_NAME} is defined. Attempting to find first.")
        qt_path = os.getenv(QT_LOC_VAR_NAME).replace('"', '')
        say(f"{QT_LOC_VAR_NAME} is set to '{qt_path}'.")
        if os.path.exists(qt_path):
            say(f"'{qt_path}' exists. Using this path.")
            return qt_path
        else:
            say(f"'{qt_path}' does not exist, attempting to traverse filesystem.")

    qt_path = traverse_and_find_qt_path()
    if qt_path == "NOTFOUND":
       say("Qt path could not be find by traversal. Exiting.")
       exit_on_error(EXIT_CODE_FAILURE_BUILD_LOCATION)

    return qt_path

def traverse_and_find_qt_path():
    """
    Searches all drives (A-Z) and versions above 12 to find the
    Qt install location.
    """
    qt_folder_name = r":\\Qt\\"
    qt_visual_studio_name = r"\\msvc2017_64\bin\\"
    qt_major_version = 5
    qt_minor_versions = [12, 13, 14, 15, 16]
    qt_patch_versions = [0, 1, 2, 3, 4, 5, 6, 7, 8, 9]
    # We search the A and B drive letters even though it's
    # unlikely for the install to be there
    windows_drive_letters = string.ascii_uppercase
    for drive in windows_drive_letters:
        # Go in reverse because we want the highest versions first
        for minor in reversed(qt_minor_versions):
            for patch in reversed(qt_patch_versions):
                version = str(qt_major_version) + "." + str(minor) + "." + str(patch)
                path = str(drive) + qt_folder_name + version + qt_visual_studio_name
                if os.path.exists(path):
                    say("Found QT path: '" + path + "'")
                    return path
    say("Did not find any valid QT path.")
    return "NOTFOUND"


def create_batch_file():
    """
    Creates a batch file in the dir. Must be called by another outside batch file.
    """
    file = open(get_project_dir() + "\\build_scripts\\win\\current_build.bat", "w+")
    file.write(bat_file_contents)
    file.close()

def exit_on_error(error_number):
    """Wrapper around sys.exit."""
    sys.exit(error_number)

def set_dirs():
    """
    Sets up the directories.

    We know that the project dir will always be two dirs up from the dir the file is in.
    """
    global ORIGINAL_DIR
    global PROJECT_DIR
    global DEPLOY_DIR
    ORIGINAL_DIR = os.path.dirname(__file__)
    PROJECT_DIR = os.path.abspath(os.path.join(ORIGINAL_DIR, r"..\.."))
    DEPLOY_DIR = DEPLOY_DIR = PROJECT_DIR + OUTPUT_DIR

    print(ORIGINAL_DIR)

def get_project_dir():
    return PROJECT_DIR

def get_original_dir():
    return ORIGINAL_DIR

def get_deploy_dir():
    return DEPLOY_DIR

