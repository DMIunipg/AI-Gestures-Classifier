/*
 * Android Myo library by darken
 * Matthias Urhahn (matthias.urhahn@rwth-aachen.de)
 * mHealth - Uniklinik RWTH-Aachen.
 */
package eu.darken.myolib;

import android.annotation.TargetApi;
import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothDevice;
import android.bluetooth.BluetoothManager;
import android.bluetooth.le.BluetoothLeScanner;
import android.bluetooth.le.ScanCallback;
import android.bluetooth.le.ScanFilter;
import android.bluetooth.le.ScanResult;
import android.bluetooth.le.ScanSettings;
import android.content.Context;
import android.os.Build;
import android.support.annotation.Nullable;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;
import java.util.UUID;

import eu.darken.myolib.services.Control;
import eu.darken.myolib.tools.ByteHelper;
import eu.darken.myolib.tools.Logy;

/**
 * Wrapper for {@link BluetoothAdapter} that finds Myo devices
 */
public class MyoConnector implements BluetoothAdapter.LeScanCallback {
    private static final String TAG = "MyoLib:RawMyoConnector";
    private final Context mContext;
    private final BluetoothLeScanner mBluetoothLeScanner;
    private final BluetoothAdapter mBluetoothAdapter;
    private final Map<String, Myo> mDeviceMap = new HashMap<>();
    private final Map<String, Myo> mScanMap = new HashMap<>();
    private ScannerCallback mCallback;
    private Runnable mScanRunnable;
    private Thread mScanThread;

    public MyoConnector(Context context) {
        mContext = context.getApplicationContext();
        BluetoothManager bluetoothManager = (BluetoothManager) mContext.getSystemService(Context.BLUETOOTH_SERVICE);
        mBluetoothAdapter = bluetoothManager.getAdapter();
        //scanner
        if ( android.os.Build.VERSION.SDK_INT >= Build.VERSION_CODES.LOLLIPOP) {
            mBluetoothLeScanner = mBluetoothAdapter.getBluetoothLeScanner();
        }
        else {
            mBluetoothLeScanner = null;
        }
    }

    public Context getContext() {
        return mContext;
    }

    /**
     * Scans for Myo devices
     * Results will only be returned after the scan finished.
     *
     * @param timeout  How long the scan lasts in milliseconds.
     * @param callback optional callback with results, also available via {@link #getMyos()}
     * @return true if a scan was started, false if a scan was already running.
     */
    /*
    @TargetApi(Build.VERSION_CODES.KITKAT)
    public boolean scan(final long timeout, @Nullable final ScannerCallback callback) {
        //scannable?
        if (mScanRunnable != null)
            return false;
        //restart
        mScanMap.clear();
        //save callback
        mCallback = callback;
        //find devices
        mScanRunnable = new Runnable() {
            public void run() {
                mBluetoothAdapter.startLeScan(MyoConnector.this);
                try {
                    Thread.sleep(timeout);
                } catch (InterruptedException e) {
                    e.printStackTrace();
                }
                Logy.d(TAG, "Scan stopped (timeout:" + timeout + ")");
                stopScan();
            }
        };
        new Thread(mScanRunnable).start();
        return true;
    }
    */
    @TargetApi(Build.VERSION_CODES.LOLLIPOP)
    public boolean scan(final long timeout, @Nullable final ScannerCallback callback) {
        //scannable?
        if (mScanRunnable != null)
            return false;
        //restart
        mScanMap.clear();
        //save callback
        mCallback = callback;
        //new api
        if ( android.os.Build.VERSION.SDK_INT >= Build.VERSION_CODES.LOLLIPOP){
            // callback

            final ScanCallback scanCallback = new ScanCallback() {
                @Override
                public void onScanResult(int callbackType, ScanResult result) {
                    super.onScanResult(callbackType, result);
                    MyoConnector.this.onLeScan(
                            result.getDevice(),
                            result.getRssi(),
                            result.getScanRecord().getBytes()
                    );
                }

                @Override
                public void onBatchScanResults(List<ScanResult> results) {
                    super.onBatchScanResults(results);
                }

                @Override
                public void onScanFailed(int errorCode) {
                    super.onScanFailed(errorCode);
                }
            };
            //find devices
            mScanRunnable = new Runnable() {
                public void run() {
                    //wait
                    try {
                        Thread.sleep(timeout);
                    } catch (InterruptedException e) {
                        e.printStackTrace();
                    }
                    Logy.d(TAG, "Scan stopped (timeout:" + timeout + ")");
                    //stop
                    mBluetoothLeScanner.stopScan(scanCallback);
                    stopScan();
                }
            };
            // scan for devices
            mBluetoothLeScanner.startScan(scanCallback);
            //
        } else {
            //find devices
            mScanRunnable = new Runnable() {
                public void run() {
                    mBluetoothAdapter.startLeScan(MyoConnector.this);
                    try {
                        Thread.sleep(timeout);
                    } catch (InterruptedException e) {
                        e.printStackTrace();
                    }
                    Logy.d(TAG, "Scan stopped (timeout:" + timeout + ")");
                    //stop
                    mBluetoothAdapter.stopLeScan(MyoConnector.this);
                    //Stop
                    stopScan();
                }
            };
        }
        //
        mScanThread = new Thread(mScanRunnable);
        mScanThread.start();
        //
        return true;
    }

    public void stopScan() {
        //stop
        if(mScanRunnable != null && mScanThread!=null) {
            //to null
            mScanRunnable = null;
            mScanThread = null;
            //call
            if (mCallback != null)
                mCallback.onScanFinished(new ArrayList<>(mDeviceMap.values()));
        }
    }

    @Override
    public void onLeScan(BluetoothDevice device, int rssi, byte[] scanRecord) {
        List<AdRecord> adRecords = AdRecord.parseScanRecord(scanRecord);
        UUID uuid = null;
        for (AdRecord adRecord : adRecords) {
            // TYPE_UUID128_INC
            if (adRecord.getType() == 0x6) {
                uuid = new ByteHelper(adRecord.getData()).getUUID();
                break;
            }
        }
        if (Control.getServiceUUID().equals(uuid)) {
            if (!mScanMap.containsKey(device.getAddress())) {
                Myo myo = mDeviceMap.get(device.getAddress());
                if (myo == null) {
                    myo = new Myo(getContext(), device);
                    mDeviceMap.put(device.getAddress(), myo);
                }
                mScanMap.put(device.getAddress(), myo);
                if(mCallback != null)
                    mCallback.onScanFind(myo);
            }
        }
    }

    public interface ScannerCallback {
        void onScanFinished(List<Myo> myos);
        void onScanFind(Myo myo);
    }

    public ArrayList<Myo> getMyos() {
        return new ArrayList<>(mDeviceMap.values());
    }
}
