pragma Singleton
import QtQuick 2.7
import QtMultimedia 5.6

QtObject {
	property SoundEffect activationSound: SoundEffect {
		source: vrRuntimePath + "tools/content/panorama/sounds/activation.wav"
	}
	property SoundEffect focusChangedSound: SoundEffect {
		source: vrRuntimePath + "tools/content/panorama/sounds/focus_change.wav"
	}
}
