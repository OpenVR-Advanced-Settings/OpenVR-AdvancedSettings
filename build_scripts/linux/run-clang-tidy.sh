set +e

if [[ -z $CLANG_TIDY_EXECUTABLE ]]; then
    CLANG_TIDY_EXECUTABLE='clang-tidy'
fi

$CLANG_TIDY_EXECUTABLE --version

ERROR=0
for file in $(find ../src -name "*.cpp"); do
    echo $file
    $CLANG_TIDY_EXECUTABLE $file

    EXIT=$?
    if [[ $EXIT != 0 ]]; then
        ERROR=1
    fi
done
set -e
exit $ERROR
