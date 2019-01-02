"""
Utility functions for building OpenVR Advanced Settings.
"""
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

BUILD_CLANG_VAR_NAME = "BUILD_CLANG"
BUILD_VSPROJ_VAR_NAME = "BUILD_VSPROJ"
BUILD_DEBUG_VAR_NAME = "BUILD_DEBUG"

PLATFORM_TARGET = "AMD64"

#Mutable globals
ORIGINAL_DIR = ""
PROJECT_DIR = ""

current_activity = ""

bat_file_contents = ""

def set_current_activity(activity: str):
    global current_activity
    current_activity = activity

def say(message: str):
    global current_activity
    print(current_activity + ": " + message)
    
def say_error(message: str):
    global current_activity
    print(current_activity + ": " + message)    
    
def is_env_var_set(env_var_name: str):
    var = os.getenv(env_var_name)
    if var is None:
        return False
    return True
    
def add_line_to_run_bat_file(line: str):
    global bat_file_contents
    bat_file_contents += line + "\n"

def add_error_handling_line_to_bat_file():
    global bat_file_contents
    bat_file_contents += "IF ERRORLEVEL 1 EXIT /B " + str(EXIT_CODE_FAILURE_BUILD_APP) + "\n"

def get_required_env_var_path(env_var_name: str, default_env_var_value: str):
    if not is_env_var_set(env_var_name):
        say(f"{env_var_name} not defined. Using default value.")
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
    file = open("test.bat", "w+")
    file.write(bat_file_contents)
    file.close()    

def exit_on_error(error_number):
    sys.exit(error_number)
    
def set_original_folder(path):
    global ORIGINAL_DIR
    ORIGINAL_DIR = path
        
def set_project_folder(path):
    global PROJECT_DIR
    PROJECT_DIR = path        
        
def get_project_dir():
    return PROJECT_DIR
        
def get_original_dir():
    return ORIGINAL_DIR        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        