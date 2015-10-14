package com.unipg.blearduinoandroid;

import android.app.Activity;
import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothDevice;
import android.bluetooth.BluetoothGattCharacteristic;
import android.bluetooth.BluetoothGattService;
import android.bluetooth.BluetoothManager;
import android.content.BroadcastReceiver;
import android.content.ComponentName;
import android.content.Context;
import android.content.Intent;
import android.content.IntentFilter;
import android.content.ServiceConnection;
import android.os.Bundle;
import android.os.IBinder;
import android.support.v7.app.AppCompatActivity;
import android.util.Log;

import com.redbear.simplecontrols.RBLGattAttributes;
import com.redbear.simplecontrols.RBLService;

import java.util.Locale;

/**
 * Created by Gabriele on 13/10/15.
 */
public class BLEArduinoAppCompatActivity extends AppCompatActivity{

    private static final int REQUEST_ENABLE_BT = 1;
    private BluetoothGattCharacteristic mCharacteristicTx = null;
    private RBLService mBluetoothLeService = null;
    private BluetoothAdapter mBluetoothAdapter = null;
    private BluetoothDevice mArduinoDevice = null;
    private boolean mConnected  = false;
    private byte[] mDataRecived = null;
    private long SCAN_PERIOD = 10000;


    /**
     * Event filters
     */
    private static IntentFilter makeGattUpdateIntentFilter() {
        final IntentFilter intentFilter = new IntentFilter();

        intentFilter.addAction(RBLService.ACTION_GATT_CONNECTED);
        intentFilter.addAction(RBLService.ACTION_GATT_DISCONNECTED);
        intentFilter.addAction(RBLService.ACTION_GATT_SERVICES_DISCOVERED);
        intentFilter.addAction(RBLService.ACTION_DATA_AVAILABLE);
        intentFilter.addAction(RBLService.ACTION_GATT_RSSI);

        return intentFilter;
    }

    /**
     * Connect to service
     */
    private final ServiceConnection mServiceConnection = new ServiceConnection() {

        @Override
        public void onServiceConnected(ComponentName componentName, IBinder service) {
            RBLService.LocalBinder rblBilder = ((RBLService.LocalBinder) service);
            mBluetoothLeService = rblBilder.getService();
            if (!mBluetoothLeService.initialize()) {
                Log.e("BLE Shild", "Unable to initialize Bluetooth");
                finish();
            }
        }

        @Override
        public void onServiceDisconnected(ComponentName componentName) {
            mBluetoothLeService = null;
        }
    };

    /**
     * Service Events
     */
    private final BroadcastReceiver mGattUpdateReceiver = new BroadcastReceiver() {
        @Override
        public void onReceive(Context context, Intent intent) {
            final String action = intent.getAction();

            if (RBLService.ACTION_GATT_DISCONNECTED.equals(action)) {
                mConnected = false;
                onArduinoDisconnected();
            } else if (RBLService.ACTION_GATT_SERVICES_DISCOVERED.equals(action)) {
                getGattService(mBluetoothLeService.getSupportedGattService());
            } else if (RBLService.ACTION_DATA_AVAILABLE.equals(action)) {
                mDataRecived = intent.getByteArrayExtra(RBLService.EXTRA_DATA);
            } else if (RBLService.ACTION_GATT_RSSI.equals(action)) {
                /* none */
            }
        }
    };

    /**
     * Search arduino device
     */
    private BluetoothAdapter.LeScanCallback mLeScanCallback = new BluetoothAdapter.LeScanCallback() {
        @Override
        public void onLeScan(final BluetoothDevice device, final int rssi,
                             final byte[] scanRecord) {

            runOnUiThread(new Runnable() {
                @Override
                public void run() {
                    byte[] serviceUuidBytes = new byte[16];
                    String serviceUuid = "";
                    for (int i = 32, j = 0; i >= 17; i--, j++) {
                        serviceUuidBytes[j] = scanRecord[i];
                    }
                    serviceUuid = bytesToHex(serviceUuidBytes);
                    if (stringToUuidString(serviceUuid).equals(RBLGattAttributes.BLE_SHIELD_SERVICE.toUpperCase(Locale.ENGLISH))) {
                        //save device
                        mArduinoDevice = device;
                        //stop scan
                        BLEArduinoAppCompatActivity.this.mBluetoothAdapter.stopLeScan(mLeScanCallback);
                        //connect
                        mBluetoothLeService.connect(mArduinoDevice.getAddress());
                    }
                }
            });
        }
    };


    final private static char[] hexArray = { '0', '1', '2', '3', '4', '5', '6', '7',
            '8', '9', 'a', 'b', 'c', 'd', 'e', 'f' };
    private String bytesToHex(byte[] bytes) {
        char[] hexChars = new char[bytes.length * 2];
        int v;
        for (int j = 0; j < bytes.length; j++) {
            v = bytes[j] & 0xFF;
            hexChars[j * 2] = hexArray[v >>> 4];
            hexChars[j * 2 + 1] = hexArray[v & 0x0F];
        }
        return new String(hexChars);
    }

    private String stringToUuidString(String uuid) {
        StringBuffer newString = new StringBuffer();
        newString.append(uuid.toUpperCase(Locale.ENGLISH).substring(0, 8));
        newString.append("-");
        newString.append(uuid.toUpperCase(Locale.ENGLISH).substring(8, 12));
        newString.append("-");
        newString.append(uuid.toUpperCase(Locale.ENGLISH).substring(12, 16));
        newString.append("-");
        newString.append(uuid.toUpperCase(Locale.ENGLISH).substring(16, 20));
        newString.append("-");
        newString.append(uuid.toUpperCase(Locale.ENGLISH).substring(20, 32));

        return newString.toString();
    }

    private void getGattService(BluetoothGattService gattService) {
        if (gattService == null)  return;
        mConnected = true;
        mCharacteristicTx = gattService.getCharacteristic(RBLService.UUID_BLE_SHIELD_TX);
        BluetoothGattCharacteristic characteristicRx = gattService.getCharacteristic(RBLService.UUID_BLE_SHIELD_RX);
        mBluetoothLeService.setCharacteristicNotification(characteristicRx, true);
        mBluetoothLeService.readCharacteristic(characteristicRx);
        onArduinoConnected(mArduinoDevice);
    }

    public  void onArduinoConnected(BluetoothDevice device){

    }
    public  void onArduinoDisconnected(){

    }

    public void arduinoMove(char dir){
        byte[] buf = new byte[] {(byte)'m', (byte)dir};
        mCharacteristicTx.setValue(buf);
        mBluetoothLeService.writeCharacteristic(mCharacteristicTx);
    }

    public void arduinoReset(){
        byte[] buf = new byte[] {(byte)'r',(byte)0};
        mCharacteristicTx.setValue(buf);
        mBluetoothLeService.writeCharacteristic(mCharacteristicTx);
    }

    public void scanLeDevice() {
        new Thread() {
            @Override
            public void run() {
                mBluetoothAdapter.startLeScan(mLeScanCallback);

                try {
                    Thread.sleep(SCAN_PERIOD);
                } catch (InterruptedException e) {
                    e.printStackTrace();
                }

                mBluetoothAdapter.stopLeScan(mLeScanCallback);
            }
        }.start();
    }

    public boolean isBluetoothSupport()
    {
        return mBluetoothAdapter!=null;
    }

    public boolean isConnected()
    {
        return mConnected;
    }

    public boolean canExecuteCommand()
    {
        return isConnected() && mCharacteristicTx!=null && mBluetoothLeService != null;
    }

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        //connection
        final BluetoothManager mBluetoothManager = (BluetoothManager) getSystemService(Context.BLUETOOTH_SERVICE);
        mBluetoothAdapter = mBluetoothManager.getAdapter();
        //bind service
        Intent gattServiceIntent = new Intent(BLEArduinoAppCompatActivity.this,  RBLService.class);
        bindService(gattServiceIntent, mServiceConnection, BIND_AUTO_CREATE);
        //reg input listener
        registerReceiver(mGattUpdateReceiver, makeGattUpdateIntentFilter());
    }



    @Override
    protected void onResume() {
        super.onResume();
        if (!mBluetoothAdapter.isEnabled()) {
            Intent enableBtIntent = new Intent(BluetoothAdapter.ACTION_REQUEST_ENABLE);
            startActivityForResult(enableBtIntent, REQUEST_ENABLE_BT);
        }
        registerReceiver(mGattUpdateReceiver, makeGattUpdateIntentFilter());
    }

    @Override
    protected void onStop() {
        super.onStop();
        mConnected = false;
        unregisterReceiver(mGattUpdateReceiver);
        onArduinoDisconnected();
    }

    @Override
    protected void onDestroy() {
        super.onDestroy();
        if (mServiceConnection != null) unbindService(mServiceConnection);
    }

    @Override
    protected void onActivityResult(int requestCode, int resultCode, Intent data) {
        // User chose not to enable Bluetooth.
        if (requestCode == REQUEST_ENABLE_BT  && resultCode == Activity.RESULT_CANCELED) {
            finish();
            return;
        }
        super.onActivityResult(requestCode, resultCode, data);
    }

}
