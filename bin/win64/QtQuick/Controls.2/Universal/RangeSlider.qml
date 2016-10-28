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
import QtQuick.Controls.Universal 2.0

T.RangeSlider {
    id: control

    implicitWidth: Math.max(background ? background.implicitWidth : 0,
        Math.max(background ? background.implicitWidth : 0,
            first.handle ? first.handle.implicitWidth : 0,
                second.handle ? second.handle.implicitWidth : 0) + leftPadding + rightPadding)
    implicitHeight: Math.max(background ? background.implicitHeight : 0,
        Math.max(background ? background.implicitHeight : 0,
            first.handle ? first.handle.implicitHeight : 0,
                second.handle ? second.handle.implicitHeight : 0) + topPadding + bottomPadding)

    padding: 6

    //! [firstHandle]
    first.handle: Rectangle {
        implicitWidth: horizontal ? 8 : 24
        implicitHeight: horizontal ? 24 : 8

        readonly property bool horizontal: control.orientation === Qt.Horizontal

        x: control.leftPadding + (horizontal ? control.first.visualPosition * (control.availableWidth - width) : (control.availableWidth - width) / 2)
        y: control.topPadding + (horizontal ? (control.availableHeight - height) / 2 : control.first.visualPosition * (control.availableHeight - height))

        radius: 4
        color: control.first.pressed ? control.Universal.chromeHighColor : control.enabled ? control.Universal.accent : control.Universal.chromeDisabledHighColor
    }
    //! [firstHandle]

    //! [secondHandle]
    second.handle: Rectangle {
        implicitWidth: horizontal ? 8 : 24
        implicitHeight: horizontal ? 24 : 8

        readonly property bool horizontal: control.orientation === Qt.Horizontal

        x: control.leftPadding + (horizontal ? control.second.visualPosition * (control.availableWidth - width) : (control.availableWidth - width) / 2)
        y: control.topPadding + (horizontal ? (control.availableHeight - height) / 2 : control.second.visualPosition * (control.availableHeight - height))

        radius: 4
        color: control.second.pressed ? control.Universal.chromeHighColor : control.enabled ? control.Universal.accent : control.Universal.chromeDisabledHighColor
    }
    //! [secondHandle]

    //! [background]
    background: Item {
        implicitWidth: horizontal ? 200 : 18
        implicitHeight: horizontal ? 18 : 200

        readonly property bool horizontal: control.orientation === Qt.Horizontal

        x: control.leftPadding + (horizontal ? 0 : (control.availableWidth - width) / 2)
        y: control.topPadding + (horizontal ? (control.availableHeight - height) / 2 : 0)
        width: horizontal ? control.availableWidth : implicitWidth
        height: horizontal ? implicitHeight : control.availableHeight

        scale: horizontal && control.mirrored ? -1 : 1

        Rectangle {
            x: parent.horizontal ? 0 : (parent.width - width) / 2
            y: parent.horizontal ? (parent.height - height) / 2 : 0
            width: parent.horizontal ? parent.width : 2 // SliderBackgroundThemeHeight
            height: !parent.horizontal ? parent.height : 2 // SliderBackgroundThemeHeight

            color: control.enabled ? control.Universal.baseMediumLowColor : control.Universal.chromeDisabledHighColor
        }

        Rectangle {
            x: parent.horizontal ? control.first.position * parent.width : (parent.width - width) / 2
            y: parent.horizontal ? (parent.height - height) / 2 : control.second.visualPosition * parent.height
            width: parent.horizontal ? control.second.position * parent.width - control.first.position * parent.width : 2 // SliderBackgroundThemeHeight
            height: !parent.horizontal ? control.second.position * parent.height - control.first.position * parent.height : 2 // SliderBackgroundThemeHeight

            color: control.enabled ? control.Universal.accent : control.Universal.chromeDisabledHighColor
        }
    }
    //! [background]
}
