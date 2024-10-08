// SPDX-FileCopyrightText: 2024 - 2027 UnionTech Software Technology Co., Ltd.
// SPDX-License-Identifier: GPL-3.0-or-later
import QtQuick
import QtQuick.Controls 2.3
import QtQuick.Layouts
import org.deepin.dcc 1.0
import org.deepin.dtk 1.0 as D
import org.deepin.dtk.style 1.0 as DS

DccObject {
    id: shortcutSettingsView
    name: "shortcutSettingsView"
    parentName: "Keyboard"
    displayName: qsTr("Shortcuts")
    icon: "dcc_nav_keyboard" // TODO: shortcut icon required
    weight: parent.weight // 300
    hasBackground: false
    page: DccSettingsView {
    }

    DccObject {
        id: shortcutSettingsBody
        property bool isEditing: false
        name: "shortcutSettingsBody"
        parentName: "shortcutSettingsView"
        displayName: qsTr("Shortcuts body")
        weight: 30
        hasBackground: false
        pageType: DccObject.Item

        page: ColumnLayout {
            spacing: 10
            Timer {
                id: timer
                interval: 100
                onTriggered: {
                    shortcutView.model.setFilterWildcard(searchEdit.text);
                }
            }

            D.SearchEdit {
                id: searchEdit
                // Layout.leftMargin: 10
                // Layout.rightMargin: 10
                Layout.topMargin: 10
                Layout.alignment: Qt.AlignHCenter
                // implicitWidth: 280
                Layout.fillWidth: true
                placeholder: qsTr("Search shortcuts")
                onTextChanged: {
                    timer.start()
                }
                onEditingFinished: {
                    timer.start()
                }
                Component.onCompleted: {
                    // clear
                    shortcutView.model.setFilterWildcard("");
                }
            }

            ListView {
                id: shortcutView
                property Item editItem
                property Item conflictText
                property string conflictAccels
                clip: true
                interactive: false // 外层有滚动了，listview 就别滚了
                implicitHeight: contentHeight
                implicitWidth: 600
                Layout.fillWidth: true

                model: dccData.shortcutSearchModel()

                section.property: "section"
                section.criteria: ViewSection.FullString
                section.delegate: RowLayout {
                    width: ListView.view.width
                    height: childrenRect.height

                    required property string section

                    Label {
                        text: parent.section
                        font.bold: true
                        font.pointSize: 13
                        leftPadding: 20
                        bottomPadding: 10
                        topPadding: 16
                    }

                    D.Button {
                        id: button
                        visible: parent.section === qsTr("Custom")
                        checkable: true
                        checked: shortcutSettingsBody.isEditing
                        Layout.alignment: Qt.AlignRight | Qt.AlignVCenter
                        Layout.rightMargin: 10
                        text: shortcutSettingsBody.isEditing ? qsTr("done") : qsTr("edit")
                        font.pointSize: 13
                        background: null
                        textColor: D.Palette {
                            normal {
                                common: D.DTK.makeColor(D.Color.Highlight)
                                crystal: D.DTK.makeColor(D.Color.Highlight)
                            }
                        }
                        onCheckedChanged: {
                            shortcutSettingsBody.isEditing = button.checked

                            if (!shortcutView.editItem)
                                return
                            shortcutView.editItem.keys = shortcutView.editItem.keySequence
                            shortcutView.editItem.focus = false
                            shortcutView.conflictText.visible = false
                            shortcutView.editItem = null
                            shortcutView.conflictText = null
                        }
                    }
                }

                delegate: ItemDelegate {
                    id: editorDelegate
                    checkable: false
                    // item: shortcutSettingsBody
                    implicitWidth: ListView.view.width
                    topInset: 0
                    bottomInset: 0
                    // separatorVisible: true
                    backgroundVisible: true
                    Layout.fillWidth: true
                    corners: model.corners

                    background: DccListViewBackground {
                        id: background
                        separatorVisible: true
                    }

                    contentItem: ColumnLayout {
                        RowLayout {
                            id: editLayout
                            visible: shortcutSettingsBody.isEditing && model.isCustom
                            D.IconButton {
                                id: editButton
                                property bool needShowDialog: false
                                icon.name: "edit"
                                icon.width: 24
                                icon.height: 24
                                implicitWidth: 36
                                implicitHeight: 36
                                background: null
                                onClicked: {
                                    console.log("need edit shortcut")
                                    editButton.needShowDialog = true
                                }

                                Loader {
                                    id: dialogloader
                                    active: editButton.needShowDialog
                                    sourceComponent: ShortcutSettingDialog {
                                        onClosing: {
                                            editButton.needShowDialog = false
                                            shortcutSettingsBody.isEditing = false
                                        }
                                    }
                                    onLoaded: {
                                        item.keyId = model.id
                                        item.keyName = model.display
                                        item.cmdName = model.command
                                        item.keySequence = model.keySequence
                                        item.accels = model.accels

                                        // console.log("prepare....:", item.keyId, item.keyName, item.cmdName, item.keySequence)
                                        item.show()
                                    }
                                }
                            }
                            D.IconButton {
                                id: removeButton
                                icon.name: "user-trash-symbolic"
                                icon.width: 24
                                icon.height: 24
                                implicitWidth: 36
                                implicitHeight: 36
                                background: null
                                onClicked: {
                                    console.log("need remove shortcut", index)
                                    dccData.deleteCustomShortcut(index)
                                }
                            }
                        }

                        KeySequenceDisplay {
                            id: edit
                            property var keySequence: model.keySequence
                            visible: !editLayout.visible
                            text: model.display
                            keys: keySequence
                            placeholderText: qsTr("Please enter a new shortcut")
                            background.visible: conflictText.visible
                            backgroundColor: conflictText.visible ? DS.Style.edit.alertBackground : DS.Style.keySequenceEdit.background
                            Layout.alignment: Qt.AlignRight
                            Layout.bottomMargin:  conflictText.visible ? 8 : 0
                            Layout.fillWidth: true
                            Keys.forwardTo: parent
                            onRequestKeys: {
                                if (shortcutView.editItem) {
                                    shortcutView.editItem.restore()
                                    return
                                }

                                edit.keys = ""
                                dccData.updateKey(model.id)
                                shortcutView.editItem = edit
                                shortcutView.conflictText = conflictText
                                shortcutSettingsBody.isEditing = false
                            }

                            D.IconButton {
                                id: warnningBtn
                                flat: true
                                background: null
                                visible: model.accels.length > 0 && shortcutView.conflictAccels === model.accels
                                icon.name: "icon_info"
                                icon.width: 32
                                icon.height: 32
                                anchors.right: edit.right
                                anchors.verticalCenter: edit.verticalCenter
                                anchors.rightMargin: edit.iconPadding()
                            }

                            function iconPadding() {
                                let keySequenceLabels = edit.contentItem.children[1]
                                return keySequenceLabels.width + 10
                            }

                            function modifyShortcut(accels) {
                                if (accels.length > 0)
                                    dccData.modifyShortcut(model.id, accels)
                            }

                            function restore() {
                                edit.keys = edit.keySequence
                                edit.focus = false
                                conflictText.visible = false

                                shortcutView.conflictAccels = ""
                                shortcutView.editItem = null
                                shortcutView.conflictText = null
                            }
                        }

                        Item {
                            id: conflictText
                            visible: false
                            implicitHeight: 20
                            implicitWidth: row.implicitWidth
                            // Layout.fillWidth: true
                            Layout.alignment: Qt.AlignRight
                            Layout.rightMargin: 20

                            Row {
                                id: row
                                anchors.fill: parent
                                spacing: 3
                                Text {
                                    text: dccData.conflictText + ","
                                }
                                Text {
                                    text: qsTr("Click")
                                }
                                Text {
                                    text: qsTr("Cancel")
                                    color: palette.highlight
                                    MouseArea {
                                        anchors.fill: parent
                                        onClicked: {
                                            edit.restore()
                                            // shortcutView.conflictAccels = ""
                                        }
                                    }
                                }
                                Text {
                                    text: qsTr("or")
                                }
                                Text {
                                    text: qsTr("Replace")
                                    color: palette.highlight
                                    MouseArea {
                                        anchors.fill: parent
                                        onClicked: {
                                            // console.log("Replace....clicked.....", "id", model.id, model.keySequence, "==>", shortcutView.conflictAccels)
                                            edit.modifyShortcut(shortcutView.conflictAccels)
                                            shortcutView.conflictAccels = ""
                                        }
                                    }
                                }
                            }
                        }

                    }
                }

                Connections {
                    target: dccData
                    function onRequestRestore() {
                        if (!shortcutView.editItem)
                            return
                        shortcutView.editItem.restore()

                        // shortcutView.editItem = null
                        // shortcutView.conflictText = null
                    }
                    function onRequestClear() {
                        onRequestRestore()
                    }
                    function onKeyConflicted(oldAccels, newAccels) {
                        if (!shortcutView.editItem)
                            return
                        shortcutView.conflictText.visible = true
                        shortcutView.conflictAccels = newAccels
                    }
                    function onKeyDone(accels) {
                        if (!shortcutView.editItem)
                            return
                        shortcutView.editItem.focus = false
                        shortcutView.editItem.keys = dccData.formatKeys(accels)
                        shortcutView.conflictText.visible = false

                        shortcutView.editItem.modifyShortcut(accels)

                        shortcutView.editItem = null
                        shortcutView.conflictText = null

                    }
                    function onKeyEvent(accels) {
                        if (!shortcutView.editItem)
                            return

                        shortcutView.editItem.focus = false
                        shortcutView.editItem.keys = dccData.formatKeys(accels)
                        shortcutView.conflictText.visible = false
                    }
                }
            }
        }
    }

    DccObject {
        name: "bottomAreaFoot"
        parentName: "shortcutSettingsView"
        displayName: qsTr("Shortcuts bottom area view1")
        weight: 40
        hasBackground: false
        pageType: DccObject.Item

        DccObject {
            name: "bottomAreaButton1"
            parentName: "bottomAreaFoot"
            pageType: DccObject.Item
            page: Button {
                text: qsTr("Restore default")
                onClicked: {
                    console.log("restore button clicked...")
                    dccData.resetAllShortcuts()
                }
            }
        }

        DccObject {
            name: "bottomAreaSpacer"
            parentName: "bottomAreaFoot"
            pageType: DccObject.Item
            page: Item {
                Layout.fillWidth: true
            }
        }

        DccObject {
            name: "bottomAreaButton2"
            parentName: "bottomAreaFoot"
            pageType: DccObject.Item
            page: Button {
                id: addButton
                property bool needShowDialog: false
                text: qsTr("Add custom shortcut")

                Loader {
                    id: loader
                    active: addButton.needShowDialog
                    sourceComponent: ShortcutSettingDialog {
                        id: shortcutSettingDialog
                        onClosing: {
                            addButton.needShowDialog = false
                        }
                    }
                    onLoaded: {
                        item.show()
                    }
                }

                onClicked: {
                    addButton.needShowDialog = true
                }
            }
        }
    }
}
