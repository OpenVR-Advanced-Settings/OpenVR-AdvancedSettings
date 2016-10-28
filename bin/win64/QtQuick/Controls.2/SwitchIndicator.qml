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

Item {
    implicitWidth: 56
    implicitHeight: 28

    property Item control

    Rectangle {
        y: parent.height / 2 - height / 2
        width: 56
        height: 16
        radius: 8
        opacity: control.enabled ? 1 : 0.3
        color: control.checked ? (control.visualFocus ? "#0066ff" : "#353637") : "#e0e0e0"
        border.width: control.visualFocus ? 2 : 0
        border.color: "#0066ff"
    }

    Rectangle {
        x: Math.max(0, Math.min(parent.width - width, control.visualPosition * parent.width - (width / 2)))
        y: (parent.height - height) / 2
        width: 28
        height: 28
        radius: 16
        color: control.enabled ? (control.down ? (control.visualFocus ? "#cce0ff" : "#f6f6f6") : (control.visualFocus ? "#f0f6ff" : "#ffffff")) : "#fdfdfd"
        border.width: control.visualFocus ? 2 : 1
        border.color: control.enabled ? (control.visualFocus ? "#0066ff" : (control.down ? "#808080" : "#909090")) : "#d6d6d6"

        Behavior on x {
            enabled: !control.down
            SmoothedAnimation { velocity: 200 }
        }
    }
}
