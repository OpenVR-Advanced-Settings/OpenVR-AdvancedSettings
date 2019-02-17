QMAKE_CXXFLAGS += -Wextra -Wmost

QMAKE_CXXFLAGS += -Werror
#All includes from the third-party directory will not warn.
QMAKE_CXXFLAGS += --system-header-prefix=third-party

QMAKE_CXXFLAGS += -Wshadow

QMAKE_CXXFLAGS += -Wold-style-cast

QMAKE_CXXFLAGS += -Wnon-virtual-dtor

QMAKE_CXXFLAGS += -Wpedantic

QMAKE_CXXFLAGS += -Wcast-align -Wunused -Woverloaded-virtual -Wformat=2
