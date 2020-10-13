# comments are the value of CLANG_VERSION
CLANG_VERSION = $$system("clang++ --version | grep 'clang version'")
# clang version 6.0.0-1ubuntu2 (tags/RELEASE_600/final)
CLANG_VERSION = $$split(CLANG_VERSION, ' ')
# CLANG_VERSION is now a list, this is shown as spaces in QMAKE
# clang version 6.0.0-1ubuntu2 (tags/RELEASE_600/final)
CLANG_VERSION = $$member(CLANG_VERSION, 2)
# 6.0.0-1ubuntu2
CLANG_VERSION = $$split(CLANG_VERSION, '.')
# 6 0 0-1ubuntu2
CLANG_VERSION = $$member(CLANG_VERSION, 0)
# 6

greaterThan(CLANG_VERSION, 4) {
    message('clang++' version is above 4. Using regular clang++.)
    QMAKE_CXX = clang++
    QMAKE_LINK = clang++
}
else {
    message('clang++' version is not above 4. Attempting to use highest specific version.)
    system("clang++-5 --version") {
        QMAKE_CXX = clang++-5
        QMAKE_LINK = clang++-5
        message('clang++-5' found.)
    }
    system("clang++-6 --version") {
        QMAKE_CXX = clang++-6
        QMAKE_LINK = clang++-6
        message('clang++-6' found.)
    }
    system("clang++-7 --version") {
        QMAKE_CXX = clang++-7
        QMAKE_LINK = clang++-7
        message('clang++-7' found.)
    }
    system("clang++-8 --version") {
        QMAKE_CXX = clang++-8
        QMAKE_LINK = clang++-8
        message('clang++-8' found.)
    }
}

system("$$QMAKE_CXX --version")

include(clang-gcc-common-switches.pri)

QMAKE_CXXFLAGS += -Wmost

QMAKE_CXXFLAGS += -Wpedantic

warnings_as_errors {
    QMAKE_CXXFLAGS += -Werror
}

#All includes from the third-party directory will not warn.
QMAKE_CXXFLAGS += --system-header-prefix=third-party
QMAKE_CXXFLAGS += --system-header-prefix=qt

# Sign conversion warns on auto generated Qt MOC files.
QMAKE_CXXFLAGS += -Weffc++ -Wconversion -Wno-sign-conversion

QMAKE_CXXFLAGS += -Wextra-semi -Wzero-as-null-pointer-constant  -Wmissing-variable-declarations
