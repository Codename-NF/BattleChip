package com.nf.battlechip;

import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothDevice;
import android.bluetooth.BluetoothSocket;
import android.util.Log;

import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.util.Optional;
import java.util.UUID;

public class BluetoothThread {

    private static final String BLUETOOTH_DEBUG = "Bluetooth";
    private static BluetoothThread instance = null;
    private static Thread readingThread = null;

    private final String MAC_ID = "B8:9A:2A:30:2B:35"; // TODO: Update this to be correct address
    private final UUID uuid = UUID.fromString("94f39d29-7d6d-437d-973b-fba39e49d4ee");

    private BluetoothSocket bluetoothSocket;
    private InputStream inputStream;
    private OutputStream outputStream;

    BluetoothThread() {
        BluetoothAdapter adapter = BluetoothAdapter.getDefaultAdapter();
        // List all devices
        for (BluetoothDevice device : adapter.getBondedDevices()) {
            Log.d(BLUETOOTH_DEBUG, device.getAddress() + " " + device.getName());
        }
        Optional<BluetoothDevice> device = adapter.getBondedDevices().stream()
                .filter(bondedDevice -> MAC_ID.equals(bondedDevice.getAddress())).findAny();
        bluetoothSocket = null;
        inputStream = null;
        outputStream = null;

        // create socket
        try {
            bluetoothSocket = device.isPresent() ? device.get().createRfcommSocketToServiceRecord(uuid)
                    : null;
        } catch (IOException e) {
            Log.d(BLUETOOTH_DEBUG, "Socket creation failed\n" + e.toString());
        }

        // connect socket
        if (bluetoothSocket != null) {
            try {
                bluetoothSocket.connect();
                // get streams
                try {
                    inputStream = bluetoothSocket.getInputStream();
                    outputStream = bluetoothSocket.getOutputStream();
                } catch (IOException e) {
                    Log.d(BLUETOOTH_DEBUG, "Exception getting streams\n" + e.toString());
                }
            } catch (IOException exception) {
                close();
            }

        }
    }

    public static BluetoothThread getInstance() {
        if (instance == null || !instance.isValidThread()) {
            instance = new BluetoothThread();
        }
        return instance;
    }

    public void startReading() {
        if (readingThread == null) {
            if (bluetoothSocket != null && bluetoothSocket.isConnected()) {
                readingThread = new Thread(this::read);
                readingThread.start();
            } else {
                Log.d(BLUETOOTH_DEBUG, "Can't read, socket not connected");
            }
        }
    }

    public void read() {
        Log.d(BLUETOOTH_DEBUG, "Starting thread");
        try {
            byte[] readBuffer = new byte[1024];
            while (inputStream != null) {
                int bytesRead = inputStream.read(readBuffer);
                String message = new String(readBuffer);
                message = message.substring(0, bytesRead);
                Log.d(BLUETOOTH_DEBUG, "Read: " + message);
                UnityMessage.processBluetoothMessage(message);
            }
        } catch (IOException e) {
            close();
            readingThread = null;
            Log.d(BLUETOOTH_DEBUG, "Read failed, exiting\n" + e.toString());
        }
    }

    public boolean isValidThread() {
        return bluetoothSocket != null && inputStream != null && outputStream != null;
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

    private void close() {
        if (bluetoothSocket != null) {
            try {
                bluetoothSocket.close();
            } catch (IOException e) {
                Log.d(BLUETOOTH_DEBUG, "Failed to close socket\n" + e.toString());
            } finally {
                inputStream = null;
                outputStream = null;
                bluetoothSocket = null;
            }
        }
    }
}

