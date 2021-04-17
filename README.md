# BattleChip
* `Scrum meetings.pdf` contains our scrum records
    * It has been converted to pdf from our Google Docs document

# Code
## Mobile App
### Android
* `software/battlechip/app/src/main/java/com/nf/battlechip` contains the Java source files for our app
* `software/battlechip/app/src/androidTest/java/com/nf/battlechip` contains the Espresso JUnit tests for our app
* `software/battlechip/app/src/main/res` contains the various XML files for the layout/strings/colors

### Custom Unity Library (Game)
* `software/unity/Assets/Scripts` contains the C# scripts used to control the Unity interface (game-manager, ships, board, cells, etc.)

## Cloud Server
* `software/cloud/` contains the source files for the Node.js server
* `software/cloud/test/unit` contains the automated Jest tests that are called by Travis CI

## Game Engine
* `software/gamelogic/` contains the source files for the game engine 
* `software/gamelogic/utilstest.cpp` contains the automated Acutest test plans and test suites

## Hardware

### Ai
* `hardware/AI_Alg/` contains the verilog files for the Ai and the test bench

### Bluetooth
* `hardware/Bluetooth_firm/` contains the c files for the bluetooth firmware and files to run/test bluetooth

### VGA
* `hardware/VGA_algs/` contains the C and verilog files to run and test the VGA graphics components

### WiFi
* `hardware/Wifi_algs/` contains the C files to run wifi components, as well as the firmware and lua scripts that were flashed to the RFS board
#
![Cloud architecture diagram](BattleChip.png)
