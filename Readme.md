
![language](https://img.shields.io/badge/Language-C%2B%2B11-green.svg) ![dependencies](https://img.shields.io/badge/Dependencies-OpenVR%2C%20Qt5-green.svg) ![license_gpl3](https://img.shields.io/badge/License-GPL%203.0-green.svg)

# OpenVR Advanced Settings Overlay

Adds an overlay to the OpenVR dashboard that allows access to advanced settings.

# Features
## - SteamVR Settings Tab:

- **Supersampling**: Allows to change SteamVR's supersampling setting (Requires restart).
- **Allow Reprojection**: Enable/disable reprojection (Requires restart).
- **Force Reprojection**: Enable/disable forced reprojection (Requires restart).
- **Restart SteamVR**: Restart SteamVR.

## - Chaperone Settings Tab:

- **Center Marker**: Displays a marker that represents the center of the playspace.
- **Play Space Maker**: Displays the outlines of the rectangle representing the playspace.
- **Force Bounds**: Force chaperone bounds always on.
- **Flip Orientation**: Flips the orientation of the play space.
- **Fade Distance**: Allows to configure the distance at which the chaperone bounds are shown. When set to 0 chaperone bounds are completely invisible.
- **Chaperone Bounds Visibility**: Allows to configure the visibility of the chaperone bounds. Unlike the slider in the chaperone settings, this one is not capped at 30%. When set to 0 chaperone bounds are completely invisible.
- **Reload from Disk**: Reloads the chaperone bounds from disk.

## - Move Center Tab:

Allows to temporarily move and rotate the center of the playspace. This allows to reach interaction elements that are just inside our real-world walls or otherwise inaccessible (e.g. when your playspace is smaller than the recommended one). Can also be used to discover the terrors that lie outside of the intended playspace (ever wondered what's behind the door in The Lab?).

The chaperone bounds stay in place when the playspace is moved or rotated (I don't want anyone to get hurt. Unfortunately this does not work when moving up/down).

## - Fix Floor Tab:

Allows you to fix the height of your floor. Just place one controller on your floor and press the button.

## - Statistics Tab:

- **HMD Distance Moved**: Shows the distance the headset has moved on the xz-plane.
- **HMD Rotations**: Shows the number of rotations around the y-axis (Useful for untangling the cord).
- **Left Controller Max Speed**: Shows the max speed of the left controller.
- **Right Controller Max Speed**: Shows the max speed of the right controller.
- **Dropped Frames**: Number of frames dropped in the currently running application.
- **Reprojected Frames**: Number of frames reprojected in the currently running application.
- **Reprojected Frames**: Number of times the currently running application timed out.

## - Settings Tab:

- **Auto Start Toggle:** Allows you to enable/disable auto start.

# Notes:

- The center marker and the play space marker are provided by the chaperone subsystem, and therefore they will have the same color and visibility settings as the chaperone bounds.

- When you have installed SteamVR to a different path than the default one, "Restart SteamVR" will not work. To resolve the issue go to the application directory, open the file restartvrserver.bat and change the path to wherever you have SteamVR installed.

# Usage

Just start the executable once. It will register with OpenVR and automatically start whenever OpenVR starts (Can be disabled in the settings).

# Known Bugs

- Center Marker and Playspace Marker are sometimes not working in the SteamVR room. My observation is that you have to start an application first before they are shown in the SteamVR room.

# License

This software is released under GPL 3.0.
