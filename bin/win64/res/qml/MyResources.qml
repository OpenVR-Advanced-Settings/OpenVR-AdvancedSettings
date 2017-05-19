pragma Singleton
import QtQuick 2.7
import QtMultimedia 5.6
import matzman666.advsettings 1.0

QtObject {
	function playActivationSound() {
		if (!OverlayController.soundDisabled()) {
			activationSound.play()
		}
	}
	function playFocusChangedSound() {
		if (!OverlayController.soundDisabled()) {
			focusChangedSound.play()
		}
	}
	property SoundEffect activationSound: SoundEffect {
		source: vrRuntimePath + "content/panorama/sounds/activation.wav"
	}
	property SoundEffect focusChangedSound: SoundEffect {
		source: vrRuntimePath + "content/panorama/sounds/focus_change.wav"
	}
}
