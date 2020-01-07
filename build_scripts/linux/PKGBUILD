# Maintainer: Vitaly Utkin <vautkin AT teknik DOT io>
pkgname=ovras
pkgver=4.0.1
pkgrel=2
epoch=0
pkgdesc="Advanced settings and custom behavior for SteamVR using OpenVR."
arch=("x86_64")
url="https://github.com/OpenVR-Advanced-Settings/OpenVR-AdvancedSettings"
license=("GPL")
depends=("qt5-declarative"
         "qt5-multimedia"
         "libudev0-shim"
         "mesa")
optdepends=("dbus: media player support"
            "xorg-server: send keyboard keys")
source=("https://github.com/OpenVR-Advanced-Settings/OpenVR-AdvancedSettings/archive/v$pkgver.tar.gz")
sha256sums=("371d5a3f81a986ec5b3446c62a36cbf5eb951c97ea2b4a64752c4709bb8e6c76")

build() {
    cd "OpenVR-AdvancedSettings-$pkgver"

    _additionalOptions=

    # Attempting to compile without package will result in compile error
    pacman -Qi xorg-server >/dev/null 2>&1
    if [ $? -ne 0 ]; then
        _additionalOptions="CONFIG+=noX11"
        echo "X11 features disabled."
    else
        echo "X11 features enabled."
    fi

    # Attempting to compile without package will result in compile error
    pacman -Qi dbus >/dev/null 2>&1
    if [ $? -ne 0 ]; then
        _additionalOptions+=" CONFIG+=noDBUS"
        echo "DBUS features disabled."
    else
        echo "DBUS features enabled."
    fi

    qmake PREFIX="$pkgdir/opt/" $_additionalOptions
    make
}

package() {
    cd "OpenVR-AdvancedSettings-$pkgver"

    # Add .desktop file
    mkdir -p "$pkgdir/usr/share/applications"
    cp "src/package_files/linux/AdvancedSettings.desktop" "$pkgdir/usr/share/applications/"
    sed -i 's/Exec=.*/Exec=\/opt\/AdvancedSettings\/AdvancedSettings/' "$pkgdir/usr/share/applications/AdvancedSettings.desktop"

    # Add correct desktop icon to desktop file
    sed -i 's/Icon=.*/Icon=\/opt\/AdvancedSettings\/AdvancedSettings.png/' "$pkgdir/usr/share/applications/AdvancedSettings.desktop"
    # Dir doesn't exist before `make install`
    mkdir -p "$pkgdir/opt/AdvancedSettings/"
    cp "src/res/img/icons/thumbicon.png" "$pkgdir/opt/AdvancedSettings/AdvancedSettings.png"

    # Make program use correct working dir
    echo "Path=/opt/AdvancedSettings" >> "$pkgdir/usr/share/applications/AdvancedSettings.desktop"

    # Enable command line usage
    mkdir -p "$pkgdir/usr/bin/"
    ln -s /opt/AdvancedSettings/AdvancedSettings "$pkgdir/usr/bin/ovras"

    # Install
    make install
}
