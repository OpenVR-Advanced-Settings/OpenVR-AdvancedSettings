- [Requirements](#requirements)
  * [Compiler and Build Essentials](#compiler-and-build-essentials)
  * [Qt](#qt)
  * [Official Qt Installer](#official-qt-installer)
    + [`qtchooser`](#-qtchooser-)
  * [Unofficial Ubuntu Packages](#unofficial-ubuntu-packages)
  * [`qtchooser` and versions](#-qtchooser--and-versions)
  * [X11](#x11)
  * [DBUS](#dbus)
  * [`clang-tidy` and `bear`](#-clang-tidy--and--bear-)
- [TL;DR: for Ubuntu](#tl-dr--for-ubuntu)
  * [Ubuntu 16.04 Xenial](#ubuntu-1604-xenial)
  * [Ubuntu 18.04 Bionic](#ubuntu-1804-bionic)
- [Locations and Environment Variables](#locations-and-environment-variables)
- [Building](#building)
- [Installing](#installing)
- [Contributing](#contributing)

# Requirements
## Compiler and Build Essentials

You will need either `clang` og `g++`. At least `g++-7` or `clang` version `5.0` is required due to C++17 support.
You will additionally need the `build-essential` and `libgl1-mesa-dev` packages.

If they are available from your package manager they can be installed with `sudo apt install build-essential libgl1-mesa-dev`. If the versions in your package manager are not up to date, you can get the packages on Ubuntu with 
```bash
sudo add-apt-repository ppa:ubuntu-toolchain-r/test
sudo apt update
sudo apt install g++-7
```
## Qt

At least version `5.12` is required.

## Official Qt Installer

The easiest way to get it is from the [official Qt installer](https://www.qt.io/download-qt-installer).

### `qtchooser`
After installing using the official installer you will likely need to `qtchooser -install <name> <qmake-location>` in order for Qt to use the correct version. See the `qtchooser` section below.

## Unofficial Ubuntu Packages

If you don't want to use the offical installer, you can use [this](https://launchpad.net/~beineri) PPA.

You will need at least `qt512-meta-minimal qt512multimedia qt512declarative qt512quickcontrols2 qt512quickcontrols qt512tools qt512imageformats qt512svg qt512base`, although if you have the drive space it's probably easier just to get `qt512-meta-full`.

For Ubuntu 16.04
```bash
sudo add-apt-repository ppa:beineri/opt-qt-5.12.2-xenial
sudo apt-get update
sudo apt install qt512-meta-full
```
For Ubuntu 18.04
```bash
sudo add-apt-repository ppa:beineri/opt-qt-5.12.2-bionic
sudo apt-get update
sudo apt install qt512-meta-full
```
If you choose the wrong Ubuntu version you will get an error from `apt` about there not being a `Release` file. Try the other one instead.

## `qtchooser` and versions

If you have multiple Qt installations you might need to select the correct one using `qtchooser`.

Type `qmake --version` into the terminal to see which version you're currently using:
```bash
$ qmake --version
QMake version 3.1
Using Qt version 5.12.2 in /opt/qt512/lib
```
If you get a version equal to or above `5.12` you don't need to use `qtchooser`.

If you get `qmake: could not find a Qt installation of ''` or a version lower than `5.12` then you'll need to follow the steps below.

You can list currently installed versions with `qtchooser -l`:
```bash
$ qtchooser -l
4
5
opt-qt512
qt4-x86_64-linux-gnu
qt4
qt5-x86_64-linux-gnu
qt5
```

If you installed from the PPA above, you'll need to use `opt-qt512`. Activate this by setting the `QT_SELECT` environment variable:

```bash
export QT_SELECT=opt-qt512
```

Run `qmake --version` again to make sure the correct version is set.

If you don't see the required version in the list of currently installed versions you will need to install it. You do this with `qtchooser -install <name> <path-to-qmake>`. If you used the PPA above then you'll need to write 
```bash
qtchooser -install opt-qt512 /opt/qt512/bin/qmake
```

Afterwards you should set the `QT_SELECT` environment variable to the name you chose (we used `opt-qt512`).

## X11

X11 packages are currently needed for sending keystrokes to the desktop from VR. Install the packages with `sudo apt-get install libx11-dev libxt-dev libxtst-dev`. 

## DBUS

DBUS is  needed for controlling media players from VR. 

## `clang-tidy` and `bear`

In order to use `clang-tidy` you will need `bear clang-tidy` in addition to the above (and `clang`). You will only need this if you're going to make contributions to the codebase.

```bash
sudo apt install bear clang-tidy
```

# TL;DR: for Ubuntu
## Ubuntu 16.04 Xenial
```bash
sudo add-apt-repository ppa:ubuntu-toolchain-r/test
sudo add-apt-repository ppa:beineri/opt-qt-5.12.2-xenial
sudo apt update
sudo apt install g++-7
sudo apt-get install build-essential libgl1-mesa-dev
sudo apt-get install qt512-meta-full
sudo apt-get install libx11-dev libxt-dev libxtst-dev
sudo apt-get install bear clang-tidy
qtchooser -install opt-qt512 /opt/qt512/bin/qmake
export QT_SELECT=opt-qt512
```

## Ubuntu 18.04 Bionic
```bash
sudo add-apt-repository ppa:beineri/opt-qt-5.12.2-bionic
sudo apt update
sudo apt install g++-7
sudo apt-get install build-essential libgl1-mesa-dev
sudo apt-get install qt512-meta-full
sudo apt-get install libx11-dev libxt-dev libxtst-dev
sudo apt-get install bear clang-tidy
qtchooser -install opt-qt512 /opt/qt512/bin/qmake
export QT_SELECT=opt-qt512
```

## Ubuntu 19.04 Disco

```bash
sudo apt install build-essential libgl1-mesa-dev
sudo apt install g++7
sudo apt install qt5-default qtmultimedia5-dev libqt5multimediawidgets5 libqt5multimedia5-plugins libqt5multimedia5 qml-module-qtquick-extras qml-module-qtquick-controls2 qml-module-qtquick-dialogs qtdeclarative5-dev
sudo apt-get install libx11-dev libxt-dev libxtst-dev
sudo apt install bear clang-tidy
```
You should not need to use `qtchooser` to set the Qt version on 19.04.


# Locations and Environment Variables

The following environmental variables are relevant for building the project.

| Environment Variable  | Purpose |
| --------------------  | ------------- |
| `QMAKE_SPEC`              | The [mkspec](https://forum.qt.io/topic/70970/what-is-mkspecs-used-for-how-to-configure-for-my-hardware) to compile to. Either `linux-g++` or `linux-clang`. Defaults to `linux-g++`.   |
| `USE_TIDY`              | If set a compilation database will be created and the project linted. Can only be used with `clang`.  |
| `NO_X11`              | If set the application will be compiled without X11 specific libraries. This disables certain things like sending keystrokes from VR.  |
| `NO_DBUS`              | If set the application will be compiled without DBUS specific functionality. This disables certain things like media keys.  |
| `MAKE_JOBS`              | Argument to `make --jobs`. Defaults to nothing (unlimited amount of jobs).  |
| `CLANG_TIDY_EXECUTABLE`              | Name of the `clang-tidy` executable. Defaults to `clang-tidy`. Used for specifying a specific version or path, for example `clang-tidy-7`.  |


# Building

With the programs above installed and environment variables set, go into the root folder of the repository and run `./build_scripts/linux/build_linux.sh`.

# Installing

The application can be installed by running `sudo make install` in the directory of the `Makefile`. 
This will copy the build directory to `/opt/OpenVR-AdvancedSettings/AdvancedSettings` by default.
The `PREFIX` location can be changed while running `qmake` by writing `qmake PREFIX=/your/dir/here`, then `make` and `sudo make install`.

# Contributing

For full details, see [CONTRIBUTING.md](CONTRIBUTING.md).
