package com.nf.battlechip.activity;

import android.content.Intent;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.widget.Toast;

import androidx.annotation.Nullable;

import com.google.android.gms.auth.api.signin.GoogleSignIn;
import com.google.android.gms.auth.api.signin.GoogleSignInAccount;
import com.google.android.gms.auth.api.signin.GoogleSignInClient;
import com.google.android.gms.common.api.ApiException;
import com.google.android.gms.tasks.Task;
import com.nf.battlechip.GoogleHelper;
import com.nf.battlechip.R;
import com.nf.battlechip.RetrofitHelper;
import com.nf.battlechip.pojo.User;

import retrofit2.Call;
import retrofit2.Callback;
import retrofit2.Response;
import retrofit2.internal.EverythingIsNonNull;

public class LoginActivity extends SetThemeActivity {

    private GoogleSignInClient client;

    private static final int SIGN_IN_REQUEST_CODE = 1;

    private static final String LOGIN_DEBUG_TAG = "Login";

    // https://developers.google.com/identity/sign-in/android/sign-in
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        client = GoogleHelper.getClient(this);

        // user is already signed in
        GoogleSignInAccount account = GoogleSignIn.getLastSignedInAccount(this);
        if (account != null) {
            Log.d(LOGIN_DEBUG_TAG, "Already signed in!");
            startMainActivity();
        }

        setContentView(R.layout.activity_login);

        findViewById(R.id.sign_in_button).setOnClickListener(this::signIn);
    }

    public void signIn(View v) {
        startActivityForResult(client.getSignInIntent(), SIGN_IN_REQUEST_CODE);
    }

    @Override
    protected void onActivityResult(int requestCode, int resultCode, @Nullable Intent data) {
        super.onActivityResult(requestCode, resultCode, data);

        if (requestCode == SIGN_IN_REQUEST_CODE) {
            handleSignedInUser(GoogleSignIn.getSignedInAccountFromIntent(data));
        }
    }

    private void handleSignedInUser(Task<GoogleSignInAccount> completedTask) {
        try {
            GoogleSignInAccount account = completedTask.getResult(ApiException.class);
            if (account != null) {
                Log.d(LOGIN_DEBUG_TAG, account.getDisplayName() + " " + account.getEmail());
                startMainActivity();
            }
        } catch (ApiException e) {
            Log.d(LOGIN_DEBUG_TAG, e.toString());
        }
    }

    private void startMainActivity() {
        RetrofitHelper.initRetrofit(client);
        Call<User> login = RetrofitHelper.getUserService().login();
        login.enqueue(new Callback<User>() {
            @Override
            @EverythingIsNonNull
            public void onResponse(Call<User> call, Response<User> response) {
                if (response.isSuccessful() && response.body() != null) {
                    startActivity(new Intent(LoginActivity.this, MainActivity.class)
                            .setFlags(Intent.FLAG_ACTIVITY_CLEAR_TASK | Intent.FLAG_ACTIVITY_NEW_TASK)
                            .putExtra("playerId", response.body().getPlayerId()));
                    finish();
                } else {
                    Log.d(LOGIN_DEBUG_TAG, response.toString());
                    Toast.makeText(LoginActivity.this, "Failed to login", Toast.LENGTH_SHORT).show();
                }
            }

            @Override
            @EverythingIsNonNull
            public void onFailure(Call<User> call, Throwable t) {
                Log.d(LOGIN_DEBUG_TAG, t.toString());
                Toast.makeText(LoginActivity.this, "Failed to login", Toast.LENGTH_SHORT).show();
            }
        });
    }
}