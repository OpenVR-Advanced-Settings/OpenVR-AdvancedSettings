#!/usr/bin/env bash
set -e

if [[ -z $QMAKE_SPEC ]]; then
    QMAKE_SPEC='linux-g++'
fi

if [[ -z $USE_TIDY ]]; then
    MAKE_COMMAND="make --jobs ${MAKE_JOBS}"
    CLANG_TIDY=
else
    # Bear replaces the compile database, it does not update it.
    # If you run 'bear make' and there is nothing to compile, you
    # will get an empty database.
    # Clean up the artifacts and run everything together in that case.
    MAKE_COMMAND='bear make --jobs'
    QMAKE_SPEC='linux-clang'
    CLANG_TIDY="$SCRIPT_DIR/run-clang-tidy.sh"
fi

if [[ -n $NO_X11 ]]; then
    QMAKE_EXTRAS='CONFIG+=noX11'
fi
if [[ -n $NO_DBUS ]]; then
    QMAKE_EXTRAS='CONFIG+=noDBUS'
fi
