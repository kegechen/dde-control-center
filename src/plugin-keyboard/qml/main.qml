// SPDX-FileCopyrightText: 2024 UnionTech Software Technology Co., Ltd.
// SPDX-License-Identifier: GPL-3.0-or-later
import QtQuick 2.15
import QtQuick.Controls 2.3

import org.deepin.dcc 1.0

DccObject {
    DccObject {
        name: "KeyboardCommon"
        parentName: "Keyboard"
        displayName: qsTr("Common")
        weight: 10
        pageType: DccObject.Item
        page: DccGroupView {
            spacing: 5
            isGroup: false
            height: implicitHeight + 20
        }
        Common {}
    }
    DccObject {
        name: "KeyboardLayout"
        parentName: "Keyboard"
        displayName: qsTr("Keyboard layout")
        icon: "dcc_nav_keyboard"
        weight: 100
        page: DccRightView {
            spacing: 5
        }
        KeyboardLayout {}
    }
    DccObject {
        name: "InputMethod"
        parentName: "Keyboard"
        displayName: qsTr("Input method")
        icon: "dcc_nav_keyboard"
        weight: 200
        page: DccRightView {
            spacing: 5
        }
    }
    DccObject {
        name: "Shortcuts"
        parentName: "Keyboard"
        displayName: qsTr("Shortcuts")
        icon: "dcc_nav_keyboard"
        weight: 300
        page: DccRightView {
            spacing: 5
        }
    }
}
