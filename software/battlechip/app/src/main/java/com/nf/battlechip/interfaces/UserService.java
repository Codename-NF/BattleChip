package com.nf.battlechip.interfaces;

import com.nf.battlechip.pojo.Matches;
import com.nf.battlechip.pojo.User;

import retrofit2.Call;
import retrofit2.http.GET;
import retrofit2.http.POST;

public interface UserService {

    // Called when displaying statistics
    @GET("user")
    Call<User> getUser();

    // Called when logging in
    @POST("login")
    Call<User> login();

    // Called when displaying statistics
    @GET("match")
    Call<Matches> getMatches();

}
