package com.unipg.myoclassifierparrot;

import android.Manifest;
import android.app.AlertDialog;
import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothDevice;
import android.bluetooth.BluetoothManager;
import android.content.ComponentName;
import android.content.Context;
import android.content.DialogInterface;
import android.content.Intent;
import android.content.IntentFilter;
import android.content.ServiceConnection;
import android.content.pm.ActivityInfo;
import android.content.pm.PackageManager;
import android.graphics.Color;
import android.graphics.PorterDuff;
import android.graphics.drawable.Drawable;
import android.location.LocationManager;
import android.os.Bundle;
import android.os.IBinder;
import android.provider.Settings;
import android.support.annotation.NonNull;
import android.support.v4.app.ActivityCompat;
import android.support.v4.content.ContextCompat;
import android.support.v7.app.AppCompatActivity;
import android.support.v4.content.LocalBroadcastManager;
import android.util.Log;
import android.view.View;
import android.widget.Button;
import android.widget.RelativeLayout;
import android.widget.TextView;

import com.parrot.arsdk.arcommands.ARCOMMANDS_JUMPINGSUMO_MEDIARECORDEVENT_PICTUREEVENTCHANGED_ERROR_ENUM;
import com.parrot.arsdk.arcontroller.ARCONTROLLER_DEVICE_STATE_ENUM;
import com.parrot.arsdk.arcontroller.ARControllerCodec;
import com.parrot.arsdk.arcontroller.ARFrame;
import com.parrot.arsdk.ardiscovery.ARDISCOVERY_PRODUCT_ENUM;
import com.unipg.parrot.JumpingSumo;
import com.vassiliev.androidfilebrowser.FileBrowserActivity;

import eu.darken.myolib.BaseMyo;
import eu.darken.myolib.Myo;
import eu.darken.myolib.MyoCmds;
import eu.darken.myolib.msgs.MyoMsg;
import eu.darken.myolib.processor.emg.EmgProcessor;
import eu.darken.myolib.processor.imu.ImuProcessor;

import com.parrot.arsdk.ARSDK;
import com.parrot.arsdk.ardiscovery.ARDiscoveryDeviceService;
import com.parrot.arsdk.ardiscovery.ARDiscoveryService;
import com.parrot.arsdk.ardiscovery.receivers.ARDiscoveryServicesDevicesListUpdatedReceiver;
import com.parrot.arsdk.ardiscovery.receivers.ARDiscoveryServicesDevicesListUpdatedReceiverDelegate;

import java.util.ArrayList;
import java.util.List;

import static com.parrot.arsdk.arcontroller.ARCONTROLLER_DEVICE_STATE_ENUM.ARCONTROLLER_DEVICE_STATE_RUNNING;
import static com.parrot.arsdk.arcontroller.ARCONTROLLER_DEVICE_STATE_ENUM.ARCONTROLLER_DEVICE_STATE_STOPPED;

/**
 * Created by Gabriele on 12/10/15.
 */
public class MainMyoClassifierParrot extends AppCompatActivity implements BaseMyo.ConnectionListener {

    static {
        System.loadLibrary("GesturesClassifier");
        ARSDK.loadSDKLibs();
    };


    /**
     * Runtime permission request id
     */
    private static final int  REQUEST_READ_EXTERNAL_STORAGE_AND_ACCESS_CORE_LOCATION = 200;
    private static final int  SETTING_ENABLE_LOCATION = 201;
    /**
     * Bluetooth
     */
    private BluetoothManager mBluetoothManager = null;
    private BluetoothAdapter mBluetoothAdapter = null;

    /**
     * Runtime permission
     */
    void getRuntimePermission(){
        //test permission storage
        boolean acceptSTORAGE_LOCATION =
                ContextCompat.checkSelfPermission(this, Manifest.permission.READ_EXTERNAL_STORAGE)  == PackageManager.PERMISSION_GRANTED &&
                        ContextCompat.checkSelfPermission(this, Manifest.permission.ACCESS_COARSE_LOCATION) == PackageManager.PERMISSION_GRANTED &&
                        ContextCompat.checkSelfPermission(this, Manifest.permission.ACCESS_FINE_LOCATION) == PackageManager.PERMISSION_GRANTED;
        //show message
        if(!acceptSTORAGE_LOCATION)
        {
            requestRuntimePermission();
        }
    }

    /**
     * Location permission
     */
    public void getLocationPermission()
    {
        if(!testLocationPermission()) {
            this.startActivityForResult(new Intent(Settings.ACTION_LOCATION_SOURCE_SETTINGS), SETTING_ENABLE_LOCATION);
        }
    }

    /**
     * Test Location permission
     */
    public boolean testLocationPermission(){
        LocationManager lm = (LocationManager)getBaseContext().getSystemService(Context.LOCATION_SERVICE);
        boolean gps_enabled = false;
        boolean network_enabled = false;

        try {
            gps_enabled = lm.isProviderEnabled(LocationManager.GPS_PROVIDER);
        } catch(Exception ex) {}

        try {
            network_enabled = lm.isProviderEnabled(LocationManager.NETWORK_PROVIDER);
        } catch(Exception ex) {}

        return gps_enabled || network_enabled;

    }


    /**
     * Runtime permission request
     */
    public void requestRuntimePermission()
    {
        ActivityCompat.requestPermissions(this, new String[]{
                        Manifest.permission.READ_EXTERNAL_STORAGE,
                        Manifest.permission.ACCESS_COARSE_LOCATION,
                        Manifest.permission.ACCESS_FINE_LOCATION,
                        Manifest.permission.BLUETOOTH,
                        Manifest.permission.BLUETOOTH_ADMIN,
                        Manifest.permission.INTERNET},
                REQUEST_READ_EXTERNAL_STORAGE_AND_ACCESS_CORE_LOCATION);
    }

    /**
     * Callback after user accept (or not) the permission request
     * @param requestCode
     * @param permissions
     * @param grantResults
     */
    @Override
    public void onRequestPermissionsResult(int requestCode, String permissions[], int[] grantResults)
    {
        switch (requestCode)
        {
            case REQUEST_READ_EXTERNAL_STORAGE_AND_ACCESS_CORE_LOCATION:
                if (grantResults.length > 0 && grantResults[0] == PackageManager.PERMISSION_GRANTED)
                {
                    //success
                }
                else
                {
                    AlertDialog.Builder alert = new AlertDialog.Builder(MainMyoClassifierParrot.this);
                    alert.setTitle("Error");
                    alert.setMessage("Grant phone permission before using application!");
                    alert.setPositiveButton("OK", new DialogInterface.OnClickListener() {
                        public void onClick(DialogInterface dialog, int id) {
                            requestRuntimePermission();
                        }
                    });
                    alert.setNegativeButton("Close", new DialogInterface.OnClickListener() {
                        public void onClick(DialogInterface dialog, int id) {
                            MainMyoClassifierParrot.this.finish();

                        }
                    });
                    AlertDialog dialog = alert.create();
                    dialog.show();
                }
                break;
            default:
                break;
        }
    }

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
     * Search JumpingSumo Listener
     */
    public interface FindJumpingSumoListener {
        public void onFindJumpingSumo(ARDiscoveryDeviceService drone);
        public void onNotFindJumpingSumo();
    }

    /**
     * Search JumpingSumo
     */
    class FindJumpingSumo
    {

        private ARDiscoveryService mArdiscoveryService = null;
        private ServiceConnection mArdiscoveryServiceConnection = null;
        private ARDiscoveryServicesDevicesListUpdatedReceiver mArdiscoveryServicesDevicesListUpdatedReceiver = null;
        private final List<ARDiscoveryDeviceService> mMatchingDrones = new ArrayList<>();
        private FindJumpingSumoListener mListener = null;
        private boolean mStartDiscoveryAfterConnection = false;

        private final ARDiscoveryServicesDevicesListUpdatedReceiverDelegate mDiscoveryListener =
        new ARDiscoveryServicesDevicesListUpdatedReceiverDelegate() {
            @Override
            public void onServicesDevicesListUpdated() {
                if (mArdiscoveryService != null) {
                    // clear current list
                    mMatchingDrones.clear();
                    List<ARDiscoveryDeviceService> deviceList = mArdiscoveryService.getDeviceServicesArray();

                    if (deviceList != null)
                    {
                        for (ARDiscoveryDeviceService service : deviceList)
                        {
                            mMatchingDrones.add(service);
                        }
                    }
                    notifyServiceDiscoveredJumpingSumo();
                }
            }
        };

        /**
         * FindJumpingSumo
         */
        public FindJumpingSumo(@NonNull FindJumpingSumoListener listener){
            //save listener
            mListener = listener;
            //service
            mArdiscoveryServicesDevicesListUpdatedReceiver = new ARDiscoveryServicesDevicesListUpdatedReceiver(mDiscoveryListener);
            // registerReceivers
            LocalBroadcastManager localBroadcastMgr = LocalBroadcastManager.getInstance(getBaseContext());
            localBroadcastMgr.registerReceiver(
                    mArdiscoveryServicesDevicesListUpdatedReceiver,
                    new IntentFilter(ARDiscoveryService.kARDiscoveryServiceNotificationServicesDevicesListUpdated)
            );

            // create the service connection
            if (mArdiscoveryServiceConnection == null) {
                mArdiscoveryServiceConnection = new ServiceConnection() {
                    @Override
                    public void onServiceConnected(ComponentName name, IBinder service) {
                        mArdiscoveryService = ((ARDiscoveryService.LocalBinder) service).getService();

                        if (mStartDiscoveryAfterConnection) {
                            startDiscovering();
                            mStartDiscoveryAfterConnection = false;
                        }
                    }

                    @Override
                    public void onServiceDisconnected(ComponentName name) {
                        mArdiscoveryService = null;
                    }
                };
            }

            if (mArdiscoveryService == null) {
                // if the discovery service doesn't exists, bind to it
                Intent i = new Intent(getBaseContext(), ARDiscoveryService.class);
                getBaseContext().bindService(i, mArdiscoveryServiceConnection, Context.BIND_AUTO_CREATE);
            }
        }

        /**
         * Start discovering Parrot drones
         * For Wifi drones, the device should be on the drone's network
         * When drones will be discovered, you will be notified through {@link Listener#onDronesListUpdated(List)}
         */
        public void startDiscovering() {
            if (mArdiscoveryService != null) {
                mDiscoveryListener.onServicesDevicesListUpdated();
                mArdiscoveryService.start();
                mStartDiscoveryAfterConnection = false;
            } else {
                mStartDiscoveryAfterConnection = true;
            }
        }

        /**
         * Stop discovering Parrot drones
         */
        public void stopDiscovering() {
            if (mArdiscoveryService != null) {
                mArdiscoveryService.stop();
            }
            mStartDiscoveryAfterConnection = false;
        }

        /**
         * Cleanup the object
         * Should be called when the object is not used anymore
         */
        public void cleanup() {
            stopDiscovering();

            if (mArdiscoveryService != null) {
                new Thread(new Runnable() {
                    @Override
                    public void run() {
                        mArdiscoveryService.stop();

                        getBaseContext().unbindService(mArdiscoveryServiceConnection);
                        mArdiscoveryService = null;
                    }
                }).start();
            }
            // unregister receivers
            LocalBroadcastManager localBroadcastMgr = LocalBroadcastManager.getInstance(getBaseContext());
            localBroadcastMgr.unregisterReceiver(mArdiscoveryServicesDevicesListUpdatedReceiver);
        }

        /**
         * notify
         */
        void notifyServiceDiscoveredJumpingSumo() {
            for(ARDiscoveryDeviceService drone : mMatchingDrones) {
                ARDISCOVERY_PRODUCT_ENUM product = ARDiscoveryService.getProductFromProductID(drone.getProductID());

                switch (product) {
                    case ARDISCOVERY_PRODUCT_ARDRONE:
                    case ARDISCOVERY_PRODUCT_JS:
                    case ARDISCOVERY_PRODUCT_JS_EVO_LIGHT:
                    case ARDISCOVERY_PRODUCT_JS_EVO_RACE:
                        mListener.onFindJumpingSumo(drone);
                        break;
                    default:
                        Log.e("FindJunpingSumo", "The type " + product + " is not supported by this sample");
                        mListener.onNotFindJumpingSumo();
                        break;
                }
            }
        }

    }

    /**
     * Pointer to Jumping sumo scanner
     */
    public FindJumpingSumo mFindJumpingSumo;

    /**
     * Pointer to last Jumping sumo
     */
    JumpingSumo mJumpingSumo = null;

    /**
     * Listner Jumping sumo
     */
    final JumpingSumo.Listener mJumpingSumoListner = new JumpingSumo.Listener() {
        @Override
        public void onDroneConnectionChanged(ARCONTROLLER_DEVICE_STATE_ENUM state) {

            if(state == ARCONTROLLER_DEVICE_STATE_RUNNING)
            {
                onJumpingSumoConnected();
            }
            else if(state == ARCONTROLLER_DEVICE_STATE_STOPPED)
            {
                disconnectJumpingSumo();
            }
        }

        @Override
        public void onBatteryChargeChanged(int batteryPercentage) {

        }

        @Override
        public void onPictureTaken(ARCOMMANDS_JUMPINGSUMO_MEDIARECORDEVENT_PICTUREEVENTCHANGED_ERROR_ENUM error) {

        }

        @Override
        public void onAudioStateReceived(boolean inputEnabled, boolean outputEnabled) {

        }

        @Override
        public void configureDecoder(ARControllerCodec codec) {

        }

        @Override
        public void onFrameReceived(ARFrame frame) {

        }

        @Override
        public void configureAudioDecoder(ARControllerCodec codec) {

        }

        @Override
        public void onAudioFrameReceived(ARFrame frame) {

        }

        @Override
        public void onMatchingMediasFound(int nbMedias) {

        }

        @Override
        public void onDownloadProgressed(String mediaName, int progress) {

        }

        @Override
        public void onDownloadComplete(String mediaName) {

        }
    };

    /**
     * @brief isBluetoothSupport
     * @return true if bluetooth is supported
     */

    boolean isBluetoothSupport(){
        if (mBluetoothAdapter != null) {
            if (mBluetoothAdapter.isEnabled()) {
                return true;
            }
        }
        return true;
    }

    /**
     * @brief executeArduinoAction, called when when was classified a new gesture
     * @param className, class name of the last gesture classified
     */
    protected void executeJumpingSumoAction(String className) {
        if(mJumpingSumo != null)  {
            if(className.equals("normal")) {
                mJumpingSumo.setSpeed((byte) 0);
                mJumpingSumo.setFlag((byte) 0);
            }
            else if(className.equals("fist")) {
                mJumpingSumo.setSpeed((byte) 40);
                mJumpingSumo.setTurn((byte) 0);
                mJumpingSumo.setFlag((byte) 1);
            }
            else if(className.equals("right")) {
                mJumpingSumo.setSpeed((byte) 10);
                mJumpingSumo.setTurn((byte) 20);
                mJumpingSumo.setFlag((byte) 1);
            }
            else if(className.equals("left")) {
                mJumpingSumo.setSpeed((byte) 10);
                mJumpingSumo.setTurn((byte) -20);
                mJumpingSumo.setFlag((byte) 1);
            }
        }
    }

    /**
     * @brief bluetoothUnsupportedDialog, show a dialog to notify the user about the bluetooth connection support 
     */
    protected void bluetoothUnsupportedDialog()
    {
        final AlertDialog.Builder closeApplicationDialog = new AlertDialog.Builder(MainMyoClassifierParrot.this);
        closeApplicationDialog.setMessage("Bluetooth unsupported");
        closeApplicationDialog.setCancelable(false);
        closeApplicationDialog.setPositiveButton("Close application",
                new DialogInterface.OnClickListener() {
                    public void onClick(DialogInterface dialog, int id) {
                        dialog.dismiss();
                        MainMyoClassifierParrot.this.mKillApp = true;
                        MainMyoClassifierParrot.this.finish();
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
        //alloc Bluetooth manager
        mBluetoothManager =  (BluetoothManager) getSystemService(Context.BLUETOOTH_SERVICE);
        mBluetoothAdapter = mBluetoothManager.getAdapter();
        //request runtime permission
        getRuntimePermission();
        //get Location active
        getLocationPermission();
        //create scanner
        mFindJumpingSumo = new FindJumpingSumo(new FindJumpingSumoListener(){
            public void onFindJumpingSumo(ARDiscoveryDeviceService drone){
                mJumpingSumo = new JumpingSumo(MainMyoClassifierParrot.this.getBaseContext(),drone);
                mJumpingSumo.addListener(mJumpingSumoListner);
                if(!mJumpingSumo.connect()) {
                    disconnectJumpingSumo();
                }
                else if(isJumpingSumoConnected()){
                    onJumpingSumoConnected();
                }
            }
            public void onNotFindJumpingSumo(){
                onEndScanJumpingSumo();
            }
        });
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
                if(isJumpingSumoConnected())
                    disconnectJumpingSumo();
                else if(mUIRLMainMyoAnimationThreadLoop)
                    onEndScanJumpingSumo();
                else
                    startDiscoveringJumpingSumo();
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
                    Intent intent = new Intent(MainMyoClassifierParrot.this, MyoList.class);
                    //change
                    MainMyoClassifierParrot.this.startActivityForResult(intent, REQUEST_CODE_MYO_LIST);
                }
            }
        });

        //event
        ((Button)findViewById(R.id.btOpenModel)).setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                //create transition
                Intent fileExploreIntent = new Intent(
                        MainMyoClassifierParrot.this,
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
                MainMyoClassifierParrot.this.startActivityForResult(fileExploreIntent, REQUEST_CODE_PICK_FILE);
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
     * @brief isJumpingSumoConnected
     * @return true if JumpingSumo is connected
     */
    protected boolean isJumpingSumoConnected() {
        return mJumpingSumo != null && mJumpingSumo.getConnectionState().equals(ARCONTROLLER_DEVICE_STATE_RUNNING);
    }

    /**
     * @brief startDiscoveringJumpingSumo
     */
    protected void startDiscoveringJumpingSumo(){
        onScanJumpingSumo();
        mFindJumpingSumo.startDiscovering();
    }
    protected void stopDiscoveringJumpingSumo(){
        mFindJumpingSumo.stopDiscovering();
        onEndScanJumpingSumo();
    }
    protected void disconnectJumpingSumo(){
        onEndScanJumpingSumo();
        if(mJumpingSumo != null){
            // disconnect
            mJumpingSumo.disconnect();
            onJumpingSumoDisconnected();
        }
    }

    /**
     * @brief onScanJumpingSumo
     */
    protected void onScanJumpingSumo() {
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
        //stop animation
        if(mUIRLMainMyoAnimationThread!=null) {
            try {
                mUIRLMainMyoAnimationThreadLoop = false;
                mUIRLMainMyoAnimationThread.join();
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
        }
        //start animation
        mUIRLMainMyoAnimationThread = new Thread(new Runnable() {
            @Override
            public void run() {
                //animation factor
                double sfactor = Math.PI*0.5;
                //update
                while(mUIRLMain != null &&  MainMyoClassifierParrot.this.mUIRLMainMyoAnimationThreadLoop) {
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
                    //sleep
                    try {
                        Thread.sleep(33, 0);
                    } catch (Exception e) {
                        //none
                    }
                }
            }
        });
        mUIRLMainMyoAnimationThreadLoop = true;
        mUIRLMainMyoAnimationThread.start();
    }

    /**
     * @brief onEndScanJumpingSumo
     */
    protected void onEndScanJumpingSumo() {
        if(mUIRLMainMyoAnimationThreadLoop && mUIRLMainMyoAnimationThread!=null) {
            try {
                mUIRLMainMyoAnimationThreadLoop = false;
                mUIRLMainMyoAnimationThread.join();
                mUIRLMainMyoAnimationThread = null;
                //get main relative layout
                mUIRLMain = (RelativeLayout)findViewById(R.id.rlMainUi);
                if(mUIRLMain!=null) {
                    mUIRLMain.postInvalidate();
                    mUIRLMain.post(new Runnable() {
                        @Override
                        public void run() {
                            Drawable background = mUIRLMain.getBackground();
                            if(background != null) {
                                mUIRLMain.setBackgroundResource(0);
                            }
                        }
                    });
                }
            } catch (Exception e) {
                e.printStackTrace();
            }
        }
    }

    /**
     * @brief onJumpingSumoConnected
     */
    protected  void onJumpingSumoConnected(){
        onEndScanJumpingSumo();
        ((Button)findViewById(R.id.btConnect)).post(new Runnable() {
            @Override
            public void run() {
                ((Button)findViewById(R.id.btConnect)).setText("JumpingSumo Connected");
            }
        });
    }

    /**
     * @brief  onJumpingSumoDisconnected
     */
    protected  void onJumpingSumoDisconnected(){
        ((Button)findViewById(R.id.btConnect)).post(new Runnable() {
            @Override
            public void run() {
                ((Button) findViewById(R.id.btConnect)).setText("Connect to JumpingSumo");
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
            disconnectJumpingSumo();
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
        //..
        if(requestCode == SETTING_ENABLE_LOCATION) {
            if(!testLocationPermission()) {
                AlertDialog.Builder alert = new AlertDialog.Builder(MainMyoClassifierParrot.this);
                alert.setTitle("Error");
                alert.setMessage("Please enable location service");
                alert.setPositiveButton("OK", new DialogInterface.OnClickListener() {
                    public void onClick(DialogInterface dialog, int id) {
                        getLocationPermission();
                    }
                });
                alert.setNegativeButton("Close", new DialogInterface.OnClickListener() {
                    public void onClick(DialogInterface dialog, int id) {
                        MainMyoClassifierParrot.this.finish();

                    }
                });
                AlertDialog dialog = alert.create();
                dialog.show();
            }
        }
        //load file
        else if(requestCode == REQUEST_CODE_PICK_FILE)  {

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
                mNClassifierManager = new MyoNativeClassifierManager(typeClassifier);
                //load
                mNClassifierManager.loadModel(path);
                //to do... check success to load
                //set filter
                mNClassifierManager.setProbabilityFilter(0.5);
                //set callback
                mNClassifierManager.setListener(new MyoNativeClassifierManager.ClassifierListener() {
                    public void classified(final String className)
                    {
                        final TextView tvClassifier= (TextView)MainMyoClassifierParrot.this.findViewById(R.id.tvClassifier);
                        tvClassifier.post(new Runnable() {
                            @Override
                            public void run() {
                                tvClassifier.setText(className);
                            }
                        });
                        //execute an Arduino action
                        executeJumpingSumoAction(className);
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
        myo.addConnectionListener(this);
        myo.connect();
        myo.setTimeoutSendQueue(1000);
        myo.setConnectionSpeed(BaseMyo.ConnectionSpeed.HIGH);
        myo.writeSleepMode(MyoCmds.SleepMode.NEVER, null);
        myo.writeMode(MyoCmds.EmgMode.FILTERED,
                      MyoCmds.ImuMode.RAW,
                      MyoCmds.ClassifierMode.DISABLED,
                      null);
        //change button
        ((Button)findViewById(R.id.btSearchMyo)).post(new Runnable() {
            @Override
            public void run() {
                ((Button)findViewById(R.id.btSearchMyo)).setText("Myo Connected");
            }
        });
    }

    public void onConnectionStateChanged(final BaseMyo myo, BaseMyo.ConnectionState state)
    {
        if (state == BaseMyo.ConnectionState.CONNECTED) {
            //...
            mMyo.writeUnlock(MyoCmds.UnlockType.HOLD, new Myo.MyoCommandCallback() {
                @Override
                public void onCommandDone(Myo myo, MyoMsg msg) {
                    //send command
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
        else if (state == BaseMyo.ConnectionState.DISCONNECTED) {
            //event
            ((Button)findViewById(R.id.btSearchMyo)).post(new Runnable() {
                @Override
                public void run() {
                    disconnectMyo();
                    ((Button)findViewById(R.id.btSearchMyo)).setText("Search Myo");
                }
            });
        }
    }
    /**
     * @brief disconnectMyo, call this when you want disconnet from Myo
     */
    public void disconnectMyo() {
        if (mMyo != null &&
                (  mMyo.getConnectionState() == BaseMyo.ConnectionState.CONNECTED||
                   mMyo.getConnectionState() == BaseMyo.ConnectionState.CONNECTING  )) {
            mMyo.removeConnectionListener(this);
            mMyo.setConnectionSpeed(BaseMyo.ConnectionSpeed.BALANCED);
            mMyo.writeSleepMode(MyoCmds.SleepMode.NORMAL, null);
            mMyo.writeMode(MyoCmds.EmgMode.NONE, MyoCmds.ImuMode.NONE, MyoCmds.ClassifierMode.DISABLED, null);
            mMyo.disconnect();
        }
        mMyo = null;
    }
}
