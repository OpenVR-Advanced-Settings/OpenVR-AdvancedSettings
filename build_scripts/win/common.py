"""
Utility functions for building OpenVR Advanced Settings.
"""
import os
import sys
import argparse
import subprocess
import shutil
import distutils.dir_util

#Const globals
#Version string must be bumped every release.
#TODO: See developer documentation.
VERSION_STRING = "3-1-0-RC1"

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
        
def copy_file(src, dest):
    """Copies a file and outputs status to console."""
    say(f"Copying {src} to {dest}")
    shutil.copy(src, dest)
    say(f"{src} to {dest} copied.")    
        
def copy_folder(src, dest):
    """Copies a folder and outputs status to console."""
    say(f"Copying {src} to {dest}")
    distutils.dir_util.copy_tree(src, dest)        
    say(f"{src} to {dest} copied.") 
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
