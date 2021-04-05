package com.nf.battlechip;

import android.util.Log;

import com.nf.battlechip.activity.LobbyActivity;
import com.unity3d.player.UnityPlayer;

import java.util.Locale;

public class UnityMessage {

    private static String UNITY_MESSAGE_DEBUG = "Unity";

    // Called by Unity to inform Android of placement message
    public static void placement(String message) {
        String[] splitMessage = message.split("\\s");
        byte[] bytes = new byte[splitMessage.length / 2 + 2];
        bytes[0] = 'p';
        bytes[bytes.length - 1] = '~';
        if (splitMessage.length % 4 != 0) {
            Log.d(UNITY_MESSAGE_DEBUG, "Placement message not properly formatted");
        }
        for (int i = 0; i < splitMessage.length; i += 4) {
            bytes[i / 2 + 1] = (byte) (Byte.parseByte(splitMessage[i]) + Byte.parseByte(splitMessage[i + 1]) * 10);
            bytes[i / 2 + 2] = (byte) (Byte.parseByte(splitMessage[i + 2]) * 2 + Byte.parseByte(splitMessage[i + 3]));
        }

        for (byte b : bytes) {
            Log.d(UNITY_MESSAGE_DEBUG, "" + b);
        }
        BluetoothThread.getInstance().write(bytes);
    }

    // Called by Unity to inform Android of where the user shot
    public static void shoot(int x, int y) {
        String message = String.format(Locale.ENGLISH, "s %d %d~", x, y);
        BluetoothThread.getInstance().write(message.getBytes());
    }

    // Called by Unity to inform Android that a user is forfeiting
    public static void forfeit() {
        BluetoothThread.getInstance().write("f~".getBytes());
    }

    // Called in Android to send create game message
    // mode = 0 for easy, 1 for hard, 2 for multiplayer
    public static void create(int playerId, int mode) {
        String message = String.format(Locale.ENGLISH, "c %d %d~", mode, playerId);
        BluetoothThread.getInstance().write(message.getBytes());
    }

    // Called in Android to send join game message
    public static void join(int playerId) {
        String message = String.format(Locale.ENGLISH, "j %d", playerId);
        BluetoothThread.getInstance().write(message.getBytes());
    }

    // Pass the Bluetooth message to Unity
    public static void processBluetoothMessage(String message) {
        String[] splitMessage = message.split("\\s");
        String command = splitMessage[0];
        if ("create".equals(command)) {
            // failed to create game
            if ("0".equals(splitMessage[2])) {
                LobbyActivity.failedToCreateGame();
                // single-player game ready
            } else if (Integer.parseInt(splitMessage[1]) <= 1) {
                LobbyActivity.gamesIsReady();
            }
            // failed to join an existing game
        } else if ("join".equals(command) && "0".equals(splitMessage[1])) {
            LobbyActivity.failedToCreateGame();
        } else if ("ready".equals(command)) {
            LobbyActivity.gamesIsReady();
        } else {
            UnityPlayer.UnitySendMessage("PR_GameManager", "AndroidToUnity", message);
        }
    }
}
