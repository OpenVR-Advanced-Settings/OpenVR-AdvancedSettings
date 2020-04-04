#!/usr/bin/env bash
set -e

SCRIPT_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" >/dev/null 2>&1 && pwd )"
PROJECT_DIR=$SCRIPT_DIR/../../
BUILD_DIR=$PROJECT_DIR/build/
EXE_DIR=$BUILD_DIR/bin/linux/AdvancedSettings/

#wget -c -nv "https://github.com/probonopd/linuxdeployqt/releases/download/continuous/linuxdeployqt-continuous-x86_64.AppImage"
#chmod a+x linuxdeployqt-continuous-x86_64.AppImage
mkdir -p $EXE_DIR/usr
mkdir -p $EXE_DIR/usr/bin
mkdir -p $EXE_DIR/usr/lib
mkdir -p $EXE_DIR/usr/share
mkdir -p $EXE_DIR/usr/share/applications
mkdir -p $EXE_DIR/usr/share/icons
mkdir -p $EXE_DIR/usr/share/icons/hicolor
mkdir -p $EXE_DIR/usr/share/icons/hicolor/256x256
mkdir -p $EXE_DIR/usr/share/icons/hicolor/256x256/apps

# https://github.com/probonopd/linuxdeployqt/issues/25
# linuxdeployqt fails to deploy QtQuick/Dialogs and QtQuick/LocalStorage
mkdir -p $EXE_DIR/usr/qml/QtQuick/Dialogs
cp -avr /opt/qt512/qml/QtQuick/Dialogs $EXE_DIR/usr/qml/QtQuick/

# https://github.com/probonopd/linuxdeployqt/issues/82
# linuxdeployqt fails to deploy plugins/imageformats/libqsvg.so
mkdir -p $EXE_DIR/usr/plugins/imageformats
cp -av /opt/qt512/plugins/imageformats/libqsvg.so $EXE_DIR/usr/plugins/imageformats/libqsvg.so

mv $EXE_DIR/AdvancedSettings $EXE_DIR/usr/bin
mv $EXE_DIR/res $EXE_DIR/usr/bin
mv $EXE_DIR/default_action_manifests $EXE_DIR/usr/bin
mv $EXE_DIR/action_manifest.json $EXE_DIR/usr/bin
mv $EXE_DIR/libopenvr_api.so $EXE_DIR/usr/bin
mv $EXE_DIR/manifest.vrmanifest $EXE_DIR/usr/bin

cp $PROJECT_DIR/src/res/img/icons/thumbicon.png $EXE_DIR/usr/share/icons/hicolor/256x256/apps/AdvancedSettings.png

cp $PROJECT_DIR/src/package_files/linux/AdvancedSettings.desktop $EXE_DIR/usr/share/applications/AdvancedSettings.desktop

qmake --version

$PROJECT_DIR/linuxdeployqt-continuous-x86_64.AppImage -version --appimage-extract-and-run

$PROJECT_DIR/linuxdeployqt-continuous-x86_64.AppImage $EXE_DIR/usr/share/applications/AdvancedSettings.desktop --appimage-extract-and-run -appimage -verbose=2 -always-overwrite -qmldir=$PROJECT_DIR/src/res/qml
