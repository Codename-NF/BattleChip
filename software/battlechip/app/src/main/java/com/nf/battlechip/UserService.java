package com.nf.battlechip;

import com.nf.battlechip.pojo.User;

import retrofit2.Call;
import retrofit2.http.GET;
import retrofit2.http.POST;

public interface UserService {

    @GET("user")
    Call<User> getUser();

    @POST("login")
    Call<Void> login();

}
