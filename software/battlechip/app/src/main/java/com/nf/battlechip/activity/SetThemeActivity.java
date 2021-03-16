package com.nf.battlechip.activity;

import android.content.SharedPreferences;
import android.os.Bundle;
import android.preference.PreferenceManager;
import android.util.TypedValue;

import androidx.annotation.ColorInt;
import androidx.appcompat.app.AppCompatActivity;
import androidx.core.content.ContextCompat;

import com.nf.battlechip.R;

import java.util.Objects;

public class SetThemeActivity extends AppCompatActivity {

    @ColorInt
    protected int color;

    @Override

    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setTheme();
    }

    protected void setTheme() {
        SharedPreferences preferences = PreferenceManager.getDefaultSharedPreferences(this);
        setTheme(preferences.getInt("theme", R.style.Theme_BattleChip));
        TypedValue value = new TypedValue();
        this.getTheme().resolveAttribute(R.attr.colorPrimary, value, true);
        color = value.data;

        // Rerender logo since the primary color may have changed
        Objects.requireNonNull(ContextCompat.getDrawable(this, R.drawable.ic_battlechip_logo)).invalidateSelf();
    }
}
