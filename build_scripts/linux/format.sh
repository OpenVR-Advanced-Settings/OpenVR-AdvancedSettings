#!/usr/bin/env bash
set -e

clang-format --version

if [ $? -eq 0 ]; then
    echo "Clang-format found."
    build_path="`dirname \"$0\"`"
    project_path=$build_path/../..

    find $project_path/src/ -regex '.*\.\(cpp\|h\)' -exec clang-format -style=file -i {} \; -exec echo "Iteration 1: {}" \;
    find $project_path/src/ -regex '.*\.\(cpp\|h\)' -exec clang-format -style=file -i {} \; -exec echo "Iteration 2: {}" \;
    echo "DONE"
else
    echo "Could not find clang-format."
fi

