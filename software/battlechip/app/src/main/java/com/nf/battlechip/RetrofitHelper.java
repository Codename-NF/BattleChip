package com.nf.battlechip;

import okhttp3.OkHttpClient;
import okhttp3.logging.HttpLoggingInterceptor;
import retrofit2.Retrofit;
import retrofit2.converter.gson.GsonConverterFactory;

public class RetrofitHelper {

    private static String SERVER_URL = "http://ec2-54-151-115-99.us-west-1.compute.amazonaws.com:9091/";
    private static UserService userService = null;
    private static Retrofit retrofit = null;

    public static UserService getUserService() {
        if (userService == null) {
            userService = retrofit.create(UserService.class);
        }
        return userService;
    }

    public static void initRetrofit(String customHeader) {
        if (retrofit == null) {
            OkHttpClient.Builder httpClientBuilder = new OkHttpClient().newBuilder();
            HttpLoggingInterceptor loggingInterceptor = new HttpLoggingInterceptor();
            loggingInterceptor.level(HttpLoggingInterceptor.Level.BODY);
            httpClientBuilder.addNetworkInterceptor(loggingInterceptor);

            if (customHeader != null) {
                httpClientBuilder.addInterceptor(chain ->
                        chain.proceed(chain.request().newBuilder().addHeader("id_token", customHeader).build())
                );
            }
            retrofit = new Retrofit.Builder()
                    .baseUrl(SERVER_URL)
                    .addConverterFactory(GsonConverterFactory.create())
                    .client(httpClientBuilder.build()).build();
        }
    }
}

/*

 */