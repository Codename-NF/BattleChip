package com.nf.battlechip;

import android.content.Context;
import android.preference.PreferenceManager;

import androidx.test.espresso.ViewInteraction;
import androidx.test.ext.junit.rules.ActivityScenarioRule;
import androidx.test.ext.junit.runners.AndroidJUnit4;
import androidx.test.filters.LargeTest;
import androidx.test.uiautomator.By;
import androidx.test.uiautomator.UiDevice;

import com.nf.battlechip.activity.LoginActivity;

import org.hamcrest.Matchers;
import org.junit.Assert;
import org.junit.BeforeClass;
import org.junit.Rule;
import org.junit.Test;
import org.junit.runner.RunWith;

import java.util.regex.Pattern;

import static androidx.test.espresso.Espresso.onView;
import static androidx.test.espresso.action.ViewActions.click;
import static androidx.test.espresso.assertion.ViewAssertions.matches;
import static androidx.test.espresso.matcher.ViewMatchers.isDisplayed;
import static androidx.test.espresso.matcher.ViewMatchers.withId;
import static androidx.test.espresso.matcher.ViewMatchers.withText;
import static androidx.test.platform.app.InstrumentationRegistry.getInstrumentation;
import static com.nf.battlechip.TestUtils.ensureMainMenuIsVisible;

@RunWith(AndroidJUnit4.class)
public class MainMenuTests {

    private static UiDevice device;

    @Rule
    public ActivityScenarioRule<LoginActivity> mActivityTestRule = new ActivityScenarioRule<>(LoginActivity.class);

    @BeforeClass
    public static void setup() {
        device = UiDevice.getInstance(getInstrumentation());
    }

    @LargeTest
    @Test
    public void singlePlayer() {
        ViewInteraction singlePlayerButton = onView(Matchers.allOf(withId(R.id.single_player_button), isDisplayed()));
        singlePlayerButton.perform(click());

        ViewInteraction cancelButton = checkButtonsAreVisible(new String[]{"Easy", "Hard", "Cancel"});
        cancelButton.perform(click());

        ensureMainMenuIsVisible();
    }

    @Test
    public void multiPlayer() {
        ViewInteraction multiPlayerButton = onView(Matchers.allOf(withId(R.id.multi_player_button), isDisplayed()));
        multiPlayerButton.perform(click());

        ViewInteraction cancelButton = checkButtonsAreVisible(new String[]{"Join", "Create", "Cancel"});
        cancelButton.perform(click());

        ensureMainMenuIsVisible();
    }

    @Test
    public void playerStats() {
        ViewInteraction statsButton = onView(Matchers.allOf(withId(R.id.player_stats_button), isDisplayed()));
        statsButton.perform(click());

        ViewInteraction matchHistoryText = onView(withId(R.id.match_history_text_view));
        matchHistoryText.check(matches(isDisplayed()));

        // Check that player name and stats are shown
        Assert.assertTrue(device.hasObject(By.text("Name: Paul Lin")));
        Assert.assertTrue(device.hasObject(By.text(Pattern.compile("Wins: [0-9]*"))));
        Assert.assertTrue(device.hasObject(By.text(Pattern.compile("Losses: [0-9]*"))));

        // Check that there are some game results
        Assert.assertTrue(device.hasObject(By.text(Pattern.compile("Opponent: .*"))));
        Assert.assertTrue(device.hasObject(By.text(Pattern.compile("Hits: .*"))));

        device.pressBack();
        ensureMainMenuIsVisible();
    }

    @Test
    public void options() {
        Context context = getInstrumentation().getTargetContext();

        ViewInteraction optionsButton = onView(Matchers.allOf(withId(R.id.options_button), isDisplayed()));
        optionsButton.perform(click());

        ViewInteraction confirmButton = onView(withText("Confirm"));
        // ensure that cancelling leads back to the main menu
        ViewInteraction cancelButton = onView(withText("Cancel"));
        cancelButton.check(matches(isDisplayed()));
        cancelButton.perform(click());
        ensureMainMenuIsVisible();

        int[] ids = {R.id.chip_blue, R.id.chip_green, R.id.chip_orange, R.id.chip_pink, R.id.chip_purple, R.id.chip_teal};
        int[] themes = {R.style.Theme_BlueBattleChip, R.style.Theme_GreenBattleChip, R.style.Theme_OrangeBattleChip,
                R.style.Theme_PinkBattleChip, R.style.Theme_BattleChip, R.style.Theme_TealBattleChip};

        for (int i = 0; i < ids.length; i++) {
            optionsButton.perform(click());

            // Select each color
            ViewInteraction chip = onView(withId(ids[i]));
            chip.check(matches(isDisplayed()));
            chip.perform(click());
            confirmButton.perform(click());

            // check that main menu is shown and the theme color is correct
            ensureMainMenuIsVisible();
            Assert.assertEquals(PreferenceManager.getDefaultSharedPreferences(context).getInt("theme", 0), themes[i]);
        }
    }

    // Checks that there are buttons with the provided button texts
    // Returns the ViewInteraction corresponding to the last button
    private ViewInteraction checkButtonsAreVisible(String[] buttonTexts) {
        ViewInteraction button = null;
        for (String buttonText : buttonTexts) {
            button = onView(withText(buttonText));
            button.check(matches(isDisplayed()));
        }
        return button;
    }

}