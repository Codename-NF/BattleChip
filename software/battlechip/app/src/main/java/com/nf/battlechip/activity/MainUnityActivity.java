package com.nf.battlechip.activity;

import android.os.Bundle;
import android.util.Log;
import android.view.Gravity;
import android.view.ViewGroup;
import android.widget.Button;

import com.nf.battlechip.BluetoothThread;
import com.nf.battlechip.R;
import com.unity3d.player.UnityPlayer;
import com.unity3d.player.UnityPlayerActivity;

public class MainUnityActivity extends UnityPlayerActivity {

    private static MainUnityActivity instance = null;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        instance = this;

        Button testButton = new Button(this);
        testButton.setText(getString(R.string.logging_button_label));
        testButton.setGravity(Gravity.CENTER);
        testButton.setOnClickListener(view -> {
            Log.d("LoggingButton", "Logging button clicked");
            sendBluetoothMessageToUnity("Test message");
        });
        mUnityPlayer.addView(testButton, ViewGroup.LayoutParams.WRAP_CONTENT, ViewGroup.LayoutParams.WRAP_CONTENT);
    }

    // Function receives messages from Unity
    public void sendBluetoothMessageToConsole(String message) {
        Log.d("sendBluetooth", message);
        BluetoothThread.getInstance().write(message.getBytes());
    }

    public static void sendBluetoothMessageToUnity(String message) {
        UnityPlayer.UnitySendMessage("PR_GameManager",
                "ReceiveBluetoothMessageFromConsole", message);
    }

    @Override
    protected void onDestroy() {
        super.onDestroy();
        instance = null;
    }
}
