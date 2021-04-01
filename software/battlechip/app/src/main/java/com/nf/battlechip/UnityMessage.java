package com.nf.battlechip;

import com.nf.battlechip.activity.LobbyActivity;
import com.unity3d.player.UnityPlayer;

import java.util.Locale;

public class UnityMessage {

    // Called by Unity to inform Android of placement message
    public static void placement(String message) {
        BluetoothThread.getInstance().write(message.getBytes());
    }

    // Called by Unity to inform Android of where the user shot
    public static void shoot(int x, int y) {
        String message = String.format(Locale.ENGLISH, "shoot %d %d~", x, y);
        BluetoothThread.getInstance().write(message.getBytes());
    }

    // Called by Unity to inform Android that a user is forfeiting
    public static void forfeit() {
        BluetoothThread.getInstance().write("forfeit~".getBytes());
    }

    // Called in Android to send create game message
    public static void create(String playerEmail, int numPlayers) {
        String message = String.format(Locale.ENGLISH, "create %s %d~", playerEmail, numPlayers);
        BluetoothThread.getInstance().write(message.getBytes());
    }

    // Called in Android to send join game message
    public static void join(String playerEmail) {
        String message = String.format(Locale.ENGLISH, "join %s", playerEmail);
        BluetoothThread.getInstance().write(message.getBytes());
    }

    // Pass the Bluetooth message to Unity
    public static void processBluetoothMessage(String message) {
        String[] splitMessage = message.split("\\s");
        String command = splitMessage[0];
        if ("create".equals(command)) {
            if ("0".equals(splitMessage[2])) { // failed to create game
                LobbyActivity.failedToCreateGame();
            } else if ("1".equals(splitMessage[1])) { // single-player game ready
                LobbyActivity.gamesIsReady();
            }
        } else if ("join".equals(command)) {
            LobbyActivity.failedToCreateGame();
        } else if ("ready".equals(command)) {
            LobbyActivity.gamesIsReady();
        } else {
            UnityPlayer.UnitySendMessage("PR_GameManager", "AndroidToUnity", message);
        }
    }
}
