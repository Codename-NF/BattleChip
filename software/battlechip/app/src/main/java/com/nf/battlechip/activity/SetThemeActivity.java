package com.nf.battlechip.activity;

import android.content.SharedPreferences;
import android.os.Bundle;
import android.preference.PreferenceManager;

import androidx.appcompat.app.AppCompatActivity;
import androidx.core.content.ContextCompat;

import com.nf.battlechip.R;

public class SetThemeActivity extends AppCompatActivity {

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setTheme();
    }

    protected void setTheme() {
        SharedPreferences preferences = PreferenceManager.getDefaultSharedPreferences(this);
        setTheme(preferences.getInt("theme", R.style.Theme_BattleChip));
        // Rerender logo since the primary color may have changed
        ContextCompat.getDrawable(this, R.drawable.ic_battlechip_logo).invalidateSelf();
    }
}
