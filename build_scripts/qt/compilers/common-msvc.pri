#Removing -W3 from both FLAGS is necessary, otherwise compiler will give
#D9025: overriding '/W4' with '/W3'
QMAKE_CFLAGS_WARN_ON -= -W3
QMAKE_CXXFLAGS_WARN_ON -= -W3
#C4127 is a warning in qvarlengtharray.h that can not be turned off
#on the current version of MSVC.
QMAKE_CXXFLAGS += /W4 /wd4127
# The codecvt header being used in AudioManagerWindows.cpp is deprecated by the standard,
# but no suitable replacement has been standardized yet. It is possible to use the Windows
# specific MultiByteToWideChar() and WideCharToMultiByte() from <Windows.h>
DEFINES += _SILENCE_CXX17_CODECVT_HEADER_DEPRECATION_WARNING

# Enables Whole Program Optimization
QMAKE_CXXFLAGS += /Gw
