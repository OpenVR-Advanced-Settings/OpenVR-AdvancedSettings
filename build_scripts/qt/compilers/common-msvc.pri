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

# 'identfier': conversion from 'type1' to 'type1', possible loss of data
QMAKE_CXXFLAGS += /w14242
# 'operator': conversion from 'type1:field_bits' to 'type2:field_bits', possible loss of data
QMAKE_CXXFLAGS += /w14254
# 'function': member function does not override any base class virtual member function
QMAKE_CXXFLAGS += /w14263
# 'classname': class has virtual functions, but destructor is not virtual instances of this class may not be destructed correctly
QMAKE_CXXFLAGS += /w14265
# 'operator': unsigned/negative constant mismatch
QMAKE_CXXFLAGS += /w14287
# nonstandard extension used: 'variable': loop control variable declared in the for-loop is used outside the for-loop scope
QMAKE_CXXFLAGS += /we4289
# 'operator': expression is always 'boolean_value'
QMAKE_CXXFLAGS += /w14296
# 'variable': pointer truncation from 'type1' to 'type2'
QMAKE_CXXFLAGS += /w14311
# expression before comma evaluates to a function which is missing an argument list
QMAKE_CXXFLAGS += /w14545
# function call before comma missing argument list
QMAKE_CXXFLAGS += /w14546
# 'operator': operator before comma has no effect; expected operator with side-effect
QMAKE_CXXFLAGS += /w14547
# 'operator': operator before comma has no effect; did you intend 'operator'?
QMAKE_CXXFLAGS += /w14549
# expression has no effect; expected expression with side-effect
QMAKE_CXXFLAGS += /w14555
# Enable warning on thread un-safe static member initialization
QMAKE_CXXFLAGS += /w14640
# Conversion from 'type1' to 'type_2' is sign-extended. This may cause unexpected runtime behavior.
QMAKE_CXXFLAGS += /w14826
# wide string literal cast to 'LPSTR'
QMAKE_CXXFLAGS += /w14905
# string literal cast to 'LPWSTR'
QMAKE_CXXFLAGS += /w14906
# illegal copy-initialization; more than one user-defined conversion has been implicitly applied
QMAKE_CXXFLAGS += /w14928
