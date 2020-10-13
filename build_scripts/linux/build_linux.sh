#!/usr/bin/env bash
set -e

QMAKE_SPEC='linux-g++'

SCRIPT_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" >/dev/null 2>&1 && pwd )"
PROJECT_DIR=$SCRIPT_DIR/../../
BUILD_DIR=$PROJECT_DIR/build/

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

echo $MAKE_COMMAND
mkdir -p $BUILD_DIR
cd $BUILD_DIR
qmake --version
qmake -spec $QMAKE_SPEC $PROJECT_DIR CONFIG+=warnings_as_errors
$MAKE_COMMAND
$CLANG_TIDY

