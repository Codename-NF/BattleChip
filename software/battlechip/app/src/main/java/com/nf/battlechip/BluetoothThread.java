package com.nf.battlechip;

import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothDevice;
import android.bluetooth.BluetoothSocket;
import android.util.Log;

import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.util.Arrays;
import java.util.HashSet;
import java.util.Optional;
import java.util.Set;
import java.util.UUID;

public class BluetoothThread {

    private static final String BLUETOOTH_DEBUG = "Bluetooth";
    private static BluetoothThread instance = null;

    private static final Set<String> CHIP_ONE_MAC_IDS = new HashSet<>(Arrays.asList("20:18:11:21:24:72", "B8:9A:2A:30:2B:35", "34:F6:4B:D7:53:65", "5C:F3:70:9F:40:7A")); // TODO: rely only on device name?
    private static final Set<String> CHIP_ONE_DEVICE_NAMES = new HashSet<>(Arrays.asList("hc01.com HC-05"));
    private static final Set<String> CHIP_TWO_MAC_IDS = new HashSet<>(); // TODO: fill
    private static final Set<String> CHIP_TWO_DEVICE_NAMES = new HashSet<>(); // TODO: fill
    private static final UUID SERVICE_UUID = UUID.fromString("00001101-0000-1000-8000-00805F9B34FB");

    private BluetoothSocket bluetoothSocket;
    private InputStream inputStream;
    private OutputStream outputStream;
    private final int chipId;

    BluetoothThread(int chipId) throws IOException {
        BluetoothAdapter adapter = BluetoothAdapter.getDefaultAdapter();
        // List all devices
        for (BluetoothDevice device : adapter.getBondedDevices()) {
            Log.d(BLUETOOTH_DEBUG, device.getAddress() + " " + device.getName());
        }
        Optional<BluetoothDevice> device = adapter.getBondedDevices().stream()
                .filter(chipId == 1 ? BluetoothThread::isChipOneDevice : BluetoothThread::isChipTwoDevice).findAny();
        // create socket
        try {
            bluetoothSocket = device.isPresent() ? device.get().createRfcommSocketToServiceRecord(SERVICE_UUID)
                    : null;
            if (bluetoothSocket == null) {
                Log.d(BLUETOOTH_DEBUG, "Failed to find device\n");
                throw new IOException();
            }
        } catch (IOException e) {
            Log.d(BLUETOOTH_DEBUG, "Socket creation failed\n" + e.toString());
            throw e;
        }

        // connect socket
        try {
            bluetoothSocket.connect();
            // get streams
            inputStream = bluetoothSocket.getInputStream();
            outputStream = bluetoothSocket.getOutputStream();
        } catch (IOException e) {
            Log.d(BLUETOOTH_DEBUG, "Exception getting connection/retrieving streams\n" + e.toString());
            close();
            throw e;
        }
        this.chipId = chipId;
    }

    private static boolean isChipOneDevice(BluetoothDevice device) {
        return CHIP_ONE_MAC_IDS.contains(device.getAddress())
                || CHIP_ONE_DEVICE_NAMES.contains(device.getName());
    }

    private static boolean isChipTwoDevice(BluetoothDevice device) {
        return CHIP_TWO_MAC_IDS.contains(device.getAddress())
                || CHIP_TWO_DEVICE_NAMES.contains(device.getName());
    }

    public static void createInstance(int chipId) throws IOException {
        if (instance != null) {
            instance.close();
        }
        instance = new BluetoothThread(chipId);
        instance.startReading();
    }

    public static BluetoothThread getInstance() {
        if (instance != null && instance.isInvalidThread()) {
            try {
                instance = new BluetoothThread(instance.chipId);
            } catch (IOException exception) {
                exception.printStackTrace();
            }
        }
        return instance;
    }

    public void startReading() {
        if (bluetoothSocket.isConnected()) {
            new Thread(this::read).start();
        } else {
            Log.d(BLUETOOTH_DEBUG, "Can't read, socket not connected");
        }
    }

    public void read() {
        Log.d(BLUETOOTH_DEBUG, "Starting thread");
        try {
            byte[] readBuffer = new byte[1024];
            StringBuilder builder = new StringBuilder();

            while (inputStream != null) {
                int bytesRead = inputStream.read(readBuffer); // possible to read portions of the message at a time
                String readCharacters = new String(readBuffer).substring(0, bytesRead);
                builder.append(readCharacters);
                Log.d(BLUETOOTH_DEBUG, "Read: " + readCharacters);

                // Only send to Unity if entire message has been read
                while (builder.indexOf("~") != -1) {
                    UnityMessage.processBluetoothMessage(builder.substring(0, builder.indexOf("~")));
                    builder = builder.delete(0, builder.indexOf("~") + 1); // remove last message
                }
            }
        } catch (IOException e) {
            close();
            Log.d(BLUETOOTH_DEBUG, "Read failed, exiting\n" + e.toString());
        }
    }

    public boolean isInvalidThread() {
        return !bluetoothSocket.isConnected() || inputStream == null || outputStream == null;
    }

    public void write(byte[] bytes) {
        if (isInvalidThread()) {
            Log.d(BLUETOOTH_DEBUG, "Can't write, invalid thread");
        } else {
            try {
                Log.d(BLUETOOTH_DEBUG, "Write: " + new String(bytes));
                outputStream.write(bytes);
            } catch (IOException e) {
                close();
                Log.d(BLUETOOTH_DEBUG, "Write failed\n" + e.toString());
            }
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

