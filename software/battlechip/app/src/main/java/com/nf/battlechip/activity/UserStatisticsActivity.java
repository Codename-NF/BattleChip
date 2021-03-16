package com.nf.battlechip.activity;

import android.graphics.Typeface;
import android.os.Bundle;
import android.text.Spannable;
import android.text.SpannableString;
import android.text.style.ForegroundColorSpan;
import android.text.style.StyleSpan;
import android.util.Log;
import android.widget.TextView;

import com.nf.battlechip.R;
import com.nf.battlechip.RetrofitHelper;
import com.nf.battlechip.UserService;
import com.nf.battlechip.pojo.User;

import retrofit2.Call;
import retrofit2.Callback;
import retrofit2.Response;
import retrofit2.internal.EverythingIsNonNull;

public class UserStatisticsActivity extends SetThemeActivity {

    private static final String USER_STATISTICS_DEBUG = "User_Statistics";

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_user_statistics);

        getUser();
    }

    public void getUser() {
        UserService service = RetrofitHelper.getUserService();
        Call<User> call = service.getUser();
        call.enqueue(new Callback<User>() {
            @Override
            @EverythingIsNonNull
            public void onResponse(Call<User> call, Response<User> response) {
                if (response.isSuccessful()) {
                    User user = response.body();

                    if (user != null) {
                        updateViews(user);
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

    private void updateViews(User user) {
        TextView userView = findViewById(R.id.user_name_text_view);
        TextView statsView = findViewById(R.id.statistics_text_view);

        Spannable userSpannable = new SpannableString("Name: " + user.getFirstName() + " " + user.getLastName());
        userSpannable.setSpan(new ForegroundColorSpan(color), 0, "Name:".length(), Spannable.SPAN_INCLUSIVE_EXCLUSIVE);
        userSpannable.setSpan(new StyleSpan(Typeface.BOLD), 0, "Name:".length(), Spannable.SPAN_INCLUSIVE_EXCLUSIVE);
        userView.setText(userSpannable, TextView.BufferType.SPANNABLE);

        String statsString = "Wins: " + user.getWins()
                + "\nLosses: " + user.getLosses();
        Spannable statSpannable = new SpannableString(statsString);
        statSpannable.setSpan(new ForegroundColorSpan(color), 0, "Wins:".length(), Spannable.SPAN_INCLUSIVE_EXCLUSIVE);
        statSpannable.setSpan(new StyleSpan(Typeface.BOLD), 0, "Wins:".length(), Spannable.SPAN_INCLUSIVE_EXCLUSIVE);
        statSpannable.setSpan(new ForegroundColorSpan(color), statsString.indexOf("Losses:"), statsString.indexOf("Losses:") + "Losses:".length(), Spannable.SPAN_INCLUSIVE_EXCLUSIVE);
        statSpannable.setSpan(new StyleSpan(Typeface.BOLD), statsString.indexOf("Losses:"), statsString.indexOf("Losses:") + "Losses:".length(), Spannable.SPAN_INCLUSIVE_EXCLUSIVE);
        statsView.setText(statSpannable, TextView.BufferType.SPANNABLE);
    }

}