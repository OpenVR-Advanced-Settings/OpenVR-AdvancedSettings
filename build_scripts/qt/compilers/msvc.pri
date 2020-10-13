include(common-msvc.pri)

warnings_as_errors {
    # Warnings as errors
    QMAKE_CXXFLAGS += /WX
}

# Enables Link Time Optimization
QMAKE_CXXFLAGS += /GL

# Force standards-conformance
QMAKE_CXXFLAGS += /permissive-
