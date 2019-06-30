GCC_VERSION = $$system("g++ -dumpversion")
greaterThan(GCC_VERSION, 6) {
    message('g++' version is above 6. Using regular g++.)
}
else {
    message('g++' version is not above 6. Attempting to use highest specific version.)
    system(g++-7 --version) {
        message('g++-7' found.)
        QMAKE_CXX = g++-7
    }
    system(g++-8 --version) {
        message('g++-8' found.)
        QMAKE_CXX = g++-8
    }
    system(g++-9 --version) {
        message('g++-9' found.)
        QMAKE_CXX = g++-9
    }
}

include(clang-gcc-common-switches.pri)

QMAKE_CXXFLAGS += -Wpedantic

QMAKE_CXXFLAGS += -Werror

# GCC only switches
QMAKE_CXXFLAGS += -Wduplicated-branches -Wduplicated-cond -Wlogical-op -Wrestrict -Wnull-dereference

# Sign conversion warns on auto generated Qt MOC files.
QMAKE_CXXFLAGS += -Wconversion -Wno-sign-conversion

QMAKE_CXXFLAGS += -Wzero-as-null-pointer-constant
