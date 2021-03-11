package com.nf.battlechip;

import com.unity3d.player.UnityPlayer;

import java.util.Locale;

public class UnityMessage {

    // Called by Unity to inform Android of placement message
    public static void placement(String message) {
        BluetoothThread.getInstance().write(message.getBytes());
    }

    // Called by Unity to inform Android of where the user shot
    public static void shoot(int x, int y) {
        String message = String.format(Locale.ENGLISH, "shoot %d %d\n\n", x, y);
        BluetoothThread.getInstance().write(message.getBytes());
    }

    // Calls the correct Unity function with the remaining arguments based on the command
    public static void processBluetoothMessage(String message) {
        int firstSpace = message.indexOf(' ');
        String arguments = message.trim().substring(firstSpace + 1);
        UnityPlayer.UnitySendMessage("PR_GameManager", message.substring(0, firstSpace), arguments);
    }
}
