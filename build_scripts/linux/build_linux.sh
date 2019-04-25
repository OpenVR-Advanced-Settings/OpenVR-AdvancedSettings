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
cp -r $PROJECT_DIR/src/res $BUILD_DIR/bin/linux/AdvancedSettings/
cp $PROJECT_DIR/src/package_files/action_manifest.json $BUILD_DIR/bin/linux/AdvancedSettings/
cp $PROJECT_DIR/src/package_files/manifest.vrmanifest $BUILD_DIR/bin/linux/AdvancedSettings/
cp -r $PROJECT_DIR/src/package_files/default_action_manifests $BUILD_DIR/bin/linux/AdvancedSettings/
cp $PROJECT_DIR/third-party/openvr/lib/linux64/libopenvr_api.so $BUILD_DIR/bin/linux/AdvancedSettings/
