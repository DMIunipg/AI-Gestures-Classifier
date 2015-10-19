package com.unipg.myoclassifier;

import android.app.Activity;
import android.app.AlertDialog;
import android.app.Dialog;
import android.bluetooth.BluetoothDevice;
import android.content.DialogInterface;
import android.content.Intent;
import android.content.pm.ActivityInfo;
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

/**
 * Created by Gabriele on 12/10/15.
 */
public class MainMyoClassifier extends com.unipg.blearduinoandroid.BLEArduinoAppCompatActivity {

    static {
        System.loadLibrary("GesturesClassifier");
    };

    /**
     * constant, it used when view list myo is shown
     */
    private final int REQUEST_CODE_MYO_LIST = 1;

    /**
     * constant, it used when file manager is shown
     */
    private final int REQUEST_CODE_PICK_FILE = 2;

    /**
     * Pointer to classifier manager
     */
    private MyoNativeClassifierManager mNClassifierManager;

    /**
     * Set this variable to true before to call finish method when you wanna kill all the process
     */
    private boolean mKillApp = false;

    /**
     * Pointer to last myo connected
     */
    Myo mMyo = null;

    /**
     * @brief executeArduinoAction, called when when was classified a new gesture
     * @param className, class name of the last gesture classified
     */
    protected void executeArduinoAction(String className) {
        if(isConnected())  {
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

    /**
     * @brief bluetoothUnsupportedDialog, show a dialog to notify the user about the bluetooth connection support 
     */
    protected void bluetoothUnsupportedDialog()
    {
        final AlertDialog.Builder closeApplicationDialog = new AlertDialog.Builder(MainMyoClassifier.this);
        closeApplicationDialog.setMessage("Bluetooth unsupported");
        closeApplicationDialog.setCancelable(false);
        closeApplicationDialog.setPositiveButton("Close application",
                new DialogInterface.OnClickListener() {
                    public void onClick(DialogInterface dialog, int id) {
                        dialog.dismiss();
                        MainMyoClassifier.this.mKillApp = true;
                        MainMyoClassifier.this.finish();
                    }
                });
        final AlertDialog dialog=closeApplicationDialog.create();
        runOnUiThread(new Runnable() {
            @Override
            public void run() {
                dialog.show();
            }
        });
    }

    /**
     * @brief onCreate
     * @param savedInstanceState
     */
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        //set orientation
        super.setRequestedOrientation(ActivityInfo.SCREEN_ORIENTATION_SENSOR_PORTRAIT);
        //set view
        setContentView(R.layout.activity_main_myo_classifier);
        //if bluetooth unsupported then not attach listeners to ui
        if(!isBluetoothSupport())
        {
            return;
        }
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
                        false
                );
                fileExploreIntent.putExtra(
                        FileBrowserActivity.filterExtension,
                        ".*\\.(knn|svm|net)"
                );
                //change
                MainMyoClassifier.this.startActivityForResult(fileExploreIntent, REQUEST_CODE_PICK_FILE);
            }
        });

    }


    /**
     * @brief onStart
     */
    @Override
    protected void onStart() {
        super.onStart();
        //if bluetooth unsupported then show alert dialog
        if(!isBluetoothSupport())
        {
            bluetoothUnsupportedDialog();
        }
    }

    /**
     * Myo Relative Layout Widget
     */
    private RelativeLayout mUIRLMain = null;
    private Thread mUIRLMainMyoAnimationThread = null;
    private boolean mUIRLMainMyoAnimationThreadLoop = false;

    /**
     * @brief onArduinoStartScan
     */
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

    /**
     * @brief onArduinoEndScan
     */
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

    /**
     * @brief onArduinoConnected
     * @param device
     */
    @Override
    protected  void onArduinoConnected(BluetoothDevice device){
        ((Button)findViewById(R.id.btConnect)).post(new Runnable() {
            @Override
            public void run() {
                ((Button)findViewById(R.id.btConnect)).setText("Arduino Connected");
            }
        });
    }

    /**
     * @brief  onArduinoDisconnected
     */
    @Override
    protected  void onArduinoDisconnected(){
        ((Button)findViewById(R.id.btConnect)).post(new Runnable() {
            @Override
            public void run() {
                ((Button) findViewById(R.id.btConnect)).setText("Connect to Arduino");
            }
        });
    }

    /**
     * @brief onDestroy, called when application was destroyed
     */
    @Override
    protected void  onDestroy()
    {
        //close connections
        if(isBluetoothSupport()) {
            disconnectMyo();
            disconnectArduino();
        }
        else if(mKillApp) {
            //kill process
            android.os.Process.killProcess(android.os.Process.myPid());
        }
        //call parent
        super.onDestroy();
    }

    /**
     * @brief onActivityResult, called when a child activity was closed
     * @param requestCode 
     * @param resultCode  
     * @param data        
     */
    @Override
    public void onActivityResult(int requestCode, int resultCode, Intent data) {
        //load file
        if(requestCode == REQUEST_CODE_PICK_FILE)  {

            if(resultCode == this.RESULT_OK) {
                String path =  data.getStringExtra(FileBrowserActivity.returnFileParameter);
                //default type NONE
                int typeClassifier = -1;
                //get type of model
                if(path.endsWith("knn"))
                    typeClassifier = MyoNativeClassifierManager.kNN;
                else if(path.endsWith("svm"))
                    typeClassifier = MyoNativeClassifierManager.SVM;
                else if(path.endsWith("net"))
                    typeClassifier = MyoNativeClassifierManager.RBFNETWORK;
                //fail to load
                if(typeClassifier == -1) return;
                //alloc native classifier
                mNClassifierManager = new MyoNativeClassifierManager(MyoNativeClassifierManager.kNN);
                //load
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
                            }
                        });
                        //execute an Arduino action
                        executeArduinoAction(className);
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

    /**
     * @brief onConnectedToMyo
     * @param myo, connection object
     */
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

    /**
     * @brief disconnectMyo, call this when you want disconnet from Myo
     */
    public void disconnectMyo() {
        if (mMyo != null &&
                (  mMyo.getConnectionState() == BaseMyo.ConnectionState.CONNECTED||
                   mMyo.getConnectionState() == BaseMyo.ConnectionState.CONNECTING  )) {
            mMyo.disconnect();
        }
        mMyo = null;
    }
}
