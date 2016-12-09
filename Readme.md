
![language](https://img.shields.io/badge/Language-C%2B%2B11%2C%20QML-green.svg) ![dependencies](https://img.shields.io/badge/Dependencies-OpenVR%2C%20Qt5-green.svg) ![license_gpl3](https://img.shields.io/badge/License-GPL%203.0-green.svg)

# OpenVR Advanced Settings Overlay

Adds an overlay to the OpenVR dashboard that allows access to advanced settings.

![Example Screenshot](https://raw.githubusercontent.com/matzman666/OpenVR-AdvancedSettings/master/docs/screenshots/InVRScreenshot.png)

# Features

- Set supersampling values.
- Enable/disable asynchronous and interleaved reprojection.
- Change several chaperone settings not accessible via SteamVR settings (to e.g. make it completely invisible).
- Chaperone Profiles: Allows to quickly switch chaperone geometry and settings while in VR.
- Temporarily move and rotate the playspace.
- Floor height fix.
- Displays several performance statistics and other statistics (e.g. headset rotations).
- Mute/unmute and set volume of audio mirror device.
- Mute/unmute and set volume of microphone device.
- Implements push-to-talk and allows to configure push-to-talk profiles.
- Desktop mode shows a window on the desktop instead of a vr overlay.

# Usage

Download the newest windows binary from the [release section](https://github.com/matzman666/OpenVR-AdvancedSettings/releases).

To install for the first time simply start AdvancedSettings.exe once while SteamVRis running. It will register with SteamVRand automatically start whenever SteamVR starts (Can be disabled in the settings).

To upgrade an existing installation first stop SteamVR, then either override the old files or delete the old files (very important, otherwise SteamVR will always start the old version). At last start SteamVR and run AdvancedSettings.exe once (only when you don't have overridden the old files).

Executing startdesktopmode.bat shows a window on the desktop instead of a vr overlay. Desktop mode can be used alongside an already running vr overlay instance.

# Documentation

## Top Page:

![Root Page](https://raw.githubusercontent.com/matzman666/OpenVR-AdvancedSettings/master/docs/screenshots/RootPage.png)

- **Chaperone Profile**: Allows to apply chaperone profiles. Profiles can be created on the Chaperone page
- **Microphone**: Allows to set the microphone volume and to mute/unmute it.
- **Push-to-Talk**: Enable/disable push-to-talk and allows to apply push-to-talk profiles. Profiles can be created on the Audio page.

## - SteamVR Page:

![SteamVR Page](https://raw.githubusercontent.com/matzman666/OpenVR-AdvancedSettings/master/docs/screenshots/SteamVRPage.png)

- **Application Supersampling**: Supersampling setting for OpenVR applications/games (Requires restart).
- **Compositor Supersampling**: Supersampling setting for the OpenVR compositor (Requires restart).
- **Allow Asynchronous Reprojection**: Enable/disable asynchronous reprojection.
- **Allow Interleaved Reprojection**: Enable/disable interleaved reprojection.
- **Force Reprojection**: Enable/disable forced reprojection (Requires restart).
- **Restart SteamVR**: Restart SteamVR.

## - Chaperone Page:

![Chaperone Page](https://raw.githubusercontent.com/matzman666/OpenVR-AdvancedSettings/master/docs/screenshots/ChaperonePage.png)

- **Profile**: Allows to apply/define/delete chaperone profiles that save geometry info, style info or other chaperone settings (What exactly is saved in a chaperone profile can be selected when a profile is created).
- **Visibility**: Allows to configure the visibility of the chaperone bounds. Unlike the slider in the chaperone settings, this one is not capped at 30%. When set to 0 chaperone bounds are completely invisible.
- **Fade Distance**: Allows to configure the distance at which the chaperone bounds are shown. When set to 0 chaperone bounds are completely invisible.
- **Center Marker**: Displays a marker that represents the center of the playspace.
- **Play Space Maker**: Displays the outlines of the rectangle representing the playspace.
- **Force Bounds**: Force chaperone bounds always on.
- **Flip Orientation**: Flips the orientation of the play space.
- **Reload from Disk**: Reloads the chaperone bounds geometry from disk.

## - Play Space Page:

![Play Space Page](https://raw.githubusercontent.com/matzman666/OpenVR-AdvancedSettings/master/docs/screenshots/PlayspacePage.png)

Allows to temporarily move and rotate the center of the playspace. This allows to reach interaction elements that are just inside our real-world walls or otherwise inaccessible (e.g. when your playspace is smaller than the recommended one). Can also be used to discover the terrors that lie outside of the intended playspace (ever wondered what's behind the door in The Lab?).

The chaperone bounds stay in place when the playspace is moved or rotated (I don't want anyone to get hurt). Unfortunately this does not work when moving up/down.

## - Floor Fix Page:

![Floor Fix Page](https://raw.githubusercontent.com/matzman666/OpenVR-AdvancedSettings/master/docs/screenshots/FloorFixPage.png)

Allows you to fix the height of your floor. Just place one controller on your floor and press the button.

## - Audio Page:

![Audio Page](https://raw.githubusercontent.com/matzman666/OpenVR-AdvancedSettings/master/docs/screenshots/AudioPage.png)

- **Mirror Device**: Allows to set the volume of the audio mirror device and to mute/unmute it.
- **Microphone**: Allows to set the volume of the microphone device and to mute/unmute it.
- **Push-to-Talk**: Enable/disable push-to-talk. When push-to-talk is activated the microphone gets muted unless one of the configured buttons is pressed on one of the configured controllers.
- **Left Controller**: Configure the left controller for push-to-talk.
- **Right Controller**: Configure the right controller for push-to-talk.
- **Show Notification**: Shows a notification icon in the headset when push-to-talk is enabled a configured button is pressed.
- **Profile**: Allows to apply/define/delete push-to-talk profiles that save buttons, controller and notification settings.

## - Statistics Page:

![Statistics Page](https://raw.githubusercontent.com/matzman666/OpenVR-AdvancedSettings/master/docs/screenshots/StatisticsPage.png)

- **HMD Distance Moved**: Shows the distance the headset has moved on the xz-plane.
- **HMD Rotations**: Shows the number of rotations around the y-axis (Useful for untangling the cord).
- **Left Controller Max Speed**: Shows the max speed of the left controller.
- **Right Controller Max Speed**: Shows the max speed of the right controller.
- **Dropped Frames**: Number of frames dropped in the currently running application.
- **Reprojected Frames**: Number of frames reprojected in the currently running application.
- **Reprojected Frames**: Number of times the currently running application timed out.

## - Settings Page:

![Settings Page](https://raw.githubusercontent.com/matzman666/OpenVR-AdvancedSettings/master/docs/screenshots/SettingsPage.png)

- **Autostart:** Allows you to enable/disable auto start.

# Notes:

- The center marker and the play space marker are provided by the chaperone subsystem, and therefore they will have the same color and visibility settings as the chaperone bounds.

- Application autostart can also be configured in the SteamVR menu (SteamVR->Settings->Applications->"Advanced Settings").

- You can modify the shutdown wait time when restarting SteamVR by opening restartvrserver.bat and changing the number behind "timeout /t" (unit is seconds).

# Known Bugs

- None yet.

# License

This software is released under GPL 3.0.
