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
        x: control.leftPadding + (horizontal ? control.first.visualPosition * (control.availableWidth - width) : (control.availableWidth - width) / 2)
        y: control.topPadding + (horizontal ? (control.availableHeight - height) / 2 : control.first.visualPosition * (control.availableHeight - height))
        implicitWidth: 28
        implicitHeight: 28
        radius: width / 2
        border.width: activeFocus ? 2 : 1
        border.color: control.enabled ? (activeFocus ? "#0066ff" : (control.first.pressed ? "#808080" : "#909090")) : "#d6d6d6"
        color: control.enabled ? (first.pressed ? (activeFocus ? "#cce0ff" : "#f6f6f6") : (activeFocus ? "#f0f6ff" : "#ffffff")) : "#fdfdfd"

        readonly property bool horizontal: control.orientation === Qt.Horizontal
    }
    //! [firstHandle]

    //! [secondHandle]
    second.handle: Rectangle {
        x: control.leftPadding + (horizontal ? control.second.visualPosition * (control.availableWidth - width) : (control.availableWidth - width) / 2)
        y: control.topPadding + (horizontal ? (control.availableHeight - height) / 2 : control.second.visualPosition * (control.availableHeight - height))
        implicitWidth: 28
        implicitHeight: 28
        radius: width / 2
        border.width: activeFocus ? 2 : 1
        border.color: control.enabled ? (activeFocus ? "#0066ff" : (control.first.pressed ? "#808080" : "#909090")) : "#d6d6d6"
        color: control.enabled ? (second.pressed ? (activeFocus ? "#cce0ff" : "#f6f6f6") : (activeFocus ? "#f0f6ff" : "#ffffff")) : "#fdfdfd"

        readonly property bool horizontal: control.orientation === Qt.Horizontal
    }
    //! [secondHandle]

    //! [background]
    background: Rectangle {
        x: control.leftPadding + (horizontal ? 0 : (control.availableWidth - width) / 2)
        y: control.topPadding + (horizontal ? (control.availableHeight - height) / 2 : 0)
        implicitWidth: horizontal ? 200 : 6
        implicitHeight: horizontal ? 6 : 200
        width: horizontal ? control.availableWidth : implicitWidth
        height: horizontal ? implicitHeight : control.availableHeight
        radius: 3
        opacity: control.enabled ? 1 : 0.3
        color: "#e0e0e0"
        scale: horizontal && control.mirrored ? -1 : 1

        readonly property bool horizontal: control.orientation === Qt.Horizontal
    }
    //! [background]
}
