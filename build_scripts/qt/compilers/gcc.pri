#g++-7 is needed for C++17 features. travis does not supply this by default.
QMAKE_CXX = g++-7

include(clang-gcc-common-switches.pri)

QMAKE_CXXFLAGS += -Wpedantic

QMAKE_CXXFLAGS += -Werror

# GCC only switches
QMAKE_CXXFLAGS += -Wduplicated-branches -Wduplicated-cond -Wlogical-op -Wrestrict -Wnull-dereference
