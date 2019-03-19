#!/usr/bin/env bash
set -e

clang-format --version;

if [ $? -eq 0 ]; then
    echo "Clang-format found."
    build_path="`dirname \"$0\"`"
    project_path=$build_path/../..

    python $project_path/build_scripts/run-clang-format.py $project_path/src -r --color always;
    if [ $? -eq 0 ]; then
        echo "FORMATTED CORRECTLY."
    else
        echo "NOT FORMATTED CORRECTLY."
    fi
else
    echo "Could not find clang-format."
fi

