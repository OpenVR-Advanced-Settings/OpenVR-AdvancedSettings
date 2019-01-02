import os
from common import *

def deploy():
    set_current_activity("DEPLOY")
    set_original_folder(os.getcwd())
    set_project_folder(os.path.abspath(os.path.join(ORIGINAL_DIR , "..\..")))

    say("Testing if all required build environment variables are set:")
    QT_LOC = get_required_env_var_path(QT_LOC_VAR_NAME, QT_LOC_DEFAULT)
    say("All required build environment values are set.")

    if is_env_var_set(BUILD_DEBUG_VAR_NAME):
        COMPILE_MODE = "debug"
        say(f"{BUILD_DEBUG_VAR_NAME} defined. Deploying '{COMPILE_MODE}' version.")
    else:
        COMPILE_MODE = "release"
        say(f"{BUILD_DEBUG_VAR_NAME} not defined. Deploying '{COMPILE_MODE}' version.")

    say("Adding windeployqt to file.")

    #Extremely long line
    add_line_to_run_bat_file('"' + "windeployqt" + '"' + "--dir $DEPLOY_DIRECTORY\qtdata --libdir $DEPLOY_DIRECTORY --plugindir $DEPLOY_DIRECTORY\qtdata\plugins --no-system-d3d-compiler --no-opengl-sw "+ COMPILE_MODE + " --qmldir $PROJECT_DIRECTORY\src\res\qml\ $DEPLOY_DIRECTORY\AdvancedSettings.exe")

    say("windeployqt added to file.")        

    say("Copying necessary files:")

    #readme
    copy_file(get_project_dir() + "\Readme.md", get_deploy_dir() + "\Readme.md")

    #license
    copy_file(get_project_dir() + "\\LICENSE", get_deploy_dir() + "\\LICENSE-GPL.txt")
    copy_file(get_project_dir() + "\\third-party\\openvr\\LICENSE", get_deploy_dir() + "\\LICENSE-VALVE.txt")
    copy_file(get_project_dir() + "\\third-party\\easylogging++\\LICENSE", get_deploy_dir() + "\\LICENSE-MIT.txt")
    
    #res
    copy_folder(get_project_dir() + "\\src\\res", get_deploy_dir() + "\\res")

    #package files
    copy_folder(get_project_dir() + "\\src\\package_files", get_deploy_dir())
    
    #openvr dll
    copy_file(get_project_dir() + "\\third-party\\openvr\\bin\\win64\\openvr_api.dll", get_deploy_dir() + "openvr_api.dll") 

def build():
    set_current_activity("BUILD")
    set_original_folder(os.getcwd())
    set_project_folder(os.path.abspath(os.path.join(ORIGINAL_DIR , "..\..")))

    COMPILE_MODE = ""
    COMPILER = ""
    
    say("Testing if all required build environment variables are set:")
    QT_LOC = get_required_env_var_path(QT_LOC_VAR_NAME, QT_LOC_DEFAULT)
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
    deploy()



    
