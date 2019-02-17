#g++-7 is needed for C++17 features. travis does not supply this by default.
QMAKE_CXX = g++-7

QMAKE_CXXFLAGS += -Werror

QMAKE_CXXFLAGS += -Wshadow
