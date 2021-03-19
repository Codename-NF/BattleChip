package com.nf.battlechip.pojo;

import androidx.annotation.Nullable;

import com.google.gson.annotations.SerializedName;

import java.text.SimpleDateFormat;
import java.util.Date;
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

    @SerializedName("player_one_name")
    private String playerOneName;

    @SerializedName("player_two_name")
    private String playerTwoName;

    private long date;

    public String getPlayerOne() {
        return Optional.ofNullable(playerOne).orElse("");
    }

    public String getPlayerTwo() {
        return Optional.ofNullable(playerTwo).orElse("");
    }

    public String getWinner() {
        return Optional.ofNullable(winner).orElse("");
    }

    public String getPlayerOneName() {
        return Optional.ofNullable(playerOneName).orElse("");
    }

    public String getPlayerTwoName() {
        return Optional.ofNullable(playerTwoName).orElse("");
    }

    public int getPlayerOneScore() {
        return playerOneScore;
    }

    public int getPlayerTwoScore() {
        return playerTwoScore;
    }

    public String getDate() {
        return new SimpleDateFormat("MM/dd/yyyy HH:mm").format(new Date(date / 1000));
    }
}
