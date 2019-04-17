#!/usr/bin/env bash
set -e
DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" >/dev/null 2>&1 && pwd )"
. $DIR/configure.sh

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
