#!/usr/bin/env bash
set -e
if [[ -z $QMAKE_SPEC ]]; then
    QMAKE_SPEC='linux-g++'
fi

if [[ -z $USE_TIDY ]]; then
    MAKE_COMMAND='make --jobs'
    CLANG_TIDY=
else
    # Bear replaces the compile database, it does not update it.
    # If you run 'bear make' and there is nothing to compile, you
    # will get an empty database.
    # Clean up the artifacts and run everything together in that case.
    MAKE_COMMAND='bear make --jobs'
    QMAKE_SPEC='linux-clang'
    CLANG_TIDY='../build_scripts/linux/run-clang-tidy.sh'
fi

echo $MAKE_COMMAND
mkdir -p build
cd build
qmake --version
qmake -spec $QMAKE_SPEC ..
$MAKE_COMMAND
$CLANG_TIDY
cp -r ../src/res ./bin/linux/AdvancedSettings/
cp ../src/package_files/action_manifest.json ./bin/linux/AdvancedSettings/
cp ../src/package_files/manifest.vrmanifest ./bin/linux/AdvancedSettings/
cp -r ../src/package_files/default_action_manifests ./bin/linux/AdvancedSettings/
cp ../third-party/openvr/lib/linux64/libopenvr_api.so ./bin/linux/AdvancedSettings/
cp ../build_scripts/linux/run-with-library.sh ./bin/linux/AdvancedSettings/
