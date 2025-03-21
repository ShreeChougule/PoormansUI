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
        anchors.fill: parent
        spacing: 5

        // Search Bar
        TextField {
            id: searchField
            width: parent.width
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
            width: parent.width
            height: parent.height - headerField.height - searchField.height - sendButton.height - 20

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
                        // Period (Editable only when "Auto" is selected)
                        TextField {
                            id: periodField
                            width: parent.width / 5
                            anchors.verticalCenter: parent.verticalCenter
                            text: model.period
                            enabled: modeCombo.currentIndex === 1  // ✅ Initially enable only if "Auto" is selected
                        }                        // Editable Value Column
                        TextField {
                            id: valueField
                            width: parent.width / 5
                            anchors.verticalCenter: parent.verticalCenter
                            text: model.value  // Bind to value column

                            onTextChanged: {
                                editedRows[model.index] = { "name": model.name, "value": valueField.text };
                            }
                        }
                    }

                }
            }
        }

        Button {
            id: sendButton
            text: "Send"
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
                } else {
                    console.log("No changes detected.");
                }
            }
        }


    }
}

