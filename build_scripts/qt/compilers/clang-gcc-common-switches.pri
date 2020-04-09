QMAKE_CXXFLAGS += -Wextra

QMAKE_CXXFLAGS += -Wshadow

QMAKE_CXXFLAGS += -Wold-style-cast

QMAKE_CXXFLAGS += -Wnon-virtual-dtor

QMAKE_CXXFLAGS += -Wcast-align -Wunused -Woverloaded-virtual -Wformat=2

QMAKE_CXXFLAGS += -Wdouble-promotion

# Enable Link Time Optimization
QMAKE_CXXFLAGS += -flto
# Clang LTO requires the gold linker instead of ld
QMAKE_LFLAGS += -flto -fuse-ld=gold
