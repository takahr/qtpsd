import QtQuick
import QtQuick.Layouts

Item {
    height: 240
    width: 320
    Item {
        anchors.fill: parent
        Rectangle {
            color: "#012401"
            height: 240
            width: 320
            x: 0
            y: 0
        }
        Text {
            anchors.fill: parent
            color: "#eac3c3"
            font.family: "源ノ角ゴシック JP"
            font.pointSize: 33.3333
            horizontalAlignment: Text.AlignHCenter
            text: ""
            verticalAlignment: Text.AlignVCenter
        }
        Text {
            color: "#eac3c3"
            font.family: "源ノ角ゴシック JP"
            font.pointSize: 20
            height: 110
            horizontalAlignment: Text.AlignHCenter
            text: "文字列中に\n改行"
            verticalAlignment: Text.AlignVCenter
            width: 160
            x: 2
            y: 10
        }
        Item {
            height: 98
            width: 144
            x: 169
            y: 10
            Column {
                anchors.centerIn: parent
                RowLayout {
                    anchors.horizontalCenter: parent.horizontalCenter
                    spacing: 0
                    Text {
                        Layout.fillHeight: true
                        color: "#eac3c3"
                        font.family: "KozGoPr6N-Regular"
                        font.pointSize: 20
                        text: "文字列"
                        verticalAlignment: Text.AlignVCenter
                    }
                    Text {
                        Layout.fillHeight: true
                        color: "#eac3c3"
                        font.family: "KozGoPr6N-Regular"
                        font.pointSize: 10.6667
                        text: "中"
                        verticalAlignment: Text.AlignVCenter
                    }
                    Text {
                        Layout.fillHeight: true
                        color: "#eac3c3"
                        font.family: "KozGoPr6N-Regular"
                        font.pointSize: 20
                        text: "に"
                        verticalAlignment: Text.AlignVCenter
                    }
                }
                RowLayout {
                    anchors.horizontalCenter: parent.horizontalCenter
                    spacing: 0
                    Text {
                        Layout.fillHeight: true
                        color: "#eac3c3"
                        font.family: "KozGoPr6N-Regular"
                        font.pointSize: 20
                        text: "別"
                        verticalAlignment: Text.AlignVCenter
                    }
                    Text {
                        Layout.fillHeight: true
                        color: "#eac3c3"
                        font.family: "源ノ角ゴシック JP"
                        font.pointSize: 12
                        text: "フォント"
                        verticalAlignment: Text.AlignVCenter
                    }
                }
                RowLayout {
                    anchors.horizontalCenter: parent.horizontalCenter
                    spacing: 0
                    Text {
                        Layout.fillHeight: true
                        color: "#eac3c3"
                        font.family: "源ノ角ゴシック JP"
                        font.pointSize: 12
                        text: ""
                        verticalAlignment: Text.AlignVCenter
                    }
                }
            }
        }
        Text {
            color: "#eac3c3"
            font.family: "KozGoPr6N-Regular"
            font.pointSize: 20
            height: 89
            horizontalAlignment: Text.AlignHCenter
            text: "Shift\n+改行"
            verticalAlignment: Text.AlignVCenter
            width: 86
            x: 25
            y: 110
        }
        Text {
            color: "#eac3c3"
            font.family: "KozGoPr6N-Regular"
            font.pointSize: 16
            height: 116
            horizontalAlignment: Text.AlignHCenter
            text: "段落テキストは折り返される"
            verticalAlignment: Text.AlignVCenter
            width: 158
            x: 160
            y: 110
        }
    }
}
