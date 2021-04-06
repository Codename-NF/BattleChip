package com.nf.battlechip.activity;

import android.content.SharedPreferences;
import android.os.Bundle;
import android.preference.PreferenceManager;

import com.nf.battlechip.R;
import com.unity3d.player.UnityPlayerActivity;

public class MainUnityActivity extends UnityPlayerActivity {

    private static MainUnityActivity instance = null;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        instance = this;
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
        // bitwise & promotes the color to a long, then removes the leading 1s (makes the number positive)
        return preferences.getLong("color", instance.getColor(R.color.purple_500) & 0xFFFFFFFFL);
    }

}
