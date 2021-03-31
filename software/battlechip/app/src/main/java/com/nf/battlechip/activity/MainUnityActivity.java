package com.nf.battlechip.activity;

import android.content.SharedPreferences;
import android.os.Bundle;
import android.preference.PreferenceManager;
import android.util.Log;
import android.view.Gravity;
import android.view.ViewGroup;
import android.widget.Button;

import com.nf.battlechip.R;
import com.nf.battlechip.UnityMessage;
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
            UnityMessage.processBluetoothMessage("gameStart true");
        });
        mUnityPlayer.addView(testButton, ViewGroup.LayoutParams.WRAP_CONTENT, ViewGroup.LayoutParams.WRAP_CONTENT);
    }

    @Override
    protected void onDestroy() {
        super.onDestroy();
        instance = null;
    }

    public static long getColor() {
        if (instance == null) {
            return 0;
        }
        SharedPreferences preferences = PreferenceManager.getDefaultSharedPreferences(instance);
        long colorVal = instance.getColor(R.color.purple_500);
        Log.d("color", "" + String.valueOf(colorVal & 0xFFFFFFFFL));
        return preferences.getLong("color", instance.getColor(R.color.orange_500) & 0xFFFFFFFFL);
    }

}
