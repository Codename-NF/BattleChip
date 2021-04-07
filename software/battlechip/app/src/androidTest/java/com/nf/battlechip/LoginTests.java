package com.nf.battlechip;

import androidx.test.espresso.ViewInteraction;
import androidx.test.ext.junit.rules.ActivityScenarioRule;
import androidx.test.ext.junit.runners.AndroidJUnit4;
import androidx.test.filters.LargeTest;
import androidx.test.uiautomator.UiDevice;
import androidx.test.uiautomator.UiObjectNotFoundException;
import androidx.test.uiautomator.UiSelector;

import com.nf.battlechip.activity.LoginActivity;

import org.junit.Assert;
import org.junit.FixMethodOrder;
import org.junit.Rule;
import org.junit.Test;
import org.junit.runner.RunWith;
import org.junit.runners.MethodSorters;

import static androidx.test.espresso.Espresso.onView;
import static androidx.test.espresso.action.ViewActions.click;
import static androidx.test.espresso.assertion.ViewAssertions.matches;
import static androidx.test.espresso.matcher.ViewMatchers.isDisplayed;
import static androidx.test.espresso.matcher.ViewMatchers.withId;
import static androidx.test.platform.app.InstrumentationRegistry.getInstrumentation;
import static com.nf.battlechip.TestUtils.ensureMainMenuIsVisible;

@RunWith(AndroidJUnit4.class)
@FixMethodOrder(MethodSorters.NAME_ASCENDING)
public class LoginTests {

    @Rule
    public ActivityScenarioRule<LoginActivity> mActivityTestRule = new ActivityScenarioRule<>(LoginActivity.class);

    @LargeTest
    @Test
    public void test1_notLoggedIn() {
        UiDevice device = UiDevice.getInstance(getInstrumentation());

        ViewInteraction signInButton = onView(withId(R.id.sign_in_button));

        // sign-in button should be initially visible
        signInButton.check(matches(isDisplayed()));

        // click then press back
        signInButton.perform(click());
        device.pressBack();
        signInButton.check(matches(isDisplayed()));

        // click and then select the account
        signInButton.perform(click());
        try {
            device.findObject(new UiSelector().text("Paul Lin")).clickAndWaitForNewWindow();
        } catch (UiObjectNotFoundException e) {
            Assert.assertFalse("User not found", false);
        }

        // Allow background location access
        try {
            device.findObject(new UiSelector().text("Allow")).click();
        } catch (UiObjectNotFoundException e) {
            Assert.assertFalse("Allow not found", false);
        }

        ensureMainMenuIsVisible();
    }

    @Test
    public void test_2loggedIn() {
        ensureMainMenuIsVisible();
    }

}