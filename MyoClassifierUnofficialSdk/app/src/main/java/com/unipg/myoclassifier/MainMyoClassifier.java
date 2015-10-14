package com.unipg.myoclassifier;

import android.bluetooth.BluetoothDevice;
import android.content.Intent;
import android.graphics.Color;
import android.graphics.PorterDuff;
import android.graphics.drawable.Drawable;
import android.os.Bundle;
import android.view.View;
import android.widget.Button;
import android.widget.RelativeLayout;
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
    //return code when select a file
    private final int REQUEST_CODE_MYO_LIST = 1;
    private final int REQUEST_CODE_PICK_FILE = 2;
    //native classifier
    private MyoNativeClassifierManager mNClassifierManager;
    //last myo connected
    Myo mMyo = null;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main_myo_classifier);
        //connect
        ((Button)findViewById(R.id.btConnect)).setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                if(isConnected())
                    disconnectArduino();
                else
                    scanLeDevice();
            }
        });
        //event
        ((Button)findViewById(R.id.btSearchMyo)).setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                //disconnect from myo
                if(mMyo!=null) {
                    disconnectMyo();
                    ((Button)findViewById(R.id.btSearchMyo)).post(new Runnable() {
                        @Override
                        public void run() {
                            ((Button) findViewById(R.id.btSearchMyo)).setText("Search Myo");
                        }
                    });
                }
                else {
                    //create transition
                    Intent intent = new Intent(MainMyoClassifier.this, MyoList.class);
                    //change
                    MainMyoClassifier.this.startActivityForResult(intent, REQUEST_CODE_MYO_LIST);
                }
            }
        });

        //event
        ((Button)findViewById(R.id.btOpenModel)).setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                //create transition
                Intent fileExploreIntent = new Intent(
                        MainMyoClassifier.this,
                        com.vassiliev.androidfilebrowser.FileBrowserActivity.class
                );
                fileExploreIntent.putExtra(FileBrowserActivity.setActionParameter,
                        FileBrowserActivity.INTENT_ACTION_SELECT_FILE
                );
                fileExploreIntent.putExtra(
                        FileBrowserActivity.showCannotReadParameter,
                        false);
                //change
                MainMyoClassifier.this.startActivityForResult(fileExploreIntent, REQUEST_CODE_PICK_FILE);
            }
        });

        //alloc native classifier
        mNClassifierManager = new MyoNativeClassifierManager(MyoNativeClassifierManager.kNN);
    }



    /**
     * Myo Relative Layout Widget
     */
    private RelativeLayout mUIRLMain = null;
    private Thread mUIRLMainMyoAnimationThread = null;
    private boolean mUIRLMainMyoAnimationThreadLoop = false;

    @Override
    protected void onArduinoStartScan() {
        //get main relative layout
        mUIRLMain = (RelativeLayout)findViewById(R.id.rlMainUi);
        //ui main
        if(mUIRLMain != null) {
            mUIRLMain.post(new Runnable() {
                @Override
                public void run() {
                    mUIRLMain.setBackgroundResource(R.drawable.wifi_arduino_rs);
                    mUIRLMain.getBackground().setColorFilter(Color.argb(0, 0, 0, 0), PorterDuff.Mode.MULTIPLY);
                }
            });
        }
        //start animation
        mUIRLMainMyoAnimationThread = new Thread(new Runnable() {
            @Override
            public void run() {
                //animation factor
                double sfactor = Math.PI*0.5;
                //update
                while(mUIRLMain != null && mUIRLMainMyoAnimationThreadLoop) {
                    try {
                        Thread.sleep(33, 0);
                    } catch (Exception e) {
                        //none
                    }
                    //next color
                    sfactor += 0.1f;
                    final int color = (int)(((Math.sin(sfactor)+1.0)*0.5)*255);
                    //post to ui thread
                    mUIRLMain.post(new Runnable() {
                        @Override
                        public void run() {
                            Drawable background = mUIRLMain.getBackground();
                            if(background != null) {
                                mUIRLMain.getBackground().setColorFilter(
                                        Color.argb(color, color / 3, color / 3, color),
                                        PorterDuff.Mode.MULTIPLY);
                            }
                        }
                    });
                }
            }
        });
        mUIRLMainMyoAnimationThreadLoop = true;
        mUIRLMainMyoAnimationThread.start();
    }

    @Override
    protected void onArduinoEndScan() {
        if(mUIRLMainMyoAnimationThreadLoop && mUIRLMainMyoAnimationThread!=null) {
            mUIRLMainMyoAnimationThreadLoop = false;
            mUIRLMainMyoAnimationThread = null;
            mUIRLMain.post(new Runnable() {
                @Override
                public void run() {
                    mUIRLMain.setBackgroundResource(0);
                }
            });
        }
    }

    @Override
    protected  void onArduinoConnected(BluetoothDevice device){
        ((Button)findViewById(R.id.btConnect)).post(new Runnable() {
            @Override
            public void run() {
                ((Button)findViewById(R.id.btConnect)).setText("Arduino Connected");
            }
        });
    }

    @Override
    protected  void onArduinoDisconnected(){
        ((Button)findViewById(R.id.btConnect)).post(new Runnable() {
            @Override
            public void run() {
                ((Button)findViewById(R.id.btConnect)).setText("Connect to Arduino");
            }
        });
    }

    @Override
    public void onActivityResult(int requestCode, int resultCode, Intent data) {
        //load file
        if(requestCode == REQUEST_CODE_PICK_FILE)  {

            if(resultCode == this.RESULT_OK) {
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
        else if(requestCode == REQUEST_CODE_MYO_LIST) {
            //if myo was selected
            if (MyoList.getMyoSelected() != null)
                onConnectedToMyo(MyoList.getMyoSelected());
        }
        else {
            //send to super
            super.onActivityResult(requestCode, resultCode, data);
        }
    }

    private void onConnectedToMyo(Myo myo){
        //save last myo
        mMyo = myo;
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
        //change button
        ((Button)findViewById(R.id.btSearchMyo)).post(new Runnable() {
            @Override
            public void run() {
                ((Button)findViewById(R.id.btSearchMyo)).setText("Myo Connected");
            }
        });
    }

    public void disconnectMyo() {
        if (mMyo != null &&
                (  mMyo.getConnectionState() == BaseMyo.ConnectionState.CONNECTED||
                   mMyo.getConnectionState() == BaseMyo.ConnectionState.CONNECTING  )) {
            mMyo.disconnect();
        }
        mMyo = null;
    }
}
