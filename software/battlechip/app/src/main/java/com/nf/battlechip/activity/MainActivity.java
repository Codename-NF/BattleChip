package com.nf.battlechip.activity;

import android.Manifest;
import android.app.AlertDialog;
import android.bluetooth.BluetoothAdapter;
import android.content.DialogInterface;
import android.content.Intent;
import android.content.pm.PackageManager;
import android.os.Build;
import android.os.Bundle;
import android.preference.PreferenceManager;
import android.widget.Toast;

import androidx.annotation.NonNull;
import androidx.core.app.ActivityCompat;
import androidx.core.content.ContextCompat;

import com.google.android.gms.auth.api.signin.GoogleSignIn;
import com.google.android.material.chip.Chip;
import com.google.android.material.chip.ChipGroup;
import com.nf.battlechip.BluetoothThread;
import com.nf.battlechip.R;
import com.nf.battlechip.UnityMessage;

public class MainActivity extends SetThemeActivity implements ActivityCompat.OnRequestPermissionsResultCallback {

    private final int BACKGROUND_LOCATION_REQUEST_CODE = 1;
    private final int ENABLE_BT_REQUEST_CODE = 2;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        getBackgroundPermissionsIfNecessary();
        setUpBluetooth();

        String email = GoogleSignIn.getLastSignedInAccount(this).getEmail();

        findViewById(R.id.app_name_text_view).setOnClickListener(view -> startUnityActivity()); // TODO: remove this Unity shortcut
        findViewById(R.id.options_button).setOnClickListener(view -> showColorDialog());
        findViewById(R.id.single_player_button).setOnClickListener(view -> {
            BluetoothThread.createInstance(1);
            UnityMessage.create(email, 1);
            startLobbyActivity();
        });
        findViewById(R.id.multi_player_button).setOnClickListener(view -> showMultiplayerDialog(email));
        findViewById(R.id.player_stats_button).setOnClickListener(view -> startActivity(new Intent(this, UserStatisticsActivity.class)));
        findViewById(R.id.bluetooth_test_button).setOnClickListener(view -> {
            BluetoothThread.createInstance(1);
            BluetoothThread thread = BluetoothThread.getInstance();
            thread.startReading();
            if (thread.isValidThread()) {
                thread.write("Test message~".getBytes());
            }
        });
    }

    @Override
    protected void onRestart() {
        super.onRestart();

        BluetoothThread thread = BluetoothThread.getInstance();
        if (thread != null) {
            thread.close();
        }
    }

    private void showMultiplayerDialog(String email) {
        AlertDialog.Builder builder = new AlertDialog.Builder(this);
        builder.setMessage("Do you want to create or join a lobby?");
        builder.setNeutralButton("Cancel", ((dialog, which) -> dialog.dismiss()));
        builder.setNegativeButton("Join", ((dialog, which) -> {
            BluetoothThread.createInstance(2);
            UnityMessage.join(email);
            startLobbyActivity();
        }));
        builder.setPositiveButton("Create", ((dialog, which) -> {
            BluetoothThread.createInstance(1);
            UnityMessage.create(email, 2);
            startLobbyActivity();
        }));
        builder.create().show();
    }

    private void showColorDialog() {
        AlertDialog.Builder builder = new AlertDialog.Builder(this);
        builder.setMessage("Select a color");
        builder.setView(R.layout.dialog_color);
        builder.setNegativeButton("Cancel", (dialog, which) -> dialog.dismiss());
        builder.setPositiveButton("Confirm", (dialog, which) -> {}); // this gets replaced later
        AlertDialog dialog = builder.create();
        dialog.show();

        ChipGroup group = dialog.findViewById(R.id.color_chip_group);

        dialog.setButton(DialogInterface.BUTTON_POSITIVE, "Confirm", (alertDialog, which) -> {
            Chip checkedChip = dialog.findViewById(group.getCheckedChipId());
            String color = (String) checkedChip.getTag(R.string.color_key);
            String themeId = (String) checkedChip.getTag(R.string.theme_key);
            PreferenceManager.getDefaultSharedPreferences(this).edit()
                    .putInt("theme", Integer.parseInt(themeId.substring(1))).apply(); // remove @ symbol with substring
            PreferenceManager.getDefaultSharedPreferences(this).edit()
                    .putLong("color", Long.parseLong(color.substring(1).toUpperCase(), 16)).apply();
            recreate();
            dialog.dismiss();
        });
    }

    private void startUnityActivity() {
        startActivity(new Intent(this, MainUnityActivity.class).setFlags(Intent.FLAG_ACTIVITY_REORDER_TO_FRONT | Intent.FLAG_ACTIVITY_SINGLE_TOP));
    }

    private void startLobbyActivity() {
        startActivity(new Intent(this, LobbyActivity.class).setFlags(Intent.FLAG_ACTIVITY_NO_HISTORY));
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