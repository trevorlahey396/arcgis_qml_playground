import QtQuick 2.6
import QtQuick.Controls 2.2
import Esri.display_a_scene 1.0
import QtQuick.Layouts 1.3

  Rectangle {
    // add rectangle for gui code
    id: searchRect
    width: parent.width
    height: childrenRect.height
    property int cellHeight: 40;
    radius: 6

    Column {
      anchors {
        fill: parent
        margins: 10
      }
      Rectangle {
//        color: "#f7f8fa"
        color: "#80FFFF00"
        border {
          color: "#7B7C7D"
        }
        radius: 6
        width: parent.width
        height: childrenRect.height

        GridLayout {
          width: parent.width
          columns: 4
          TextField {
            id: textField
            Layout.margins: 5
            Layout.fillWidth: true
            font.pixelSize: 14
            placeholderText: "Type in an address"
            selectByMouse: true

            onTextChanged: {
              if (text.length > 0 && suggestView)
                suggestView.visible = true;
              search.setSuggestions(text);
            }

            onAccepted: {
              suggestView.visible = false;
              search.geocode(textField.text);
              Qt.inputMethod.hide();
            }
          }

          Rectangle {
            Layout.margins: 5
            width: height
            height: textField.height
            color: "#f7f8fa"
            visible: textField.length === 0
            enabled: visible

            Button {
              anchors.fill: parent
              text: "reset"
              font.pixelSize: 16
              MouseArea {
                anchors.fill: parent
                onClicked: {
                  textField.focus = true;
                  suggestView.visible = !suggestView.visible;
                }
              }
            }
          }

          Rectangle {
            Layout.margins: 5
            width: height
            color: "transparent"
            height: textField.height
            visible: textField.length !== 0
            enabled: visible


            Button {
              anchors.fill: parent
              text: "reset"
              font.pixelSize: 16
              MouseArea {
                anchors.fill: parent
                onClicked: {
                  textField.text = "";
                  search.clearGraphics();
                }
              }
            }
          }
        }
      }

      //Tutorial add ListView to display suggested location
//      results and bind it to the locator task list model.

      ListView {
        id: suggestView
        height: 20 * searchRect.cellHeight
        width: textField.width
        model: search.suggestions
        visible: false
        clip: true
        delegate: Component {
          Rectangle {
            id: rect
            width: textField.width
            height: searchRect.cellHeight
            color: "#f7f8fa"

            Text {
              anchors {
                verticalCenter: parent.verticalCenter
                leftMargin: 5
                rightMargin: 5
              }

              font {
                weight: Font.Black
                pixelSize: 16
              }

              width: textField.width
              text: label
              elide: Text.ElideRight
              leftPadding: 5
              color: "black"
            }

            MouseArea {
              anchors.fill: parent
              onClicked: {
                textField.text = label;
                suggestView.visible = false;
                search.geocode(label);
                Qt.inputMethod.hide();
              }
            }
          }
        }
      }
    }
  }