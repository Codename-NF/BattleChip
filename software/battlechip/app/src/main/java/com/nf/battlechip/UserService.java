package com.nf.battlechip;

import com.nf.battlechip.pojo.User;

import retrofit2.Call;
import retrofit2.Retrofit;
import retrofit2.converter.gson.GsonConverterFactory;
import retrofit2.http.GET;
import retrofit2.http.Path;

public interface UserService {

    String SERVER_URL = "http://ec2-54-151-115-99.us-west-1.compute.amazonaws.com:9091/";

    @GET("user/{username}")
    Call<User> getUser(@Path("username") String username);

    static UserService getUserService() {
        return new Retrofit.Builder()
                .baseUrl(SERVER_URL)
                .addConverterFactory(GsonConverterFactory.create())
                .build().create(UserService.class);
    }
}
