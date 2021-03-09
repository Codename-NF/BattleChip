package com.nf.battlechip;

import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothDevice;
import android.bluetooth.BluetoothSocket;
import android.util.Log;

import com.nf.battlechip.activity.MainUnityActivity;

import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.util.Optional;
import java.util.UUID;

public class BluetoothThread implements Runnable {

    private static final String BLUETOOTH_DEBUG = "Bluetooth";

    private final String MAC_ID = "B8:9A:2A:30:2B:35"; // TODO: Update this to be correct address
    private final UUID uuid = UUID.fromString("94f39d29-7d6d-437d-973b-fba39e49d4ee");

    private final BluetoothSocket bluetoothSocket;
    private InputStream inputStream;
    private OutputStream outputStream;

    public BluetoothThread() {
        BluetoothAdapter adapter = BluetoothAdapter.getDefaultAdapter();
        // List all devices
        for (BluetoothDevice device : adapter.getBondedDevices()) {
            Log.d(BLUETOOTH_DEBUG, device.getAddress() + " " + device.getName());
        }
        Optional<BluetoothDevice> device = adapter.getBondedDevices().stream()
                .filter(bondedDevice -> MAC_ID.equals(bondedDevice.getAddress())).findAny();
        BluetoothSocket socket = null;
        inputStream = null;
        outputStream = null;

        // create socket
        try {
            socket = device.isPresent() ? device.get().createRfcommSocketToServiceRecord(uuid)
                    : null;
        } catch (IOException e) {
            Log.d(BLUETOOTH_DEBUG, "Socket creation failed\n" + e.toString());
        }
        bluetoothSocket = socket;

        // connect socket
        if (bluetoothSocket != null) {
            try {
                bluetoothSocket.connect();
            } catch (IOException exception) {
                close();
            }

            // get streams
            try {
                inputStream = bluetoothSocket.getInputStream();
                outputStream = bluetoothSocket.getOutputStream();
            } catch (IOException e) {
                Log.d(BLUETOOTH_DEBUG, "Exception getting streams\n" + e.toString());
            }
        }
    }

    @Override
    public void run() {
        Log.d(BLUETOOTH_DEBUG, "Starting thread");
        try {
            byte[] readBuffer = new byte[1024];
            while (inputStream != null) {
                inputStream.read(readBuffer);
                String message = new String(readBuffer);
                message = message.substring(0, message.indexOf("\n") + 1);
                Log.d(BLUETOOTH_DEBUG, "Read: " + message);
                MainUnityActivity.sendBluetoothMessageToUnity(message);
            }
        } catch (IOException e) {
            Log.d(BLUETOOTH_DEBUG, "Read failed, exiting\n" + e.toString());
        }
    }

    public boolean isValidThread() {
        return inputStream != null && outputStream != null;
    }

    public void write(byte[] bytes) {
        try {
            Log.d(BLUETOOTH_DEBUG, "Write: " + new String(bytes));
            outputStream.write(bytes);
        } catch (IOException e) {
            close();
            Log.d(BLUETOOTH_DEBUG, "Write failed\n" + e.toString());
        }
    }

    public void close() {
        try {
            bluetoothSocket.close();
        } catch (IOException e) {
            Log.d(BLUETOOTH_DEBUG, "Failed to close socket\n" + e.toString());
        } finally {
            inputStream = null;
            outputStream = null;
        }
    }
}

