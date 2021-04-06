package com.nf.battlechip.interfaces;

import android.content.Context;

import com.google.android.gms.auth.api.signin.GoogleSignIn;
import com.google.android.gms.auth.api.signin.GoogleSignInClient;
import com.google.android.gms.auth.api.signin.GoogleSignInOptions;

public class GoogleHelper {

    public static GoogleSignInClient getClient(Context context) {
        return GoogleSignIn.getClient(context, new GoogleSignInOptions.Builder(GoogleSignInOptions.DEFAULT_SIGN_IN)
                .requestEmail()
                .requestIdToken("110124662131-u2iujveotr90trk5phf99i2kq2agomgf.apps.googleusercontent.com")
                .build());
    }
}
