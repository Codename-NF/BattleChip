package com.nf.battlechip.pojo;

import androidx.annotation.Nullable;

import com.google.gson.annotations.SerializedName;

public class User {
    @Nullable
    @SerializedName("first_name")
    private String firstName;

    @Nullable
    @SerializedName("last_name")
    private String lastName;

    @Nullable
    private String email;

    private int wins;

    private int losses;

    public String getFirstName() {
        return firstName == null ? "" : firstName;
    }

    public String getLastName() {
        return lastName == null ? "" : lastName;
    }

    public String getEmail() {
        return email == null ? "" : email;
    }

    public int getWins() {
        return wins;
    }

    public int getLosses() {
        return losses;
    }
}
