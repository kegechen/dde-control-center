// SPDX-FileCopyrightText: 2024 - 2027 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later
import QtQuick 2.15
import QtQuick.Controls 2.3
import org.deepin.dcc 1.0

DccObject {
    DccObject {
        name: "KeyboardLayoutAdder"
        parentName: "KeyboardLayout"
        displayName: qsTr("Keyboard layout")
        description: qsTr("Select current keyboard layout")
        weight: 10
        hasBackground: true
        pageType: DccObject.Editor
        page: Button {
            implicitWidth: 80
            text: qsTr("add")
        }
    }

    DccObject {
        name: "KeyboardLayoutRepeater"
        parentName: "KeyboardLayout"
        weight: 20
        pageType: DccObject.Item
        page: DccGroupView {

        }

        model: 3
        delegate: DccObject {
            name: "KeyboardLayoutItem" + index
            parentName: "KeyboardLayoutRepeater"
            weight: 20 + index * 10
            pageType: DccObject.Item
            hasBackground: true
            page: Label {
                font.pointSize: 12
                text: "keyboard layout" + index
            }
        }
    }
}
