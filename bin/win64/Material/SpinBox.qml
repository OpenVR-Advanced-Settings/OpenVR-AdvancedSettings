/****************************************************************************
**
** Copyright (C) 2016 The Qt Company Ltd.
** Contact: http://www.qt.io/licensing/
**
** This file is part of the Qt Quick Controls 2 module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL3$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see http://www.qt.io/terms-conditions. For further
** information use the contact form at http://www.qt.io/contact-us.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 3 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPLv3 included in the
** packaging of this file. Please review the following information to
** ensure the GNU Lesser General Public License version 3 requirements
** will be met: https://www.gnu.org/licenses/lgpl.html.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 2.0 or later as published by the Free
** Software Foundation and appearing in the file LICENSE.GPL included in
** the packaging of this file. Please review the following information to
** ensure the GNU General Public License version 2.0 requirements will be
** met: http://www.gnu.org/licenses/gpl-2.0.html.
**
** $QT_END_LICENSE$
**
****************************************************************************/

import QtQuick 2.6
import QtQuick.Templates 2.0 as T
import QtQuick.Controls.Material 2.0
import QtQuick.Controls.Material.impl 2.0

T.SpinBox {
    id: control

    implicitWidth: Math.max(background ? background.implicitWidth : 0,
                            contentItem.implicitWidth +
                            (up.indicator ? up.indicator.implicitWidth : 0) +
                            (down.indicator ? down.indicator.implicitWidth : 0))
    implicitHeight: Math.max(contentItem.implicitHeight + topPadding + bottomPadding,
                             background ? background.implicitHeight : 0,
                             up.indicator ? up.indicator.implicitHeight : 0,
                             down.indicator ? down.indicator.implicitHeight : 0)
    baselineOffset: contentItem.y + contentItem.baselineOffset

    spacing: 6
    topPadding: 8
    bottomPadding: 16
    leftPadding: (control.mirrored ? (up.indicator ? up.indicator.width : 0) : (down.indicator ? down.indicator.width : 0))
    rightPadding: (control.mirrored ? (down.indicator ? down.indicator.width : 0) : (up.indicator ? up.indicator.width : 0))

    //! [validator]
    validator: IntValidator {
        locale: control.locale.name
        bottom: Math.min(control.from, control.to)
        top: Math.max(control.from, control.to)
    }
    //! [validator]

    //! [contentItem]
    contentItem: TextInput {
        text: control.textFromValue(control.value, control.locale)

        font: control.font
        color: control.Material.primaryTextColor
        selectionColor: control.Material.textSelectionColor
        selectedTextColor: control.Material.primaryTextColor
        horizontalAlignment: Qt.AlignHCenter
        verticalAlignment: Qt.AlignVCenter
        cursorDelegate: Rectangle {
            id: cursor
            color: control.Material.accentColor
            width: 2
            visible: control.activeFocus && contentItem.selectionStart === contentItem.selectionEnd

            Connections {
                target: contentItem
                onCursorPositionChanged: {
                    // keep a moving cursor visible
                    cursor.opacity = 1
                    timer.restart()
                }
            }

            Timer {
                id: timer
                running: control.activeFocus
                repeat: true
                interval: Qt.styleHints.cursorFlashTime
                onTriggered: cursor.opacity = !cursor.opacity ? 1 : 0
                // force the cursor visible when gaining focus
                onRunningChanged: cursor.opacity = 1
            }
        }

        readOnly: !control.editable
        validator: control.validator
        inputMethodHints: Qt.ImhFormattedNumbersOnly
    }
    //! [contentItem]

    //! [up.indicator]
    up.indicator: PaddedRectangle {
        x: control.mirrored ? 0 : parent.width - width
        implicitWidth: 48
        implicitHeight: 48
        height: parent.height
        width: height
        padding: control.spacing
        radius: 3
        color: Qt.tint(Qt.tint(control.Material.buttonColor,
                               control.activeFocus ? control.Material.buttonHoverColor : "transparent"),
                               control.up.pressed ? control.Material.buttonPressColor: "transparent")

        Rectangle {
            x: (parent.width - width) / 2
            y: (parent.height - height) / 2
            width: Math.min(parent.width / 3, parent.width / 3)
            height: 2
            color: enabled ? control.Material.primaryTextColor : control.Material.spinBoxDisabledIconColor
        }
        Rectangle {
            x: (parent.width - width) / 2
            y: (parent.height - height) / 2
            width: 2
            height: Math.min(parent.width / 3, parent.width / 3)
            color: enabled ? control.Material.primaryTextColor : control.Material.spinBoxDisabledIconColor
        }
    }
    //! [up.indicator]

    //! [down.indicator]
    down.indicator: PaddedRectangle {
        x: control.mirrored ? parent.width - width : 0
        implicitWidth: 48
        implicitHeight: 48
        height: parent.height
        width: height
        padding: control.spacing
        radius: 3
        color: Qt.tint(Qt.tint(control.Material.buttonColor,
                               control.activeFocus ? control.Material.buttonHoverColor : "transparent"),
                               control.down.pressed ? control.Material.buttonPressColor : "transparent")

        Rectangle {
            x: (parent.width - width) / 2
            y: (parent.height - height) / 2
            width: parent.width / 3
            height: 2
            color: enabled ? control.Material.primaryTextColor : control.Material.spinBoxDisabledIconColor
        }
    }
    //! [down.indicator]

    //! [background]
    background: Item {
        implicitWidth: 192
        implicitHeight: 48

        Rectangle {
            x: parent.width / 2 - width / 2
            y: parent.y + parent.height - height - control.bottomPadding / 2
            width: control.availableWidth
            height: control.activeFocus ? 2 : 1
            color: control.activeFocus ? control.Material.accentColor : control.Material.hintTextColor
        }
    }
    //! [background]
}
