# Makes sure the "g++" command used to invoke the compilation is abvove version 7.
# If it's at or above version 7 then we can use the default g++, otherwise we'll
# specifically need g++-7.
GCC_VERSION = $$system("g++ -dumpversion")
!greaterThan(GCC_VERSION, 6) {
    message('g++' version is not above 7. Manually using 'g++-7'.)
    !system(g++-7 --version) {
        error(At least g++-7 required.)
    }
    #g++-7 is needed for C++17 features. travis does not supply this by default.
    QMAKE_CXX = g++-7
}

include(clang-gcc-common-switches.pri)

QMAKE_CXXFLAGS += -Wpedantic

QMAKE_CXXFLAGS += -Werror

# GCC only switches
QMAKE_CXXFLAGS += -Wduplicated-branches -Wduplicated-cond -Wlogical-op -Wrestrict -Wnull-dereference

# Sign conversion warns on auto generated Qt MOC files.
QMAKE_CXXFLAGS += -Wconversion -Wno-sign-conversion

QMAKE_CXXFLAGS += -Wzero-as-null-pointer-constant
