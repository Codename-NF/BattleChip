package com.nf.battlechip.activity;

import android.content.Intent;
import android.os.Bundle;
import android.util.Log;
import android.view.View;

import androidx.annotation.Nullable;
import androidx.appcompat.app.AppCompatActivity;

import com.google.android.gms.auth.api.signin.GoogleSignIn;
import com.google.android.gms.auth.api.signin.GoogleSignInAccount;
import com.google.android.gms.auth.api.signin.GoogleSignInClient;
import com.google.android.gms.auth.api.signin.GoogleSignInOptions;
import com.google.android.gms.common.api.ApiException;
import com.google.android.gms.tasks.Task;
import com.nf.battlechip.R;
import com.nf.battlechip.RetrofitHelper;

public class LoginActivity extends AppCompatActivity {

    private GoogleSignInClient client;

    private static final int SIGN_IN_REQUEST_CODE = 1;

    private static final String LOGIN_DEBUG_TAG = "Login";

    // https://developers.google.com/identity/sign-in/android/sign-in
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        // user is already signed in
        GoogleSignInAccount account = GoogleSignIn.getLastSignedInAccount(this);
        if (account != null) {
            Log.d(LOGIN_DEBUG_TAG, "Already signed in!");
            startMainActivity(account.getIdToken());
        }

        setContentView(R.layout.activity_login);

        client = GoogleSignIn.getClient(this, new GoogleSignInOptions.Builder(GoogleSignInOptions.DEFAULT_SIGN_IN)
                .requestEmail()
                .requestIdToken("110124662131-u2iujveotr90trk5phf99i2kq2agomgf.apps.googleusercontent.com")
                .build());

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
                startMainActivity(account.getIdToken());
            }
        } catch (ApiException e) {
            Log.d(LOGIN_DEBUG_TAG, e.toString());
        }
    }

    private void startMainActivity(String idToken) {
        RetrofitHelper.initRetrofit(idToken);
        // TODO: check token with backend first
        startActivity(new Intent(this, MainActivity.class).setFlags(Intent.FLAG_ACTIVITY_CLEAR_TASK | Intent.FLAG_ACTIVITY_NEW_TASK));
        finish();
    }
}