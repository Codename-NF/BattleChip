package com.nf.battlechip;

import androidx.test.espresso.ViewInteraction;
import androidx.test.ext.junit.rules.ActivityScenarioRule;
import androidx.test.ext.junit.runners.AndroidJUnit4;

import com.nf.battlechip.activity.LoginActivity;

import org.junit.Rule;
import org.junit.Test;
import org.junit.runner.RunWith;

import static androidx.test.espresso.Espresso.onView;
import static androidx.test.espresso.action.ViewActions.click;
import static androidx.test.espresso.assertion.ViewAssertions.matches;
import static androidx.test.espresso.matcher.ViewMatchers.isDisplayed;
import static androidx.test.espresso.matcher.ViewMatchers.withId;

@RunWith(AndroidJUnit4.class)
public class LogoutTests {

    @Rule
    public ActivityScenarioRule<LoginActivity> mActivityTestRule = new ActivityScenarioRule<>(LoginActivity.class);

    @Test
    public void logout() {
        // check that the logout button is visible and click it
        ViewInteraction logoutButton = onView(withId(R.id.logout_button));
        logoutButton.check(matches(isDisplayed()));
        logoutButton.perform(click());

        // check that we are now back in the LoginActivity
        ViewInteraction signInButton = onView(withId(R.id.sign_in_button));
        signInButton.check(matches(isDisplayed()));
    }
}
