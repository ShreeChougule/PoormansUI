import QtQuick 2.12
import QtQuick.Controls 2.5
import QtQuick.Layouts 1.12
import QtQuick.Controls.Material 2.5

ApplicationWindow {
    id: applicationWindow
    visible: true
    width: 800
    height: 400
    title: "Signal Table from SQLite"
    Material.theme: Material.Light

    property var editedRows: ({})  // JavaScript object to track changes
    Column {
        id: column1
        anchors.fill: parent

        // ✅ Manually creating a MenuBar at the top
        MenuBar {
            Menu {
                title: "Connection"
                MenuItem {
                    text: "Connect"
                    onTriggered: connectDialog.open()
                }
                MenuItem {
                    text: "Disconnect"
                    onTriggered: {
                        console.log("Disconnecting");
                        signalModel.disconnectFromServer();  // ✅ Call SignalModel function
                    }
                }
            }
        }


        Column {
            id: column
            anchors.topMargin: 30
            anchors.fill: parent
            spacing: 5

        // Search Bar
        TextField {
            id: searchField
            width: parent.width
            anchors.top: parent.top
            anchors.topMargin: 0
            placeholderText: "Search by Name"
            onTextChanged: signalModel.filterData(searchField.text)
            padding: 10
            font.pixelSize: 18
        }

        // Header Row
        Row {
            id: headerField
            width: parent.width
            height: 40
            anchors.top: searchField.bottom
            anchors.topMargin: 5
            spacing: 2

            Text { width: parent.width / 6; text: "Index"; font.bold: true; horizontalAlignment: Text.AlignHCenter }
            Text { width: parent.width / 5; text: "VSS Parameter"; font.bold: true; horizontalAlignment: Text.AlignHCenter }
            Text { width: parent.width / 5; text: "Mode"; font.bold: true; horizontalAlignment: Text.AlignHCenter }
            Text { width: parent.width / 5; text: "Period (ms)"; font.bold: true; horizontalAlignment: Text.AlignHCenter }
            Text { width: parent.width / 5; text: "Value"; font.bold: true; horizontalAlignment: Text.AlignHCenter }
        }

        // Scrollable Table View (Reduced Height to Fit Button)
        ScrollView {
            id: scrollView
            anchors.right: parent.right
            anchors.rightMargin: 0
            anchors.left: parent.left
            anchors.leftMargin: 0
            anchors.bottom: sendButton.top
            anchors.bottomMargin: 40
            anchors.top: headerField.bottom
            anchors.topMargin: 5

            ListView {
                id: tableView
                width: parent.width
                spacing: 2
                model: signalModel

                delegate: Rectangle {
                    width: tableView.width
                    height: 40
                    border.color: "blue"
                    color: model.index % 2 === 0 ? "lightgray" : "white"

                    Row {
                        anchors.fill: parent

                        Text { width: parent.width / 6; text: (model.index + 1).toString(); horizontalAlignment: Text.AlignHCenter }
                        Text { width: parent.width / 5; text: model.name; horizontalAlignment: Text.AlignHCenter }
                        // Mode ComboBox (Override / Auto)
                        ComboBox {
                            id: modeCombo
                            width: parent.width / 5
                            anchors.verticalCenter: parent.verticalCenter
                            model: ["Override", "Auto"]
                            currentIndex: model.mode === "Auto" ? 1 : 0 // Set initial value

                            onCurrentIndexChanged: {
                                periodField.enabled = (currentIndex === 1); // ✅ Enable Period when "Auto" is selected
                            }
                        }

                        // Period Column (Track Changes Only if Enabled)
                        TextField {
                            id: periodField
                            width: parent.width / 5
                            anchors.verticalCenter: parent.verticalCenter
                            text: model.period
                            enabled: modeCombo.currentIndex === 1
                            onEditingFinished: {
                                if (enabled && text !== model.period) {  // ✅ Store only if modified & in Auto mode
                                    applicationWindow.editedRows[model.name] = { name: model.name, value: text };
                                    console.log("Edited Rows Updated:", JSON.stringify(applicationWindow.editedRows));
                                }
                            }
                        }

                        // Value Column (Track Changes)
                        TextField {
                            id: valueField
                            width: parent.width / 5
                            anchors.verticalCenter: parent.verticalCenter
                            text: model.value
                            onEditingFinished: {
                                if (text !== model.value) {  // ✅ Store only if modified
                                    applicationWindow.editedRows[model.name] = { name: model.name, value: text };
                                    console.log("Edited Rows Updated:", JSON.stringify(applicationWindow.editedRows));
                                }
                            }
                        }
                    }

                }
            }
        }

        Button {
            id: sendButton
            text: "Send"
            anchors.bottom: parent.bottom
            anchors.bottomMargin: 0
            width: parent.width * 0.2
            anchors.horizontalCenter: parent.horizontalCenter

            onClicked: {
                var dataToSend = [];
                for (var key in applicationWindow.editedRows) {
                    var entry = applicationWindow.editedRows[key];
                    dataToSend.push(entry.name + " " + entry.value);
                }

                if (dataToSend.length > 0) {
                    console.log("Sending data:", dataToSend);
                    signalModel.sendData(dataToSend);

                    // ✅ Clear editedRows after sending data
                    applicationWindow.editedRows = {};
                    console.log("Edited rows cleared:", JSON.stringify(applicationWindow.editedRows));
                } else {
                    console.log("No changes detected.");
                }
            }
        }



        // ✅ Server IP Popup Dialog
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

            onAccepted: {
                console.log("Connecting to IP:", ipInput.text);
                signalModel.connectToServer(ipInput.text);  // ✅ Call SignalModel function
            }
        }
    }

    }
}


/*##^##
Designer {
    D{i:13;anchors_height:199;anchors_width:800}
}
##^##*/
