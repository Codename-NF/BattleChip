package com.nf.battlechip.activity;

import android.os.Bundle;
import android.text.Html;
import android.util.Log;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.TextView;

import androidx.annotation.ColorInt;
import androidx.annotation.ColorRes;
import androidx.annotation.NonNull;
import androidx.recyclerview.widget.DividerItemDecoration;
import androidx.recyclerview.widget.LinearLayoutManager;
import androidx.recyclerview.widget.RecyclerView;

import com.google.android.gms.auth.api.signin.GoogleSignIn;
import com.nf.battlechip.R;
import com.nf.battlechip.RetrofitHelper;
import com.nf.battlechip.UserService;
import com.nf.battlechip.pojo.Match;
import com.nf.battlechip.pojo.Matches;
import com.nf.battlechip.pojo.User;

import java.util.List;

import retrofit2.Call;
import retrofit2.Callback;
import retrofit2.Response;
import retrofit2.internal.EverythingIsNonNull;

import static androidx.core.text.HtmlCompat.FROM_HTML_MODE_LEGACY;

public class UserStatisticsActivity extends SetThemeActivity {

    private static final String USER_STATISTICS_DEBUG = "User_Statistics";

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_user_statistics);

        getUser();
        getMatches();
    }

    private void getUser() {
        UserService service = RetrofitHelper.getUserService();
        Call<User> call = service.getUser();
        call.enqueue(new Callback<User>() {
            @Override
            @EverythingIsNonNull
            public void onResponse(Call<User> call, Response<User> response) {
                if (response.isSuccessful()) {
                    User user = response.body();

                    if (user != null) {
                        updateUserView(user);
                        updateStatsView(user);
                    }
                }
                Log.d(USER_STATISTICS_DEBUG, "Received a response for getUser");
            }

            @Override
            @EverythingIsNonNull
            public void onFailure(Call<User> call, Throwable t) {
                Log.d(USER_STATISTICS_DEBUG, "getUser failed " + t);
            }
        });
    }

    private void updateUserView(User user) {
        TextView userView = findViewById(R.id.user_name_text_view);
        userView.setText(Html.fromHtml(String.format(getString(R.string.statistics_name_text), color, user.getFirstName(), user.getLastName()), Html.FROM_HTML_MODE_LEGACY));
    }

    private void updateStatsView(User user) {
        TextView winsView = findViewById(R.id.wins_text_view);
        TextView lossesView = findViewById(R.id.losses_text_view);
        winsView.setText(Html.fromHtml(String.format(getString(R.string.statistics_win_text), color, user.getWins()), Html.FROM_HTML_MODE_LEGACY));
        lossesView.setText(Html.fromHtml(String.format(getString(R.string.statistics_loss_text), color, user.getLosses()), Html.FROM_HTML_MODE_LEGACY));
    }

    private void getMatches() {
        UserService service = RetrofitHelper.getUserService();
        Call<Matches> call = service.getMatches();
        call.enqueue(new Callback<Matches>() {
            @Override
            @EverythingIsNonNull
            public void onResponse(Call<Matches> call, Response<Matches> response) {
                if (response.isSuccessful()) {
                    updateMatchesView(response.body().getMatches());
                }
                Log.d(USER_STATISTICS_DEBUG, "Received a response for getMatches");
            }

            @Override
            @EverythingIsNonNull
            public void onFailure(Call<Matches> call, Throwable t) {
                Log.d(USER_STATISTICS_DEBUG, "getMatches failed " + t);
            }
        });
    }

    private void updateMatchesView(List<Match> matches) {
        RecyclerView recyclerView = findViewById(R.id.matches_recycler_view);
        LinearLayoutManager layoutManager = new LinearLayoutManager(this);
        DividerItemDecoration dividerItemDecoration = new DividerItemDecoration(recyclerView.getContext(),
                layoutManager.getOrientation());

        recyclerView.setLayoutManager(new LinearLayoutManager(this));
        recyclerView.addItemDecoration(dividerItemDecoration);
        recyclerView.setAdapter(new MatchAdapter(matches));
    }

    private class MatchAdapter extends RecyclerView.Adapter<MatchAdapter.ViewHolder> {

        private final List<Match> matches;

        private class ViewHolder extends RecyclerView.ViewHolder {

            private final View view;

            public ViewHolder(View view) {
                super(view);
                this.view = view;
            }

            public void setMatch(Match match) {
                String email = GoogleSignIn.getLastSignedInAccount(UserStatisticsActivity.this).getEmail();
                boolean isPlayerOne = email.equals(match.getPlayerOne());
                setResultView(match, isPlayerOne);
                setDateView(match.getDate());
                setOpponentView(isPlayerOne ? match.getPlayerTwoName() : match.getPlayerOneName());
            }

            private void setResultView(Match match, boolean isPlayerOne) {
                boolean winnerIsPlayerOne = match.getWinner().equals(match.getPlayerOne());
                TextView resultView = view.findViewById(R.id.result_text_view);
                TextView scoreTextView = view.findViewById(R.id.score_text_view);
                String winOrLossMessage;
                @ColorRes int playerColourRes, opponentColourRes;
                @ColorInt int playerColour, opponentColour;

                if ((isPlayerOne && winnerIsPlayerOne) || (!isPlayerOne && !winnerIsPlayerOne)) {
                    winOrLossMessage = "Win";
                    playerColourRes = R.color.winning_green;
                } else {
                    winOrLossMessage = "Loss";
                    playerColourRes = R.color.losing_red;
                }
                opponentColourRes = playerColourRes == R.color.winning_green ? R.color.losing_red : R.color.winning_green;
                playerColour = getColor(playerColourRes);
                opponentColour = getColor(opponentColourRes);

                if (isPlayerOne) {
                    scoreTextView.setText(Html.fromHtml(String.format(getString(R.string.hits_text),
                            color, playerColour, match.getPlayerOneScore(), opponentColour, match.getPlayerTwoScore()), FROM_HTML_MODE_LEGACY));
                } else {
                    scoreTextView.setText(Html.fromHtml(String.format(getString(R.string.hits_text),
                            color, playerColour, match.getPlayerTwoScore(), opponentColour, match.getPlayerOneScore()), FROM_HTML_MODE_LEGACY));
                }

                resultView.setText(Html.fromHtml(String.format(getString(R.string.match_result_text), playerColour, winOrLossMessage), Html.FROM_HTML_MODE_LEGACY));
            }

            private void setDateView(String date) {
                TextView dateView = view.findViewById(R.id.date_text_view);
                dateView.setText(date);
            }

            private void setOpponentView(String opponent) {
                TextView opponentView = view.findViewById(R.id.opponent_text_view);
                opponentView.setText(Html.fromHtml(String.format(getString(R.string.opponent_text), color, opponent), Html.FROM_HTML_MODE_LEGACY));
            }
        }

        public MatchAdapter(List<Match> matches) {
            this.matches = matches;
        }

        @NonNull
        @Override
        public ViewHolder onCreateViewHolder(@NonNull ViewGroup parent, int viewType) {
            View view = LayoutInflater.from(parent.getContext()).inflate(R.layout.recycler_view_match, parent, false);
            return new ViewHolder(view);
        }

        @Override
        public void onBindViewHolder(@NonNull ViewHolder holder, int position) {
            holder.setMatch(matches.get(position));
        }

        @Override
        public int getItemCount() {
            return matches.size();
        }
    }

}