* [Requirements](#requirements)
  * [Git](#git)
     * [Ubuntu](#ubuntu)
     * [Arch/Manjaro](#archmanjaro)
  * [Compiler and Build Essentials](#compiler-and-build-essentials)
     * [Ubuntu](#ubuntu-1)
     * [Arch/Manjaro](#archmanjaro-1)
  * [Qt](#qt)
  * [Package Manager](#package-manager)
     * [Ubuntu](#ubuntu-2)
     * [Arch/Manjaro](#archmanjaro-2)
  * [Official Qt Installer](#official-qt-installer)
     * [qtchooser](#qtchooser)
  * [Unofficial Ubuntu Packages](#unofficial-ubuntu-packages)
     * [Arch/Manjaro](#archmanjaro-3)
  * [qtchooser and versions](#qtchooser-and-versions)
  * [X11](#x11)
  * [DBUS](#dbus)
  * [PulseAudio](#pulseaudio)
  * [SteamVR](#steamvr)
  * [clang-tidy and bear](#clang-tidy-and-bear)
* [TL;DRs](#tldrs)
  * [Ubuntu 16.04 Xenial](#ubuntu-1604-xenial)
  * [Ubuntu 18.04 Bionic](#ubuntu-1804-bionic)
  * [Ubuntu 19.04 Disco and newer](#ubuntu-1904-disco-and-newer)
  * [Arch/Manjaro](#archmanjaro-4)
* [Building](#building)
* [Configuring](#configuring)
* [Installing](#installing)
* [Contributing](#contributing)


Unless you have a good reason not to, just copy and paste the correct TL;DR section into your terminal.

# Requirements

## Git

You will need `git` to clone the repo.

### Ubuntu

```bash
sudo apt-get -y install git
```

### Arch/Manjaro

```bash
sudo pacman -S git
```

## Compiler and Build Essentials

You will need either `clang` version `>=5.0` or `gcc` version `>=7` due to C++17 support.
You will additionally need the basic build packages for your distro and OpenGL development headers.

### Ubuntu

For Ubuntu versions older than 18.04 (only 16.04 is officially tested) you'll need to get a backported `gcc`:
```bash
sudo add-apt-repository ppa:ubuntu-toolchain-r/test
sudo apt-get -y update
sudo apt-get -y install build-essential g++-7 libgl1-mesa-dev
```

For Ubuntu versions of 18.04 or newer `build-essential` will contain an appropriate `gcc` version:
```bash
sudo apt-get -y install build-essential libgl1-mesa-dev
```

### Arch/Manjaro

`base-devel` contains `gcc` and `mesa` contains the necessary OpenGL things.

```bash
sudo pacman -S base-devel mesa
```

## Qt

At least version `5.12` is required.

## Package Manager

Ubuntu newer than than 19.04 and Arch/Manjaro will have up to date Qt packages in the official repositories.

### Ubuntu

```bash
sudo apt-get -y install qt5-default qtmultimedia5-dev libqt5multimediawidgets5 libqt5multimedia5-plugins libqt5multimedia5 qml-module-qtquick-extras qml-module-qtquick-controls2 qml-module-qtquick-dialogs qml-module-qtmultimedia qtdeclarative5-dev qtchooser
```

### Arch/Manjaro

```bash
sudo pacman -S qt5-base qt5-multimedia
```

## Official Qt Installer

The easiest way to get it if your packages aren't up to date is from the [official Qt installer](https://www.qt.io/download-qt-installer).

### `qtchooser`
After installing using the official installer you will likely need to `qtchooser -install <name> <qmake-location>` in order for Qt to use the correct version. See the `qtchooser` section below.

## Unofficial Ubuntu Packages

If you don't want to use the offical installer, you can use [this](https://launchpad.net/~beineri) PPA.

You will need at least `qt512-meta-minimal qt512multimedia qt512declarative qt512quickcontrols2 qt512quickcontrols qt512tools qt512imageformats qt512svg qt512base`, although if you have the drive space it's probably easier just to get `qt512-meta-full`.

For Ubuntu 16.04
```bash
sudo add-apt-repository ppa:beineri/opt-qt-5.12.6-xenial
sudo apt-get update
sudo apt-get -y install qt512-meta-full
```

For Ubuntu 18.04
```bash
sudo add-apt-repository ppa:beineri/opt-qt-5.12.6-bionic
sudo apt-get update
sudo apt-get -y install qt512-meta-full
```

If you choose the wrong Ubuntu version you will get an error from `apt` about there not being a `Release` file. Try the other one instead.

### Arch/Manjaro

The `qt5-declarative`, `qt5-quickcontrols` and `qt5-multimedia` packages are required.

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

If you don't see the required version in the list of currently installed versions you will need to install it.
You do this with `qtchooser -install <name> <path-to-qmake>`.
If you used the Ubuntu PPA above then you'll need to write
```bash
qtchooser -install opt-qt512 /opt/qt512/bin/qmake
```

Afterwards you should set the `QT_SELECT` environment variable to the name you chose (we used `opt-qt512`).
```bash
export QT_SELECT=opt-qt512
```

## X11

X11 packages are currently needed for sending keystrokes to the desktop from VR.
Install the packages on Ubuntu with `sudo apt-get -y install libx11-dev libxt-dev libxtst-dev`.

This feature and dependency can be disabled during compilation.

## DBUS

DBUS is  needed for controlling media players from VR. 

This feature and dependency can be disabled during compilation.

## PulseAudio

PulseAudio is needed for controlling audio devices through the Audio tab.

This feature and dependency can be disabled during compilation.

## SteamVR

On a completely fresh Ubuntu install the program also requires the following libraries to run.

```bash
sudo apt-get -y install libsdl2-dev libvulkan-dev
```

## `clang-tidy` and `bear`

This is only relevant for building on CI servers.
Unless you're checking code quality you will not need this.

In order to use `clang-tidy` you will need `bear clang-tidy` in addition to the above (and `clang`).
You will only need this if you're going to make contributions to the codebase.

```bash
sudo apt-get -y install bear clang-tidy
```

# TL;DRs
## Ubuntu 16.04 Xenial
```bash
# Add repos and update
sudo add-apt-repository ppa:ubuntu-toolchain-r/test
sudo add-apt-repository ppa:beineri/opt-qt-5.12.6-xenial
sudo apt-get update
# Install git
sudo apt-get -y install git
# Clone repo and change directory
git clone https://github.com/OpenVR-Advanced-Settings/OpenVR-AdvancedSettings --depth=1
cd OpenVR-AdvancedSettings
# Install build tools
sudo apt-get -y install g++-7
sudo apt-get -y install build-essential libgl1-mesa-dev
# Install Qt
sudo apt-get -y install qt512-meta-full qtchooser
# Install additional features
sudo apt-get -y install libx11-dev libxt-dev libxtst-dev libpulse-dev
# Install necessary SteamVR libraries
sudo apt-get -y install libsdl2-dev libvulkan-dev
# Set up qtchooser
qtchooser -install opt-qt512 /opt/qt512/bin/qmake
export QT_SELECT=opt-qt512
# Compile
qmake
make -j2
```

## Ubuntu 18.04 Bionic
```bash
# Add repos and update
sudo add-apt-repository ppa:beineri/opt-qt-5.12.6-bionic
sudo apt-get update
# Install git
sudo apt-get -y install git
# Clone repo and change directory
git clone https://github.com/OpenVR-Advanced-Settings/OpenVR-AdvancedSettings --depth=1
cd OpenVR-AdvancedSettings
# Install build tools
sudo apt-get -y install g++-7
sudo apt-get -y install build-essential libgl1-mesa-dev
# Install Qt
sudo apt-get -y install qt512-meta-full qtchooser
# Install additional features
sudo apt-get -y install libx11-dev libxt-dev libxtst-dev libpulse-dev
# Install necessary SteamVR libraries
sudo apt-get -y install libsdl2-dev libvulkan-dev
# Set up qtchooser
qtchooser -install opt-qt512 /opt/qt512/bin/qmake
export QT_SELECT=opt-qt512
# Compile
qmake
make -j2
```

## Ubuntu 19.04 Disco and newer

```bash
# Install git
sudo apt-get -y install git
# Clone repo and change directory
git clone https://github.com/OpenVR-Advanced-Settings/OpenVR-AdvancedSettings --depth=1
cd OpenVR-AdvancedSettings
# Install build tools
sudo apt-get -y install build-essential libgl1-mesa-dev
# Install Qt
sudo apt-get -y install qt5-default qtmultimedia5-dev libqt5multimediawidgets5 libqt5multimedia5-plugins libqt5multimedia5 qml-module-qtquick-extras qml-module-qtquick-controls2 qml-module-qtquick-dialogs qml-module-qtmultimedia qtdeclarative5-dev qtchooser
# Install additional features
sudo apt-get -y install libx11-dev libxt-dev libxtst-dev libpulse-dev
# Install necessary SteamVR libraries
sudo apt-get -y install libsdl2-dev libvulkan-dev
# Set up qtchooser
export QT_SELECT=5
# Compile
qmake
make -j2
```

## Arch/Manjaro

```bash
# Install git
sudo pacman --noconfirm -S git
# Clone repo and change directory
git clone https://github.com/OpenVR-Advanced-Settings/OpenVR-AdvancedSettings --depth=1
cd OpenVR-AdvancedSettings
# Install build tools
sudo pacman --noconfirm -S base-devel mesa libudev0-shim
# Install Qt
sudo pacman --noconfirm -S qt5-declarative qt5-multimedia
# Install additional features
sudo pacman --noconfirm -S xorg-server dbus pulseaudio
# Compile
qmake
make -j2
```

A stable version can also be found on the [Arch User Repository](https://wiki.archlinux.org/index.php/Arch_User_Repository) as `ovras`.
It can be installed with `pamac build ovras`.

# Building

You will need the requirements above.

If you want a default build run `qmake; make` in the project directory.
This is what most users will want.

Build artifacts can then be cleaned up with `make clean`.

# Configuring

Configuration changes must be made in the `qmake` step by passing arguments.
If you don't know why you would change any of these then you don't need to.

If you wanted to use `clang` instead of `gcc` you would type
```bash
qmake -spec linux-clang
```

The following values can be appended to the `CONFIG` internal variable in order to enable or disable features.

| Value | Purpose |
| ----- | ------- |
| `noX11` | Disables X11 specific features (VR to keyboard input). |
| `noDBUS` | Disables DBUS specific features (control media players). |
| `noPulse` | Disables PulseAudio specific features (change audio devices). |
| `debugSymbolsAndLogs` | Enables debug symbols and debug logging calls (while still having release optimizations). |
| `warnings_as_errors` | Makes compiler warnings failing errors (not needed unless you're changing the source). |

The values are case sensitive.

If you wanted to disable X11 specific features you would type
```bash
qmake CONFIG+=noX11
```

If you want to disable both X11 and DBUS features you would type
```bash
qmake CONFIG+=noX11 CONFIG+=noDBUS
```

If you want to disable X11, DBUS and PulseAudio features you would type
```bash
qmake CONFIG+=noX11 CONFIG+=noDBUS CONFIG+=noPulse
```

If you want to compile with warnings as errors you would type
```bash
qmake CONFIG+=warnings_as_errors
```

The location for `make install` can be configured in the `qmake` step.
This is done by setting the `PREFIX` variable.
```bash
qmake PREFIX=/my/path/
```
If no `PREFIX` variable is set a default value will be used.

# Installing

The application can be installed by running `sudo make install` in the directory of the `Makefile`. 
This will copy the build directory to `/opt/OpenVR-AdvancedSettings/AdvancedSettings` by default.
The `PREFIX` location can be changed while running `qmake` by writing `qmake PREFIX=/your/dir/here`, then `make` and `sudo make install`.

# Contributing

For full details, see [CONTRIBUTING.md](CONTRIBUTING.md).
