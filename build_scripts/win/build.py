"""
Build script to compile OpenVR Advanced Settings on Windows.

The entire purpose of the script is to locate the correct install locations
for seven different applications:

1. Qt Framework, used for compiling.
2. Visual Studio, used for setting up the variables needed to compile with Qt.
3. jom.exe, a part of Qt, basically a multi core nmake.
4. 7zip. Used for creating the portable versions of the app.
5. NSIS. Used for creating the installer versions of the app.
6. LLVM. Used for clang-format and clang-cl.
7. Qmake, a part of Qt, used for creating the makefiles.

The script does error checking on environmental variables before outputting
a simple Windows batch file that will be called by other batch scripts.

The reason that a batch file is created and run outside of python is that if
you don't do it like that, you have these options

1) Run applications one by one in python.

    This is not viable because the build() function requires that a batch file
    be run to set up environmental variables for QMAKE and nmake, and all environmental
    variables and settings are lost between sessions.

2) Batch all applications together and run them in python.

    This is still a viable way of doing things, but creating an actual file
    makes debugging and experimentation a lot easier by less experienced people.
    Furthermore the stdout and stderr output wouldn't be real time without adding
    unnecessary complexity.

3) Write the entire thing in another language (like Powershell).

    This was tried and even though Powershell has decent stdlib functionality like
    testing paths and replacing chars in strings, it is still lacking in some areas
    like actually running .exe files without resorting to weird tricks and cmd line hacks.
    In addition, Powershell has some weird tendencies compared to other "real"
    programming languages that would add unnecessary complexity.

Writing this script in pure Windows batch file language is definitely possible,
but the large amounts of replication and low amounts of abstraction possibilities
will make development and extension a chore.
"""

import os
from sys import argv
import inspect
from common import *

def package():
    """
    Runs 7zip and NSIS to create portable and installer versions.
    """
    set_current_activity("PACKAGE")
    set_dirs()

    say("Testing if all required build environment variables are set:")
    ZIP_LOC = get_required_env_var_path(ZIP_LOC_VAR_NAME, ZIP_LOC_DEFAULT)
    NSIS_LOC = get_required_env_var_path(NSIS_LOC_VAR_NAME, NSIS_LOC_DEFAULT)
    say("All required build environment values are set.")

    say("Adding 7zip to file:")
    ZIP_NAME = "AdvancedSettings-" + VERSION_STRING + ".7z"

    add_line_to_run_bat_file("@ECHO Starting 7zip:")
    # -t7z means to output a 7zip file.
    add_line_to_run_bat_file('"' + ZIP_LOC + '"' + " a -t7z " + get_project_dir() + "\\bin\\win64\\" + ZIP_NAME + " " + get_deploy_dir() + "\\*")
    add_error_handling_line_to_bat_file()
    add_line_to_run_bat_file("@ECHO 7zip done.")
    say("7zip added to file.")

    say("Adding NSIS to file.")
    add_line_to_run_bat_file("@ECHO Starting NSIS:")
    add_line_to_run_bat_file("cd " + NSIS_LOC)
    add_line_to_run_bat_file("makensis /V2 /WX " + get_project_dir() + "\\installer\\installer.nsi")
    add_error_handling_line_to_bat_file()
    add_line_to_run_bat_file("@ECHO NSIS done.")
    say("NSIS added to file.")

    create_batch_file()

def build():
    """
    Runs:
        'vcvarsall.bat', a Visual Studio file to set up the environment for QMAKE.
        QMAKE, creates the makefiles.
        jom/nmake, builds the makefiles.
    """
    set_current_activity("BUILD")
    set_dirs()

    COMPILE_MODE = ""
    COMPILER = ""

    say("Attempting to build version: " + VERSION_STRING)

    say("Testing if all required build environment variables are set:")
    QT_LOC = find_qt_path()
    VS_LOC = get_required_env_var_path(VS_LOC_VAR_NAME, VS_LOC_DEFAULT)

    if is_env_var_set(BUILD_CLANG_VAR_NAME):
        say(f"{BUILD_CLANG_VAR_NAME} defined. Building for win32-clang-msvc.")
        get_required_env_var_path(LLVM_LOC_VAR_NAME, LLVM_LOC_DEFAULT)
        COMPILER = "win32-clang-msvc"
    else:
        say(f"{BUILD_CLANG_VAR_NAME} not defined. Building for msvc.")
        COMPILER = "win32-msvc"

    say("All required build environment values are set.")

    #Otherwise qmake gets confused
    add_line_to_run_bat_file("cd " + get_project_dir())

    say("Adding 'vcvarsall.bat' to batch file.")
    add_line_to_run_bat_file("@ECHO Setting up environment with Visual Studio 'vcvarsall.bat':")
    add_line_to_run_bat_file("call " + '"' + VS_LOC + '"' + " " + PLATFORM_TARGET)
    add_error_handling_line_to_bat_file()
    add_line_to_run_bat_file("@ECHO Environment set up.")
    say("'vcvarsall.bat' added to batch file.")

    QMAKE_LOC = QT_LOC + r"\qmake.exe"

    if is_env_var_set(BUILD_VSPROJ_VAR_NAME):
        say(f"{BUILD_VSPROJ_VAR_NAME} defined. Building Visual Studio project files.")
        add_line_to_run_bat_file("@ECHO Running Visual Studio project file building:")
        add_line_to_run_bat_file('"' + QMAKE_LOC + '"' +  " -tp vc")
        add_error_handling_line_to_bat_file()
        add_line_to_run_bat_file("@ECHO Done building Visual Studio project files.")
    else:
        say(f"{BUILD_VSPROJ_VAR_NAME} not defined. Not building Visual Studio project files.")

    if is_env_var_set(BUILD_DEBUG_VAR_NAME):
        COMPILE_MODE = "debug"
        say(f"{BUILD_DEBUG_VAR_NAME} defined. Building '{COMPILE_MODE}' version.")
    else:
        COMPILE_MODE = "release"
        say(f"{BUILD_DEBUG_VAR_NAME} not defined. Building '{COMPILE_MODE}' version.")

    add_line_to_run_bat_file("@ECHO Running qmake:")
    if is_env_var_set("OVRAS_WARNINGS_AS_ERRORS"):
    	add_line_to_run_bat_file('"' + QMAKE_LOC + '"' +  " -spec " + COMPILER + " CONFIG+=X86_64 " + "CONFIG+=" + COMPILE_MODE + " CONFIG+=warnings_as_errors")
    else:
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

    add_line_to_run_bat_file("cd " + get_original_dir())

    create_batch_file()

if __name__ == "__main__":
    if argv[1] == "build":
        build()
    elif argv[1] == "package":
        package()

