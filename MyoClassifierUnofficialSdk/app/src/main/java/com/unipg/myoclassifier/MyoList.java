package com.unipg.myoclassifier;

import android.app.Activity;
import android.os.Bundle;
import android.view.View;
import android.view.ViewGroup;
import android.widget.Button;
import android.widget.LinearLayout;

import java.util.List;

import eu.darken.myolib.Myo;
import eu.darken.myolib.MyoConnector;
import eu.darken.myolib.msgs.MyoMsg;

/**
 * Created by Gabriele on 12/10/15.
 */
public class MyoList extends Activity {

    /**
     * myo selected
     */
    static protected Myo mMyoSelected = null;

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
        //put "selected myo"  to null
        mMyoSelected = null;
        //serach myos
        MyoConnector connector = new MyoConnector(getBaseContext());
        connector.scan(2000, new MyoConnector.ScannerCallback() {
            @Override
            public void onScanFinished(List<Myo> myos) {
                for(Myo myo:myos)
                {
                    //device button
                    final Button deviceButton=MyoList.this.addButton(myo);
                    //add name
                    myo.readDeviceName(new Myo.ReadDeviceNameCallback() {
                        @Override
                        public void onDeviceNameRead(Myo myo, MyoMsg msg, final String deviceName) {
                            deviceButton.post(new Runnable() {
                                @Override
                                public void run() {
                                    deviceButton.setText(deviceName);
                                }
                            });
                        }
                    });
                }
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
        //get list
        final LinearLayout uiListMyo=(LinearLayout)findViewById(R.id.myoListLinearLayout);
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
                MyoList.this.setResult(RESULT_OK,null);
                MyoList.this.finish();
            }
        });
        //update ui
        uiListMyo.post(new Runnable() {
            @Override
            public void run() {
                //push into layout
                uiListMyo.addView(deviceButton,
                        new ViewGroup.LayoutParams(
                                ViewGroup.LayoutParams.MATCH_PARENT,
                                ViewGroup.LayoutParams.WRAP_CONTENT)
                );
            }
        });
        //return ui object
        return deviceButton;
    }


}
