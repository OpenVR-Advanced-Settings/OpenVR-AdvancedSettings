include(common-msvc.pri)

#win32-clang-msvc is unable to correctly build the first time
#with warnings as errors on. There seems to be an issue with the moc_predefs.h
#file not being built correctly when /WX is on. Until the issue is fixed,
#win32-clang-msvc is built without /WX.

#clang-msvc does not enable the c++17 flag with qmake's c++1z flag.
QMAKE_CXXFLAGS += /std:c++17

QMAKE_CXXFLAGS += -Wshadow

QMAKE_CXXFLAGS += -Wold-style-cast

QMAKE_CXXFLAGS += -Wnon-virtual-dtor
