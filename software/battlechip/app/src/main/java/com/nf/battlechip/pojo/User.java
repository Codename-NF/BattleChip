package com.nf.battlechip.pojo;

import androidx.annotation.Nullable;

public class User {
    @Nullable
    private String username;

    private int wins;

    private int losses;

    public String getUsername() {
        return username == null ? "" : username;
    }

    public int getWins() {
        return wins;
    }

    public int getLosses() {
        return losses;
    }
}
