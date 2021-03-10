package com.nf.battlechip;

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
}
