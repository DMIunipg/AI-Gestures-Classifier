package com.unipg.myoclassifierparrot;

import android.app.Activity;
import android.graphics.Color;
import android.graphics.PorterDuff;
import android.graphics.drawable.Drawable;
import android.os.Bundle;
import android.view.View;
import android.view.ViewGroup;
import android.widget.Button;
import android.widget.LinearLayout;

import java.util.List;

import eu.darken.myolib.BaseMyo;
import eu.darken.myolib.Myo;
import eu.darken.myolib.MyoConnector;
import eu.darken.myolib.msgs.MyoMsg;

/**
 * Created by Gabriele on 12/10/15.
 */
public class MyoList extends Activity {

    /**
     * time to scan
     */
    private int mMyoScanTime = 10000;

    /**
     * myo selected
     */
    static protected Myo mMyoSelected = null;

    /**
     * Myo scanner
     */
    private MyoConnector mConnector = null;

    /**
     * Myo List Widget
     */
    private LinearLayout mUIListMyo = null;

    /**
     * Animation thread
     */
    private Thread mUIListMyoAnimationThread = null;
    
    /**
     * Animation loop flag
     */
    private boolean mUIListMyoAnimationThreadLoop = false;

    /**
     * @breif startLoadingAnimation
     */
    private void startLoadingAnimation() {
        mUIListMyoAnimationThread = new Thread(new Runnable() {
            @Override
            public void run() {
                //animation factor
                double sfactor = Math.PI*0.5;
                //update
                while(mUIListMyo != null && mUIListMyoAnimationThreadLoop) {
                    try {
                        Thread.sleep(33, 0);
                    } catch (Exception e) {
                        //none
                    }
                    //next color
                    sfactor += 0.1f;
                    final int color = (int)(((Math.sin(sfactor)+1.0)*0.5)*255);
                    //post to ui thread
                    mUIListMyo.post(new Runnable() {
                        @Override
                        public void run() {
                            Drawable background = mUIListMyo.getBackground();
                            if(background != null) {
                                mUIListMyo.getBackground().setColorFilter(
                                        Color.argb(color, color / 3, color / 3, color),
                                        PorterDuff.Mode.MULTIPLY);
                            }
                        }
                    });
                }
            }
        });
        mUIListMyoAnimationThreadLoop = true;
        mUIListMyoAnimationThread.start();
    }

    /**
     * @brief stopLoadingAnimation
     */
    private void stopLoadingAnimation(){
        if(mUIListMyoAnimationThreadLoop && mUIListMyoAnimationThread!=null) {
            mUIListMyoAnimationThreadLoop = false;
            mUIListMyoAnimationThread = null;
        }
    }

    /**
     * View loading backgound
     * @brief showLoading
     */
    private void showLoading(){
        if(mUIListMyo != null) {
            mUIListMyo.post(new Runnable() {
                @Override
                public void run() {
                    mUIListMyo.setBackgroundResource(R.drawable.wifi_rs);
                    mUIListMyo.setPadding(100, 100, 100, 100);
                    mUIListMyo.getBackground().setColorFilter(Color.argb(0, 0, 0, 0), PorterDuff.Mode.MULTIPLY);
                }
            });
            startLoadingAnimation();
        }
    }

    /**
     * @brief hideLoading, hide the loading icon
     */
    private  void hideLoading() {
        stopLoadingAnimation();

        if(mUIListMyo != null) {
            mUIListMyo.post(new Runnable() {
                @Override
                public void run() {
                    mUIListMyo.setBackgroundResource(0);
                }
            });
        }
    }

    /**
     * @brief getMyoSelected
     * @return myo selected
     */
    static public Myo getMyoSelected()
    {
        return mMyoSelected;
    }

    /**
     * @brief onCreate
     * @param savedInstanceState
     */
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_myo_list);
        //get list ui
        mUIListMyo=(LinearLayout)findViewById(R.id.myoListLinearLayout);
        //put "selected myo"  to null
        mMyoSelected = null;
        //start scan
        startScanning();
    }

    /**
     * @brief onDestroy
     */
    @Override
    protected void onDestroy() {
        super.onDestroy();
        if(mConnector!=null)
            this.mConnector.stopScan();
    }

    /**
     * @brief startScanning
     */
    private void startScanning(){
        //stop last search
        if(mConnector!=null){
            mConnector.stopScan();
        }
        //show loading image
        showLoading();
        //serach myos
        mConnector = new MyoConnector(getBaseContext());
        mConnector.scan(mMyoScanTime, new MyoConnector.ScannerCallback() {
            @Override
            public void onScanFinished(List<Myo> myos) {
                hideLoading();
                if (mUIListMyo != null) {
                    if (mUIListMyo.getChildCount() == 0) {
                        //add retry button
                        addRetryButton();
                    }
                }
            }

            @Override
            public void onScanFind(Myo myo) {
                //device button
                final Button deviceButton = MyoList.this.addButton(myo);
                //add name
                myo.readDeviceName(new Myo.ReadDeviceNameCallback() {
                    @Override
                    public void onDeviceNameRead(final Myo myo, MyoMsg msg, final String deviceName) {
                        //no connect to myo
                        if( myo.getConnectionState() == BaseMyo.ConnectionState.CONNECTED||
                            myo.getConnectionState() == BaseMyo.ConnectionState.CONNECTING  ) {
                            myo.disconnect();
                        }
                        //set name text
                        deviceButton.post(new Runnable() {
                            @Override
                            public void run() {
                                //set text
                                deviceButton.setText(deviceName);
                            }
                        });
                    }
                });

            }
        });
    }

    /**
     * @brief addButton
     * @param myo
     * @param name
     */
    public Button addButton(final Myo myo)
    {
        //get names
        final String nameDevice = myo.getDeviceName();
        final String nameManufacturer = myo.getManufacturerName();
        //new button
        final Button deviceButton = new Button(this);
        deviceButton.setText("acquired name..");
        deviceButton.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                MyoList.mMyoSelected = myo;
                MyoList.this.setResult(RESULT_OK, null);
                MyoList.this.finish();
                if (mConnector != null)
                    MyoList.this.mConnector.stopScan();
            }
        });
        deviceButton.setTextColor(Color.BLACK);
        deviceButton.setBackgroundResource(R.drawable.buttom_standard);
        //update ui
        mUIListMyo.post(new Runnable() {
            @Override
            public void run() {
                //push into layout
                mUIListMyo.addView(deviceButton,
                           new ViewGroup.LayoutParams(
                                   ViewGroup.LayoutParams.MATCH_PARENT,
                                   ViewGroup.LayoutParams.WRAP_CONTENT)
                );
            }
        });
        //return ui object
        return deviceButton;
    }

    /**
     * Add the retry button
     * @breif addRetryButton
     */
    private void addRetryButton()
    {
        final Button deviceButton = new Button(getBaseContext());
        deviceButton.setText("Retry to search");
        deviceButton.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                startScanning();
                mUIListMyo.removeAllViews();
            }
        });
        deviceButton.setTextColor(Color.BLACK);
        deviceButton.setBackgroundResource(R.drawable.buttom_standard);
        //update ui
        mUIListMyo.post(new Runnable() {
            @Override
            public void run() {
                //push into layout
                mUIListMyo.addView(deviceButton,
                        new ViewGroup.LayoutParams(
                                ViewGroup.LayoutParams.MATCH_PARENT,
                                ViewGroup.LayoutParams.WRAP_CONTENT)
                );
            }
        });
    }


}
