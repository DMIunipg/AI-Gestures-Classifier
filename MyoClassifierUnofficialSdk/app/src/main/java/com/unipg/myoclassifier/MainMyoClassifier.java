package com.unipg.myoclassifier;

import android.app.Activity;
import android.bluetooth.BluetoothDevice;
import android.content.Intent;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.view.View;
import android.widget.Button;
import android.widget.TextView;

import com.vassiliev.androidfilebrowser.FileBrowserActivity;

import eu.darken.myolib.BaseMyo;
import eu.darken.myolib.Myo;
import eu.darken.myolib.MyoCmds;
import eu.darken.myolib.msgs.MyoMsg;
import eu.darken.myolib.processor.emg.EmgProcessor;
import eu.darken.myolib.processor.imu.ImuProcessor;

public class MainMyoClassifier extends com.unipg.blearduinoandroid.BLEArduinoAppCompatActivity {

    static {
        System.loadLibrary("GesturesClassifier");
    }
    //default activity
    final Activity mActivityForButton = this;
    private final int REQUEST_CODE_PICK_FILE = 1;
    //native classifier
    MyoNativeClassifierManager mNClassifierManager;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main_myo_classifier);
        //connect
        ((Button)findViewById(R.id.btConnect)).setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                scanLeDevice();
            }
        });
        //event
        ((Button)findViewById(R.id.btSearchMyo)).setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                //create transition
                Intent intent = new Intent(MainMyoClassifier.this, MyoList.class);
                //change
                MainMyoClassifier.this.startActivityForResult(intent, 0);
            }
        });

        //event
        ((Button)findViewById(R.id.btOpenModel)).setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                //create transition
                Intent fileExploreIntent = new Intent(
                        com.vassiliev.androidfilebrowser.FileBrowserActivity.INTENT_ACTION_SELECT_FILE,
                        null,
                        mActivityForButton,
                        com.vassiliev.androidfilebrowser.FileBrowserActivity.class
                );
                fileExploreIntent.putExtra(
                        com.vassiliev.androidfilebrowser.FileBrowserActivity.showCannotReadParameter,
                        false);
                //change
                MainMyoClassifier.this.startActivityForResult(fileExploreIntent, REQUEST_CODE_PICK_FILE);
            }
        });

        //alloc native classifier
        mNClassifierManager = new MyoNativeClassifierManager(MyoNativeClassifierManager.kNN);
    }

    @Override
    public  void onArduinoConnected(BluetoothDevice device){
        ((Button)findViewById(R.id.btConnect)).post(new Runnable() {
            @Override
            public void run() {
                ((Button)findViewById(R.id.btConnect)).setEnabled(false);
                ((Button)findViewById(R.id.btConnect)).setText("Connected to Arduino");
            }
        });
    }

    @Override
    public  void onArduinoDisconnected(){
        ((Button)findViewById(R.id.btConnect)).post(new Runnable() {
            @Override
            public void run() {
                ((Button)findViewById(R.id.btConnect)).setEnabled(true);
                ((Button)findViewById(R.id.btConnect)).setText("Connect to Arduino");
            }
        });
    }

    @Override
    public void onActivityResult(int requestCode, int resultCode, Intent data) {
        super.onActivityResult(requestCode, resultCode, data);
        //load file
        if(requestCode == REQUEST_CODE_PICK_FILE)
        {
            if(resultCode == this.RESULT_OK)
            {
                String path =  data.getStringExtra(FileBrowserActivity.returnFileParameter);
                mNClassifierManager.loadModel(path);
                //to do... check success to load
                //set filter
                mNClassifierManager.setProbabilityFilter(0.5);
                //set callback
                mNClassifierManager.setListener(new MyoNativeClassifierManager.ClassifierListener() {
                    public void classified(final String className)
                    {
                        final TextView tvClassifier= (TextView)MainMyoClassifier.this.findViewById(R.id.tvClassifier);
                        tvClassifier.post(new Runnable() {
                            @Override
                            public void run() {
                                tvClassifier.setText(className);
                                if(isConnected())
                                {
                                    if(className.equals("normal"))
                                        arduinoMove('x');
                                    else if(className.equals("fist"))
                                        arduinoMove('^');
                                    else if(className.equals("right"))
                                        arduinoMove('>');
                                    else if(className.equals("left"))
                                        arduinoMove('<');
                                }
                            }
                        });
                    }
                });
            }
        }
        //if myo was selected
        if(MyoList.getMyoSelected()!=null)
            onConnectedToMyo(MyoList.getMyoSelected());
    }

    private void onConnectedToMyo(Myo myo){
        //connect to myo
        myo.connect();
        myo.setConnectionSpeed(BaseMyo.ConnectionSpeed.HIGH);
        myo.writeSleepMode(MyoCmds.SleepMode.NEVER, null);
        myo.writeMode(MyoCmds.EmgMode.FILTERED,
                      MyoCmds.ImuMode.RAW,
                      MyoCmds.ClassifierMode.DISABLED,
                      null);
        myo.writeUnlock(MyoCmds.UnlockType.HOLD, new Myo.MyoCommandCallback() {
            @Override
            public void onCommandDone(Myo myo, MyoMsg msg) {
                myo.writeVibrate(MyoCmds.VibrateType.LONG, null);
            }
        });
        //create processors
        EmgProcessor      emgProcessor = new EmgProcessor();
        ImuProcessor      imuProcessor = new ImuProcessor();
        MyoNativeListener myoNListener = new MyoNativeListener();
        //attach all to myo
        myo.addProcessor(emgProcessor);
        myo.addProcessor(imuProcessor);
        //add listeners
        emgProcessor.addListener(myoNListener);
        imuProcessor.addListener(myoNListener);

    }
}
