pragma Singleton
import QtQuick 2.7
import QtMultimedia 5.6
import ovras.advsettings 1.0

QtObject {
	function playActivationSound() {
        OverlayController.playActivationSound()
	}
	function playFocusChangedSound() {
        OverlayController.playFocusChangedSound()
    }
}
