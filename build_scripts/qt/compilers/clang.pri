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
