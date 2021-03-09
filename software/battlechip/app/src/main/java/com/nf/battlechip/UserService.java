package com.nf.battlechip;

import com.nf.battlechip.pojo.User;

import retrofit2.Call;
import retrofit2.http.GET;
import retrofit2.http.POST;
import retrofit2.http.Path;

public interface UserService {

    @GET("user/{username}")
    Call<User> getUser(@Path("username") String username);

    @POST("login")
    Call<Void> login();

}
