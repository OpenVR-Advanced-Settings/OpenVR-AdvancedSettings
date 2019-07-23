#!/usr/bin/env bash
set -e
SCRIPT_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" >/dev/null 2>&1 && pwd )"
PROJECT_DIR=$SCRIPT_DIR/../../
BUILD_DIR=$PROJECT_DIR/build/
. $SCRIPT_DIR/configure.sh

echo $MAKE_COMMAND
mkdir -p $BUILD_DIR
cd $BUILD_DIR
qmake --version
qmake -spec $QMAKE_SPEC $PROJECT_DIR $QMAKE_EXTRAS
$MAKE_COMMAND
$CLANG_TIDY

