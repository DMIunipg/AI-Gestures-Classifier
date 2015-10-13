package com.unipg.myoclassifier;

import android.app.Application;
import android.test.ApplicationTestCase;

/**
 * <a href="http://d.android.com/tools/testing/testing_android.html">Testing Fundamentals</a>
 */
public class ApplicationTest extends ApplicationTestCase<Application> {

    static {
        System.loadLibrary("GesturesClassifier");
    }

    public ApplicationTest() {
        super(Application.class);
    }
}