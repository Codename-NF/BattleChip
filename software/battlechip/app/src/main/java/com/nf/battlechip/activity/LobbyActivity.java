package com.nf.battlechip.activity;

import android.content.Intent;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.widget.Button;
import android.widget.TextView;

import androidx.appcompat.app.AppCompatActivity;

import com.nf.battlechip.R;

public class LobbyActivity extends AppCompatActivity {

    private static boolean gameIsReady = false;
    private static boolean failedToCreateGame = false;
    private static LobbyActivity instance = null;

    private static final String LOBBY_DEBUG = "Lobby";

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_lobby);

        instance = this;

        // check flags and transition if they are set
        if (gameIsReady) {
            gameIsReady = false;
            startUnityActivity();
        }
        if (failedToCreateGame) {
            failedToCreateGame = false;
            handleCreateGameFailed();
        }
    }

    @Override
    protected void onDestroy() {
        super.onDestroy();
        instance = null;
    }

    public static void gamesIsReady() {
        Log.d(LOBBY_DEBUG, "Game is ready");
        if (instance == null) { // set flag if lobby has not been loaded yet
            gameIsReady = true;
        } else {
            instance.startUnityActivity();
        }
    }

    public static void failedToCreateGame() {
        Log.d(LOBBY_DEBUG, "Failed to create game");
        if (instance == null) { // set flag if lobby has not been loaded yet
            failedToCreateGame = true;
        } else {
            instance.handleCreateGameFailed();
        }
    }

    private void handleCreateGameFailed() {
        Button returnButton = findViewById(R.id.return_button);
        TextView lobbyMessage = findViewById(R.id.lobby_text_view);

        returnButton.setVisibility(View.VISIBLE);
        returnButton.setOnClickListener(view -> startActivity(new Intent(this, MainActivity.class).setFlags(Intent.FLAG_ACTIVITY_CLEAR_TASK | Intent.FLAG_ACTIVITY_NEW_TASK)));
        lobbyMessage.setText(getString(R.string.failed_to_create_game_text));
    }

    private void startUnityActivity() {
        startActivity(new Intent(this, MainUnityActivity.class).setFlags(Intent.FLAG_ACTIVITY_REORDER_TO_FRONT | Intent.FLAG_ACTIVITY_SINGLE_TOP));
    }

}