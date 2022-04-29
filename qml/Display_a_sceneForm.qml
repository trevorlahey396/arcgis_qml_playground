
// Copyright 2019 ESRI
//
// All rights reserved under the copyright laws of the United States
// and applicable international laws, treaties, and conventions.
//
// You may freely redistribute and use this sample code, with or
// without modification, provided you include the original copyright
// notice and use restrictions.
//
// See the Sample code usage restrictions document for further information.
//

import QtQuick 2.6
import QtQuick.Controls 2.2
import Esri.display_a_scene 1.0
import QtQuick.Layouts 1.3

Item {

  // Create SceneQuickView here, and create its Scene etc. in C++ code
  SceneView {
    id: view
    anchors.fill: parent
  }

  // Declare the C++ instance which creates the scene etc. and supply the view
  Display_a_scene {
    id: search
    sceneView: view
  }

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
        bottom: parent.bottom
        margins: 10
      }
      Rectangle {
//        color: "#f7f8fa"
        color: "#800000FF"
        border {
          color: "#7B7C7D"
        }
        radius: 6
        width: parent.width
        height: childrenRect.height

        GridLayout {
          Layout.margins: 5
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
            id: controls
            width: height * 3
            height: textField.height
            color: "transparent"
            enabled: true

            Button {
              id: reset
//              anchors.fill: parent
              anchors.margins : 5
              anchors.verticalCenter: parent.verticalCenter
              anchors.right: parent.right
              text: "reset"
              background: Rectangle {
                color: reset.hovered ? "#99FFFF00" : "#60FFFF00"
                Layout.margins: 5
                radius: 6
              }
              font.pixelSize: 16
              MouseArea {
                hoverEnabled: true
                anchors.fill: parent
                onEntered: {

                }
                onClicked: {
                  textField.focus = true;
                  textField.text = "";
                }
              }
            }
            Button {
              id: submit
//              anchors.fill: parent
              anchors.margins : 5
              anchors.verticalCenter: parent.verticalCenter
              anchors.right: reset.left
              text: "submit"
              background: Rectangle {
                Layout.margins: 5
                color: submit.hovered ? "#9900FF00" : "#6000FF00"
                radius: 6
              }
              font.pixelSize: 16
              MouseArea {
                hoverEnabled: true
                anchors.fill: parent
//                }
              }
            }
          }
//
//          Rectangle {
//            Layout.margins: 5
//            width: height * 3
////            color: "transparent"
//            color: "#8000FF00"
//            height: textField.height
//            visible: textField.length !== 0
//            enabled: visible
//
//
//            Button {
//              anchors.fill: parent
//              text: "reset"
//              font.pixelSize: 16
//              MouseArea {
//                anchors.fill: parent
//                onClicked: {
//                  textField.text = "";
//                  search.clearGraphics();
//                }
//                onEntered: {
//
//                }
//              }
//            }
//          }
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
}