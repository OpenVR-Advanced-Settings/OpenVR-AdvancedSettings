include(common-msvc.pri)

# Warnings as errors
QMAKE_CXXFLAGS += /WX

# Enables Link Time Optimization
QMAKE_CXXFLAGS += /GL
