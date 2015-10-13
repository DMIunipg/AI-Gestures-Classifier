package com.unipg.myoclassifier;


import eu.darken.myolib.processor.imu.ImuData;
import eu.darken.myolib.processor.imu.ImuProcessor.ImuDataListener;
import eu.darken.myolib.processor.emg.EmgData;
import eu.darken.myolib.processor.emg.EmgProcessor.EmgDataListener;

/**
 * Created by Gabriele on 12/10/15.
 */
public class MyoNativeListener implements EmgDataListener, ImuDataListener {

    public native void onEmg(int emg0,
                             int emg1,
                             int emg2,
                             int emg3,
                             int emg4,
                             int emg5,
                             int emg6,
                             int emg7);
    public native void onQuaternion(double x, double y, double z, double w);
    public native void onAccelerometer(double x, double y, double z);
    public native void onGyroscope(double x, double y, double z);


    @Override
    public void onNewEmgData(EmgData emgData) {
        this.onEmg( emgData.getData()[0],
                    emgData.getData()[1],
                    emgData.getData()[2],
                    emgData.getData()[3],
                    emgData.getData()[4],
                    emgData.getData()[5],
                    emgData.getData()[6],
                    emgData.getData()[7]);

    }

    @Override
    public void onNewImuData(ImuData imuData) {
        this.onAccelerometer(imuData.getAccelerometerData()[0],
                             imuData.getAccelerometerData()[1],
                             imuData.getAccelerometerData()[2]);

        this.onGyroscope(imuData.getGyroData()[0],
                         imuData.getGyroData()[1],
                         imuData.getGyroData()[2]);

        this.onQuaternion(imuData.getOrientationData()[0],
                          imuData.getOrientationData()[1],
                          imuData.getOrientationData()[2],
                          imuData.getOrientationData()[3]);
    }
}
