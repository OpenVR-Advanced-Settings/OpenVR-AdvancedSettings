set +e
clang-tidy --version

ERROR=0
for file in $(find ../src -name "*.cpp"); do
    echo $file
    clang-tidy $file

    EXIT=$?
    if [[ $EXIT != 0 ]]; then
        ERROR=1
    fi
done
set -e
exit $ERROR
