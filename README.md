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
#
![Cloud architecture diagram](BattleChip.png)
