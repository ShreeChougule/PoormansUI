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

    Column {
        anchors.fill: parent

        // Search Bar for dynamic search
        TextField {
            id: searchField
            width: parent.width
            placeholderText: "Search by Name"
            onTextChanged: {
                signalModel.filterData(searchField.text);  // Call filter function in C++
            }
            padding: 10
            font.pixelSize: 18
        }

        // Custom header row (fixed at the top)
        Row {
            width: parent.width
            height: 40
            spacing: 2

            // Index header (narrower than others)
            Text {
                width: parent.width / 6  // Adjusted width for index
                anchors.verticalCenter: parent.verticalCenter
                text: "Index"
                font.bold: true
                horizontalAlignment: Text.AlignHCenter
            }

            // VSS Parameter header
            Text {
                width: parent.width / 5
                anchors.verticalCenter: parent.verticalCenter
                text: "VSS Parameter"
                font.bold: true
                horizontalAlignment: Text.AlignHCenter
            }

            // Mode header
            Text {
                width: parent.width / 5
                anchors.verticalCenter: parent.verticalCenter
                text: "Mode"
                font.bold: true
                horizontalAlignment: Text.AlignHCenter
            }

            // Period header
            Text {
                width: parent.width / 5
                anchors.verticalCenter: parent.verticalCenter
                text: "Period (ms)"
                font.bold: true
                horizontalAlignment: Text.AlignHCenter
            }

            // Value header
            Text {
                width: parent.width / 5
                anchors.verticalCenter: parent.verticalCenter
                text: "Value"
                font.bold: true
                horizontalAlignment: Text.AlignHCenter
            }
        }

        // ScrollView to make the ListView scrollable
        ScrollView {
            id: scrollView
            width: parent.width
            height: parent.height - 40 // Adjusting height to account for the header row

            // ListView displaying data inside the scroll area
            ListView {
                id: tableView
                width: parent.width
                spacing: 2
                model: signalModel  // Updated model

                delegate: Rectangle {
                    width: tableView.width
                    height: 40
                    border.color: "blue"
                    color: model.index % 2 === 0 ? "lightgray" : "white"

                    Row {
                        anchors.fill: parent
                        anchors.centerIn: parent

                        // Index column (narrower)
                        Text {
                            width: parent.width / 6  // Adjusted width for index
                            anchors.verticalCenter: parent.verticalCenter
                            text: (model.index + 1).toString()  // Display row number (1-based)
                            horizontalAlignment: Text.AlignHCenter
                        }

                        // VSS Parameter (from Signal table)
                        Text {
                            width: parent.width / 5
                            anchors.verticalCenter: parent.verticalCenter
                            text: model.name  // The `name` field from Signal table
                            horizontalAlignment: Text.AlignHCenter
                        }

                        // Mode ComboBox (Override / Auto)
                        ComboBox {
                            id: modeCombo
                            width: parent.width / 5
                            anchors.verticalCenter: parent.verticalCenter
                            model: ["Override", "Auto"]
                            currentIndex: 0  // Default to "Override"
                            // Bind the "Period (ms)" text field enabled property based on selection
                            onCurrentIndexChanged: {
                                periodField.enabled = (currentIndex === 1); // Enable when "Auto" is selected
                            }
                        }

                        // Period (Editable only when "Auto" is selected)
                        TextField {
                            id: periodField
                            width: parent.width / 5
                            anchors.verticalCenter: parent.verticalCenter
                            text: model.period  // Bind the text field to the `period` column
                            enabled: false  // Default to disabled
                        }

                        // Value (Editable TextField)
                        TextField {
                            id: valueField
                            width: parent.width / 5
                            anchors.verticalCenter: parent.verticalCenter
                            text: model.value  // Bind the text field to the `value` column
                        }
                    }
                }
            }
        }
    }
}
