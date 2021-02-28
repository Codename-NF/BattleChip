package com.nf.battlechip;

import android.os.Bundle;
import android.util.Log;
import android.view.Gravity;
import android.view.ViewGroup;
import android.widget.Button;

import com.unity3d.player.UnityPlayer;
import com.unity3d.player.UnityPlayerActivity;

public class MainUnityActivity extends UnityPlayerActivity {

    private static MainUnityActivity instance = null;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        instance = this;

        Button testButton = new Button(this);
        testButton.setText("Logging");
        testButton.setGravity(Gravity.CENTER);
        testButton.setOnClickListener(view -> {
            Log.d("tag", "button clicked");
            UnityPlayer.UnitySendMessage("PR_GameManager", "ReceiveBluetoothMessageFromConsole", "Test message");
        });

        mUnityPlayer.addView(testButton, ViewGroup.LayoutParams.WRAP_CONTENT, ViewGroup.LayoutParams.WRAP_CONTENT);
    }

    // Function receives messages from Unity
    public void sendBluetoothMessageToConsole(String message) {
        Log.d("sendBluetooth", message);
        // TODO: actually send this to the console
    }

    @Override
    protected void onDestroy() {
        super.onDestroy();
        instance = null;
    }
}
