package com.nf.battlechip.interfaces;

import com.google.android.gms.auth.api.signin.GoogleSignInClient;
import com.google.android.gms.tasks.Tasks;

import java.util.concurrent.ExecutionException;

import okhttp3.OkHttpClient;
import okhttp3.logging.HttpLoggingInterceptor;
import retrofit2.Retrofit;
import retrofit2.converter.gson.GsonConverterFactory;

public class RetrofitHelper {

    private static final String SERVER_URL = "http://ec2-54-151-115-99.us-west-1.compute.amazonaws.com:9091/";
    private static UserService userService = null;
    private static Retrofit retrofit = null;

    public static UserService getUserService() {
        if (userService == null) {
            userService = retrofit.create(UserService.class);
        }
        return userService;
    }

    public static void initRetrofit(GoogleSignInClient client) {
        if (retrofit == null) {
            OkHttpClient.Builder httpClientBuilder = new OkHttpClient().newBuilder();
            HttpLoggingInterceptor loggingInterceptor = new HttpLoggingInterceptor();
            loggingInterceptor.level(HttpLoggingInterceptor.Level.BODY);
            httpClientBuilder.addNetworkInterceptor(loggingInterceptor);

            // Interceptor allows us to get the latest ID token from Google Sign-In each time
            httpClientBuilder.addInterceptor(chain -> {
                String idToken = "";
                try {
                    idToken = Tasks.await(client.silentSignIn()).getIdToken();
                } catch (ExecutionException | InterruptedException e) {
                    e.printStackTrace();
                }
                if (idToken == null) {
                    idToken = "";
                }
                return chain.proceed(chain.request().newBuilder().addHeader("id_token", idToken).build());
            });

            retrofit = new Retrofit.Builder()
                    .baseUrl(SERVER_URL)
                    .addConverterFactory(GsonConverterFactory.create())
                    .client(httpClientBuilder.build()).build();
        }
    }
}
