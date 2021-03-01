package com.nf.battlechip.activity;

import android.Manifest;
import android.bluetooth.BluetoothAdapter;
import android.content.Intent;
import android.content.pm.PackageManager;
import android.os.Build;
import android.os.Bundle;
import android.view.View;
import android.widget.Toast;

import androidx.annotation.NonNull;
import androidx.appcompat.app.AppCompatActivity;
import androidx.core.app.ActivityCompat;
import androidx.core.content.ContextCompat;

import com.nf.battlechip.BluetoothThread;
import com.nf.battlechip.R;

public class MainActivity extends AppCompatActivity implements ActivityCompat.OnRequestPermissionsResultCallback {

    private final int BACKGROUND_LOCATION_REQUEST_CODE = 1;
    private final int ENABLE_BT_REQUEST_CODE = 2;

    private BluetoothThread bluetoothThread;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        findViewById(R.id.options_button).setOnClickListener(view -> {
            getBackgroundPermissionsIfNecessary();
            setUpBluetooth();
            bluetoothThread = new BluetoothThread();
        });

        findViewById(R.id.single_player_button).setOnClickListener(this::startUnityActivity);
        findViewById(R.id.multi_player_button).setOnClickListener(this::startUnityActivity);
        findViewById(R.id.player_stats_button).setOnClickListener(view -> startActivity(new Intent(this, UserStatisticsActivity.class)));
    }

    private void startUnityActivity(View view) {
        startActivity(new Intent(this, MainUnityActivity.class).setFlags(Intent.FLAG_ACTIVITY_REORDER_TO_FRONT | Intent.FLAG_ACTIVITY_SINGLE_TOP));
    }

    private void getBackgroundPermissionsIfNecessary() {
        // https://developer.android.com/guide/topics/connectivity/bluetooth
        // get background location permission
        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.Q) {
            if (ContextCompat.checkSelfPermission(this.getApplicationContext(),
                    Manifest.permission.ACCESS_BACKGROUND_LOCATION)
                    != PackageManager.PERMISSION_GRANTED) {
                ActivityCompat.requestPermissions(
                        MainActivity.this,
                        new String[]{Manifest.permission.ACCESS_BACKGROUND_LOCATION},
                        BACKGROUND_LOCATION_REQUEST_CODE);
            }
        }
    }

    private void setUpBluetooth() {
        // https://developer.android.com/guide/topics/connectivity/bluetooth
        // get bluetooth adapter and ensure that phone supports bluetooth
        BluetoothAdapter bluetoothAdapter = BluetoothAdapter.getDefaultAdapter();
        if (bluetoothAdapter == null) {
            Toast.makeText(this, "Phone does not support bluetooth", Toast.LENGTH_LONG).show();
            return;
        }

        // https://developer.android.com/guide/topics/connectivity/bluetooth
        // prompt user to enable bluetooth
        if (!bluetoothAdapter.isEnabled()) {
            Intent enableBtIntent = new Intent(BluetoothAdapter.ACTION_REQUEST_ENABLE);
            startActivityForResult(enableBtIntent, ENABLE_BT_REQUEST_CODE);
        }

    }

    @Override
    public void onRequestPermissionsResult(int requestCode, @NonNull String[] permissions,
                                           @NonNull int[] grantResults) {
        if (requestCode == BACKGROUND_LOCATION_REQUEST_CODE && grantResults.length > 0 && grantResults[0] == PackageManager.PERMISSION_DENIED) {
            Toast.makeText(this, "We need background location for bluetooth", Toast.LENGTH_LONG).show();
        }
    }

    @Override
    public void onActivityResult(int requestCode, int resultCode, Intent data) {
        super.onActivityResult(requestCode, resultCode, data);
        if (requestCode == ENABLE_BT_REQUEST_CODE && resultCode == RESULT_CANCELED) {
            Toast.makeText(this, "We need to enable bluetooth", Toast.LENGTH_LONG).show();
        }
    }

}