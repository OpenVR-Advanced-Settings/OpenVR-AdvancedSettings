#!/usr/bin/env bash
set -e

if [ -x /usr/bin/clang-format-10 ]; then
    CLANG_FORMAT_EXE=clang-format-10
else
    CLANG_FORMAT_EXE=clang-format
fi

CLANG_VERSION="$($CLANG_FORMAT_EXE --version)"
echo $CLANG_VERSION
if [[ "$CLANG_VERSION" != *"clang-format version 10"* ]]; then
    echo "Wrong clang-format version found! Install version 10"
    exit 1
fi

if [ $? -eq 0 ]; then
    echo "Clang-format found."
    build_path="`dirname \"$0\"`"
    project_path=$build_path/../..

    find $project_path/src/ -regex '.*\.\(cpp\|h\)' -exec $CLANG_FORMAT_EXE -style=file -i {} \; -exec echo "Iteration 1: {}" \;
    find $project_path/src/ -regex '.*\.\(cpp\|h\)' -exec $CLANG_FORMAT_EXE -style=file -i {} \; -exec echo "Iteration 2: {}" \;
    echo "DONE"
else
    echo "Could not find clang-format-10."
fi

