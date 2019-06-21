# Makes sure the "clang++" command used to invoke the compilation is abvove version 8.
# If it's at or above version 7 then we can use the default clang++ otherwise we'll
# specifically need clang++-8.
CLANG_VERSION = $$system("clang++ -dumpversion")
!greaterThan(CLANG_VERSION, 7) {
    message('clang++' version is not above 8. Manually using 'clang++-8'.)
    !system(clang++-8 --version) {
        error(At least clang++-8 required.)
    }
    #clang++-8 is needed for C++17 features. travis does not supply this by default.
    QMAKE_CXX = clang++-8
}

include(clang-gcc-common-switches.pri)

QMAKE_CXXFLAGS += -Wmost

QMAKE_CXXFLAGS += -Wpedantic

QMAKE_CXXFLAGS += -Werror
#All includes from the third-party directory will not warn.
QMAKE_CXXFLAGS += --system-header-prefix=third-party
QMAKE_CXXFLAGS += --system-header-prefix=qt

# Sign conversion warns on auto generated Qt MOC files.
QMAKE_CXXFLAGS += -Weffc++ -Wconversion -Wno-sign-conversion

QMAKE_CXXFLAGS += -Wextra-semi -Wzero-as-null-pointer-constant  -Wmissing-variable-declarations
