import 'package:flutter/material.dart';

class MainWindow extends StatelessWidget {
  const MainWindow({super.key});

  @override
  Widget build(BuildContext context) {
    return SizedBox(
      height: 240,
      width: 320,
      child: Stack(
        children: [
          Container(
            color: Color.fromARGB(255, 247, 231, 144),
            height: 240,
            width: 320,
            child: Stack(
              children: [
                Positioned(
                  height: 43,
                  left: 125,
                  top: 80,
                  width: 9,
                  child: Text(
                    ".", 
                    textAlign: TextAlign.center,
                    textScaler: TextScaler.linear(1),
                    style: TextStyle(
                      color: Color.fromARGB(255, 0, 0, 0),
                      fontFamily: "源ノ角ゴシック JP",
                      fontSize: 30,
                      fontVariations: [FontVariation.weight(600)],
                      height: 1,
                    ),
                  ),
                ),
              ],
            ),
          ),
        ],
      ),
    );
  }
}
