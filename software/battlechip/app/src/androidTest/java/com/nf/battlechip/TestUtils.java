package com.nf.battlechip;

import androidx.test.espresso.ViewInteraction;
import androidx.test.espresso.matcher.ViewMatchers;

import static androidx.test.espresso.Espresso.onView;
import static androidx.test.espresso.assertion.ViewAssertions.matches;
import static androidx.test.espresso.matcher.ViewMatchers.isDisplayed;

public class TestUtils {

    static void ensureMainMenuIsVisible() {
        // Ensure that all expected buttons are visible
        int[] ids = {R.id.single_player_button, R.id.multi_player_button, R.id.player_stats_button, R.id.options_button, R.id.logout_button};

        for (int id : ids) {
            ViewInteraction button = onView(ViewMatchers.withId(id));
            button.check(matches(isDisplayed()));
        }
    }
}
