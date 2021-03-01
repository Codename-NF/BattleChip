package com.nf.battlechip.activity;

import android.os.Bundle;
import android.util.Log;
import android.widget.TextView;

import androidx.appcompat.app.AppCompatActivity;

import com.nf.battlechip.R;
import com.nf.battlechip.UserService;
import com.nf.battlechip.pojo.User;

import retrofit2.Call;
import retrofit2.Callback;
import retrofit2.Response;
import retrofit2.internal.EverythingIsNonNull;

public class UserStatisticsActivity extends AppCompatActivity {

    private static final String USER_STATISTICS_DEBUG = "User_Statistics";

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_user_statistics);

        getUser();
    }

    public void getUser() {
        UserService service = UserService.getUserService();
        Call<User> call = service.getUser("Freetumble"); // TODO: replace this with variable user
        call.enqueue(new Callback<User>() {
            @Override
            @EverythingIsNonNull
            public void onResponse(Call<User> call, Response<User> response) {
                if (response.isSuccessful()) {
                    TextView userView = findViewById(R.id.user_name_text_view);
                    User user = response.body();
                    userView.setText("Name: " + user.getUsername() + "\nWins: " + user.getWins() + "\nLosses: " + user.getLosses());
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

}