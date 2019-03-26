![language](https://img.shields.io/badge/Language-C%2B%2B17%2C%20QML-green.svg) ![dependencies](https://img.shields.io/badge/Dependencies-OpenVR%2C%20Qt5%2C%20Python3-green.svg)
![license_gpl3](https://img.shields.io/badge/License-GPL%203.0-green.svg) 

[![Build status](https://ci.appveyor.com/api/projects/status/8oivf5xws6vsq2x3/branch/master?svg=true)](https://ci.appveyor.com/project/icewind1991/openvr-advancedsettings/branch/master)
[![Build Status](https://travis-ci.org/OpenVR-Advanced-Settings/OpenVR-AdvancedSettings.svg?branch=master)](https://travis-ci.org/OpenVR-Advanced-Settings/OpenVR-AdvancedSettings)

 > * [Features](#features)
 > * [Usage](#usage)
 >   * [Installer](#installer)
 >   * [Standalone](#standalone)
 >   * [Bindings](#bindings)
 >   * [Default Bindings](#default_bindings)
 >   * [SteamVR Input Guide](#steamvr_input_guide)
 >   * [Command Line Arguments](#command_line_arguments)
 >   * [Preview Builds](#preview_builds)
 > * [Documentation](#documentation)
 >   * [Top Page](#top_page)
 >   * [SteamVR Page](#steamvr_page)
 >   * [Chaperone Page](#chaperone_page)
 >   * [Chaperone Proximity Warning Settings Page](#chaperone_proximity_page)
 >   * [Space Offset](#play_space_page)
 >   * [Motion Page](#motion_page)
 >   * [Space Fix Page](#space_fix_page)
 >   * [Audio Page](#audio_page)
 >   * [Utilities Page](#utilities_page)
 >   * [Statistics Page](#statistics_page)
 >   * [Settings Page](#settings_page)
 > * [How To Compile](#how_to_compile)
 >   * [Building on Windows](#building_on_windows)
 >   * [Building on Linux](#building_on_linux)
 > * [Notes](#notes)
 > * [License](#license)

<a name="top"></a>
# OpenVR Advanced Settings Overlay

Adds an overlay to the OpenVR dashboard that allows access to advanced settings.

![Example Screenshot](docs/screenshots/InVRScreenshot.png)

<a name="features"></a>
# Features

- Set supersampling values.
- Enable/disable motion smoothing and advanced supersample filtering.
- Save supersampling and reprojection settings into profiles.
- Change several chaperone settings not accessible via SteamVR settings (to e.g. make it completely invisible).
- Chaperone Proximity Warning: Several warning methods can be configured.
- Chaperone Profiles: Allows to quickly switch chaperone geometry and settings while in VR.
- Temporarily move and rotate the playspace.
- Floor height fix, for knuckles and wands.
- Floor Center fix.
- Displays several performance statistics and other statistics (e.g. headset rotations).
- Select the playback/mirror/recording audio device
- Mute/unmute and set volume of audio mirror device.
- Mute/unmute and set volume of microphone device.
- Implements push-to-talk, and push-to-mute
- Desktop mode shows a window on the desktop instead of a VR overlay.
- VR Keyboard Input Utilities
- Simple Alarm Clock
- Player height adjust toggle
- Snap Turn option.
- Advanced Space Features: gravity, space-turn, and space-drag
- Simple Media Player macro keybind
- SteamVR Input Action System implemented for maximum control of keybinds.
- Removed forced on Die-in-Game Die-in-Life setting.


<a name="usage"></a>
# Usage

<a name="installer"></a>
## Installer

Download the newest installer from the [release section](https://github.com/OpenVR-Advanced-Settings/OpenVR-AdvancedSettings/releases) and execute it.

If you have an older standalone version installed, the installer will automatically uninstall the old version and install the new version.

<a name="standalone"></a>
## Standalone

Download the newest standalone archive from the [release section](https://github.com/OpenVR-Advanced-Settings/OpenVR-AdvancedSettings/releases) and unpack the archive.

To install for the first time simply start AdvancedSettings.exe once while SteamVR is running. It will register with SteamVR and automatically starts whenever SteamVR starts (Can be disabled in the settings).

To upgrade an existing installation first stop SteamVR and delete the old application folder (or override it). Then start SteamVR and run AdvancedSettings.exe once.

<a name="bindings"></a>
## Bindings

After SteamVR beta 1.3.1 the bindings for Advanced Settings will show up in the bindings menu.


The following actions currently exist:
### Music

Actions associated with media player.

|    Action     |     Type      |  Explanation  |
| ------------- | ------------- |  ------------ |
| Play Next Track | Binary/Button | The same as using the media keys. Tells a media player to play the next song.|
| Play Previous Track | Binary/Button | The same as using the media keys. Tells a media player to play the previous song. |
| Pause/Play Track | Binary/Button | The same as using the media keys. Tells the media player to press play/pause. |
| Stop Track| Binary/Button | The same as using the media keys. Tells the media player to stop playback. |

### Motion

Actions associated with motion tab, and motion.

Override actions will take priority over non-override actions during simultaneous activation. Example: binding space turn to single click and space drag (override) to double click on the same physical button.

|    Action     |     Type      |  Explanation  |
| ------------- | ------------- |  ------------ |
| Left Hand Space Turn | Binary/Button | Rotates PlaySpace based on left controller rotation while held. |
| Right Hand Space Turn | Binary/Button | Rotates PlaySpace based on right controller rotation while held. |
| Left Hand Space Drag | Binary/Button | Moves PlaySpace based on left controller position while held. |
| Right Hand Space Drag | Binary/Button | Moves PlaySpace based on right controller position while held. |
| (Optional Override) Left Hand Space Turn | Binary/Button | Override version: will activate instead of non-override during simultaneous activation. |
| (Optional Override) Right Hand Space Turn | Binary/Button | Override version: will activate instead of non-override during simultaneous activation. |
| (Optional Override) Left Hand Space Drag | Binary/Button | Override version: will activate instead of non-override during simultaneous activation. |
| (Optional Override) Right Hand Space Drag | Binary/Button | Override version: will activate instead of non-override during simultaneous activation. |
| Swap Active Space Drag to Left Hand (Override) | Binary/Button |  Activates space drag on left controller only if right controller is currently active in space drag. (Useful for climbing motions) |
| Swap Active Space Drag to Right Hand (Override) | Binary/Button | Activates space drag on right controller only if left controller is currently active in space drag. (Useful for climbing motions) |
| Gravity Toggle | Binary/Button |  Toggles Gravity state when pressed. |
| Gravity Reverse | Binary/Button |  Temporarily Reverses Gravity while held. |
| Reset Offsets | Binary/Button |  Resets your offset and rotation to 0. |
| Height Toggle | Binary/Button |  Shifts the gravity floor level by offset configured in motion tab. If gravity is inactive: also shifts the user's current y-axis position by offset configured in motion tab. |
| Snap-Turn Left | Binary/Button |  Rotates a set value to the left based on settings in motion tab. |
| Snap-Turn Right | Binary/Button |  Rotates a set value to the right based on settings in motion tab. |

### Misc.

Actions that don't have a clear category.

|    Action     |     Type      |  Explanation  |
| ------------- | ------------- |  ------------ |
| X-Axis Lock Toggle | Binary/Button | Toggles the lock of the X-Axis for offsets.|
| Y-Axis Lock Toggle | Binary/Button | Toggles the lock of the Y-Axis for offsets.|
| Z-Axis Lock Toggle | Binary/Button | Toggles the lock of the Z-Axis for offsets.|
| Push to Talk | Binary/Button |  Acts as starter for PTT, can mute if push-to-mute is selected.|

### Haptics.

Actions to be handled by the system.

|    Action     |     Type      |  Explanation  |
| ------------- | ------------- |  ------------ |
| Haptics Left | Vibration | Handle for haptic events on the Left Controller. |
| Haptics Right | Vibration | Handle for haptic events on the Right Controller. |


<a name="default_bindings"></a>
## Default Bindings

### Vive Wands

- Menu Button (both hands)
  - **Single Click**: Space Turn (respective hand)
  - **Double Click**: Space Drag (override) (respective hand)
  - **Single Click**: Swap Active Space Drag (override) (respective hand)
- TrackPad (right hand)
  - **D-pad Down click**: Push-to-Talk
- Haptics bound

### Knuckles

- B Button (both hands)
  - **Single Click**: Space Turn (respective hand)
  - **Double Click**: Space Drag (override) (respective hand)
  - **Single Click**: Swap Active Space Drag (override) (respective hand)
- Trigger (left hand)
  - **Click**: Push-to-Talk
- Haptics bound

### Other Controllers

No current default bindings you will have to make your own.


<a name="steamvr_input_guide"></a>
## SteamVR Input Guide

A Guide to the SteamVR Input System can be found [here](docs/SteamVRInputGuide.md)

**Or** a Video guide by Kung can be found [here](https://youtu.be/2ZHdjOfnqOU)

<a name="command_line_arguments"></a>
## Command Line Arguments

The application (`AdvancedSettings.exe`) can be run with the following optional arguments:

`"--desktop-mode"`: Creates a settings window on the desktop, but not in VR. Running `startdesktopmode.bat` in the install directory has the same effect.

`"--force-no-sound"`: Forces sound effects off.

`"--force-no-manifest"`: Forces not using a `.vrmanifest`.

`"--force-install-manifest"`: Force installs the `.vrmanifest` and adds the application to autostart. If you're having issues with autostart not working try running the program once with this set. The program will exit early when this flag is set.

`"--force-remove-manifest"`: Force uninstalls the `.vrmanifest`. This should be done every time the application is uninstalled. On Windows it is automatically done by the uninstaller. The program will exit early when this flag is set.

<a name="preview_builds"></a>
## Preview builds

If you want to try latest and greatest unreleased features, you can download the latest from the CI (Continuous Integration) server for [Windows](https://ci.appveyor.com/project/icewind1991/openvr-advancedsettings/branch/master). The Linux CI does not provide binary artifacts.

These version are not stable and this should be considered for advanced users only.

<a name="documentation"></a>
# Documentation

<a name="top_page"></a>
## Top Page:

![Root Page](docs/screenshots/RootPage.png)

- **Supersampling Profile**: Allows to apply supersampling profiles. Profiles can be created on the SteamVR page
- **Chaperone Profile**: Allows to apply chaperone profiles. Profiles can be created on the Chaperone page
- **Microphone**: Allows to set the microphone volume and to mute/unmute it.
- **Push-to-Talk**: Enable/disable push-to-talk.

<a name="steamvr_page"></a>
## - SteamVR Page:

![SteamVR Page](docs/screenshots/SteamVRPage.png)

- **Profile**: Allows to apply/define/delete supersampling profiles that save supersampling and reprojection settings.
- **Application Supersampling**: Supersampling setting for OpenVR applications/games. Application supersampling [now behaves linearly](https://steamcommunity.com/games/250820/announcements/detail/1256913672017664045) where 2.0 means twice the number of pixel.
  - **Note**: Manual Supersampling Override be checked.
- **Enable Manual Supersampling Override**: Enables user control of Supersampling, instead of SteamVR auto profiles.
- **Enable Motion Smoothing**: Enables Motion Smoothing, and disables asynchronous reprojection.
- **Restart SteamVR**: Restart SteamVR (May crash the Steam overlay when SteamVR Home is running when you restart. Therefore I advice that you close SteamVR Home before restarting).
<a name="chaperone_page"></a>
## - Chaperone Page:

![Chaperone Page](docs/screenshots/ChaperonePage.png)

- **Profile**: Allows to apply/define/delete chaperone profiles that save geometry info, style info or other chaperone settings (What exactly is saved in a chaperone profile can be selected when a profile is created).
- **Visibility**: Allows to configure the visibility of the chaperone bounds. 50% to 100% opacity.
- **Fade Distance**: Allows to configure the distance at which the chaperone bounds are shown. When set to 0 chaperone bounds are completely invisible.
- **Height**: Allows to configure the height of the chaperone bounds.
- **Center Marker**: Displays a marker that represents the center of the playspace.
- **Play Space Maker**: Displays the outlines of the rectangle representing the playspace.
- **Force Bounds**: Force chaperone bounds always on.
- **Disable Chaperone**: Disables Chaperone by setting fade distance to 0.0.
- **Proximity Warning Settings**: Opens a page that allows to configure several warning methods for when the user comes too close to the chaperone bounds.
- **Flip Orientation**: Flips the orientation of the play space.
- **Reload from Disk**: Reloads the chaperone bounds geometry from disk.

<a name="chaperone_proximity_page"></a>
## - Chaperone Proximity Warning Settings Page:

![Chaperone Proximity Warning Settings Page](docs/screenshots/ChaperoneWarningPage.png)

- **Switch to Beginner Mode**: Switches the chaperone bound's style to beginner mode when the user's distance to the chaperone falls below the configured activation distance.
- **Trigger Haptic Feedback**: The left and right controller start vibrating when the user's distance to the chaperone falls below the configured activation distance. (HMD or controllers can trigger)
- **Audio Warning**: Plays an alarm sound when the user's distance to the chaperone falls below the configured activation distance.
  - **Loop Audio**: Whether the audio alarm should only be played once or in a loop.
  - **Loop Audio**: Modify audio volume as a function of the user's distance to the chaperone.
- **Open dashboard**: Opens the dashboard when the user's distance to the chaperone falls below the configured activation distance. The idea is to pause the game (most single-player games auto-pause when the dashboard is shown) to give the user time for reorientation.
- **Velocity Dependent Fade/Activation Distance**: Dynamically modifies the chaperone's fade distance and the proximity warning's activation distance as a function of the player's speed. The used formula is: *distance = old_distance * ( 1 + distance_modifier * max(left_controller_speed, right_controller_speed, hmd_speed) )*

<a name="space_offset_page"></a>
## - Space Offset Page:

![Play Space Page](docs/screenshots/OffsetPage.png)

Allows users to temporarily move and rotate the center of the playspace. This allows to reach interaction elements that are just outside our real-world walls or otherwise inaccessible (e.g. when your playspace is smaller than the recommended one). Can also be used to discover the terrors that lie outside of the intended playspace (ever wondered what's behind the door in The Lab?).

- **Adjust Chaperone**: When enabled, the chaperone bounds stay accurate even when the playspace is moved or rotated (so noone gets hurt). Depending on chaperone mode this may or may not adjust with height.

<a name="motion_page"></a>
## - Motion Page:

![Motion Page](docs/screenshots/MotionPage.png)

- **Space Drag**: Allows shifting your playspace by dragging your controller, Binds must be set via SteamVR Input system.
  - **Enable Left/Right Hand**: Toggles functionality (must be active in addition to binding via input system to work.)
  - **Drag Comfort Mode**: Limits the rate at which your movement updates, reducing smoothness so that perceived motion starts to feel more like mini-teleports. Higher values reduce smoothness more.
- **Space Turn**: Allows rotating your playspace by rotating your controller. Binds must be set via SteamVR Input system.
  - **Enable Left/Right Hand**: Toggles functionality (must be active in addition to binding via input system to work.)
  - **Turn Comfort Mode**: Limits the rate at which your rotation updates, reducing smoothness so that perceived rotation starts to feel more like mini-snap-turns. Higher values reduce smoothness more.
- **Height Toggle**: Toggle between zero and an offset for gravity floor height. If gravity is inactive the user is also moved to this offset. (Example: allows for quick switching between a seated and standing height.) Can be bound via SteamVr Input System.
  - **On**: Current toggle state, Binds directly modify this.
  - **Height Offset**: The amount of the offset (+ is down.)
  - **Set From Y-Offset**: grabs the Y-Offset value from Offset Page.
- **Gravity Settings**: Provides a gravity and momentum simulation to dynamically move your space offset.
  - **On**: Current toggle state, Binds directly modify this.
  - **Gravity Strength**: Gravity simulation's downward acceleration in meters per second squared. Planet buttons provide quick settings for well known gravity strengths. Values can also be typed in directly.
  - **Save Momentum**: whether your momentum is saved between on/off toggles of gravity.
  - **fling Strength**: adjusts the strength at which you "throw" yourself with space drag feature.
- **Snap Turn**: Allows snap (instant) turning by the specified angle. Can type in values or use the preset buttons for angles that neatly divide 360 degrees. Must bind actions via SteamVR Input interface.

<a name="space_fix_page"></a>
## - Space Fix Page:

![Playspace Fix Page](docs/screenshots/FloorFixPage.png)

- **Fix Floor** Allows you to fix the height of your floor. Just place one controller on your floor and press the button.
- **Recenter Playspace** Besides fixing the floor height, also recenters the place space around the controller on the floor.
- **Undo Fix** Removes last "fix"
- **Apply Space Settings Offsets as Center** Takes current values from Offsets page and re-caliberates center/rotation. **Caution** The reset function will then refer to this location as the new zero location, overriding the old zero location.
  
<a name="audio_page"></a>
## - Audio Page:

![Audio Page](docs/screenshots/AudioPage.png)

- **Playback Device**: Allows to select the playback device.
- **Mirror Device**: Allows to select the mirror device, set its volume and to mute/unmute it.
- **Microphone**: Allows to select the microphone device, set its volume and to mute/unmute it.
- **Push-to-Talk**: Enable/disable push-to-talk. When push-to-talk is activated the microphone gets muted unless the Push To Talk action is activated.
- **Show Notification**: Shows a notification icon in the headset when the Push To Talk action is activated.
- **Push-to-Mute**: Inverse push-to-talk. The Microphone is unmuted by default and muted when the keybind is pressed.
  - **NOTE**: The Push-to-talk box must be enabled for this feature to work.
- **Audio Profile**: Allows you to apply/define/delete audio profiles that save playback devices, mute state, and volume.


<a name="utilities_page"></a>
## - Utilities Page:

![Utilities Page](docs/screenshots/UtilitiesPage.png)

- **Keyboard Utilities:** Some applications (mostly 2D-monitor applications with tackled-on/modded-in VR support) do not support the VR keyboard but require an actual physical keyboard for text input. This utilities allow to send emulated key strokes from VR keyboard input to such application to overcome this limitation. Does not work in desktop mode.

- **Alarm Clock:** Just a simple alarm clock so you don't miss important appointments. VR can sure mess up perception of time. Does not work in desktop mode.

- **Steam Desktop Overlay Width:** Allows to increase the size of the Steam desktop overlay. Useful for multi-monitor setups. It is saved between sessions.

- **Media Control Keys:** Allows controlling a media player through the media keys. This is the same as having a keyboard with media keys and then pressing them. Should support most common media players.

<a name="statistics_page"></a>
## - Statistics Page:

![Statistics Page](docs/screenshots/StatisticsPage.png)

- **HMD Distance Moved**: Shows the distance the headset has moved on the xz-plane.
- **HMD Rotations**: Shows the number of rotations around the y-axis (Useful for untangling the cord).
- **Left Controller Max Speed**: Shows the max speed of the left controller.
- **Right Controller Max Speed**: Shows the max speed of the right controller.
- **Presented Frames**: Number of frames presented to the hmd in the currently running application.
- **Dropped Frames**: Number of frames dropped in the currently running application.
- **Reprojected Frames**: Number of frames reprojected in the currently running application.
- **Timed Out**: Number of times the currently running application timed out.
- **Reprojection Ratio**: Ratio of presented frames to reprojected frames.

<a name="settings_page"></a>
## - Settings Page:

![Settings Page](docs/screenshots/SettingsPage.png)

- **Autostart:** Allows you to enable/disable auto start.
- **Force Revive Page:** No Longer Supported Does Nothing.

<a name="how_to_compile"></a>
# How to Compile

<a name="building_on_windows"></a>
## Building on Windows

Full build instructions can be found [here](docs/building_for_windows.md).

<a name="building_on_linux"></a>
## Building on Linux

Full build instructions can be found [here](docs/building_for_linux.md).

<a name="notes"></a>
# Notes:

- The center marker and the play space marker are provided by the chaperone subsystem, and therefore they will have the same color and visibility settings as the chaperone bounds.

- Application autostart can also be configured in the SteamVR menu (SteamVR->Settings->Applications->"Advanced Settings").

- You can modify the shutdown wait time when restarting SteamVR by opening restartvrserver.bat and changing the number behind "timeout /t" (unit is seconds).

<a name="license"></a>
# License

This software is released under GPL 3.0.
