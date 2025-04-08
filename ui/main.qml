import QtQuick 2.12
import QtQuick.Controls 2.5
import QtQuick.Layouts 1.12
import QtQuick.Controls.Material 2.5

ApplicationWindow {
    id: applicationWindow
    visible: true
    width: 900
    height: 500
    title: "Poorman's UI"
    Material.theme: Material.Light

    property var editedRows: ({})  // JavaScript object to track changes

    ColumnLayout {
        anchors.fill: parent
        spacing: 5

        // ✅ MenuBar at the Top
        MenuBar {
            Menu {
                title: "Connection"
                MenuItem {
                    text: "Connect"
                    onTriggered: connectDialog.open()
                }
                MenuItem {
                    text: "Disconnect"
                    onTriggered: signalModel.disconnectFromServer()
                }
            }
        }

        // ✅ Search Bar (Aligned Properly)
        TextField {
            id: searchField
            Layout.fillWidth: true
            placeholderText: "Search by Name"
            onTextChanged: signalModel.filterData(searchField.text)
            padding: 10
            font.pixelSize: 18
        }

        // ✅ Header Row (Aligned Properly)
        Rectangle {
            Layout.fillWidth: true
            height: 40
            color: "#B0BEC5"  // Light Gray
            border.color: "#909090"

            RowLayout {
                anchors.fill: parent
                spacing: 0

                Text { Layout.preferredWidth: parent.width * 0.10; text: "Index"; font.bold: true; horizontalAlignment: Text.AlignHCenter }
                Text { Layout.preferredWidth: parent.width * 0.30; text: "VSS Parameter"; font.bold: true; horizontalAlignment: Text.AlignHCenter }
                Text { Layout.preferredWidth: parent.width * 0.20; text: "Mode"; font.bold: true; horizontalAlignment: Text.AlignHCenter }
                Text { Layout.preferredWidth: parent.width * 0.20; text: "Period (ms)"; font.bold: true; horizontalAlignment: Text.AlignHCenter }
                Text { Layout.preferredWidth: parent.width * 0.20; text: "Value"; font.bold: true; horizontalAlignment: Text.AlignHCenter }
            }
        }

        // ✅ Scrollable Table (Fixed Alignment)
        ScrollView {
            Layout.fillWidth: true
            Layout.fillHeight: true

            ListView {
                id: tableView
                width: parent.width
                spacing: 2
                model: signalModel

                delegate: Rectangle {
                    width: tableView.width
                    height: 40
                    color: model.index % 2 === 0 ? "#ECEFF1" : "white"
                    border.color: "blue"
                    property string signalName: model.name  // ✅ Explicitly bind the model's name

                    RowLayout {
                        anchors.fill: parent
                        spacing: 0

                        Text { Layout.preferredWidth: parent.width * 0.10; text: (model.index + 1).toString(); horizontalAlignment: Text.AlignHCenter }
                        Text { Layout.preferredWidth: parent.width * 0.30; text: model.name; horizontalAlignment: Text.AlignHCenter }

                        ComboBox {
                            id: modeCombo
                            Layout.preferredWidth: parent.width * 0.20
                            model: ["Auto", "Override"]
                            currentIndex: model.mode === "Override" ?  1 : 0

                            onCurrentIndexChanged:{
                                periodField.enabled = (currentIndex === 1);

                                signalModel.updateMode(signalName, currentIndex === 0 ? "Auto" : "Override")
                            }
                        }

                        // ✅ Period Field (Disabled when Mode is Auto)
                        TextField {
                            id: periodField
                            Layout.preferredWidth: parent.width * 0.20
                            text: model.period
                            enabled: modeCombo.currentIndex === 1  // Enable when Mode is "Override"
                            onEditingFinished: {
                                signalModel.updatePeriod(model.name, text)
                                editedRows[model.index] = { "name": signalName}
                            }
                        }

                        TextField {
                            id: valueField
                            Layout.preferredWidth: parent.width * 0.20
                            text: model.value
                            onEditingFinished: {
                                signalModel.updateValue(model.name, text)
                                editedRows[model.index] = { "name": signalName}
                            }
                        }
                    }
                }
            }
        }

        // ✅ Send Button (Centered & Fixed Position)
        Button {
            id: sendButton
            text: "Send"
            Layout.alignment: Qt.AlignHCenter
            Layout.preferredWidth: 200
            Layout.preferredHeight: 40

            onClicked: {
                var dataToSend = [];
                for (var key in applicationWindow.editedRows) {
                    var entry = applicationWindow.editedRows[key];
                    dataToSend.push(entry.name);
                }

                if (dataToSend.length > 0) {
                    signalModel.sendData(dataToSend);
                    applicationWindow.editedRows = {};  // ✅ Clear after sending
                }
            }
        }
    }

    // ✅ Server IP Dialog
    Dialog {
        id: connectDialog
        title: "Enter Server IP"
        modal: true
        standardButtons: Dialog.Cancel | Dialog.Ok
        closePolicy: Popup.CloseOnEscape

        ColumnLayout {
            spacing: 10
            width: parent.width
            RowLayout {
                spacing: 5
                Label { text: "Server IP" }
                TextField {
                    id: ipInput
                    Layout.fillWidth: true
                    placeholderText: "Enter IP"
                }
            }
        }

        onAccepted: signalModel.connectToServer(ipInput.text)
    }
}
