package com.nf.battlechip.pojo;

import androidx.annotation.Nullable;

import com.google.gson.annotations.SerializedName;

import java.util.Optional;

public class Match {

    @Nullable
    @SerializedName("player_one")
    private String playerOne;

    @Nullable
    @SerializedName("player_two")
    private String playerTwo;

    @Nullable
    private String winner;

    @SerializedName("player_one_score")
    private int playerOneScore;

    @SerializedName("player_two_score")
    private int playerTwoScore;

    public String getPlayerOne() {
        return Optional.ofNullable(playerOne).orElse("");
    }

    public String getPlayerTwo() {
        return Optional.ofNullable(playerTwo).orElse("");
    }

    public String getWinner() {
        return Optional.ofNullable(winner).orElse("");
    }

    public int getPlayerOneScore() {
        return playerOneScore;
    }

    public int getPlayerTwoScore() {
        return playerTwoScore;
    }
}
