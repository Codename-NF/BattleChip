package com.nf.battlechip;

import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothDevice;
import android.bluetooth.BluetoothSocket;

import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.util.Optional;
import java.util.UUID;

public class BluetoothThread {

    private final String MAC_ID = "TEMP";

    private final BluetoothSocket bluetoothSocket;

    private final InputStream inputStream;
    private final OutputStream outputStream;

    public BluetoothThread() {
        BluetoothAdapter adapter = BluetoothAdapter.getDefaultAdapter();

        Optional<BluetoothDevice> device = adapter.getBondedDevices().stream()
                .filter(bondedDevice -> MAC_ID.equals(bondedDevice.getAddress())).findAny();
        BluetoothSocket socket = null;
        InputStream input = null;
        OutputStream output = null;

        // create socket
        try {
            socket = device.isPresent() ? device.get().createRfcommSocketToServiceRecord(UUID.randomUUID())
                    : null;
        } catch (IOException ignored) {
            // ignore socket creation exception
        }
        bluetoothSocket = socket;

        // connect socket
        if (bluetoothSocket != null) {
            try {
                bluetoothSocket.connect();
            } catch (IOException exception) {
                try {
                    bluetoothSocket.close();
                } catch (IOException closeException) {
                    // ignore close exception
                }
            }

            // get streams
            try {
                input = bluetoothSocket.getInputStream();
                output = bluetoothSocket.getOutputStream();
            } catch (IOException e) {
                // ignore stream exception
            }
        }

        inputStream = input;
        outputStream = output;
    }

    public int read(byte[] readBuffer) {
        try {
            return inputStream.read(readBuffer);
        } catch (IOException e) {
            // exit the read now that socket read has failed
            return 0;
        }
    }

    public void write(byte[] bytes) {
        try {
            outputStream.write(bytes);
        } catch (IOException e) {
            // ignore write exception
        }
    }

    public void close() {
        try {
            bluetoothSocket.close();
        } catch (IOException e) {
            // ignore closing exception
        }
    }
}

